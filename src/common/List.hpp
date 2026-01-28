#pragma once

#include <cassert>
#include <cstddef>

#include "Array.hpp"

namespace dab {

template <typename T, size_t Cap>
class List : public Iterable<List<T, Cap>> {
  public:
  List() = default;

  void
  ClearAndSet(const T item);
  void
  Clear();
  void
  Append(const T item);

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
  size_t Length = 0;
};

template <typename T, size_t Cap>
void
List<T, Cap>::ClearAndSet(const T item) {
  Data.At(0) = item;
  Length = 1;
}

template <typename T, size_t Cap>
void
List<T, Cap>::Clear() {
  Length = 0;
}

template <typename T, size_t Cap>
void
List<T, Cap>::Append(const T item) {
  assert(Length < Cap);
  Data.At(Length++) = item;
}

template <typename T, size_t Cap>
size_t
List<T, Cap>::Size() const {
  return Length;
}

template <typename T, size_t Cap>
T*
List<T, Cap>::Begin() {
  return Data.Begin();
}

template <typename T, size_t Cap>
const T*
List<T, Cap>::Begin() const {
  return Data.Begin();
}

template <typename T, size_t Cap>
T*
List<T, Cap>::End() {
  return Data.Begin() + Length;
}

template <typename T, size_t Cap>
const T*
List<T, Cap>::End() const {
  return Data.Begin() + Length;
}

}  // namespace dab
