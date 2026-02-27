/*
Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2025 Xin Hu <202219120810@stu.cdut.edu.cn>

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

#include <Dab/BoardSize.h>

#include <chrono>
#include <random>
#include <type_traits>

namespace dab::__detail__::common {

class Random {
 public:
  explicit Random();

  template <typename T>
  T Range(T min, T max);
  template <typename T>
  const auto& Choice(const T& data);

 private:
  std::mt19937_64 Rng;
  std::uniform_int_distribution<Int> Dist;
};

inline Random::Random() { Rng.seed(std::chrono::steady_clock::now().time_since_epoch().count()); }

template <typename T>
T Random::Range(T min, T max) {
  if constexpr (std::is_same_v<T, Int>) {
    Dist.param(std::uniform_int_distribution<Int>::param_type(min, max));
    return Dist(Rng);
  } else {
    std::uniform_int_distribution<T> dist(min, max);
    return dist(Rng);
  }
}

template <typename T>
const auto& Random::Choice(const T& data) {
  Assert(!data.Empty());
  if (data.Size() == 1) {
    return data.At(0);
  }
  return data.At(Range(0, data.Size() - 1));
}

}  // namespace dab::__detail__::common