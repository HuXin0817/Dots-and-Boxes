#pragma once

#include <cstring>
#include <initializer_list>

#include "Macro.hpp"

namespace dab::common {

template <typename T, int Length>
class Array {
  public:
  Array() = default;

  Array&
  operator=(const Array& other) {
    std::memcpy(Data, other.Data, Length * sizeof(T));
    return *this;
  }

  Array&
  operator=(const std::initializer_list<T>& init) {
    std::memcpy(Data, init.begin(), Length * sizeof(T));
    return *this;
  }

  IterableObject(Data, Data + Length, Length);

  private:
  T Data[Length] = {};
};

}  // namespace dab::common
