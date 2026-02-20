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
#include <Dab/Tools.h>

#include <chrono>
#include <cstdint>
#include <functional>
#include <limits>
#include <numeric>

namespace dab::__detail__::board {

namespace config {
static constexpr int EnableEdgeCount = 1 << 0;
static constexpr int EnableRelativeScore = 1 << 1;
static constexpr int EnableAbsoluteScore = 1 << 2;
static constexpr int EnableLogging = 1 << 3;
static constexpr int EnableScoreableCount = 1 << 4;
static constexpr int EnableHashValue = 1 << 5;
static constexpr int MaxFlag = 1 << 6;

static constexpr bool HasFlag(int config, int flag) { return (config & flag) != 0; }

static constexpr int FixedConfig(int config) {
  int fixedConfig = config % MaxFlag;
  if (HasFlag(config, EnableRelativeScore)) {
    fixedConfig |= EnableEdgeCount;
  }
  if (HasFlag(config, EnableAbsoluteScore)) {
    fixedConfig |= EnableEdgeCount | EnableRelativeScore;
  }
  if (HasFlag(config, EnableLogging)) {
    fixedConfig |= EnableEdgeCount | EnableRelativeScore | EnableAbsoluteScore;
  }
  if (HasFlag(config, EnableScoreableCount)) {
    fixedConfig |= EnableEdgeCount;
  }
  return fixedConfig;
}

}  // namespace config

using namespace config;

template <bool Enabled>
struct EdgeCountMixin {};

template <>
struct EdgeCountMixin<true> {
  Array<uint8_t, Box::Max> Counter;
};

template <bool Enabled>
struct ScoreableCountingMixin {};

template <>
struct ScoreableCountingMixin<true> {
  Queue<Edge, Edge::Max> ScoreableEdges;
};

template <bool Enabled>
struct RelativeScoreMixin {};

template <>
struct RelativeScoreMixin<true> {
  Int Score = 0;
  Turn Turn;
};

template <bool Enabled>
struct AbsoluteScoreMixin {};

template <>
struct AbsoluteScoreMixin<true> {
  Int TotalScore;
};

template <bool Enabled>
struct LoggingMixin {};

template <>
struct LoggingMixin<true> {
  std::chrono::system_clock::time_point LastUpdateTime;
};

template <bool Enabled>
struct HashMixin {};

template <>
struct HashMixin<true> {
  static Array<uint32_t, Edge::Max> HashMapper;

