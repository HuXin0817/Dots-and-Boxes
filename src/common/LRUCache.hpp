#pragma once

#include <cstddef>
#include <unordered_map>

namespace dab::detail::common {

template <typename Key, typename Value, size_t Capacity>
class LRUCache {
  private:
  struct Node {
    Key key;
    Value value;
    Node* prev = nullptr;
    Node* next = nullptr;
  };

  public:
  LRUCache();
  ~LRUCache();

  Value*
  Get(const Key& key);

  void
  Put(const Key& key, const Value& value);

  private:
  void
  AddToHead(Node* node);

  void
  RemoveNode(Node* node);

  void
  MoveToHead(Node* node);

  Node* head = new Node{};
  Node* tail = new Node{};
  std::unordered_map<Key, Node*> map;
};

template <typename Key, typename Value, size_t Capacity>
LRUCache<Key, Value, Capacity>::LRUCache() {
  head->next = tail;
  tail->prev = head;
}

template <typename Key, typename Value, size_t Capacity>
LRUCache<Key, Value, Capacity>::~LRUCache() {
  Node* current = head->next;
  while (current != tail) {
    Node* next = current->next;
    delete current;
    current = next;
  }
  delete head;
  delete tail;
}

template <typename Key, typename Value, size_t Capacity>
Value*
LRUCache<Key, Value, Capacity>::Get(const Key& key) {
  if (auto it = map.find(key); it != map.end()) {
    Node* node = it->second;
    MoveToHead(node);
    return &node->value;
  }
  return nullptr;
}

template <typename Key, typename Value, size_t Capacity>
void
LRUCache<Key, Value, Capacity>::Put(const Key& key, const Value& value) {
  if (auto it = map.find(key); it != map.end()) {
    Node* node = it->second;
    node->value = value;
    MoveToHead(node);
    return;
  }

  if (map.size() >= Capacity) {
    Node* tailPrev = tail->prev;
    RemoveNode(tailPrev);
    map.erase(tailPrev->key);
    delete tailPrev;
  }

  Node* newNode = new Node{key, value};
  AddToHead(newNode);
  map[key] = newNode;
}

template <typename Key, typename Value, size_t Capacity>
void
LRUCache<Key, Value, Capacity>::AddToHead(Node* node) {
  node->prev = head;
  node->next = head->next;
  head->next->prev = node;
  head->next = node;
}

template <typename Key, typename Value, size_t Capacity>
void
LRUCache<Key, Value, Capacity>::RemoveNode(Node* node) {
  node->prev->next = node->next;
  node->next->prev = node->prev;
}

template <typename Key, typename Value, size_t Capacity>
void
LRUCache<Key, Value, Capacity>::MoveToHead(Node* node) {
  RemoveNode(node);
  AddToHead(node);
}

}  // namespace dab::detail::common
