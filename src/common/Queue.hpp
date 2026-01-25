#pragma once

#include <cassert>
#include <cstddef>

#include "Array.hpp"

namespace dab {

template <typename T, size_t Cap>
class Queue {
  public:
  Queue() = default;

  void
  Clear();
  void
  Append(const T item);
  T
  Pop();
  size_t
  Size() const;
  bool
  Empty() const;

  T*
  begin();
  const T*
  begin() const;
  T*
  end();
  const T*
  end() const;

  private:
  Array<T, Cap> Data;
  size_t Begin = 0;
  size_t End = 0;
};

template <typename T, size_t Cap>
void
Queue<T, Cap>::Clear() {
  Begin = 0;
  End = 0;
}

template <typename T, size_t Cap>
void
Queue<T, Cap>::Append(const T item) {
  assert(End < Cap);
  Data.At(End++) = item;
}

template <typename T, size_t Cap>
T
Queue<T, Cap>::Pop() {
  assert(!Empty());
  return Data.At(Begin++);
}

template <typename T, size_t Cap>
size_t
Queue<T, Cap>::Size() const {
  return End - Begin;
}

template <typename T, size_t Cap>
bool
Queue<T, Cap>::Empty() const {
  return Begin == End;
}

template <typename T, size_t Cap>
T*
Queue<T, Cap>::begin() {
  return Data.begin() + Begin;
}

template <typename T, size_t Cap>
const T*
Queue<T, Cap>::begin() const {
  return Data.begin() + Begin;
}

template <typename T, size_t Cap>
T*
Queue<T, Cap>::end() {
  return Data.begin() + End;
}

template <typename T, size_t Cap>
const T*
Queue<T, Cap>::end() const {
  return Data.begin() + End;
}

}  // namespace dab
