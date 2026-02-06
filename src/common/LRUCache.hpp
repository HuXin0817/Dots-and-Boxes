#pragma once

#include <tbb/concurrent_hash_map.h>
#include <tbb/spin_mutex.h>

#include <atomic>

namespace dab::detail::common {

template <typename TKey, typename TValue, uint32_t Cap>
class LRUCache {
  struct ListNode {
    ListNode();
    ListNode(const TKey& key);

    TKey Key;
    ListNode* Prev;
    ListNode* Next;

    bool
    IsInList() const;
  };

  static ListNode* const OutOfListMarker;

  struct HashMapValue {
    HashMapValue();
    HashMapValue(const TValue& value, ListNode* node);

    TValue Value;
    ListNode* Node;
  };

  using HashMap = tbb::concurrent_hash_map<TKey, HashMapValue>;
  using HashMapConstAccessor = typename HashMap::const_accessor;
  using HashMapAccessor = typename HashMap::accessor;
  using HashMapValuePair = typename HashMap::value_type;

  public:
  struct ConstAccessor {
    ConstAccessor();

    const TValue&
    operator*() const;
    const TValue*
    operator->() const;
    const TValue*
    Get() const;
    bool
    Empty() const;

private:
    friend class LRUCache;
    HashMapConstAccessor HashAccessor;
  };

  LRUCache();

  LRUCache(const LRUCache& other) = delete;
  LRUCache&
  operator=(const LRUCache&) = delete;

  ~LRUCache();

  bool
  Find(ConstAccessor& ac, const TKey& key);
  bool
  Insert(const TKey& key, const TValue& value);
  void
  Clear();

  uint32_t
  Size() const;

  private:
  void
  Delink(ListNode* node);
  void
  PushFront(ListNode* node);
  void
  Evict();

  std::atomic<uint32_t> Length;
  HashMap Map;
  ListNode Head;
  ListNode Tail;
  tbb::speculative_spin_mutex ListMutex;
};

template <typename TKey, typename TValue, uint32_t Cap>
typename LRUCache<TKey, TValue, Cap>::ListNode* const LRUCache<TKey, TValue, Cap>::OutOfListMarker =
    reinterpret_cast<ListNode*>(-1);

template <typename TKey, typename TValue, uint32_t Cap>
LRUCache<TKey, TValue, Cap>::ListNode::ListNode() : Prev(OutOfListMarker), Next(nullptr) {
}

template <typename TKey, typename TValue, uint32_t Cap>
LRUCache<TKey, TValue, Cap>::ListNode::ListNode(const TKey& key) : Key(key), Prev(OutOfListMarker), Next(nullptr) {
}

template <typename TKey, typename TValue, uint32_t Cap>
bool
LRUCache<TKey, TValue, Cap>::ListNode::IsInList() const {
  return Prev != OutOfListMarker;
}

template <typename TKey, typename TValue, uint32_t Cap>
LRUCache<TKey, TValue, Cap>::HashMapValue::HashMapValue() : Node(nullptr) {
}

template <typename TKey, typename TValue, uint32_t Cap>
LRUCache<TKey, TValue, Cap>::HashMapValue::HashMapValue(const TValue& value, ListNode* node)
    : Value(value), Node(node) {
}

template <typename TKey, typename TValue, uint32_t Cap>
LRUCache<TKey, TValue, Cap>::ConstAccessor::ConstAccessor() = default;

template <typename TKey, typename TValue, uint32_t Cap>
const TValue&
LRUCache<TKey, TValue, Cap>::ConstAccessor::operator*() const {
  return *Get();
}

template <typename TKey, typename TValue, uint32_t Cap>
const TValue*
LRUCache<TKey, TValue, Cap>::ConstAccessor::operator->() const {
  return Get();
}

template <typename TKey, typename TValue, uint32_t Cap>
const TValue*
LRUCache<TKey, TValue, Cap>::ConstAccessor::Get() const {
  return &HashAccessor->second.Value;
}

template <typename TKey, typename TValue, uint32_t Cap>
bool
LRUCache<TKey, TValue, Cap>::ConstAccessor::Empty() const {
  return HashAccessor.empty();
}

template <typename TKey, typename TValue, uint32_t Cap>
LRUCache<TKey, TValue, Cap>::LRUCache() : Length(0), Map(Cap) {
  Head.Prev = nullptr;
  Head.Next = &Tail;
  Tail.Prev = &Head;
}

template <typename TKey, typename TValue, uint32_t Cap>
LRUCache<TKey, TValue, Cap>::~LRUCache() {
  Clear();
}

template <typename TKey, typename TValue, uint32_t Cap>
bool
LRUCache<TKey, TValue, Cap>::Find(ConstAccessor& ac, const TKey& key) {
  HashMapConstAccessor& hashAccessor = ac.HashAccessor;
  if (!Map.find(hashAccessor, key)) {
    return false;
  }

  std::unique_lock lock(ListMutex, std::try_to_lock);
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

template <typename TKey, typename TValue, uint32_t Cap>
bool
LRUCache<TKey, TValue, Cap>::Insert(const TKey& key, const TValue& value) {
  ListNode* node = new ListNode(key);
  HashMapAccessor hashAccessor;
  HashMapValuePair hashMapValue(key, HashMapValue(value, node));
  if (!Map.insert(hashAccessor, hashMapValue)) {
    delete node;
    return false;
  }
  hashAccessor.release();

  uint32_t size = Length.load();
  bool evictionDone = false;
  if (size >= Cap) {
    Evict();
    evictionDone = true;
  }

  std::unique_lock lock(ListMutex);
  PushFront(node);
  lock.unlock();

  if (!evictionDone) {
    size = Length++;
  }
  if (size > Cap) {
    if (Length.compare_exchange_strong(size, size - 1)) {
      Evict();
    }
  }
  return true;
}

template <typename TKey, typename TValue, uint32_t Cap>
void
LRUCache<TKey, TValue, Cap>::Clear() {
  Map.clear();
  ListNode* node = Head.Next;
  while (node != &Tail) {
    ListNode* next = node->Next;
    delete node;
    node = next;
  }
  Head.Next = &Tail;
  Tail.Prev = &Head;
  Length = 0;
}

template <typename TKey, typename TValue, uint32_t Cap>
uint32_t
LRUCache<TKey, TValue, Cap>::Size() const {
  return Length.load();
}

template <typename TKey, typename TValue, uint32_t Cap>
void
LRUCache<TKey, TValue, Cap>::Delink(ListNode* node) {
  ListNode* prev = node->Prev;
  ListNode* next = node->Next;
  prev->Next = next;
  next->Prev = prev;
  node->Prev = OutOfListMarker;
}

template <typename TKey, typename TValue, uint32_t Cap>
void
LRUCache<TKey, TValue, Cap>::PushFront(ListNode* node) {
  ListNode* oldRealHead = Head.Next;
  node->Prev = &Head;
  node->Next = oldRealHead;
  oldRealHead->Prev = node;
  Head.Next = node;
}

template <typename TKey, typename TValue, uint32_t Cap>
void
LRUCache<TKey, TValue, Cap>::Evict() {
  std::unique_lock lock(ListMutex);
  ListNode* moribund = Tail.Prev;
  if (moribund == &Head) {
    lock.unlock();
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