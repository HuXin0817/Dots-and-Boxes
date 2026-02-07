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

#include "ImproveGreedyRobot.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize>
class SimulationRobot final : public Robot<BoardSize> {
 public:
  SimulationRobot() = default;
  Span<Edge<BoardSize>> BestCandidateEdges(const RelativeScoreBoard<BoardSize>& board) override;

 private:
  ImproveGreedyRobot<BoardSize> SubRobot;
  RelativeScoreBoard<BoardSize> SimulationBoard;
  List<Edge<BoardSize>, Edge<BoardSize>::Max> SearchEdges;
};

template <int64_t BoardSize>
Span<Edge<BoardSize>> SimulationRobot<BoardSize>::BestCandidateEdges(const RelativeScoreBoard<BoardSize>& board) {
  if (Span<Edge<BoardSize>> edges = SubRobot.BestCandidateEdges(board); SubRobot.EnemyUnscoreable()) {
    return edges;
  }

  SearchEdges.Clear();
  Int<BoardSize> maxScore = -Box<BoardSize>::Max;
  for (const Edge<BoardSize> emptyEdge : board.EmptyEdges()) {
    SimulationBoard.Reset(static_cast<EdgeCountableBoard<BoardSize>>(board));
    SimulationBoard.Add(emptyEdge);
    while (SimulationBoard.Gaming()) {
      const Edge<BoardSize> edge = SubRobot.BestCandidateEdges(SimulationBoard).Front();
      assert(board.MaxEdgeCount(edge) > 1);
      SimulationBoard.Add(edge);
    }
    if (const Int<BoardSize> score = SimulationBoard.RelativeScore(); score > maxScore) {
      maxScore = score;
      SearchEdges.ClearAndSet(emptyEdge);
    } else if (score == maxScore) {
      SearchEdges.Append(emptyEdge);
    }
  }

  return Span(SearchEdges.begin(), SearchEdges.end());
}

}  // namespace dab::detail::robot