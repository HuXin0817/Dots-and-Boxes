#pragma once

#include "Macro.hpp"

template <typename T, size_t Length>
class Array {
  public:
  Array() = default;

  void
  operator=(const Array& other) {
    memcpy(Data, other.Data, Length * sizeof(T));
  }

  IterableObject(Data, Data + Length, Length);

  private:
  T Data[Length] = {};
};
