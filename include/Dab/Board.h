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
#include <numeric>
#include <stdexcept>

namespace dab {

namespace __detail__::board {

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

  void Reset() {
    Step = 0;
    std::iota(EdgeIndexes.begin(), EdgeIndexes.end(), 0);
    std::iota(Edges.begin(), Edges.end(), 0);
    if constexpr (HasFlag(EnableEdgeCount)) {
      this->Counter = Array<uint8_t, Box::Max>();
    }
    if constexpr (HasFlag(EnableScoreableCounting)) {
      this->ScoreableEdges = Queue<Edge, Edge::Max>();
    }
    if constexpr (HasFlag(EnableRelativeScore)) {
      this->Score = 0;
      this->Turn.Reset();
    }
    if constexpr (HasFlag(EnableAbsoluteScore)) {
      this->TotalScore = 0;
    }
    if constexpr (HasFlag(EnableLogging)) {
      this->LastUpdateTime = std::chrono::system_clock::now();
    }
  }

  Int Add(Edge edge) {
    Assert(NotContains(edge));
    const Edge nowEdge = Edges.At(Step);
    const Int edgeIndex = EdgeIndexes.At(edge);
    Assert(Edges.At(edgeIndex) == edge);
    Assert(edgeIndex >= Step);
    Edges.At(Step) = edge;
    Edges.At(edgeIndex) = nowEdge;
    EdgeIndexes.At(edge) = Step;
    EdgeIndexes.At(nowEdge) = edgeIndex;
    ++Step;
    Int score = 0;
    if constexpr (HasFlag(EnableEdgeCount)) {
      for (const Box box : edge.NearBoxes()) {
        const uint8_t num = ++this->Counter.At(box);
        Assert(num <= 4);
        if (num == 4) {
          ++score;
        }
        if constexpr (HasFlag(EnableScoreableCounting)) {
          if (num == 3) {
            this->ScoreableEdges.Append(FindNotContainsEdgeInBox(box));
          }
        }
      }
      if constexpr (HasFlag(EnableRelativeScore)) {
        const class Turn turn = this->Turn;
        if (score > 0) {
          this->Score += score * this->Turn;
        } else {
          this->Turn.Add();
        }
        if constexpr (HasFlag(EnableAbsoluteScore)) {
          this->TotalScore += score;
        }
        if constexpr (HasFlag(EnableLogging)) {
          const Int step = NowStep();
          const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
          const int64_t time =
              std::chrono::duration_cast<std::chrono::milliseconds>(now - this->LastUpdateTime).count();
          LogInfo(R"({{"Step":{},"Turn":{},"Move":{},"Score":{{"Player1":{},"Player2":{}}},"Time":{}}})",
                  step,
                  turn.IsPlayer1Turn() ? 1 : 2,
                  static_cast<Int>(edge),
                  Player1Score(),
                  Player2Score(),
                  static_cast<double>(time) / 1000.0);
          if (!Gaming()) {
            if (RelativeScore() > 0) {
              LogInfo(R"({{"Winner":"Player1"}})");
            } else if (RelativeScore() < 0) {
              LogInfo(R"({{"Winner":"Player2"}})");
            } else {
              LogInfo(R"({{"Winner":"Draw"}})");
            }
          }
          this->LastUpdateTime = now;
        }
      }
    }
    return score;
  }

  bool Contains(Edge edge) const { return EdgeIndexes.At(edge) < Step; }
  bool NotContains(Edge edge) const { return EdgeIndexes.At(edge) >= Step; }
  Span<const Edge> EmptyEdges() const { return {Edges.begin() + Step, Edges.end()}; }
  Span<const Edge> MoveRecord() const { return {Edges.begin(), Edges.begin() + Step}; }
  bool Gaming() const { return Step < Edge::Max; }
  Int RemainStep() const { return Edge::Max - Step; }
  Int NowStep() const { return Step; }

  Int RelativeScore() const {
    if constexpr (HasFlag(EnableRelativeScore)) {
      return this->Score;
    } else {
      throw UnimplementedError;
    }
  }

