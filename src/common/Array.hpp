#pragma once

#include <cstring>
#include <initializer_list>

#include "Config.hpp"

template <class T, int Length>
class Array {
  public:
  Array() = default;

  Array&
  operator=(const Array& other) {
    if (this != &other) {
      std::memcpy(Data, other.Data, Length * sizeof(T));
    }
    return *this;
  }

  Array&
  operator=(std::initializer_list<T> init) {
    int i = 0;
    for (const auto& item : init) {
      Data[i++] = item;
    }
    return *this;
  }

  I(Data, Data + Length, Length);

  private:
  T Data[Length] = {};
};
