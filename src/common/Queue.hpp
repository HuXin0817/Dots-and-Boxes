#pragma once

#include "Array.hpp"

template <typename T, size_t Cap>
class Queue {
  public:
  Queue() = default;

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

  size_t
  Size() const {
    return End - Begin;
  }
  bool
  Empty() const {
    return Begin == End;
  }

  T*
  begin() {
    return Data.begin() + Begin;
  }
  const T*
  begin() const {
    return Data.begin() + Begin;
  }

  T*
  end() {
    return Data.begin() + End;
  }
  const T*
  end() const {
    return Data.begin() + End;
  }

  private:
  Array<T, Cap> Data;
  size_t Begin = 0;
  size_t End = 0;
};
