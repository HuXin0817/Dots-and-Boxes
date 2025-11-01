#pragma once

#include <cassert>

#include "Array.hpp"

template <class T, int Cap>
class List {
  public:
  void
  Reset(T item) {
    Data.At(0) = item;
    Length = 1;
  }

  void
  Clear() {
    Length = 0;
  }

  void
  Append(T item) {
    assert(Length < Cap);
    Data.At(Length++) = item;
  }

  I(Data.begin(), Data.begin() + Length, Length);

  private:
  Array<T, Cap> Data;
  int Length = 0;
};
