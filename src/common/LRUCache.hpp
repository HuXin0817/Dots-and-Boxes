#pragma once

#include <tbb/concurrent_hash_map.h>
#include <tbb/spin_mutex.h>

#include <atomic>
#include <thread>

namespace dab::detail::common {

template <class TKey, class TValue, class THash = tbb::tbb_hash_compare<TKey>>
class LRUCache {
  struct ListNode {
    ListNode() : Prev(OutOfListMarker), Next(nullptr) {
    }

    ListNode(const TKey& key) : Key(key), Prev(OutOfListMarker), Next(nullptr) {
    }

    TKey Key;
    ListNode* Prev;
    ListNode* Next;

    bool
    IsInList() const {
      return Prev != OutOfListMarker;
    }
  };

  static ListNode* const OutOfListMarker;

  struct HashMapValue {
    HashMapValue() : Node(nullptr) {
    }

    HashMapValue(const TValue& value, ListNode* node) : Value(value), Node(node) {
    }

    TValue Value;
    ListNode* Node;
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
      return *Get();
    }

    const TValue*
    operator->() const {
      return Get();
    }

    const TValue*
    Get() const {
      return &HashAccessor->second.Value;
    }

    bool
    Empty() const {
      return HashAccessor.empty();
    }

private:
    friend class LRUCache;
    HashMapConstAccessor HashAccessor;
  };

  explicit LRUCache(size_t maxSize);

  LRUCache(const LRUCache& other) = delete;
  LRUCache&
  operator=(const LRUCache&) = delete;

  ~LRUCache() {
    Clear();
  }

  bool
  Find(ConstAccessor& ac, const TKey& key);

  bool
  Insert(const TKey& key, const TValue& value);

  void
  Clear();

  size_t
  Size() const {
    return Length.load();
  }

  private:
  void
  Delink(ListNode* node);

  void
  PushFront(ListNode* node);

  void
  Evict();

  size_t MaxSize;
  std::atomic<size_t> Length;
  HashMap Map;
  ListNode Head;
  ListNode Tail;
  tbb::spin_mutex ListMutex;
};

template <class TKey, class TValue, class THash>
typename LRUCache<TKey, TValue, THash>::ListNode* const LRUCache<TKey, TValue, THash>::OutOfListMarker = (ListNode*)-1;

template <class TKey, class TValue, class THash>
LRUCache<TKey, TValue, THash>::LRUCache(size_t maxSize)
    : MaxSize(maxSize), Length(0), Map(std::thread::hardware_concurrency() * 4) {
  Head.Prev = nullptr;
  Head.Next = &Tail;
  Tail.Prev = &Head;
}

template <class TKey, class TValue, class THash>
bool
LRUCache<TKey, TValue, THash>::Find(ConstAccessor& ac, const TKey& key) {
  HashMapConstAccessor& hashAccessor = ac.HashAccessor;
  if (!Map.find(hashAccessor, key)) {
    return false;
  }

  std::unique_lock<tbb::spin_mutex> lock(ListMutex, std::try_to_lock);
  if (lock) {
    ListNode* node = hashAccessor->second.Node;
    if (node->IsInList()) {
      Delink(node);
      PushFront(node);
    }
    lock.unlock();
  }
  return true;
}

template <class TKey, class TValue, class THash>
bool
LRUCache<TKey, TValue, THash>::Insert(const TKey& key, const TValue& value) {
  ListNode* node = new ListNode(key);
  HashMapAccessor hashAccessor;
  HashMapValuePair hashMapValue(key, HashMapValue(value, node));
  if (!Map.insert(hashAccessor, hashMapValue)) {
    delete node;
    return false;
  }
  hashAccessor.release();

  size_t size = Length.load();
  bool evictionDone = false;
  if (size >= MaxSize) {
    Evict();
    evictionDone = true;
  }

  std::unique_lock<tbb::spin_mutex> lock(ListMutex);
  PushFront(node);
  lock.unlock();
  if (!evictionDone) {
    size = Length++;
  }
  if (size > MaxSize) {
    if (Length.compare_exchange_strong(size, size - 1)) {
      Evict();
    }
  }
  return true;
}

template <class TKey, class TValue, class THash>
void
LRUCache<TKey, TValue, THash>::Clear() {
  Map.clear();
  ListNode* node = Head.Next;
  ListNode* next;
  while (node != &Tail) {
    next = node->Next;
    delete node;
    node = next;
  }
  Head.Next = &Tail;
  Tail.Prev = &Head;
  Length = 0;
}

template <class TKey, class TValue, class THash>
inline void
LRUCache<TKey, TValue, THash>::Delink(ListNode* node) {
  ListNode* prev = node->Prev;
  ListNode* next = node->Next;
  prev->Next = next;
  next->Prev = prev;
  node->Prev = OutOfListMarker;
}

template <class TKey, class TValue, class THash>
inline void
LRUCache<TKey, TValue, THash>::PushFront(ListNode* node) {
  ListNode* oldRealHead = Head.Next;
  node->Prev = &Head;
  node->Next = oldRealHead;
  oldRealHead->Prev = node;
  Head.Next = node;
}

template <class TKey, class TValue, class THash>
void
LRUCache<TKey, TValue, THash>::Evict() {
  std::unique_lock<tbb::spin_mutex> lock(ListMutex);
  ListNode* moribund = Tail.Prev;
  if (moribund == &Head) {
    return;
  }
  Delink(moribund);
  lock.unlock();

  HashMapAccessor hashAccessor;
  if (!Map.find(hashAccessor, moribund->Key)) {
    return;
  }
  Map.erase(hashAccessor);
  delete moribund;
}

}  // namespace dab::detail::common