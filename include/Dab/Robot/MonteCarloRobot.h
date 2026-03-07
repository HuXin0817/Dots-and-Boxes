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

#include <chrono>
#include <cstdint>

#include "CachedRobot.h"
#include "Dab/Tools.h"
#include "SimulationRobot.h"

namespace dab::__detail__::robot {

class MonteCarloRobot {
  struct ScoreMap {
    ScoreMap() = default;

    void Reset();
    void Add(Edge edge, Int score);
    void Add(const ScoreMap& other);
    Span<const Edge> Export(List<Edge, Edge::Max>& edges);

    Array<int64_t, Edge::Max> Time;
    Array<int64_t, Edge::Max> Score;
  };

  static constexpr uint32_t SearchTime = static_cast<uint32_t>(Edge::Max) << 8;

 public:
  MonteCarloRobot(Int id = 0) : Id(id) {}
  void SetId(Int id) { Id = id; }
  template <typename Board>
  void SearchCandidateEdges(const Board& board);
  template <typename Board>
  Span<const Edge> BestCandidateEdges(const Board& board);
  template <typename Board>
  bool CanEarlyExit(const Board& board, Span<const Edge>& result);
  ScoreMap& GetSearchResult() { return SearchResult; }
  List<Edge, Edge::Max>& GetSearchEdges() { return SubRobot.GetSearchEdges(); }

 private:
  Int Id;
  CachedRobot<SimulationRobot> SubRobot;
  RelativeScoreBoard SimulationBoard;
  ScoreMap SearchResult;
};

inline void MonteCarloRobot::ScoreMap::Reset() {
  std::ranges::fill(Time, 0);
  std::ranges::fill(Score, 0);
}

inline void MonteCarloRobot::ScoreMap::Add(Edge edge, Int score) {
  ++Time.At(edge);
  Score.At(edge) += score;
}

inline void MonteCarloRobot::ScoreMap::Add(const ScoreMap& other) {
  for (const Int i : Iota<Edge>()) {
    Time.At(i) += other.Time.At(i);
    Score.At(i) += other.Score.At(i);
  }
}

inline Span<const Edge> MonteCarloRobot::ScoreMap::Export(List<Edge, Edge::Max>& edges) {
  edges.Clear();
  float maxScore = 0.0;
  for (const Edge edge : Iota<Edge>()) {
    if (Time.At(edge) > 0) {
      if (const float score = static_cast<float>(Score.At(edge)) / static_cast<float>(Time.At(edge));
          score > maxScore || edges.Empty()) {
        maxScore = score;
        edges.ClearAndSet(edge);
      } else if (score == maxScore) {
        edges.Append(edge);
      }
    }
  }
  return {edges.begin(), edges.Size()};
}

template <typename Board>
void MonteCarloRobot::SearchCandidateEdges(const Board& board) {
  Random random;
  SearchResult.Reset();
  const Int turn = board.GetTurn();
#ifndef NDEBUG
  auto lastTime = std::chrono::system_clock::now();
#endif
  for (uint32_t i = 0; i < SearchTime / board.RemainStep(); i++) {
    SimulationBoard = board;
    const Edge edge = random.Choice(SubRobot.BestCandidateEdges(SimulationBoard));
    SimulationBoard.Add(edge);
    while (SimulationBoard.Gaming()) {
      SimulationBoard.Add(random.Choice(SubRobot.BestCandidateEdges(SimulationBoard)));
    }
    SearchResult.Add(edge, turn * SimulationBoard.RelativeScore());
#ifndef NDEBUG
    auto nowTime = std::chrono::system_clock::now();
    if (nowTime - lastTime >= std::chrono::seconds(5)) {
      LogDebug(R"({{"MonteCarloRobot":{{"Id":{},"Schedule":"{}/{}","CandidateEdges":{}}}}})",
               Id,
               i * board.RemainStep(),
               SearchTime,
               static_cast<std::string>(SearchResult.Export(GetSearchEdges())));
      lastTime = nowTime;
    }
#endif
  }
#ifndef NDEBUG
  LogDebug(R"({{"MonteCarloRobot":{{"Id":{},"Schedule":"done","CandidateEdges":{}}}}})",
           Id,
           static_cast<std::string>(SearchResult.Export(GetSearchEdges())));
#endif
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