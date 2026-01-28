#pragma once

#include <cassert>
#include <cstddef>

#include "Array.hpp"
#include "IterableWapper.hpp"

namespace dab {

template <typename T, size_t Cap>
class QueueImpl : public Iterable<T> {
  public:
  QueueImpl() = default;

  void
  Clear();
  void
  Append(const T item);
  T
  Pop();

  size_t
  Size() const override;
  T*
  Begin() override;
  const T*
  Begin() const override;
  T*
  End() override;
  const T*
  End() const override;

  private:
  Array<T, Cap> Data;
  size_t beginIdx = 0;
  size_t endIdx = 0;
};

template <typename T, size_t Cap>
void
QueueImpl<T, Cap>::Clear() {
  beginIdx = 0;
  endIdx = 0;
}

template <typename T, size_t Cap>
void
QueueImpl<T, Cap>::Append(const T item) {
  assert(endIdx < Cap);
  Data.At(endIdx++) = item;
}

template <typename T, size_t Cap>
T
QueueImpl<T, Cap>::Pop() {
  assert(Size() > 0);
  return Data.At(beginIdx++);
}

template <typename T, size_t Cap>
size_t
QueueImpl<T, Cap>::Size() const {
  return endIdx - beginIdx;
}

template <typename T, size_t Cap>
T*
QueueImpl<T, Cap>::Begin() {
  return Data.Begin() + beginIdx;
}

template <typename T, size_t Cap>
const T*
QueueImpl<T, Cap>::Begin() const {
  return Data.Begin() + beginIdx;
}

template <typename T, size_t Cap>
T*
QueueImpl<T, Cap>::End() {
  return Data.Begin() + endIdx;
}

template <typename T, size_t Cap>
const T*
QueueImpl<T, Cap>::End() const {
  return Data.Begin() + endIdx;
}

template <typename T, size_t Cap>
using Queue = IterableWapper<QueueImpl<T, Cap>>;

}  // namespace dab
