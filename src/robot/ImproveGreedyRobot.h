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

#include "GreedyRobot.h"

namespace dab::__detail__::robot {

class ImproveGreedyRobot : public RobotWapper<ImproveGreedyRobot> {
 public:
  ImproveGreedyRobot() = default;

  bool EnemyUnscoreable() const { return SubRobot.EnemyUnscoreable(); }
  bool Scoreable() const { return SubRobot.Scoreable(); }
  template <typename Board>
  Span<const Edge> BestCandidateEdges(const Board& board);
  template <typename Board>
  Edge SearchOne(const Board& board);

 private:
  GreedyRobot SubRobot;
  ScoreableCountBoard SimulationBoardBackup;
  ScoreableCountBoard SimulationBoard;
};

template <typename Board>
Span<const Edge> ImproveGreedyRobot::BestCandidateEdges(const Board& board) {
  if (const Span<const Edge> edges = SubRobot.BestCandidateEdges(board); EnemyUnscoreable() || Scoreable()) {
    return edges;
  }

  Int minScore = Box::Max + 1;
  Array<Edge, Edge::Max>& candidateEdges = SubRobot.GetEdgeBuffer();
  Int candidateEdgesSize = 0;

  SimulationBoardBackup = board;
  for (const Edge edge : board.EmptyEdges()) {
    SimulationBoard = SimulationBoardBackup;
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

template <typename Board>
Edge ImproveGreedyRobot::SearchOne(const Board& board) {
  Edge result = SubRobot.SearchOne(board);
  if (result.Valid()) {
    return result;
  }

  Int minScore = Box::Max + 1;
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