#pragma once

#include "Macro.hpp"

template <typename T, size_t Length, typename SizeType>
class Array {
  public:
  Array() = default;

  Array&
  operator=(const Array& other) {
    std::memcpy(Data, other.Data, Length * sizeof(T));
    return *this;
  }

  IterableObject(Data, Data + Length, Length);

  private:
  T Data[Length] = {};
};
