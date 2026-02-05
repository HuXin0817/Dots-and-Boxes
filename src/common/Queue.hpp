#pragma once

#include <cassert>
#include <cstddef>

#include "Array.hpp"

namespace dab::detail::common {

template <typename T, uint32_t Cap>
class Queue : public Iterable<Queue<T, Cap>> {
  public:
  constexpr Queue() = default;
  constexpr Queue(const Queue& other) = default;
  constexpr Queue(Queue&& other) = default;
  constexpr Queue& operator=(const Queue& other) = default;
  constexpr Queue& operator=(Queue&& other) = default;

  constexpr void Clear();
  constexpr void Append(const T item);
  constexpr T Pop();

  constexpr uint32_t Size() const;
  constexpr T* begin();
  constexpr const T* begin() const;
  constexpr T* end();
  constexpr const T* end() const;

  private:
  Array<T, Cap> Data;
  uint32_t BeginIndex = 0;
  uint32_t EndIndex = 0;
};

template <typename T, uint32_t Cap>
constexpr void Queue<T, Cap>::Clear() {
  BeginIndex = 0;
  EndIndex = 0;
}

template <typename T, uint32_t Cap>
constexpr void Queue<T, Cap>::Append(const T item) {
  assert(EndIndex < Cap);
  Data[EndIndex++] = item;
}

template <typename T, uint32_t Cap>
constexpr T Queue<T, Cap>::Pop() {
  assert(Size() > 0);
  return Data[BeginIndex++];
}

template <typename T, uint32_t Cap>
constexpr uint32_t Queue<T, Cap>::Size() const {
  return EndIndex - BeginIndex;
}

template <typename T, uint32_t Cap>
constexpr T* Queue<T, Cap>::begin() {
  return Data.begin() + BeginIndex;
}

template <typename T, uint32_t Cap>
constexpr const T* Queue<T, Cap>::begin() const {
  return Data.begin() + BeginIndex;
}

template <typename T, uint32_t Cap>
constexpr T* Queue<T, Cap>::end() {
  return Data.begin() + EndIndex;
}

template <typename T, uint32_t Cap>
constexpr const T* Queue<T, Cap>::end() const {
  return Data.begin() + EndIndex;
}

}  // namespace dab::detail::common