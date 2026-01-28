#pragma once

#include <cassert>
#include <cstddef>

#include "Iterable.hpp"

namespace dab {

template <typename T>
class Span : public Iterable<Span<T>> {
  public:
  Span() = default;
  Span(T* begin, T* end);

  template <typename Other>
  operator Span<Other>() const;

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
template <typename Other>
Span<T>::
operator Span<Other>() const {
  return Span<Other>(const_cast<Other*>(reinterpret_cast<const Other*>(Begin())),
                     const_cast<Other*>(reinterpret_cast<const Other*>(End())));
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

template <typename T>
auto
Export(const T& arr) {
  return Span(arr.begin(), arr.end());
}

}  // namespace dab
