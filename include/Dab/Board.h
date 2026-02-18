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

#include <ranges>
#include <stdexcept>
#include <type_traits>

namespace dab {

namespace __detail__::board {

enum BoardConfig : int {
  EnableEdgeCount = 1 << 0,
  EnableRelativeScore = 1 << 1,
  EnableAbsoluteScore = 1 << 2,
  EnableLogging = 1 << 3,
  EnableScoreableCounting = 1 << 4,
};

static constexpr bool HasFlag(BoardConfig config, BoardConfig flag) { return (config & flag) != 0; }

static constexpr BoardConfig FixedConfig(BoardConfig config) {
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
  return static_cast<BoardConfig>(fixedConfig);
}

template <BoardConfig Config>
class Board {
  template <BoardConfig>
  friend class Board;
  static inline std::logic_error UnimplementedError = std::logic_error("unimplemented");
  static constexpr bool HasFlag(BoardConfig flag) { return (FixedConfig(Config) & flag) != 0; }

 public:
  Board() {
    Step = 0;
    std::iota(EdgeIndexes.begin(), EdgeIndexes.end(), 0);
    std::iota(Edges.begin(), Edges.end(), 0);
    if constexpr (HasFlag(EnableEdgeCount)) {
      Counter = Array<uint8_t, Box::Max>();
    }
    if constexpr (HasFlag(EnableScoreableCounting)) {
      ScoreableEdges = Queue<Edge, Edge::Max>();
    }
    if constexpr (HasFlag(EnableRelativeScore)) {
      Score = 0;
      Turn.Reset();
    }
    if constexpr (HasFlag(EnableAbsoluteScore)) {
      TotalScore = 0;
    }
    if constexpr (HasFlag(EnableLogging)) {
      LastUpdateTime = std::chrono::system_clock::now();
    }
  }

  void Reset() { operator=(Board()); }

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
        const uint8_t num = ++Counter.At(box);
        Assert(num <= 4);
        if (num == 4) {
          ++score;
        }
        if constexpr (HasFlag(EnableScoreableCounting)) {
          if (num == 3) {
            ScoreableEdges.Append(FindNotContainsEdgeInBox(box));
          }
        }
      }
      if constexpr (HasFlag(EnableRelativeScore)) {
        const class Turn turn = Turn;
        if (score > 0) {
          Score += score * Turn;
        } else {
          Turn.Add();
        }
        if constexpr (HasFlag(EnableAbsoluteScore)) {
          TotalScore += score;
        }
        if constexpr (HasFlag(EnableLogging)) {
          const Int step = NowStep();
          const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
          const int64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(now - LastUpdateTime).count();
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
          LastUpdateTime = now;
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
      return Score;
    } else {
      throw UnimplementedError;
    }
  }

  Int Player1Score() const {
    if constexpr (HasFlag(EnableAbsoluteScore)) {
      return (TotalScore + Score) / 2;
    } else {
      throw UnimplementedError;
    }
  }

  Int Player2Score() const {
    if constexpr (HasFlag(EnableAbsoluteScore)) {
      return (TotalScore - Score) / 2;
    } else {
      throw UnimplementedError;
    }
  }

  Turn GetTurn() const {
    if constexpr (HasFlag(EnableRelativeScore)) {
      return Turn;
    } else {
      throw UnimplementedError;
    }
  }

  bool IsPlayer1Turn() const {
    if constexpr (HasFlag(EnableRelativeScore)) {
      return Turn.IsPlayer1Turn();
    } else {
      throw UnimplementedError;
    }
  }

  bool IsPlayer2Turn() const {
    if constexpr (HasFlag(EnableRelativeScore)) {
      return Turn.IsPlayer2Turn();
    } else {
      throw UnimplementedError;
    }
  }

