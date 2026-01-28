#pragma once

#include <cassert>
#include <cstddef>
#include <type_traits>

#include "IterableWapper.hpp"

namespace dab {

template <typename T>
class SpanImpl : public Iterable<T> {
  public:
  SpanImpl() = default;
  SpanImpl(T* begin, T* end);

  size_t
  Size() const override;
  T*
  Begin() override;
  const T*
  Begin() const override;
  T*
  End() override;
  const T*
  End() const override;

  private:
  T* BeginPtr = nullptr;
  T* EndPtr = nullptr;
};

template <typename T>
SpanImpl<T>::SpanImpl(T* begin, T* end) : BeginPtr(begin), EndPtr(end) {
}

template <typename T>
size_t
SpanImpl<T>::Size() const {
  return EndPtr - BeginPtr;
}

template <typename T>
T*
SpanImpl<T>::Begin() {
  return BeginPtr;
}

template <typename T>
const T*
SpanImpl<T>::Begin() const {
  return BeginPtr;
}

template <typename T>
T*
SpanImpl<T>::End() {
  return EndPtr;
}

template <typename T>
const T*
SpanImpl<T>::End() const {
  return EndPtr;
}

template <typename T>
using Span = IterableWapper<SpanImpl<T>>;

template <typename T, typename Ele = std::remove_reference_t<decltype(T().At(0))>>
auto
Export(const T& arr) {
  return Span<const Ele>(arr.begin(), arr.end());
}

}  // namespace dab
