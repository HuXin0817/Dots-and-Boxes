#pragma once

#include <cstddef>

template <typename T>
class Span {
  public:
  Span() = default;

  Span(const T* begin, const T* end) : Begin(begin), End(end) {
  }

  size_t
  Size() const {
    return End - Begin;
  }
  bool
  Empty() const {
    return Begin == End;
  }

  const T&
  At(size_t i) const {
    return Begin[i];
  }

  const T*
  begin() const {
    return Begin;
  }

  const T*
  end() const {
    return End;
  }

  private:
  const T* Begin = nullptr;
  const T* End = nullptr;
};

template <typename T>
auto
Export(const T& arr) {
  return Span(arr.begin(), arr.end());
}
