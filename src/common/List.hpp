#pragma once

#include <cassert>
#include <cstddef>

#include "Array.hpp"

namespace dab {

template <typename T, size_t Cap>
class List {
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
  bool
  Empty() const;
  T&
  At(const size_t i);
  const T&
  At(const size_t i) const;

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
bool
List<T, Cap>::Empty() const {
  return Length == 0;
}

template <typename T, size_t Cap>
T&
List<T, Cap>::At(const size_t i) {
  assert(i < Length);
  return Data.At(i);
}

template <typename T, size_t Cap>
const T&
List<T, Cap>::At(const size_t i) const {
  assert(i < Length);
  return Data.At(i);
}

template <typename T, size_t Cap>
T*
List<T, Cap>::begin() {
  return Data.begin();
}

template <typename T, size_t Cap>
const T*
List<T, Cap>::begin() const {
  return Data.begin();
}

template <typename T, size_t Cap>
T*
List<T, Cap>::end() {
  return Data.begin() + Length;
}

template <typename T, size_t Cap>
const T*
List<T, Cap>::end() const {
  return Data.begin() + Length;
}

}  // namespace dab
