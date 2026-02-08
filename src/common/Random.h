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

#include <cassert>
#include <chrono>
#include <random>

namespace dab::detail::common {

class Random {
 public:
  explicit Random();

  Int Range(Int min, Int max);
  template <typename T>
  const auto& Choice(const T& data);

 private:
  std::mt19937_64 Rng;
  std::uniform_int_distribution<Int> Dist;
};

inline Random::Random() {
  Rng.seed(static_cast<uint64_t>(std::chrono::steady_clock::now().time_since_epoch().count()));
}

inline Int Random::Range(Int min, Int max) {
  Dist.param(std::uniform_int_distribution<Int>::param_type(min, max));
  return Dist(Rng);
}

template <typename T>
const auto& Random::Choice(const T& data) {
  assert(!data.Empty());
  if (data.Size() == 1) {
    return data.At(0);
  }
  return data.At(Range(0, data.Size() - 1));
}

}  // namespace dab::detail::common