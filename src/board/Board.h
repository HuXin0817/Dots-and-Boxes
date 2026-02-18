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

#include <Dab/Model.h>

#include <chrono>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace dab::__detail__::board {

namespace config {
static constexpr int EnableEdgeCount = 1 << 0;
static constexpr int EnableRelativeScore = 1 << 1;
static constexpr int EnableAbsoluteScore = 1 << 2;
static constexpr int EnableLogging = 1 << 3;
static constexpr int EnableScoreableCounting = 1 << 4;
}  // namespace config

using namespace config;

static constexpr bool HasFlag(int config, int flag) { return (config & flag) != 0; }

static constexpr int FixedConfig(int config) {
  int fixedConfig = config;
  if (HasFlag(config, EnableRelativeScore)) {
    fixedConfig |= EnableEdgeCount;
  }
  if (HasFlag(config, EnableAbsoluteScore)) {
    fixedConfig |= EnableEdgeCount | EnableRelativeScore;
  }
  if (HasFlag(config, EnableLogging)) {
    fixedConfig |= EnableEdgeCount | EnableRelativeScore | EnableAbsoluteScore;
  }
  if (HasFlag(config, EnableScoreableCounting)) {
    fixedConfig |= EnableEdgeCount;
  }
  return static_cast<int>(fixedConfig);
}

template <int Config, bool Enabled>
struct EdgeCountMixin;

template <int Config>
struct EdgeCountMixin<Config, true> {
  Array<uint8_t, Box::Max> Counter;
};

template <int Config>
struct EdgeCountMixin<Config, false> {};

template <int Config, bool Enabled>
struct ScoreableCountingMixin;

template <int Config>
struct ScoreableCountingMixin<Config, true> {
  Queue<Edge, Edge::Max> ScoreableEdges;
};

template <int Config>
struct ScoreableCountingMixin<Config, false> {};

template <int Config, bool Enabled>
struct RelativeScoreMixin;

template <int Config>
struct RelativeScoreMixin<Config, true> {
  Int Score;
  Turn Turn;
};

template <int Config>
struct RelativeScoreMixin<Config, false> {};

template <int Config, bool Enabled>
struct AbsoluteScoreMixin;

template <int Config>
struct AbsoluteScoreMixin<Config, true> {
  Int TotalScore;
};

template <int Config>
struct AbsoluteScoreMixin<Config, false> {};

template <int Config, bool Enabled>
struct LoggingMixin;

template <int Config>
struct LoggingMixin<Config, true> {
  std::chrono::system_clock::time_point LastUpdateTime;
};

template <int Config>
struct LoggingMixin<Config, false> {};

template <int Config>
class Board : private EdgeCountMixin<Config, HasFlag(FixedConfig(Config), EnableEdgeCount)>,
              private ScoreableCountingMixin<Config, HasFlag(FixedConfig(Config), EnableScoreableCounting)>,
              private RelativeScoreMixin<Config, HasFlag(FixedConfig(Config), EnableRelativeScore)>,
              private AbsoluteScoreMixin<Config, HasFlag(FixedConfig(Config), EnableAbsoluteScore)>,
              private LoggingMixin<Config, HasFlag(FixedConfig(Config), EnableLogging)> {
  template <int>
  friend class Board;
  static inline std::logic_error UnimplementedError = std::logic_error("unimplemented");
  static constexpr bool HasFlag(int flag) { return (FixedConfig(Config) & flag) != 0; }

 public:
  Board() { Reset(); }

  void Reset();
  Int Add(Edge edge);
  bool Contains(Edge edge) const { return EdgeIndexes.At(edge) < Step; }
  bool NotContains(Edge edge) const { return EdgeIndexes.At(edge) >= Step; }
  Span<const Edge> EmptyEdges() const { return {Edges.begin() + Step, Edges.end()}; }
  Span<const Edge> MoveRecord() const { return {Edges.begin(), Edges.begin() + Step}; }
  bool Gaming() const { return Step < Edge::Max; }
  Int RemainStep() const { return Edge::Max - Step; }
  Int NowStep() const { return Step; }
  Int RelativeScore() const;
  Int Player1Score() const;
  Int Player2Score() const;
  Turn GetTurn() const;
  bool IsPlayer1Turn() const;
  bool IsPlayer2Turn() const;
  Edge FindNotContainsEdgeInBox(Box box) const;
  Int FindScoreableEdge();
  Int MaxObtainableScore(Int endScore);
  uint8_t EdgeCount(Box box) const;
  uint8_t MaxEdgeCount(Edge edge) const;
  bool Scoreable(Edge edge) const;

  template <typename FromBoard>
  void operator=(const FromBoard& from) {
    Step = from.Step;
    Edges = from.Edges;
    EdgeIndexes = from.EdgeIndexes;

    if constexpr (HasFlag(EnableEdgeCount)) {
      static_assert(FromBoard::HasFlag(EnableEdgeCount));
      this->Counter = from.Counter;
    }
    if constexpr (HasFlag(EnableScoreableCounting)) {
      if constexpr (FromBoard::HasFlag(EnableScoreableCounting)) {
        this->ScoreableEdges = from.ScoreableEdges;
      } else {
        this->ScoreableEdges.Clear();
      }
    }
    if constexpr (HasFlag(EnableRelativeScore)) {
      static_assert(FromBoard::HasFlag(EnableRelativeScore));
      this->Score = from.Score;
      this->Turn = from.Turn;
    }
    if constexpr (HasFlag(EnableAbsoluteScore)) {
      static_assert(FromBoard::HasFlag(EnableAbsoluteScore));
      this->TotalScore = from.TotalScore;
    }
    if constexpr (HasFlag(EnableLogging)) {
      static_assert(FromBoard::HasFlag(EnableLogging));
      this->LastUpdateTime = from.LastUpdateTime;
    }
  }

 private:
  Int Step = 0;
  Array<Edge, Edge::Max> Edges;
  Array<Int, Edge::Max> EdgeIndexes;
};

}  // namespace dab::__detail__::board