#pragma once

#include "Macro.hpp"

template <class T>
class Span {
  public:
  Span() = default;

  Span(const T* begin, const T* end) : Begin(begin), End(end) {
  }

  I(Begin, End, End - Begin);

  private:
  const T* Begin = nullptr;
  const T* End = nullptr;
};

template <typename T>
auto
Export(const T& arr) {
  return Span(arr.begin(), arr.end());
}
