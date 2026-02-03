#pragma once

#include "Iterable.hpp"

namespace dab::detail::common {

template <typename T>
class Vector : public Iterable<Vector<T>> {
  public:
  constexpr Vector(T* begin, T* end);
  constexpr Vector(const Vector& other);
  constexpr Vector&
  operator=(const Vector& other);
  constexpr uint32_t
  Size() const;
  constexpr T*
  Begin();
  constexpr const T*
  Begin() const;
  constexpr T*
  End();
  constexpr const T*
  End() const;
  ~Vector();

  private:
  uint32_t Length;
  T* Data;
};

template <typename T>
constexpr Vector<T>::Vector(T* begin, T* end) : Length(end - begin), Data(new T[Length]) {
  for (uint32_t i = 0; i < Length; ++i) {
    Data[i] = begin[i];
  }
}

template <typename T>
constexpr Vector<T>::Vector(const Vector& other) : Length(other.Length), Data(new T[other.Length]) {
  for (uint32_t i = 0; i < Length; ++i) {
    Data[i] = other.Data[i];
  }
}

template <typename T>
constexpr Vector<T>&
Vector<T>::operator=(const Vector& other) {
  if (this != &other) {
    delete[] Data;
    Length = other.Length;
    Data = new T[Length];
    for (uint32_t i = 0; i < Length; ++i) {
      Data[i] = other.Data[i];
    }
  }
  return *this;
}

template <typename T>
constexpr uint32_t
Vector<T>::Size() const {
  return Length;
}

template <typename T>
constexpr T*
Vector<T>::Begin() {
  return Data;
}

template <typename T>
constexpr const T*
Vector<T>::Begin() const {
  return Data;
}

template <typename T>
constexpr T*
Vector<T>::End() {
  return Data + Length;
}

template <typename T>
constexpr const T*
Vector<T>::End() const {
  return Data + Length;
}

template <typename T>
Vector<T>::~Vector() {
  delete[] Data;
}

}  // namespace dab::detail::common