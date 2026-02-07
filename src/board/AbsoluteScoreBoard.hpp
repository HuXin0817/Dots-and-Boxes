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

#include <Dab/Model.hpp>

#include "RelativeScoreBoard.hpp"

namespace dab::detail::board {

template <int64_t BoardSize>
class AbsoluteScoreBoard : public RelativeScoreBoard<BoardSize> {
 public:
  AbsoluteScoreBoard();

  void
  Reset(const EdgeCountableBoard<BoardSize>& newBoard = EdgeCountableBoard<BoardSize>());
  Int<BoardSize>
  Add(const Edge<BoardSize> edge);
  Int<BoardSize>
  Player1Score() const;
  Int<BoardSize>
  Player2Score() const;

 private:
  Int<BoardSize> TotalScore;
};

template <int64_t BoardSize>
AbsoluteScoreBoard<BoardSize>::AbsoluteScoreBoard() {
  Reset();
}

template <int64_t BoardSize>
void
AbsoluteScoreBoard<BoardSize>::Reset(const EdgeCountableBoard<BoardSize>& newBoard) {
  RelativeScoreBoard<BoardSize>::Reset(newBoard);
  TotalScore = 0;
}

template <int64_t BoardSize>
Int<BoardSize>
AbsoluteScoreBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  Int<BoardSize> score = RelativeScoreBoard<BoardSize>::Add(edge);
  TotalScore += score;
  return score;
}

template <int64_t BoardSize>
Int<BoardSize>
AbsoluteScoreBoard<BoardSize>::Player1Score() const {
  return (TotalScore + RelativeScoreBoard<BoardSize>::RelativeScore()) / 2;
}

template <int64_t BoardSize>
Int<BoardSize>
AbsoluteScoreBoard<BoardSize>::Player2Score() const {
  return (TotalScore - RelativeScoreBoard<BoardSize>::RelativeScore()) / 2;
}

}  // namespace dab::detail::board