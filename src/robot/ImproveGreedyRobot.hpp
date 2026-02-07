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

#include "GreedyRobot.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize>
class ImproveGreedyRobot final : public GreedyRobot<BoardSize> {
 public:
  ImproveGreedyRobot() = default;

  Span<Edge<BoardSize>> BestCandidateEdges(const RelativeScoreBoard<BoardSize>& board) override;

 private:
  ScoreableEdgeBoard<BoardSize> SimulationBoard;
};

template <int64_t BoardSize>
Span<Edge<BoardSize>> ImproveGreedyRobot<BoardSize>::BestCandidateEdges(const RelativeScoreBoard<BoardSize>& board) {
  if (Span<Edge<BoardSize>> edges = GreedyRobot<BoardSize>::BestCandidateEdges(board);
      GreedyRobot<BoardSize>::EnemyUnscoreable() || GreedyRobot<BoardSize>::Scoreable()) {
    return edges;
  }

  Int<BoardSize> minScore = Box<BoardSize>::Max + 1;
  Array<Edge<BoardSize>, Edge<BoardSize>::Max>& candidateEdges = GreedyRobot<BoardSize>::GetEdgeBuffer();
  Int<BoardSize> candidateEdgesSize = 0;

  for (const Edge<BoardSize> edge : board.EmptyEdges()) {
    SimulationBoard.Reset(static_cast<EdgeCountableBoard<BoardSize>>(board));
    SimulationBoard.Add(edge);
    if (const Int<BoardSize> score = SimulationBoard.MaxObtainableScore(minScore); score < minScore) {
      minScore = score;
      candidateEdgesSize = 1;
      candidateEdges[0] = edge;
    } else if (score == minScore) {
      candidateEdges[candidateEdgesSize++] = edge;
    }
  }

  return Span(candidateEdges.begin(), candidateEdges.begin() + candidateEdgesSize);
}

}  // namespace dab::detail::robot