  Edge FindNotContainsEdgeInBox(Box box) const {
    if constexpr (HasFlag(EnableEdgeCount)) {
      Assert(Counter.At(box) == 3);
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

  Edge FindScoreableEdge() const {
    for (const Box box : std::views::iota(0, Box::Max)) {
      if (Counter.At(box) == 3) {
        return FindNotContainsEdgeInBox(box);
      }
    }
    return Edge::Invalid;
  }

  Int MaxObtainableScore(Int endScore) {
    if constexpr (HasFlag(EnableScoreableCounting)) {
      Int score = 0;
      while (Gaming() && score < endScore) {
        if (ScoreableEdges.Empty()) {
          if (const Edge edge = FindScoreableEdge(); edge != Edge::Invalid) {
            ScoreableEdges.Append(edge);
          } else {
            break;
          }
        }
        const Edge edge = ScoreableEdges.Pop();
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
      return Counter.At(box);
    } else {
      throw UnimplementedError;
    }
  }

  uint8_t MaxEdgeCount(Edge edge) const {
    if constexpr (HasFlag(EnableEdgeCount)) {
      const List<Box, 2>& nearBoxes = edge.NearBoxes();
      return std::max(Counter.At(nearBoxes.Front()), Counter.At(nearBoxes.Back()));
    } else {
      throw UnimplementedError;
    }
  }

  class None {
    None(const None&) = delete;
    None& operator=(const None&) = delete;
  };
  template <BoardConfig Flag, typename T>
  using Member = std::conditional_t<HasFlag(EnableEdgeCount), T, None>;

  template <typename ToBoard>
  operator ToBoard() {
    ToBoard to;

    to.Step = Step;
    to.Edges = Edges;
    to.EdgeIndexes = EdgeIndexes;

    if constexpr (ToBoard::If(EnableEdgeCount)) {
      static_assert(HasFlag(EnableEdgeCount));
      to.Counter = Counter;
    }
    if constexpr (ToBoard::If(EnableScoreableCounting)) {
      static_assert(HasFlag(EnableScoreableCounting));
      to.ScoreableEdges = ScoreableEdges;
    }
    if constexpr (ToBoard::If(EnableRelativeScore)) {
      static_assert(HasFlag(EnableRelativeScore));
      to.Score = Score;
      to.Turn = Turn;
    }
    if constexpr (ToBoard::If(EnableAbsoluteScore)) {
      static_assert(HasFlag(EnableAbsoluteScore));
      to.TotalScore = TotalScore;
    }
    if constexpr (ToBoard::If(EnableLogging)) {
      static_assert(HasFlag(EnableLogging));
      to.LastUpdateTime = LastUpdateTime;
    }

    return to;
  }

  template <typename FromBoard>
  void operator=(const FromBoard& from) {
    Step = from.Step;
    Edges = from.Edges;
    EdgeIndexes = from.EdgeIndexes;

    if constexpr (HasFlag(EnableEdgeCount)) {
      static_assert(FromBoard::If(EnableEdgeCount));
      Counter = from.Counter;
    }
    if constexpr (HasFlag(EnableScoreableCounting)) {
      if constexpr (FromBoard::If(EnableScoreableCounting)) {
        ScoreableEdges = from.ScoreableEdges;
      } else {
        ScoreableEdges.Clear();
      }
    }
    if constexpr (HasFlag(EnableRelativeScore)) {
      static_assert(FromBoard::If(EnableRelativeScore));
      Score = from.Score;
      Turn = from.Turn;
    }
    if constexpr (HasFlag(EnableAbsoluteScore)) {
      static_assert(FromBoard::If(EnableAbsoluteScore));
      TotalScore = from.TotalScore;
    }
    if constexpr (HasFlag(EnableLogging)) {
      static_assert(FromBoard::If(EnableLogging));
      LastUpdateTime = from.LastUpdateTime;
    }
  }

 private:
  Int Step = 0;
  Array<Edge, Edge::Max> Edges;
  Array<Int, Edge::Max> EdgeIndexes;

  Member<EnableEdgeCount, Array<uint8_t, Box::Max>> Counter;
  Member<EnableScoreableCounting, Queue<Edge, Edge::Max>> ScoreableEdges;
  Member<EnableRelativeScore, Int> Score;
  Member<EnableRelativeScore, Turn> Turn;
  Member<EnableAbsoluteScore, Int> TotalScore;
  Member<EnableLogging, std::chrono::system_clock::time_point> LastUpdateTime;
};

}  // namespace __detail__::board

using enum __detail__::board::BoardConfig;
using EdgeCountableBoard = __detail__::board::Board<EnableEdgeCount>;
using RelativeScoreBoard = __detail__::board::Board<EnableRelativeScore>;
using AbsoluteScoreBoard = __detail__::board::Board<EnableAbsoluteScore>;
using LoggingBoard = __detail__::board::Board<EnableLogging>;
using ScoreableEdgeBoard = __detail__::board::Board<EnableScoreableCounting>;

}  // namespace dab