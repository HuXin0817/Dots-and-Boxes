#pragma once

#include <cassert>

#include "Array.hpp"

template <class T, int Cap>
class Queue {
  public:
  void
  Clear() {
    Begin = 0;
    End = 0;
  }

  void
  Append(T item) {
    assert(End < Cap);
    Data.At(End++) = item;
  }

  T
  Pop() {
    assert(!Empty());
    return Data.At(Begin++);
  }

  I(Data.begin() + Begin, Data.begin() + End, End - Begin);

  private:
  Array<T, Cap> Data;
  int Begin = 0;
  int End = 0;
};
