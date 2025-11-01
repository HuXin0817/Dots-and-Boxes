#pragma once

#include <cassert>

#include "Array.h"
#include "Span.h"

template <class T, int Cap>
class List {
  public:
  void
  Reset(T item) {
    Data.At(0) = item;
    Len = 1;
  }

  void
  Clear() {
    Len = 0;
  }

  bool
  Empty() const {
    return Len == 0;
  }

  void
  Append(T item) {
    assert(Len < Cap);
    Data.At(Len++) = item;
  }

  Span<T>
  Export() const {
    return {Data.begin(), Data.begin() + Len};
  }

  int
  Size() const {
    return Len;
  }

  T*
  begin() {
    return Data.begin();
  }

  T*
  end() {
    return Data.begin() + Len;
  }

  const T*
  begin() const {
    return Data.begin();
  }

  const T*
  end() const {
    return Data.begin() + Len;
  }

  private:
  Array<T, Cap> Data;
  int Len = 0;
};
