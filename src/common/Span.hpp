#pragma once

#include "Macro.hpp"

template <typename T, typename SizeType>
class Span {
  public:
  Span() = default;

  Span(const T* begin, const T* end) : Begin(begin), End(end) {
  }

  IterableObject(Begin, End, End - Begin);

  private:
  const T* Begin = nullptr;
  const T* End = nullptr;
};

template <typename T, typename SizeType>
auto
Export(const T& arr) {
  auto ele = arr.At(0);
  return Span<decltype(ele), SizeType>(arr.begin(), arr.end());
}
