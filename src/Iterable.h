#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <random>

#include "Common.h"

namespace dab {

namespace __detail__ {

namespace iterable {

static constexpr int EnableArray = 1 << 1;
static constexpr int EnableSpan = 1 << 2;
static constexpr int EnableAllocSize = 1 << 3;
static constexpr int EnableFrontPointer = 1 << 4;
static constexpr int EnableEndPointer = 1 << 5;

static constexpr bool HasFlag(int config, int flag) { return (config & flag) != 0; }

static constexpr bool CheckConfig(int config, Int arraySize) {
  bool result = true;
  if (arraySize > 0) {
    result &= HasFlag(config, EnableArray);
  } else {
    result &= arraySize == 0;
    result &= !HasFlag(config, EnableArray);
  }
  int n = 0;
  n += HasFlag(config, EnableArray);
  n += HasFlag(config, EnableSpan);
  n += HasFlag(config, EnableAllocSize);
  result &= n == 1;
  return result;
}

template <bool Bp, typename T>
using Mixin = std::conditional_t<Bp, T, std::type_identity<T>>;

template <typename T, Int Size>
struct ArrayMixin {
  static_assert(Size > 0);
  std::array<T, Size> Data;
};

struct FrontPointerMixin {
  Int FrontPos = 0;
};

struct EndPointerMixin {
  Int EndPos = 0;
};

template <typename T>
struct PtrMixin {
  std::unique_ptr<T[]> Data = nullptr;
  Int Length = 0;
};

template <typename T>
struct SpanMixin {
  T* Data = nullptr;
};

template <int Config, typename T, Int ArraySize = 0>
class Iterable : Mixin<HasFlag(Config, EnableArray), ArrayMixin<T, ArraySize>>,
                 Mixin<HasFlag(Config, EnableSpan), SpanMixin<T>>,
                 Mixin<HasFlag(Config, EnableAllocSize), PtrMixin<T>>,
                 Mixin<HasFlag(Config, EnableFrontPointer), FrontPointerMixin>,
                 Mixin<HasFlag(Config, EnableEndPointer), EndPointerMixin> {
  static constexpr bool HasFlag(int flag) { return (Config & flag) != 0; }
  static_assert(CheckConfig(Config, ArraySize));

 public:
  constexpr Iterable() = default;
  constexpr Iterable(Int size) { Reset(size); }
  constexpr Iterable(const T* data, Int size) { Reset(data, size); }
  constexpr Iterable(const T* begin, const T* end) { Reset(begin, end); }
  constexpr Iterable(const Iterable& other) { Reset(other.begin(), other.Size()); }
  constexpr Iterable& operator=(const Iterable& other) { return Reset(other.begin(), other.Size()); }

  template <typename Other>
  constexpr Iterable(const Other& other) {
    Reset(other.begin(), other.Size());
  }
  template <typename Other>
  constexpr Iterable& operator=(const Other& other) {
    return Reset(other.begin(), other.Size());
  }

  constexpr Iterable& Reset(Int size);
  constexpr Iterable& Reset(const T* data, Int size);
  constexpr Iterable& Reset(const T* begin, const T* end) { return Reset(begin, static_cast<Int>(end - begin)); }

  constexpr T* begin() { return &this->Data[FrontIndex()]; }
  constexpr const T* begin() const { return &this->Data[FrontIndex()]; }
  constexpr T* end() { return &this->Data[EndIndex()]; }
  constexpr const T* end() const { return &this->Data[EndIndex()]; }

  constexpr Int Cap() const;
  constexpr Int Size() const { return EndIndex() - FrontIndex(); }
  constexpr bool Empty() const { return EndIndex() == FrontIndex(); }
  constexpr T& Front() { return this->Data[FrontIndex()]; }
  constexpr const T& Front() const { return this->Data[FrontIndex()]; }
  constexpr T& Back() { return this->Data[EndIndex() - 1]; }
  constexpr const T& Back() const { return this->Data[EndIndex() - 1]; }
  constexpr void Append(T ele) { this->Data[this->EndPos++] = ele; }
  constexpr T Pop() { return this->Data[this->FrontPos++]; }
  constexpr T& At(Int i) { return this->Data[CheckIndex(i)]; }
  constexpr const T& At(Int i) const { return this->Data[CheckIndex(i)]; }
  constexpr void Clear();
  constexpr void ClearAndSet(T ele);

 private:
  constexpr Int FrontIndex() const;
  constexpr Int EndIndex() const;
  constexpr Int CheckIndex(Int i) const;
};

template <int Config, typename T, Int ArraySize>
constexpr Iterable<Config, T, ArraySize>& Iterable<Config, T, ArraySize>::Reset(Int size) {
  if constexpr (HasFlag(EnableAllocSize)) {
    this->Length = size;
    this->Data = std::make_unique<T[]>(size);
  }
  if constexpr (HasFlag(EnableFrontPointer)) {
    this->FrontPos = 0;
  }
  if constexpr (HasFlag(EnableEndPointer)) {
    this->EndPos = size;
  }
  return *this;
}

template <int Config, typename T, Int ArraySize>
constexpr Iterable<Config, T, ArraySize>& Iterable<Config, T, ArraySize>::Reset(const T* data, Int size) {
  Reset(size);
  if constexpr (HasFlag(EnableArray) || HasFlag(EnableAllocSize)) {
    std::ranges::copy(data, data + size, begin());
  } else {
    this->Data = data;
  }
  return *this;
}

template <int Config, typename T, Int ArraySize>
constexpr Int Iterable<Config, T, ArraySize>::Cap() const {
  if constexpr (HasFlag(EnableArray)) {
    return ArraySize;
  } else if constexpr (HasFlag(EnableAllocSize)) {
    return this->Length;
  } else {
    static_assert(false);
  }
  return 0;
}

template <int Config, typename T, Int ArraySize>
constexpr void Iterable<Config, T, ArraySize>::Clear() {
  if constexpr (HasFlag(EnableFrontPointer)) {
    this->FrontPos = 0;
  }
  if constexpr (HasFlag(EnableEndPointer)) {
    this->EndPos = 0;
  }
}

template <int Config, typename T, Int ArraySize>
constexpr void Iterable<Config, T, ArraySize>::ClearAndSet(T ele) {
  this->Data[0] = ele;
  if constexpr (HasFlag(EnableFrontPointer)) {
    this->FrontPos = 0;
  }
  if constexpr (HasFlag(EnableEndPointer)) {
    this->EndPos = 1;
  }
}

template <int Config, typename T, Int ArraySize>
constexpr Int Iterable<Config, T, ArraySize>::FrontIndex() const {
  if constexpr (HasFlag(EnableFrontPointer)) {
    return this->FrontPos;
  } else {
    return 0;
  }
}

template <int Config, typename T, Int ArraySize>
constexpr Int Iterable<Config, T, ArraySize>::EndIndex() const {
  if constexpr (HasFlag(EnableEndPointer)) {
    if constexpr (!HasFlag(EnableSpan)) {
      assert(this->EndPos <= Cap());
    }
    return this->EndPos;
  } else {
    return Cap();
  }
}

template <int Config, typename T, Int ArraySize>
constexpr Int Iterable<Config, T, ArraySize>::CheckIndex(Int i) const {
  assert(i >= 0 && i < Size());
  return i;
}

}  // namespace iterable

template <typename T, Int Size>
using Array = iterable::Iterable<iterable::EnableArray, T, Size>;
template <typename T, Int Size>
using List = iterable::Iterable<iterable::EnableArray | iterable::EnableEndPointer, T, Size>;
template <typename T, Int Size>
using Queue =
    iterable::Iterable<iterable::EnableArray | iterable::EnableFrontPointer | iterable::EnableEndPointer, T, Size>;
template <typename T>
using Span = iterable::Iterable<iterable::EnableSpan | iterable::EnableEndPointer, T>;
template <typename T>
using Vector = iterable::Iterable<iterable::EnableAllocSize, T>;

}  // namespace __detail__

class Random {
 public:
  Random() { Rng.seed(std::chrono::steady_clock::now().time_since_epoch().count()); }

  template <typename T>
  T Range(T min, T max);
  template <typename Iterable>
  const auto& Choice(const Iterable& data);

 private:
  std::mt19937_64 Rng;
};

template <typename T>
T Random::Range(T min, T max) {
  std::uniform_int_distribution<T> dist(min, max);
  return dist(Rng);
}

template <typename Iterable>
const auto& Random::Choice(const Iterable& data) {
  assert(!data.Empty());
  return data.At(Range(0, data.Size() - 1));
}

}  // namespace dab