  Int Player1Score() const {
    if constexpr (HasFlag(EnableAbsoluteScore)) {
      return (this->TotalScore + this->Score) / 2;
    } else {
      throw UnimplementedError;
    }
  }

  Int Player2Score() const {
    if constexpr (HasFlag(EnableAbsoluteScore)) {
      return (this->TotalScore - this->Score) / 2;
    } else {
      throw UnimplementedError;
    }
  }

  Turn GetTurn() const {
    if constexpr (HasFlag(EnableRelativeScore)) {
      return this->Turn;
    } else {
      throw UnimplementedError;
    }
  }

  bool IsPlayer1Turn() const {
    if constexpr (HasFlag(EnableRelativeScore)) {
      return this->Turn.IsPlayer1Turn();
    } else {
      throw UnimplementedError;
    }
  }

  bool IsPlayer2Turn() const {
    if constexpr (HasFlag(EnableRelativeScore)) {
      return this->Turn.IsPlayer2Turn();
    } else {
      throw UnimplementedError;
    }
  }

  Edge FindNotContainsEdgeInBox(Box box) const {
    if constexpr (HasFlag(EnableEdgeCount)) {
      Assert(this->Counter.At(box) == 3);
      for (const Edge edge : box.NearEdges()) {
        if (NotContains(edge)) {
          return edge;
        }
      }
      throw std::runtime_error("unreachable");
    } else {
      throw UnimplementedError;
    }
  }

  Int FindScoreableEdge() {
    if constexpr (HasFlag(EnableScoreableCounting)) {
      for (const Edge edge : EmptyEdges()) {
        if (Scoreable(edge)) {
          this->ScoreableEdges.Append(edge);
        }
      }
      return this->ScoreableEdges.Size();
    } else {
      throw UnimplementedError;
    }
  }

  Int MaxObtainableScore(Int endScore) {
    if constexpr (HasFlag(EnableScoreableCounting)) {
      Int score = 0;
      while (Gaming() && score < endScore) {
        if (this->ScoreableEdges.Empty() && FindScoreableEdge() == 0) {
          break;
        }
        const Edge edge = this->ScoreableEdges.Pop();
        if (Contains(edge)) {
          continue;
        }
        const Int addScore = Add(edge);
        Assert(addScore > 0);
        score += addScore;
      }
      return score;
    } else {
      throw UnimplementedError;
    }
  }

  uint8_t EdgeCount(Box box) const {
    if constexpr (HasFlag(EnableEdgeCount)) {
      return this->Counter.At(box);
    } else {
      throw UnimplementedError;
    }
  }

  uint8_t MaxEdgeCount(Edge edge) const {
    if constexpr (HasFlag(EnableEdgeCount)) {
      const List<Box, 2>& nearBoxes = edge.NearBoxes();
      return std::max(this->Counter.At(nearBoxes.Front()), this->Counter.At(nearBoxes.Back()));
    } else {
      throw UnimplementedError;
    }
  }

  bool Scoreable(Edge edge) const {
    if constexpr (HasFlag(EnableEdgeCount)) {
      return MaxEdgeCount(edge) == 3;
    } else {
      throw UnimplementedError;
    }
  }

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

struct BasicBoard {
  Int Step = 0;
  Array<Edge, Edge::Max> Edges;
  Array<Int, Edge::Max> EdgeIndexes;
};
static_assert(sizeof(Board<0>) == sizeof(BasicBoard));

}  // namespace __detail__::board

using BasicBoard = __detail__::board::Board<0>;
using EdgeCountableBoard = __detail__::board::Board<__detail__::board::config::EnableEdgeCount>;
using RelativeScoreBoard = __detail__::board::Board<__detail__::board::config::EnableRelativeScore>;
using AbsoluteScoreBoard = __detail__::board::Board<__detail__::board::config::EnableAbsoluteScore>;
using LoggingBoard = __detail__::board::Board<__detail__::board::config::EnableLogging>;
using ScoreableEdgeBoard = __detail__::board::Board<__detail__::board::config::EnableScoreableCounting>;

}  // namespace dab