#pragma once

#include <cstring>
#include <initializer_list>

#include "Macro.hpp"

template <class T, int Length>
class Array {
  public:
  Array() = default;

  Array&
  operator=(const Array& other) {
    std::memcpy(Data, other.Data, Length * sizeof(T));
    return *this;
  }

  Array&
  operator=(std::initializer_list<T> init) {
    std::memcpy(Data, init.begin(), Length * sizeof(T));
    return *this;
  }

  I(Data, Data + Length, Length);

  private:
  T Data[Length] = {};
};
