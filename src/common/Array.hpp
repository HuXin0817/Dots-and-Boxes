#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>

#include "Iterable.hpp"

namespace dab {

template <typename T, size_t Length>
class Array : public Iterable<Array<T, Length>> {
  public:
  Array() = default;
  Array(const Array& other);

  void
  operator=(const Array& other);

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
  T Data[Length];
};

template <typename T, size_t Length>
Array<T, Length>::Array(const Array& other) {
  std::copy(other.Data, other.Data + Length, Data);
}

template <typename T, size_t Length>
void
Array<T, Length>::operator=(const Array& other) {
  std::copy(other.Data, other.Data + Length, Data);
}

template <typename T, size_t Length>
size_t
Array<T, Length>::Size() const {
  return Length;
}

template <typename T, size_t Length>
T*
Array<T, Length>::Begin() {
  return Data;
}

template <typename T, size_t Length>
const T*
Array<T, Length>::Begin() const {
  return Data;
}

template <typename T, size_t Length>
T*
Array<T, Length>::End() {
  return Data + Length;
}

template <typename T, size_t Length>
const T*
Array<T, Length>::End() const {
  return Data + Length;
}

}  // namespace dab
