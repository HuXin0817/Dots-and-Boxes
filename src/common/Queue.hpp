#pragma once

#include "Array.hpp"

template <typename T, size_t Cap>
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
  size_t Begin = 0;
  size_t End = 0;
};
