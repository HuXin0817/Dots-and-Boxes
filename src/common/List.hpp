#pragma once

#include <cassert>

#include "Array.hpp"

namespace dab::common {

template <typename T, int Cap>
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
  Array<T, Cap> Data;
  int Length = 0;
};

}  // namespace dab::common
