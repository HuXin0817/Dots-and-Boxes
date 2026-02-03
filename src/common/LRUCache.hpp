#pragma once

#include <tbb/concurrent_hash_map.h>
#include <tbb/spin_mutex.h>

#include <atomic>

namespace dab::detail::common {

template <class TKey, class TValue, class THash = tbb::tbb_hash_compare<TKey>>
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

  using HashMap = tbb::concurrent_hash_map<TKey, HashMapValue, THash>;
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

  explicit LRUCache(uint32_t maxSize);

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

  uint32_t MaxSize;
  std::atomic<uint32_t> Length;
  HashMap Map;
  ListNode Head;
  ListNode Tail;
  tbb::spin_mutex ListMutex;
};

template <class TKey, class TValue, class THash>
typename LRUCache<TKey, TValue, THash>::ListNode* const LRUCache<TKey, TValue, THash>::OutOfListMarker =
    reinterpret_cast<typename LRUCache<TKey, TValue, THash>::ListNode*>(-1);

template <class TKey, class TValue, class THash>
LRUCache<TKey, TValue, THash>::ListNode::ListNode() : Prev(OutOfListMarker), Next(nullptr) {
}

template <class TKey, class TValue, class THash>
LRUCache<TKey, TValue, THash>::ListNode::ListNode(const TKey& key) : Key(key), Prev(OutOfListMarker), Next(nullptr) {
}

template <class TKey, class TValue, class THash>
bool
LRUCache<TKey, TValue, THash>::ListNode::IsInList() const {
  return Prev != OutOfListMarker;
}

template <class TKey, class TValue, class THash>
LRUCache<TKey, TValue, THash>::HashMapValue::HashMapValue() : Node(nullptr) {
}

template <class TKey, class TValue, class THash>
LRUCache<TKey, TValue, THash>::HashMapValue::HashMapValue(const TValue& value, ListNode* node)
    : Value(value), Node(node) {
}

template <class TKey, class TValue, class THash>
LRUCache<TKey, TValue, THash>::ConstAccessor::ConstAccessor() = default;

template <class TKey, class TValue, class THash>
const TValue&
LRUCache<TKey, TValue, THash>::ConstAccessor::operator*() const {
  return *Get();
}

template <class TKey, class TValue, class THash>
const TValue*
LRUCache<TKey, TValue, THash>::ConstAccessor::operator->() const {
  return Get();
}

template <class TKey, class TValue, class THash>
const TValue*
LRUCache<TKey, TValue, THash>::ConstAccessor::Get() const {
  return &HashAccessor->second.Value;
}

template <class TKey, class TValue, class THash>
bool
LRUCache<TKey, TValue, THash>::ConstAccessor::Empty() const {
  return HashAccessor.empty();
}

template <class TKey, class TValue, class THash>
LRUCache<TKey, TValue, THash>::LRUCache(uint32_t maxSize) : MaxSize(maxSize), Length(0), Map(maxSize) {
  Head.Prev = nullptr;
  Head.Next = &Tail;
  Tail.Prev = &Head;
}

template <class TKey, class TValue, class THash>
LRUCache<TKey, TValue, THash>::~LRUCache() {
  Clear();
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

  uint32_t size = Length.load();
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
uint32_t
LRUCache<TKey, TValue, THash>::Size() const {
  return Length.load();
}

template <class TKey, class TValue, class THash>
void
LRUCache<TKey, TValue, THash>::Delink(ListNode* node) {
  ListNode* prev = node->Prev;
  ListNode* next = node->Next;
  prev->Next = next;
  next->Prev = prev;
  node->Prev = OutOfListMarker;
}

template <class TKey, class TValue, class THash>
void
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