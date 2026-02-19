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

#include "Board.h"

#include <chrono>
#include <numeric>

#include "Dab/Tools.h"

namespace dab::__detail__::board {

template <int Config>
void Board<Config>::Reset() {
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

template <int Config>
Int Board<Config>::Add(Edge edge) {
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
      const Turn turn = this->Turn;
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
        const int64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->LastUpdateTime).count();
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

template <int Config>
Int Board<Config>::RelativeScore() const {
  if constexpr (HasFlag(EnableRelativeScore)) {
    return this->Score;
  } else {
    throw UnimplementedError();
  }
}

template <int Config>
Int Board<Config>::Player1Score() const {
  if constexpr (HasFlag(EnableAbsoluteScore)) {
    return (this->TotalScore + this->Score) / 2;
  } else {
    throw UnimplementedError();
  }
}

template <int Config>
Int Board<Config>::Player2Score() const {
  if constexpr (HasFlag(EnableAbsoluteScore)) {
    return (this->TotalScore - this->Score) / 2;
  } else {
    throw UnimplementedError();
  }
}

template <int Config>
Turn Board<Config>::GetTurn() const {
  if constexpr (HasFlag(EnableRelativeScore)) {
    return this->Turn;
  } else {
    throw UnimplementedError();
  }
}

template <int Config>
bool Board<Config>::IsPlayer1Turn() const {
  if constexpr (HasFlag(EnableRelativeScore)) {
    return this->Turn.IsPlayer1Turn();
  } else {
    throw UnimplementedError();
  }
}

template <int Config>
bool Board<Config>::IsPlayer2Turn() const {
  if constexpr (HasFlag(EnableRelativeScore)) {
    return this->Turn.IsPlayer2Turn();
  } else {
    throw UnimplementedError();
  }
}

template <int Config>
Edge Board<Config>::FindNotContainsEdgeInBox(Box box) const {
  if constexpr (HasFlag(EnableEdgeCount)) {
    Assert(this->Counter.At(box) == 3);
    for (const Edge edge : box.NearEdges()) {
      if (NotContains(edge)) {
        return edge;
      }
    }
    throw std::runtime_error("unreachable");
  } else {
    throw UnimplementedError();
  }
}

template <int Config>
Int Board<Config>::FindScoreableEdge() {
  if constexpr (HasFlag(EnableScoreableCounting)) {
    for (const Edge edge : EmptyEdges()) {
      if (Scoreable(edge)) {
        this->ScoreableEdges.Append(edge);
      }
    }
    return this->ScoreableEdges.Size();
  } else {
    throw UnimplementedError();
  }
}

template <int Config>
Int Board<Config>::MaxObtainableScore(Int endScore) {
  if constexpr (HasFlag(EnableScoreableCounting)) {
    Int score = 0;
    while (Gaming() && score < endScore) {
      if (this->ScoreableEdges.Empty()) {
        Assert(FindScoreableEdge() == 0);
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
    throw UnimplementedError();
  }
}

template <int Config>
uint8_t Board<Config>::EdgeCount(Box box) const {
  if constexpr (HasFlag(EnableEdgeCount)) {
    return this->Counter.At(box);
  } else {
    throw UnimplementedError();
  }
}

template <int Config>
uint8_t Board<Config>::MaxEdgeCount(Edge edge) const {
  if constexpr (HasFlag(EnableEdgeCount)) {
    const List<Box, 2>& nearBoxes = edge.NearBoxes();
    return std::max(this->Counter.At(nearBoxes.Front()), this->Counter.At(nearBoxes.Back()));
  } else {
    throw UnimplementedError();
  }
}

template <int Config>
bool Board<Config>::Scoreable(Edge edge) const {
  if constexpr (HasFlag(EnableEdgeCount)) {
    return MaxEdgeCount(edge) == 3;
  } else {
    throw UnimplementedError();
  }
}

template class Board<0>;
template class Board<EnableEdgeCount>;
template class Board<EnableRelativeScore>;
template class Board<EnableAbsoluteScore>;
template class Board<EnableLogging>;
template class Board<EnableScoreableCounting>;

struct TestBasicBoardSize {
  Int Step = 0;
  Array<Edge, Edge::Max> Edges;
  Array<Int, Edge::Max> EdgeIndexes;
};
static_assert(sizeof(Board<0>) == sizeof(TestBasicBoardSize));

}  // namespace dab::__detail__::board
