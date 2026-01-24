#pragma once

#include <cassert>
#include <cstddef>

template <typename T, size_t Length>
class Array {
  public:
  Array() = default;

  void
  operator=(const Array& other) {
    memcpy(Data, other.Data, Length * sizeof(T));
  }

  T&
  At(size_t i) {
    assert(i < Length);
    return Data[i];
  }
  const T&
  At(size_t i) const {
    assert(i < Length);
    return Data[i];
  }

  T*
  begin() {
    return Data;
  }
  const T*
  begin() const {
    return Data;
  }

  const T*
  end() const {
    return Data + Length;
  }
  T*
  end() {
    return Data + Length;
  }

  private:
  T Data[Length];
};
