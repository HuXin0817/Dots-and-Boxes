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

#include "CachedRobot.h"
#include "ScoreMap.h"
#include "SimulationRobot.h"

namespace dab::__detail__::robot {

class MonteCarloRobot : public RobotWapper<MonteCarloRobot> {
  static constexpr int64_t SearchTime = static_cast<int64_t>(Edge::Max) << 6;

 public:
  MonteCarloRobot() = default;
  template <typename Board>
  void SearchCandidateEdges(const Board& board);
  template <typename Board>
  Span<const Edge> BestCandidateEdges(const Board& board);
  template <typename Board>
  bool CanEarlyExit(const Board& board, Span<const Edge>& result);
  const ScoreMap& GetSearchResult() const { return SearchResult; }
  List<Edge, Edge::Max>& GetSearchEdges() { return SubRobot.GetSearchEdges(); }

 private:
  CachedRobot<SimulationRobot> SubRobot;
  RelativeScoreBoard SimulationBoard;
  Random Random;
  ScoreMap SearchResult;
};

template <typename Board>
void MonteCarloRobot::SearchCandidateEdges(const Board& board) {
  SearchResult.Reset();
  const Turn turn = board.GetTurn();
  for (int64_t i = 0; i < SearchTime / board.RemainStep(); i++) {
    SimulationBoard = board;
    const Edge edge = Random.Choice(SubRobot.BestCandidateEdges(SimulationBoard));
    SimulationBoard.Add(edge);
    while (SimulationBoard.Gaming()) {
      SimulationBoard.Add(Random.Choice(SubRobot.BestCandidateEdges(SimulationBoard)));
    }
    SearchResult.Add(edge, turn * SimulationBoard.RelativeScore());
  }
}

template <typename Board>
Span<const Edge> MonteCarloRobot::BestCandidateEdges(const Board& board) {
  if (Span<const Edge> edges; CanEarlyExit(board, edges)) {
    return edges;
  }
  SearchCandidateEdges(board);
  return SearchResult.Export(GetSearchEdges());
}

template <typename Board>
bool MonteCarloRobot::CanEarlyExit(const Board& board, Span<const Edge>& result) {
  result = SubRobot.BestCandidateEdges(board);
  return result.Size() == 1;
}

}  // namespace dab::__detail__::robot