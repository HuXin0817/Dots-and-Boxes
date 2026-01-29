#pragma once

#include <cassert>
#include <cstddef>

#include "Array.hpp"

namespace dab::detail::common {

template <typename T, size_t Cap>
class Queue : public Iterable<Queue<T, Cap>> {
  public:
  constexpr Queue() = default;
  constexpr Queue(const Queue& other) = default;
  constexpr Queue(Queue&& other) = default;
  constexpr Queue&
  operator=(const Queue& other) = default;
  constexpr Queue&
  operator=(Queue&& other) = default;

  constexpr void
  Clear();
  constexpr void
  Append(const T item);
  constexpr T
  Pop();

  constexpr size_t
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
  size_t BeginIndex = 0;
  size_t EndIndex = 0;
};

template <typename T, size_t Cap>
constexpr void
Queue<T, Cap>::Clear() {
  BeginIndex = 0;
  EndIndex = 0;
}

template <typename T, size_t Cap>
constexpr void
Queue<T, Cap>::Append(const T item) {
  assert(EndIndex < Cap);
  Data[EndIndex++] = item;
}

template <typename T, size_t Cap>
constexpr T
Queue<T, Cap>::Pop() {
  assert(Size() > 0);
  return Data[BeginIndex++];
}

template <typename T, size_t Cap>
constexpr size_t
Queue<T, Cap>::Size() const {
  return EndIndex - BeginIndex;
}

template <typename T, size_t Cap>
constexpr T*
Queue<T, Cap>::Begin() {
  return Data.Begin() + BeginIndex;
}

template <typename T, size_t Cap>
constexpr const T*
Queue<T, Cap>::Begin() const {
  return Data.Begin() + BeginIndex;
}

template <typename T, size_t Cap>
constexpr T*
Queue<T, Cap>::End() {
  return Data.Begin() + EndIndex;
}

template <typename T, size_t Cap>
constexpr const T*
Queue<T, Cap>::End() const {
  return Data.Begin() + EndIndex;
}

}  // namespace dab::detail::common