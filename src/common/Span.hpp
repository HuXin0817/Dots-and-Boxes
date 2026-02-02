#pragma once

#include <cassert>
#include <cstddef>

#include "Iterable.hpp"

namespace dab::detail::common {

template <typename T>
class Span : public Iterable<Span<T>> {
  public:
  constexpr Span() = default;
  constexpr Span(T* begin, T* end);

  template <typename Other>
  constexpr
  operator Span<Other>() const;

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
  T* BeginPtr = nullptr;
  T* EndPtr = nullptr;
};

template <typename T>
constexpr Span<T>::Span(T* begin, T* end) : BeginPtr(begin), EndPtr(end) {
}

template <typename T>
template <typename Other>
constexpr Span<T>::
operator Span<Other>() const {
  return Span<Other>(const_cast<Other*>(reinterpret_cast<const Other*>(Begin())),
                     const_cast<Other*>(reinterpret_cast<const Other*>(End())));
}

template <typename T>
constexpr size_t
Span<T>::Size() const {
  return EndPtr - BeginPtr;
}

template <typename T>
constexpr T*
Span<T>::Begin() {
  return BeginPtr;
}

template <typename T>
constexpr const T*
Span<T>::Begin() const {
  return BeginPtr;
}

template <typename T>
constexpr T*
Span<T>::End() {
  return EndPtr;
}

template <typename T>
constexpr const T*
Span<T>::End() const {
  return EndPtr;
}

}  // namespace dab::detail::common