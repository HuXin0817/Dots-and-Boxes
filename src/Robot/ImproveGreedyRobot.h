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

#include "BoardSize.h"
#include "GreedyRobot.h"

namespace dab::__detail__::robot {

class ImproveGreedyRobot : public GreedyRobot {
 public:
  ImproveGreedyRobot() = default;

  template <typename Board>
  Span<const Edge> BestCandidateEdges(const Board& board);
  template <typename Board>
  Edge SearchOne(const Board& board);

 private:
  ScoreableCountBoard SimulationBoardBackup;
  ScoreableCountBoard SimulationBoard;
};

template <typename Board>
Span<const Edge> ImproveGreedyRobot::BestCandidateEdges(const Board& board) {
  if (const Span<const Edge> edges = GreedyRobot::BestCandidateEdges(board); EnemyUnscoreable() || Scoreable()) {
    return edges;
  }

  Int minScore = Box::Max + 1_bs;
  Array<Edge, Edge::Max>& candidateEdges = Edges;
  Int candidateEdgesSize = 0_bs;

  SimulationBoardBackup = board;
  for (const Edge edge : board.EmptyEdges()) {
    SimulationBoard = SimulationBoardBackup;
    SimulationBoard.Add(edge);
    if (const Int score = SimulationBoard.MaxObtainableScore(minScore); score < minScore) {
      minScore = score;
      candidateEdgesSize = 1_bs;
      candidateEdges.At(0_bs) = edge;
    } else if (score == minScore) {
      candidateEdges.At(candidateEdgesSize++) = edge;
    }
  }

  return {candidateEdges.begin(), candidateEdgesSize};
}

template <typename Board>
Edge ImproveGreedyRobot::SearchOne(const Board& board) {
  Edge result = GreedyRobot::SearchOne(board);
  if (result.Valid()) {
    return result;
  }

  Int minScore = Box::Max + 1_bs;
  SimulationBoardBackup = board;
  for (const Edge edge : board.EmptyEdges()) {
    SimulationBoard = SimulationBoardBackup;
    SimulationBoard.Add(edge);
    if (const Int score = SimulationBoard.MaxObtainableScore(minScore); score < minScore) {
      minScore = score;
      result = edge;
    }
  }

  return result;
}

}  // namespace dab::__detail__::robot