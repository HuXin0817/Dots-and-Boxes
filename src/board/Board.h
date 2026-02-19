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
#include <cstdint>
#include <limits>
#include <numeric>
#include <stdexcept>

namespace dab::__detail__::board {

static constexpr int EnableEdgeCount = 1 << 0;
static constexpr int EnableRelativeScore = 1 << 1;
static constexpr int EnableAbsoluteScore = 1 << 2;
static constexpr int EnableLogging = 1 << 3;
static constexpr int EnableScoreableCounting = 1 << 4;
static constexpr int EnableZobristHash = 1 << 5;

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
  return fixedConfig;
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
  Int Score = 0;
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

template <int Config, bool Enabled>
struct HashMixin;

template <int Config>
struct HashMixin<Config, true> {
  static Array<uint32_t, Edge::Max> HashMapper;

  uint32_t HashValue;
};

template <int Config>
Array<uint32_t, Edge::Max> HashMixin<Config, true>::HashMapper = []() -> Array<uint32_t, Edge::Max> {
  Random random;
  Array<uint32_t, Edge::Max> result;
  for (uint32_t& v : result) {
    v = random.Range(std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max());
  }
  return result;
}();

template <int Config>
struct HashMixin<Config, false> {};

template <int Config>
class BoardImpl : private EdgeCountMixin<Config, HasFlag(FixedConfig(Config), EnableEdgeCount)>,
                  private ScoreableCountingMixin<Config, HasFlag(FixedConfig(Config), EnableScoreableCounting)>,
                  private RelativeScoreMixin<Config, HasFlag(FixedConfig(Config), EnableRelativeScore)>,
                  private AbsoluteScoreMixin<Config, HasFlag(FixedConfig(Config), EnableAbsoluteScore)>,
                  private LoggingMixin<Config, HasFlag(FixedConfig(Config), EnableLogging)>,
                  private HashMixin<Config, HasFlag(FixedConfig(Config), EnableZobristHash)> {
  template <int>
  friend class BoardImpl;
  static std::logic_error UnimplementedError() { return std::logic_error("unimplemented"); }
  static constexpr bool HasFlag(int flag) { return (FixedConfig(Config) & flag) != 0; }

 public:
  BoardImpl() { Reset(); }

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

  BoardImpl& operator=(const BoardImpl& from) = default;
  template <typename FromBoard>
  BoardImpl& operator=(const FromBoard& from);

  template <typename Other>
  bool operator==(const Other& from) const;

 private:
  Int Step = 0;
  Array<Edge, Edge::Max> Edges;
  Array<Int, Edge::Max> EdgeIndexes;
};

template <int Config>
void BoardImpl<Config>::Reset() {
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
  if constexpr (HasFlag(EnableZobristHash)) {
    this->HashValue = 0;
  }
}

template <int Config>
Int BoardImpl<Config>::Add(Edge edge) {
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
  if constexpr (HasFlag(EnableZobristHash)) {
    this->HashValue += this->HashMapper.At(edge);
  }
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
Int BoardImpl<Config>::RelativeScore() const {
  if constexpr (HasFlag(EnableRelativeScore)) {
    return this->Score;
  } else {
    throw UnimplementedError();
  }
}

template <int Config>
Int BoardImpl<Config>::Player1Score() const {
  if constexpr (HasFlag(EnableAbsoluteScore)) {
    return (this->TotalScore + this->Score) / 2;
  } else {
    throw UnimplementedError();
  }
}

template <int Config>
Int BoardImpl<Config>::Player2Score() const {
  if constexpr (HasFlag(EnableAbsoluteScore)) {
    return (this->TotalScore - this->Score) / 2;
  } else {
    throw UnimplementedError();
  }
}

template <int Config>
Turn BoardImpl<Config>::GetTurn() const {
  if constexpr (HasFlag(EnableRelativeScore)) {
    return this->Turn;
  } else {
    throw UnimplementedError();
  }
}

template <int Config>
bool BoardImpl<Config>::IsPlayer1Turn() const {
  if constexpr (HasFlag(EnableRelativeScore)) {
    return this->Turn.IsPlayer1Turn();
  } else {
    throw UnimplementedError();
  }
}

template <int Config>
bool BoardImpl<Config>::IsPlayer2Turn() const {
  if constexpr (HasFlag(EnableRelativeScore)) {
    return this->Turn.IsPlayer2Turn();
  } else {
    throw UnimplementedError();
  }
}

template <int Config>
Edge BoardImpl<Config>::FindNotContainsEdgeInBox(Box box) const {
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
Int BoardImpl<Config>::FindScoreableEdge() {
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
Int BoardImpl<Config>::MaxObtainableScore(Int endScore) {
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
uint8_t BoardImpl<Config>::EdgeCount(Box box) const {
  if constexpr (HasFlag(EnableEdgeCount)) {
    return this->Counter.At(box);
  } else {
    throw UnimplementedError();
  }
}

template <int Config>
uint8_t BoardImpl<Config>::MaxEdgeCount(Edge edge) const {
  if constexpr (HasFlag(EnableEdgeCount)) {
    const List<Box, 2>& nearBoxes = edge.NearBoxes();
    return std::max(this->Counter.At(nearBoxes.Front()), this->Counter.At(nearBoxes.Back()));
  } else {
    throw UnimplementedError();
  }
}

template <int Config>
bool BoardImpl<Config>::Scoreable(Edge edge) const {
  if constexpr (HasFlag(EnableEdgeCount)) {
    return MaxEdgeCount(edge) == 3;
  } else {
    throw UnimplementedError();
  }
}

template <int Config>
template <typename FromBoard>
BoardImpl<Config>& BoardImpl<Config>::operator=(const FromBoard& from) {
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
      FindScoreableEdge();
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
  if constexpr (HasFlag(EnableZobristHash)) {
    static_assert(FromBoard::HasFlag(EnableZobristHash));
    this->HashValue = from.HashValue;
  }
  return *this;
}

template <int Config>
template <typename Other>
bool BoardImpl<Config>::operator==(const Other& other) const {
  if constexpr (HasFlag(EnableZobristHash) && other.HasFlag(EnableZobristHash)) {
    if (this->HashValue != other->HashValue) {
      return false;
    }
  }
  for (const Edge& edge : EmptyEdges()) {
    if (other.Contains(edge)) {
      return false;
    }
  }
  return true;
}

template <int Config>
using Board = BoardImpl<FixedConfig(Config)>;

}  // namespace dab::__detail__::board

namespace std {

using namespace dab::__detail__::board;

template <int Config>
class hash<BoardImpl<Config>> {
  uint32_t operator()(const BoardImpl<Config>& board) {
    if constexpr (HasFlag(Config, EnableZobristHash)) {
      return board.HashValue;
    } else {
      throw BoardImpl<Config>::UnimplementedError;
    }
  }
};

}  // namespace std