  uint32_t HashValue;
};

inline Array<uint32_t, Edge::Max> HashMixin<true>::HashMapper = []() -> Array<uint32_t, Edge::Max> {
  Random random;
  Array<uint32_t, Edge::Max> result;
  for (uint32_t& v : result) {
    v = random.Range(std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max());
  }
  return result;
}();

template <int Config>
class BoardImpl : EdgeCountMixin<HasFlag(Config, EnableEdgeCount)>,
                  ScoreableCountingMixin<HasFlag(Config, EnableScoreableCount)>,
                  RelativeScoreMixin<HasFlag(Config, EnableRelativeScore)>,
                  AbsoluteScoreMixin<HasFlag(Config, EnableAbsoluteScore)>,
                  LoggingMixin<HasFlag(Config, EnableLogging)>,
                  HashMixin<HasFlag(Config, EnableHashValue)> {
  template <int>
  friend class BoardImpl;
  static constexpr bool HasFlag(int flag) { return (Config & flag) != 0; }
  static_assert(Config == FixedConfig(Config));

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
  uint32_t Hash() const { return this->HashValue; }
  uint8_t EdgeCount(Box box) const { return this->Counter.At(box); }
  uint8_t MaxEdgeCount(Edge edge) const;
  bool Scoreable(Edge edge) const { return MaxEdgeCount(edge) == 3; }
  Int RelativeScore() const { return this->Score; }
  Turn GetTurn() const { return this->Turn; }
  bool IsPlayer1Turn() const { return this->Turn.IsPlayer1Turn(); }
  bool IsPlayer2Turn() const { return this->Turn.IsPlayer2Turn(); }
  Int Player1Score() const { return (this->TotalScore + this->Score) / 2; }
  Int Player2Score() const { return (this->TotalScore - this->Score) / 2; }
  Edge FindNotContainsEdgeInBox(Box box) const;
  Int FindScoreableEdge();
  Int MaxObtainableScore(Int endScore);

  BoardImpl& operator=(const BoardImpl& other) = default;
  template <typename Other>
  BoardImpl& operator=(const Other& other);
  template <typename Other>
  bool operator==(const Other& other) const;

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
    std::ranges::fill(this->Counter, 0);
  }
  if constexpr (HasFlag(EnableScoreableCount)) {
    this->ScoreableEdges.Clear();
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
  if constexpr (HasFlag(EnableHashValue)) {
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
  if constexpr (HasFlag(EnableHashValue)) {
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
      if constexpr (HasFlag(EnableScoreableCount)) {
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
        const std::string scoreMap = std::format(R"({{"Player1":{},"Player2":{}}})", Player1Score(), Player2Score());
        LogInfo(R"({{"Step":{},"Turn":{},"Move":{},"Score":{},"Time":{}}})",
                step,
                turn.IsPlayer1Turn() ? 1 : 2,
                static_cast<Int>(edge),
                scoreMap,
                static_cast<double>(time) / 1000.0);
        if (!Gaming()) {
          if (RelativeScore() > 0) {
            LogInfo(R"({{"Winner":"Player1","Score":{}}})", scoreMap);
          } else if (RelativeScore() < 0) {
            LogInfo(R"({{"Winner":"Player2","Score":{}}})", scoreMap);
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
Edge BoardImpl<Config>::FindNotContainsEdgeInBox(Box box) const {
  Assert(this->Counter.At(box) == 3);
  for (const Edge edge : box.NearEdges()) {
    if (NotContains(edge)) {
      return edge;
    }
  }
  Assert(false);
  return Edge::Invalid;
}

template <int Config>
Int BoardImpl<Config>::FindScoreableEdge() {
  for (const Edge edge : EmptyEdges()) {
    if (Scoreable(edge)) {
      this->ScoreableEdges.Append(edge);
    }
  }
  return this->ScoreableEdges.Size();
}

template <int Config>
Int BoardImpl<Config>::MaxObtainableScore(Int endScore) {
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
    Assert(Scoreable(edge));
    score += Add(edge);
  }
  return score;
}

template <int Config>
uint8_t BoardImpl<Config>::MaxEdgeCount(Edge edge) const {
  const List<Box, 2>& nearBoxes = edge.NearBoxes();
  return std::max(this->Counter.At(nearBoxes.Front()), this->Counter.At(nearBoxes.Back()));
}

template <int Config>
template <typename Other>
BoardImpl<Config>& BoardImpl<Config>::operator=(const Other& other) {
  Step = other.Step;
  Edges = other.Edges;
  EdgeIndexes = other.EdgeIndexes;
  if constexpr (HasFlag(EnableEdgeCount)) {
    static_assert(Other::HasFlag(EnableEdgeCount));
    this->Counter = other.Counter;
  }
  if constexpr (HasFlag(EnableScoreableCount)) {
    if constexpr (Other::HasFlag(EnableScoreableCount)) {
      this->ScoreableEdges = other.ScoreableEdges;
    } else {
      this->ScoreableEdges.Clear();
      FindScoreableEdge();
    }
  }
  if constexpr (HasFlag(EnableRelativeScore)) {
    static_assert(Other::HasFlag(EnableRelativeScore));
    this->Score = other.Score;
    this->Turn = other.Turn;
  }
  if constexpr (HasFlag(EnableAbsoluteScore)) {
    static_assert(Other::HasFlag(EnableAbsoluteScore));
    this->TotalScore = other.TotalScore;
  }
  if constexpr (HasFlag(EnableLogging)) {
    static_assert(Other::HasFlag(EnableLogging));
    this->LastUpdateTime = other.LastUpdateTime;
  }
  if constexpr (HasFlag(EnableHashValue)) {
    static_assert(Other::HasFlag(EnableHashValue));
    this->HashValue = other.HashValue;
  }
  return *this;
}

template <int Config>
template <typename Other>
bool BoardImpl<Config>::operator==(const Other& other) const {
  if constexpr (HasFlag(EnableHashValue) && Other::HasFlag(EnableHashValue)) {
    if (this->HashValue != other.HashValue) {
      return false;
    }
  }
  if (Step != other.Step) {
    return false;
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
  requires(HasFlag(Config, EnableHashValue))
struct hash<BoardImpl<Config>> {
  uint32_t operator()(const BoardImpl<Config>& board) const { return board.Hash(); }
};

}  // namespace std