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

enum BoardType : int {
  EnableEdgeCount = 1 << 0,
  EnableRelativeScore = 1 << 1,
  EnableAbsoluteScore = 1 << 2,
  EnableLogging = 1 << 3,
  EnableScoreableCounting = 1 << 4,
};

static constexpr bool HasFlag(BoardType type, BoardType flag) { return (type & flag) != 0; }

static constexpr BoardType FixedFlag(BoardType flag) {
  int fixedFlag = flag;
  if (HasFlag(flag, EnableRelativeScore)) {
    fixedFlag |= EnableEdgeCount;
  }
  if (HasFlag(flag, EnableAbsoluteScore)) {
    fixedFlag |= EnableEdgeCount | EnableRelativeScore;
  }
  if (HasFlag(flag, EnableLogging)) {
    fixedFlag |= EnableEdgeCount | EnableRelativeScore | EnableAbsoluteScore;
  }
  if (HasFlag(flag, EnableScoreableCounting)) {
    fixedFlag |= EnableEdgeCount;
  }
  return static_cast<BoardType>(fixedFlag);
}

template <BoardType Type>
class Board {
 public:
  static constexpr bool If(BoardType flag) { return (FixedFlag(Type) & flag) != 0; }

  Board() {
    Step = 0;
    std::iota(EdgeIndexes.begin(), EdgeIndexes.end(), 0);
    std::iota(Edges.begin(), Edges.end(), 0);
    if constexpr (If(EnableEdgeCount)) {
      Counter = Array<uint8_t, Box::Max>();
    }
    if constexpr (If(EnableScoreableCounting)) {
      ScoreableEdges = Queue<Edge, Edge::Max>();
    }
    if constexpr (If(EnableRelativeScore)) {
      Score = 0;
      Turn.Reset();
    }
    if constexpr (If(EnableAbsoluteScore)) {
      TotalScore = 0;
    }
    if constexpr (If(EnableLogging)) {
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
    if constexpr (If(EnableEdgeCount)) {
      for (const Box box : edge.NearBoxes()) {
        const uint8_t num = ++Counter.At(box);
        Assert(num <= 4);
        if (num == 4) {
          ++score;
        }
        if constexpr (If(EnableScoreableCounting)) {
          if (num == 3) {
            ScoreableEdges.Append(FindNotContainsEdgeInBox(box));
          }
        }
      }
      if constexpr (If(EnableRelativeScore)) {
        const class Turn turn = Turn;
        if (score > 0) {
          Score += score * Turn;
        } else {
          Turn.Add();
        }
        if constexpr (If(EnableAbsoluteScore)) {
          TotalScore += score;
        }
        if constexpr (If(EnableLogging)) {
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
    if constexpr (If(EnableRelativeScore)) {
      return Score;
    } else {
      throw std::runtime_error("unimplemented");
    }
  }

  Int Player1Score() const {
    if constexpr (If(EnableAbsoluteScore)) {
      return (TotalScore + Score) / 2;
    } else {
      throw std::runtime_error("unimplemented");
    }
  }

  Int Player2Score() const {
    if constexpr (If(EnableAbsoluteScore)) {
      return (TotalScore - Score) / 2;
    } else {
      throw std::runtime_error("unimplemented");
    }
  }

  bool IsPlayer1Turn() const {
    if constexpr (If(EnableRelativeScore)) {
      return Turn.IsPlayer1Turn();
    } else {
      throw std::runtime_error("unimplemented");
    }
  }

  bool IsPlayer2Turn() const {
    if constexpr (If(EnableRelativeScore)) {
      return Turn.IsPlayer2Turn();
    } else {
      throw std::runtime_error("unimplemented");
    }
  }

  Edge FindNotContainsEdgeInBox(Box box) const {
    if constexpr (If(EnableEdgeCount)) {
      Assert(Counter.At(box) == 3);
      for (const Edge edge : box.NearEdges()) {
        if (NotContains(edge)) {
          return edge;
        }
      }
      throw std::runtime_error("unreachable");
    } else {
      throw std::runtime_error("unimplemented");
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
    if constexpr (If(EnableScoreableCounting)) {
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
      throw std::runtime_error("unimplemented");
    }
  }

  uint8_t EdgeCount(Box box) const {
    if constexpr (If(EnableEdgeCount)) {
      return Counter.At(box);
    } else {
      throw std::runtime_error("unimplemented");
    }
  }

  uint8_t MaxEdgeCount(Edge edge) const {
    if constexpr (If(EnableEdgeCount)) {
      const List<Box, 2>& nearBoxes = edge.NearBoxes();
      return std::max(Counter.At(nearBoxes.Front()), Counter.At(nearBoxes.Back()));
    } else {
      throw std::runtime_error("unimplemented");
    }
  }

  class None {
    None(const None&) = delete;
    None& operator=(const None&) = delete;
  };
  template <BoardType Flag, typename T>
  using Member = std::conditional_t<If(EnableEdgeCount), T, None>;

  Int Step = 0;
  Array<Edge, Edge::Max> Edges;
  Array<Int, Edge::Max> EdgeIndexes;

  Member<EnableEdgeCount, Array<uint8_t, Box::Max>> Counter;
  Member<EnableScoreableCounting, Queue<Edge, Edge::Max>> ScoreableEdges;
  Member<EnableRelativeScore, Int> Score;
  Member<EnableRelativeScore, Turn> Turn;
  Member<EnableAbsoluteScore, Int> TotalScore;
  Member<EnableLogging, std::chrono::system_clock::time_point> LastUpdateTime;

  template <typename ToBoard>
  operator ToBoard() {
    ToBoard to;

    to.Step = Step;
    to.Edges = Edges;
    to.EdgeIndexes = EdgeIndexes;

    if constexpr (ToBoard::If(EnableEdgeCount)) {
      static_assert(If(EnableEdgeCount));
      to.Counter = Counter;
    }
    if constexpr (ToBoard::If(EnableScoreableCounting)) {
      static_assert(If(EnableScoreableCounting));
      to.ScoreableEdges = ScoreableEdges;
    }
    if constexpr (ToBoard::If(EnableRelativeScore)) {
      static_assert(If(EnableRelativeScore));
      to.Score = Score;
      to.Turn = Turn;
    }
    if constexpr (ToBoard::If(EnableAbsoluteScore)) {
      static_assert(If(EnableAbsoluteScore));
      to.TotalScore = TotalScore;
    }
    if constexpr (ToBoard::If(EnableLogging)) {
      static_assert(If(EnableLogging));
      to.LastUpdateTime = LastUpdateTime;
    }

    return to;
  }

  template <typename FromBoard>
  void operator=(const FromBoard& from) {
    Step = from.Step;
    Edges = from.Edges;
    EdgeIndexes = from.EdgeIndexes;

    if constexpr (If(EnableEdgeCount)) {
      static_assert(FromBoard::If(EnableEdgeCount));
      Counter = from.Counter;
    }
    if constexpr (If(EnableScoreableCounting)) {
      if constexpr (FromBoard::If(EnableScoreableCounting)) {
        ScoreableEdges = from.ScoreableEdges;
      } else {
        ScoreableEdges.Clear();
      }
    }
    if constexpr (If(EnableRelativeScore)) {
      static_assert(FromBoard::If(EnableRelativeScore));
      Score = from.Score;
      Turn = from.Turn;
    }
    if constexpr (If(EnableAbsoluteScore)) {
      static_assert(FromBoard::If(EnableAbsoluteScore));
      TotalScore = from.TotalScore;
    }
    if constexpr (If(EnableLogging)) {
      static_assert(FromBoard::If(EnableLogging));
      LastUpdateTime = from.LastUpdateTime;
    }
  }
};

}  // namespace __detail__::board

using enum __detail__::board::BoardType;
using EdgeCountableBoard = __detail__::board::Board<EnableEdgeCount>;
using RelativeScoreBoard = __detail__::board::Board<EnableRelativeScore>;
using AbsoluteScoreBoard = __detail__::board::Board<EnableAbsoluteScore>;
using LoggingBoard = __detail__::board::Board<EnableLogging>;
using ScoreableEdgeBoard = __detail__::board::Board<EnableScoreableCounting>;

}  // namespace dab