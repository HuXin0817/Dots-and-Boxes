#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>

namespace dab {

template <typename T, size_t Length>
class Array {
  public:
  Array() = default;
  Array(const Array& other);

  void
  operator=(const Array& other);

  T&
  At(const size_t i);
  const T&
  At(const size_t i) const;

  T*
  begin();
  const T*
  begin() const;
  T*
  end();
  const T*
  end() const;

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
T&
Array<T, Length>::At(const size_t i) {
  assert(i < Length);
  return Data[i];
}

template <typename T, size_t Length>
const T&
Array<T, Length>::At(const size_t i) const {
  assert(i < Length);
  return Data[i];
}

template <typename T, size_t Length>
T*
Array<T, Length>::begin() {
  return Data;
}

template <typename T, size_t Length>
const T*
Array<T, Length>::begin() const {
  return Data;
}

template <typename T, size_t Length>
T*
Array<T, Length>::end() {
  return Data + Length;
}

template <typename T, size_t Length>
const T*
Array<T, Length>::end() const {
  return Data + Length;
}

}  // namespace dab
