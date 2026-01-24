#pragma once

#include <cstddef>

template <typename T>
class Span {
  public:
  Span() = default;

  Span(const T* begin, const T* end);

  size_t
  Size() const;
  bool
  Empty() const;

  const T&
  At(size_t i) const;

  const T*
  begin() const;

  const T*
  end() const;

  private:
  const T* Begin = nullptr;
  const T* End = nullptr;
};

template <typename T>
Span<T>::Span(const T* begin, const T* end) : Begin(begin), End(end) {
}

template <typename T>
size_t
Span<T>::Size() const {
  return End - Begin;
}

template <typename T>
bool
Span<T>::Empty() const {
  return Begin == End;
}

template <typename T>
const T&
Span<T>::At(size_t i) const {
  return Begin[i];
}

template <typename T>
const T*
Span<T>::begin() const {
  return Begin;
}

template <typename T>
const T*
Span<T>::end() const {
  return End;
}

template <typename T>
auto
Export(const T& arr) {
  return Span(arr.begin(), arr.end());
}
