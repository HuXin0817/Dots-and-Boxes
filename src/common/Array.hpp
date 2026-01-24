#pragma once

#include <cassert>
#include <cstddef>
#include <cstring>

template <typename T, size_t Length>
class Array {
  public:
  Array() = default;

  void
  operator=(const Array& other);

  T&
  At(size_t i);
  const T&
  At(size_t i) const;

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
void
Array<T, Length>::operator=(const Array& other) {
  memcpy(Data, other.Data, Length * sizeof(T));
}

template <typename T, size_t Length>
T&
Array<T, Length>::At(size_t i) {
  assert(i < Length);
  return Data[i];
}

template <typename T, size_t Length>
const T&
Array<T, Length>::At(size_t i) const {
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
