#pragma once

#include <cassert>
#include <cstddef>

#include "Array.hpp"

namespace dab::detail::common {

template <typename T, uint32_t Cap>
class List : public Iterable<List<T, Cap>> {
  public:
  constexpr List() = default;
  constexpr List(const List& other) = default;
  constexpr List(List&& other) = default;
  constexpr List&
  operator=(const List& other) = default;
  constexpr List&
  operator=(List&& other) = default;

  constexpr void
  ClearAndSet(const T item);
  constexpr void
  Clear();
  constexpr void
  Append(const T item);

  constexpr uint32_t
  Size() const;
  constexpr T*
  Begin();
  constexpr const T*
  Begin() const;
  constexpr T*
  End();
  constexpr const T*
  End() const;

  private:
  Array<T, Cap> Data;
  uint32_t Length = 0;
};

template <typename T, uint32_t Cap>
constexpr void
List<T, Cap>::ClearAndSet(const T item) {
  Data[0] = item;
  Length = 1;
}

template <typename T, uint32_t Cap>
constexpr void
List<T, Cap>::Clear() {
  Length = 0;
}

template <typename T, uint32_t Cap>
constexpr void
List<T, Cap>::Append(const T item) {
  assert(Length < Cap);
  Data[Length++] = item;
}

template <typename T, uint32_t Cap>
constexpr uint32_t
List<T, Cap>::Size() const {
  return Length;
}

template <typename T, uint32_t Cap>
constexpr T*
List<T, Cap>::Begin() {
  return Data.Begin();
}

template <typename T, uint32_t Cap>
constexpr const T*
List<T, Cap>::Begin() const {
  return Data.Begin();
}

template <typename T, uint32_t Cap>
constexpr T*
List<T, Cap>::End() {
  return Data.Begin() + Length;
}

template <typename T, uint32_t Cap>
constexpr const T*
List<T, Cap>::End() const {
  return Data.Begin() + Length;
}

}  // namespace dab::detail::common