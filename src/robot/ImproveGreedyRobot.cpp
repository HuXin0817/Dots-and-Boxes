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

#include "ImproveGreedyRobot.h"

namespace dab::__detail__::robot {

template <typename Board>
Span<const Edge> ImproveGreedyRobot ::BestCandidateEdges(const Board& board) {
  if (const Span<const Edge> edges = GreedyRobot::BestCandidateEdges(board); EnemyUnscoreable() || Scoreable()) {
    return edges;
  }

  Int minScore = Box::Max + 1;
  Array<Edge, Edge::Max>& candidateEdges = GetEdgeBuffer();
  Int candidateEdgesSize = 0;

  for (const Edge edge : board.EmptyEdges()) {
    SimulationBoard = board;
    SimulationBoard.Add(edge);
    if (const Int score = SimulationBoard.MaxObtainableScore(minScore); score < minScore) {
      minScore = score;
      candidateEdgesSize = 1;
      candidateEdges.At(0) = edge;
    } else if (score == minScore) {
      candidateEdges.At(candidateEdgesSize++) = edge;
    }
  }

  return {candidateEdges.begin(), candidateEdges.begin() + candidateEdgesSize};
}

Span<const Edge> ImproveGreedyRobot::BestCandidateEdges(const LoggingBoard& board) {
  return BestCandidateEdges<>(board);
}

template Span<const Edge> ImproveGreedyRobot::BestCandidateEdges<RelativeScoreBoard>(const RelativeScoreBoard& board);

}  // namespace dab::__detail__::robot