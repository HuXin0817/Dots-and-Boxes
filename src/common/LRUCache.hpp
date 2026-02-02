#include <tbb/concurrent_hash_map.h>

#include <atomic>
#include <mutex>
#include <new>
#include <thread>
#include <vector>

namespace dab::detail::common {

template <class TKey, class TValue, class THash = tbb::tbb_hash_compare<TKey>>
class LRUCache {
  struct ListNode {
    ListNode() : m_prev(OutOfListMarker), m_next(nullptr) {
    }

    ListNode(const TKey& key) : m_key(key), m_prev(OutOfListMarker), m_next(nullptr) {
    }

    TKey m_key;
    ListNode* m_prev;
    ListNode* m_next;

    bool
    isInList() const {
      return m_prev != OutOfListMarker;
    }
  };

  static ListNode* const OutOfListMarker;

  struct HashMapValue {
    HashMapValue() : m_listNode(nullptr) {
    }

    HashMapValue(const TValue& value, ListNode* node) : m_value(value), m_listNode(node) {
    }

    TValue m_value;
    ListNode* m_listNode;
  };

  typedef tbb::concurrent_hash_map<TKey, HashMapValue, THash> HashMap;
  typedef typename HashMap::const_accessor HashMapConstAccessor;
  typedef typename HashMap::accessor HashMapAccessor;
  typedef typename HashMap::value_type HashMapValuePair;
  typedef std::pair<const TKey, TValue> SnapshotValue;

  public:
  struct ConstAccessor {
    ConstAccessor() {
    }

    const TValue&
    operator*() const {
      return *get();
    }

    const TValue*
    operator->() const {
      return get();
    }

    const TValue*
    get() const {
      return &m_hashAccessor->second.m_value;
    }

    bool
    empty() const {
      return m_hashAccessor.empty();
    }

private:
    friend class LRUCache;
    HashMapConstAccessor m_hashAccessor;
  };

  explicit LRUCache(size_t maxSize);

  LRUCache(const LRUCache& other) = delete;
  LRUCache&
  operator=(const LRUCache&) = delete;

  ~LRUCache() {
    clear();
  }

  bool
  find(ConstAccessor& ac, const TKey& key);

  bool
  insert(const TKey& key, const TValue& value);

  void
  clear();

  void
  snapshotKeys(std::vector<TKey>& keys);

  size_t
  size() const {
    return m_size.load();
  }

  private:
  void
  delink(ListNode* node);

  void
  pushFront(ListNode* node);

  void
  evict();

  size_t m_maxSize;
  std::atomic<size_t> m_size;
  HashMap m_map;
  ListNode m_head;
  ListNode m_tail;
  typedef std::mutex ListMutex;
  ListMutex m_listMutex;
};

template <class TKey, class TValue, class THash>
typename LRUCache<TKey, TValue, THash>::ListNode* const LRUCache<TKey, TValue, THash>::OutOfListMarker = (ListNode*)-1;

template <class TKey, class TValue, class THash>
LRUCache<TKey, TValue, THash>::LRUCache(size_t maxSize)
    : m_maxSize(maxSize), m_size(0), m_map(std::thread::hardware_concurrency() * 4) {
  m_head.m_prev = nullptr;
  m_head.m_next = &m_tail;
  m_tail.m_prev = &m_head;
}

template <class TKey, class TValue, class THash>
bool
LRUCache<TKey, TValue, THash>::find(ConstAccessor& ac, const TKey& key) {
  HashMapConstAccessor& hashAccessor = ac.m_hashAccessor;
  if (!m_map.find(hashAccessor, key)) {
    return false;
  }

  std::unique_lock<ListMutex> lock(m_listMutex, std::try_to_lock);
  if (lock) {
    ListNode* node = hashAccessor->second.m_listNode;
    if (node->isInList()) {
      delink(node);
      pushFront(node);
    }
    lock.unlock();
  }
  return true;
}

template <class TKey, class TValue, class THash>
bool
LRUCache<TKey, TValue, THash>::insert(const TKey& key, const TValue& value) {
  ListNode* node = new ListNode(key);
  HashMapAccessor hashAccessor;
  HashMapValuePair hashMapValue(key, HashMapValue(value, node));
  if (!m_map.insert(hashAccessor, hashMapValue)) {
    delete node;
    return false;
  }
  hashAccessor.release();

  size_t size = m_size.load();
  bool evictionDone = false;
  if (size >= m_maxSize) {
    evict();
    evictionDone = true;
  }

  std::unique_lock<ListMutex> lock(m_listMutex);
  pushFront(node);
  lock.unlock();
  if (!evictionDone) {
    size = m_size++;
  }
  if (size > m_maxSize) {
    if (m_size.compare_exchange_strong(size, size - 1)) {
      evict();
    }
  }
  return true;
}

template <class TKey, class TValue, class THash>
void
LRUCache<TKey, TValue, THash>::clear() {
  m_map.clear();
  ListNode* node = m_head.m_next;
  ListNode* next;
  while (node != &m_tail) {
    next = node->m_next;
    delete node;
    node = next;
  }
  m_head.m_next = &m_tail;
  m_tail.m_prev = &m_head;
  m_size = 0;
}

template <class TKey, class TValue, class THash>
void
LRUCache<TKey, TValue, THash>::snapshotKeys(std::vector<TKey>& keys) {
  keys.reserve(keys.size() + m_size.load());
  std::lock_guard<ListMutex> lock(m_listMutex);
  for (ListNode* node = m_head.m_next; node != &m_tail; node = node->m_next) {
    keys.push_back(node->m_key);
  }
}

template <class TKey, class TValue, class THash>
inline void
LRUCache<TKey, TValue, THash>::delink(ListNode* node) {
  ListNode* prev = node->m_prev;
  ListNode* next = node->m_next;
  prev->m_next = next;
  next->m_prev = prev;
  node->m_prev = OutOfListMarker;
}

template <class TKey, class TValue, class THash>
inline void
LRUCache<TKey, TValue, THash>::pushFront(ListNode* node) {
  ListNode* oldRealHead = m_head.m_next;
  node->m_prev = &m_head;
  node->m_next = oldRealHead;
  oldRealHead->m_prev = node;
  m_head.m_next = node;
}

template <class TKey, class TValue, class THash>
void
LRUCache<TKey, TValue, THash>::evict() {
  std::unique_lock<ListMutex> lock(m_listMutex);
  ListNode* moribund = m_tail.m_prev;
  if (moribund == &m_head) {
    return;
  }
  delink(moribund);
  lock.unlock();

  HashMapAccessor hashAccessor;
  if (!m_map.find(hashAccessor, moribund->m_key)) {
    return;
  }
  m_map.erase(hashAccessor);
  delete moribund;
}

}  // namespace dab::detail::common
