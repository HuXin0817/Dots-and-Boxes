#pragma once

#include "Array.hpp"

template <typename T, size_t Cap>
class List {
  public:
  List() = default;

  void
  Reset(const T item) {
    Data.At(0) = item;
    Length = 1;
  }

  void
  Clear() {
    Length = 0;
  }

  void
  Append(const T item) {
    assert(Length < Cap);
    Data.At(Length++) = item;
  }

  size_t
  Size() const {
    return Length;
  }
  bool
  Empty() const {
    return Length == 0;
  }

  T&
  At(size_t i) {
    assert(i < Length);
    return Data.At(i);
  }
  const T&
  At(size_t i) const {
    assert(i < Length);
    return Data.At(i);
  }

  T*
  begin() {
    return Data.begin();
  }
  const T*
  begin() const {
    return Data.begin();
  }

  T*
  end() {
    return Data.begin() + Length;
  }
  const T*
  end() const {
    return Data.begin() + Length;
  }

  private:
  Array<T, Cap> Data;
  size_t Length = 0;
};
