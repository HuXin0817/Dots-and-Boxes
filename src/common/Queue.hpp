#pragma once

#include <cassert>

#include "Array.hpp"

namespace dab::common {

template <class T, int Cap>
class Queue {
  public:
  void
  Clear() {
    Begin = 0;
    End = 0;
  }

  void
  Append(const T item) {
    assert(End < Cap);
    Data.At(End++) = item;
  }

  T
  Pop() {
    assert(!Empty());
    return Data.At(Begin++);
  }

  IterableObject(Data.begin() + Begin, Data.begin() + End, End - Begin);

  private:
  Array<T, Cap> Data;
  int Begin = 0;
  int End = 0;
};

}  // namespace dab::common
