#pragma once

#include <cassert>
#include <cstddef>

#include "Iterable.hpp"

namespace dab {

template <typename T, size_t Length>
class Array : public Iterable<Array<T, Length>> {
  public:
  constexpr Array() = default;
  constexpr Array(const Array& other) = default;
  constexpr Array(Array&& other) = default;
  constexpr Array&
  operator=(const Array& other) = default;
  constexpr Array&
  operator=(Array&& other) = default;

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
  T Data[Length];
};

template <typename T, size_t Length>
constexpr size_t
Array<T, Length>::Size() const {
  return Length;
}

template <typename T, size_t Length>
constexpr T*
Array<T, Length>::Begin() {
  return Data;
}

template <typename T, size_t Length>
constexpr const T*
Array<T, Length>::Begin() const {
  return Data;
}

template <typename T, size_t Length>
constexpr T*
Array<T, Length>::End() {
  return Data + Length;
}

template <typename T, size_t Length>
constexpr const T*
Array<T, Length>::End() const {
  return Data + Length;
}

}  // namespace dab
