#pragma once

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

  constexpr uint32_t
  Size() const;
  constexpr T*
  begin();
  constexpr const T*
  begin() const;
  constexpr T*
  end();
  constexpr const T*
  end() const;

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
  return Span<Other>(const_cast<Other*>(reinterpret_cast<const Other*>(begin())),
                     const_cast<Other*>(reinterpret_cast<const Other*>(end())));
}

template <typename T>
constexpr uint32_t
Span<T>::Size() const {
  return EndPtr - BeginPtr;
}

template <typename T>
constexpr T*
Span<T>::begin() {
  return BeginPtr;
}

template <typename T>
constexpr const T*
Span<T>::begin() const {
  return BeginPtr;
}

template <typename T>
constexpr T*
Span<T>::end() {
  return EndPtr;
}

template <typename T>
constexpr const T*
Span<T>::end() const {
  return EndPtr;
}

}  // namespace dab::detail::common