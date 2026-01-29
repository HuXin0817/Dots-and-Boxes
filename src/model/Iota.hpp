#pragma once

#include "Types.hpp"

namespace dab::detail::model {

template <typename T>
class IotaViewIterator {
  public:
  constexpr IotaViewIterator() = default;
  constexpr IotaViewIterator(T v);
  constexpr void
  operator++();
  constexpr T
  operator*() const;
  constexpr bool
  operator!=(IotaViewIterator o) const;

  private:
  T v;
};

template <typename T>
constexpr IotaViewIterator<T>::IotaViewIterator(T v) : v(v) {
}

template <typename T>
constexpr void
IotaViewIterator<T>::operator++() {
  ++v;
}

template <typename T>
constexpr T
IotaViewIterator<T>::operator*() const {
  return v;
}

template <typename T>
constexpr bool
IotaViewIterator<T>::operator!=(IotaViewIterator<T> o) const {
  return v != o.v;
}

template <typename T>
class Iota {
  public:
  constexpr Iota() = default;
  constexpr IotaViewIterator<T>
  begin();
  constexpr IotaViewIterator<T>
  end();
};

template <typename T>
constexpr IotaViewIterator<T>
Iota<T>::begin() {
  return IotaViewIterator<T>(0);
}

template <typename T>
constexpr IotaViewIterator<T>
Iota<T>::end() {
  return IotaViewIterator<T>(Limits<T>::Max);
}

}  // namespace dab::detail::model