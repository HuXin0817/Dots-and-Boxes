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
  constexpr List& operator=(const List& other) = default;
  constexpr List& operator=(List&& other) = default;

  constexpr void ClearAndSet(const T item);
  constexpr void Clear();
  constexpr void Append(const T item);

  constexpr uint32_t Size() const;
  constexpr T* begin();
  constexpr const T* begin() const;
  constexpr T* end();
  constexpr const T* end() const;

  private:
  Array<T, Cap> Data;
  uint32_t Length = 0;
};

template <typename T, uint32_t Cap>
constexpr void List<T, Cap>::ClearAndSet(const T item) {
  Data[0] = item;
  Length = 1;
}

template <typename T, uint32_t Cap>
constexpr void List<T, Cap>::Clear() {
  Length = 0;
}

template <typename T, uint32_t Cap>
constexpr void List<T, Cap>::Append(const T item) {
  assert(Length < Cap);
  Data[Length++] = item;
}

template <typename T, uint32_t Cap>
constexpr uint32_t List<T, Cap>::Size() const {
  return Length;
}

template <typename T, uint32_t Cap>
constexpr T* List<T, Cap>::begin() {
  return Data.begin();
}

template <typename T, uint32_t Cap>
constexpr const T* List<T, Cap>::begin() const {
  return Data.begin();
}

template <typename T, uint32_t Cap>
constexpr T* List<T, Cap>::end() {
  return Data.begin() + Length;
}

template <typename T, uint32_t Cap>
constexpr const T* List<T, Cap>::end() const {
  return Data.begin() + Length;
}

}  // namespace dab::detail::common