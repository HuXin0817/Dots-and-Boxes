#pragma once

#include <initializer_list>

template <class T, int Size>
class Array {
  public:
  Array() = default;

  T&
  At(int i) {
    assert(i >= 0 && i < Size);
    return Data[i];
  }

  const T&
  At(int i) const {
    assert(i >= 0 && i < Size);
    return Data[i];
  }

  Array&
  operator=(const Array& other) {
    if (this != &other) {
      std::memcpy(Data, other.Data, Size * sizeof(T));
    }
    return *this;
  }

  Array&
  operator=(std::initializer_list<T> init) {
    int i = 0;
    for (const auto& item : init) {
      if (i < Size) {
        Data[i++] = item;
      }
    }
    return *this;
  }

  T*
  begin() {
    return Data;
  }

  T*
  end() {
    return Data + Size;
  }

  const T*
  begin() const {
    return Data;
  }

  const T*
  end() const {
    return Data + Size;
  }

  private:
  T Data[Size];
};
