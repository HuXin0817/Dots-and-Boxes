#pragma once

#include "Array.hpp"

namespace dab::common {

template <typename T, size_t Cap, typename SizeType>
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
  Array<T, Cap, SizeType> Data;
  SizeType Begin = 0;
  SizeType End = 0;
};

}  // namespace dab::common
