#pragma once

#include <cassert>
#include <cstddef>
#include <type_traits>

#include "Iterable.hpp"

namespace dab {

template <typename T>
class Span : public Iterable<Span<T>> {
  public:
  Span() = default;
  Span(T* begin, T* end);

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
  T* BeginPtr = nullptr;
  T* EndPtr = nullptr;
};

template <typename T>
Span<T>::Span(T* begin, T* end) : BeginPtr(begin), EndPtr(end) {
}

template <typename T>
size_t
Span<T>::Size() const {
  return EndPtr - BeginPtr;
}

template <typename T>
T*
Span<T>::Begin() {
  return BeginPtr;
}

template <typename T>
const T*
Span<T>::Begin() const {
  return BeginPtr;
}

template <typename T>
T*
Span<T>::End() {
  return EndPtr;
}

template <typename T>
const T*
Span<T>::End() const {
  return EndPtr;
}

template <typename T, typename Ele = std::remove_reference_t<decltype(T().At(0))>>
auto
Export(const T& arr) {
  return Span<const Ele>(arr.begin(), arr.end());
}

}  // namespace dab
