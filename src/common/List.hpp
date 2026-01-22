#pragma once

#include "Array.hpp"

template <typename T, size_t Cap, typename SizeType>
class List {
  public:
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

  IterableObject(Data.begin(), Data.begin() + Length, Length);

  private:
  Array<T, Cap, SizeType> Data;
  SizeType Length = 0;
};
