#pragma once

#include <cassert>
#include <cstddef>

#include "Array.hpp"
#include "IterableWapper.hpp"

namespace dab {

template <typename T, size_t Cap>
class ListImpl : public Iterable<T> {
  public:
  ListImpl() = default;

  void
  ClearAndSet(const T item);
  void
  Clear();
  void
  Append(const T item);

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
  size_t Length = 0;
};

template <typename T, size_t Cap>
void
ListImpl<T, Cap>::ClearAndSet(const T item) {
  Data.At(0) = item;
  Length = 1;
}

template <typename T, size_t Cap>
void
ListImpl<T, Cap>::Clear() {
  Length = 0;
}

template <typename T, size_t Cap>
void
ListImpl<T, Cap>::Append(const T item) {
  assert(Length < Cap);
  Data.At(Length++) = item;
}

template <typename T, size_t Cap>
size_t
ListImpl<T, Cap>::Size() const {
  return Length;
}

template <typename T, size_t Cap>
T*
ListImpl<T, Cap>::Begin() {
  return Data.Begin();
}

template <typename T, size_t Cap>
const T*
ListImpl<T, Cap>::Begin() const {
  return Data.Begin();
}

template <typename T, size_t Cap>
T*
ListImpl<T, Cap>::End() {
  return Data.Begin() + Length;
}

template <typename T, size_t Cap>
const T*
ListImpl<T, Cap>::End() const {
  return Data.Begin() + Length;
}

template <typename T, size_t Cap>
using List = IterableWapper<ListImpl<T, Cap>>;

}  // namespace dab
