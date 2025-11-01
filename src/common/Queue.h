#pragma once

#include <cassert>

#include "Array.h"
#include "Span.h"

template <class T, int Cap>
class Queue {
  public:
  void
  Clear() {
    Begin = 0;
    End = 0;
  }

  bool
  Empty() const {
    return Begin == End;
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

  Span<T>
  Export() const {
    return {Data.begin() + Begin, Data.begin() + End};
  }

  private:
  Array<T, Cap> Data;
  int Begin = 0;
  int End = 0;
};
