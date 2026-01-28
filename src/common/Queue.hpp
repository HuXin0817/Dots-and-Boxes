#pragma once

#include <cassert>
#include <cstddef>

#include "Array.hpp"

namespace dab {

template <typename T, size_t Cap>
class Queue : public Iterable<Queue<T, Cap>> {
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
  T*
  Begin();
  const T*
  Begin() const;
  T*
  End();
  const T*
  End() const;

  private:
  Array<T, Cap> Data;
  size_t BeginIndex = 0;
  size_t EndIndex = 0;
};

template <typename T, size_t Cap>
void
Queue<T, Cap>::Clear() {
  BeginIndex = 0;
  EndIndex = 0;
}

template <typename T, size_t Cap>
void
Queue<T, Cap>::Append(const T item) {
  assert(endIdx < Cap);
  Data.At(EndIndex++) = item;
}

template <typename T, size_t Cap>
T
Queue<T, Cap>::Pop() {
  assert(Size() > 0);
  return Data.At(BeginIndex++);
}

template <typename T, size_t Cap>
size_t
Queue<T, Cap>::Size() const {
  return EndIndex - BeginIndex;
}

template <typename T, size_t Cap>
T*
Queue<T, Cap>::Begin() {
  return Data.Begin() + BeginIndex;
}

template <typename T, size_t Cap>
const T*
Queue<T, Cap>::Begin() const {
  return Data.Begin() + BeginIndex;
}

template <typename T, size_t Cap>
T*
Queue<T, Cap>::End() {
  return Data.Begin() + EndIndex;
}

template <typename T, size_t Cap>
const T*
Queue<T, Cap>::End() const {
  return Data.Begin() + EndIndex;
}

}  // namespace dab
