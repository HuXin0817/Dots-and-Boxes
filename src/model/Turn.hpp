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

#include "Types.hpp"

namespace dab::detail::model {

template <int64_t BoardSize>
class Turn : public IntWapper<BoardSize> {
 public:
  constexpr Turn();

  constexpr void
  Reset();
  constexpr void
  Add();
  constexpr bool
  IsPlayer1Turn() const;
  constexpr bool
  IsPlayer2Turn() const;

 private:
  static constexpr Int<BoardSize> Player1Turn = 1;
  static constexpr Int<BoardSize> Player2Turn = -Player1Turn;
};

template <int64_t BoardSize>
constexpr Turn<BoardSize>::Turn() : IntWapper<BoardSize>(1) {
}

template <int64_t BoardSize>
constexpr void
Turn<BoardSize>::Reset() {
  IntWapper<BoardSize>::v = Player1Turn;
}

template <int64_t BoardSize>
constexpr void
Turn<BoardSize>::Add() {
  IntWapper<BoardSize>::v = -IntWapper<BoardSize>::v;
}

template <int64_t BoardSize>
constexpr bool
Turn<BoardSize>::IsPlayer1Turn() const {
  return IntWapper<BoardSize>::v == Player1Turn;
}

template <int64_t BoardSize>
constexpr bool
Turn<BoardSize>::IsPlayer2Turn() const {
  return IntWapper<BoardSize>::v == Player2Turn;
}

}  // namespace dab::detail::model