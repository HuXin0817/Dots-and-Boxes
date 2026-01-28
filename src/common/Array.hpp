#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>

#include "IterableWapper.hpp"

namespace dab {

template <typename T, size_t Length>
class ArrayImpl : public Iterable<T> {
  public:
  ArrayImpl() = default;
  ArrayImpl(const ArrayImpl& other);

  void
  operator=(const ArrayImpl& other);

  size_t
  Size() const override;
  T*
  Begin() override;
  const T*
  Begin() const override;
  T*
  End() override;
  const T*
  End() const override;

  private:
  T Data[Length];
};

template <typename T, size_t Length>
ArrayImpl<T, Length>::ArrayImpl(const ArrayImpl& other) {
  std::copy(other.Data, other.Data + Length, Data);
}

template <typename T, size_t Length>
void
ArrayImpl<T, Length>::operator=(const ArrayImpl& other) {
  std::copy(other.Data, other.Data + Length, Data);
}

template <typename T, size_t Length>
size_t
ArrayImpl<T, Length>::Size() const {
  return Length;
}

template <typename T, size_t Length>
T*
ArrayImpl<T, Length>::Begin() {
  return Data;
}

template <typename T, size_t Length>
const T*
ArrayImpl<T, Length>::Begin() const {
  return Data;
}

template <typename T, size_t Length>
T*
ArrayImpl<T, Length>::End() {
  return Data + Length;
}

template <typename T, size_t Length>
const T*
ArrayImpl<T, Length>::End() const {
  return Data + Length;
}

template <typename T, size_t Length>
using Array = IterableWapper<ArrayImpl<T, Length>>;

}  // namespace dab
