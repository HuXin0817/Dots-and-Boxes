/*
Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2025 Xin Hu <huxin0817.hx@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <random>

#include "BoardSize.h"
#include "Tools.h"

namespace dab {

namespace __detail__::iterable {

namespace config {

static constexpr int EnableArray = 1 << 1;
static constexpr int EnableSpan = 1 << 2;
static constexpr int EnableAllocSize = 1 << 3;
static constexpr int EnableFrontPointer = 1 << 4;
static constexpr int EnableEndPointer = 1 << 5;

static constexpr bool HasFlag(int config, int flag) { return (config & flag) != 0; }

static constexpr bool CheckConfig(int config, Int arraySize) {
  int ans = true;
  if (arraySize > 0) {
    ans &= HasFlag(config, EnableArray);
  } else {
    ans &= arraySize == 0;
    ans &= !HasFlag(config, EnableArray);
  }
  int n = 0;
  n += HasFlag(config, EnableArray);
  n += HasFlag(config, EnableSpan);
  n += HasFlag(config, EnableAllocSize);
  ans &= n == 1;
  return ans;
}

}  // namespace config

using namespace config;

template <typename T, Int Size>
struct ArrayMixin {
  static_assert(Size > 0_bs);
  std::array<T, Size> Data;
};

struct FrontPointerMixin {
  Int FrontPos = 0_bs;
};

struct EndPointerMixin {
  Int EndPos = 0_bs;
};

template <typename T>
struct PtrMixin {
  std::unique_ptr<T[]> Data = nullptr;
  Int Length = 0_bs;
};

template <typename T>
struct SpanMixin {
  T* Data = nullptr;
};

template <int Config, typename T, Int ArraySize = 0_bs>
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
  constexpr T& Back() { return this->Data[EndIndex() - 1_bs]; }
  constexpr const T& Back() const { return this->Data[EndIndex() - 1_bs]; }
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
    this->FrontPos = 0_bs;
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
  return 0_bs;
}

template <int Config, typename T, Int ArraySize>
constexpr void Iterable<Config, T, ArraySize>::Clear() {
  if constexpr (HasFlag(EnableFrontPointer)) {
    this->FrontPos = 0_bs;
  }
  if constexpr (HasFlag(EnableEndPointer)) {
    this->EndPos = 0_bs;
  }
}

template <int Config, typename T, Int ArraySize>
constexpr void Iterable<Config, T, ArraySize>::ClearAndSet(T ele) {
  this->Data[0_bs] = ele;
  if constexpr (HasFlag(EnableFrontPointer)) {
    this->FrontPos = 0_bs;
  }
  if constexpr (HasFlag(EnableEndPointer)) {
    this->EndPos = 1_bs;
  }
}

template <int Config, typename T, Int ArraySize>
constexpr Int Iterable<Config, T, ArraySize>::FrontIndex() const {
  if constexpr (HasFlag(EnableFrontPointer)) {
    return this->FrontPos;
  } else {
    return 0_bs;
  }
}

template <int Config, typename T, Int ArraySize>
constexpr Int Iterable<Config, T, ArraySize>::EndIndex() const {
  if constexpr (HasFlag(EnableEndPointer)) {
    if constexpr (!HasFlag(EnableSpan)) {
      Assert(this->EndPos <= Cap(), K(this->EndPos), K(Cap()));
    }
    return this->EndPos;
  } else {
    return Cap();
  }
}

template <int Config, typename T, Int ArraySize>
constexpr Int Iterable<Config, T, ArraySize>::CheckIndex(Int i) const {
  Assert(i >= 0_bs && i < Size(), K(i), K(Size()));
  return i;
}

}  // namespace __detail__::iterable

using namespace __detail__::iterable::config;
using __detail__::iterable::Iterable;
template <typename T, Int Size>
using Array = Iterable<EnableArray, T, Size>;
template <typename T, Int Size>
using List = Iterable<EnableArray | EnableEndPointer, T, Size>;
template <typename T, Int Size>
using Queue = Iterable<EnableArray | EnableFrontPointer | EnableEndPointer, T, Size>;
template <typename T>
using Span = Iterable<EnableSpan | EnableEndPointer, T>;
template <typename T>
using Vector = Iterable<EnableAllocSize, T>;

class Random {
 public:
  Random();

  template <typename T>
  T Range(T min, T max);
  template <typename Iterable>
  const auto& Choice(const Iterable& data);

 private:
  std::mt19937_64 Rng;
};

inline Random::Random() { Rng.seed(std::chrono::steady_clock::now().time_since_epoch().count()); }

template <typename T>
T Random::Range(T min, T max) {
  std::uniform_int_distribution<T> dist(min, max);
  return dist(Rng);
}

template <typename Iterable>
const auto& Random::Choice(const Iterable& data) {
  Assert(!data.Empty());
  if (data.Size() == 1) {
    return data.At(0);
  }
  return data.At(Range(0, data.Size() - 1));
}

}  // namespace dab