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

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <nlohmann/json.hpp>
#include <numeric>
#include <unordered_set>

#include "Iterable.h"
#include "Model.h"
#include "Tools.h"

namespace dab {

namespace __detail__::board {

namespace config {

static constexpr int EnableEdgeCount = 1 << 0;
static constexpr int EnableRelativeScore = 1 << 1;
static constexpr int EnableAbsoluteScore = 1 << 2;
static constexpr int EnableLogging = 1 << 3;
static constexpr int EnableScoreableCount = 1 << 4;
static constexpr int EnableHashValue = 1 << 5;
static constexpr int EnableOwner = 1 << 6;
static constexpr int MaxFlag = 1 << 7;

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
  if (HasFlag(config, EnableOwner)) {
    fixedConfig |= EnableEdgeCount | EnableRelativeScore;
  }
  return fixedConfig;
}

}  // namespace config

using namespace config;

struct BasicMixin {
  Int Step = 0;
  Array<Edge, Edge::Max> Edges;
  Array<Int, Edge::Max> EdgeIndexes;
};

struct EdgeCountMixin {
  Array<uint8_t, Box::Max> Counter;
};

struct ScoreableCountMixin {
  Queue<Edge, Edge::Max> ScoreableEdges;
};

struct RelativeScoreMixin {
  static constexpr Int Player1Turn = 1;
  static constexpr Int Player2Turn = -Player1Turn;

  Int Score = 0;
  Int Turn = Player1Turn;
};

struct AbsoluteScoreMixin {
  Int TotalScore;
};

struct LoggingMixin {
  std::chrono::system_clock::time_point LastUpdateTime;
  int64_t Player1MovingTime = 0;
  int64_t Player2MovingTime = 0;
};

struct HashValueMixin {
  static Array<uint64_t, Edge::Max> HashMapper;

  uint64_t HashValue;
};

inline Array<uint64_t, Edge::Max> HashValueMixin::HashMapper = []() -> Array<uint64_t, Edge::Max> {
  Random random;
  Array<uint64_t, Edge::Max> result;
  std::unordered_set<uint64_t> visited;
  for (uint64_t& v : result) {
    v = random.Range(static_cast<uint64_t>(1), std::numeric_limits<uint64_t>::max());
    while (visited.contains(v)) {
      v = random.Range(static_cast<uint64_t>(1), std::numeric_limits<uint64_t>::max());
    }
    visited.insert(v);
  }
  return result;
}();

enum class Owner {
  None,
  Player1,
  Player2,
};

struct OwnerMixin {
  Array<Owner, Edge::Max> EdgeOwner;
  Array<Owner, Box::Max> BoxOwner;
};

template <int Config>
class BoardImpl : BasicMixin,
                  Mixin<HasFlag(Config, EnableEdgeCount), EdgeCountMixin>,
                  Mixin<HasFlag(Config, EnableScoreableCount), ScoreableCountMixin>,
                  Mixin<HasFlag(Config, EnableRelativeScore), RelativeScoreMixin>,
                  Mixin<HasFlag(Config, EnableAbsoluteScore), AbsoluteScoreMixin>,
                  Mixin<HasFlag(Config, EnableLogging), LoggingMixin>,
                  Mixin<HasFlag(Config, EnableHashValue), HashValueMixin>,
                  Mixin<HasFlag(Config, EnableOwner), OwnerMixin> {
  template <int>
  friend class BoardImpl;
  static constexpr bool HasFlag(int flag) { return (Config & flag) != 0; }
  static_assert(Config == FixedConfig(Config));

 public:
  constexpr BoardImpl() { Reset(); }

  constexpr void Reset();
  constexpr Int Add(Edge edge);
  constexpr bool Contains(Edge edge) const { return EdgeIndexes.At(edge) < Step; }
  constexpr bool NotContains(Edge edge) const { return EdgeIndexes.At(edge) >= Step; }
  constexpr Span<const Edge> EmptyEdges() const { return {Edges.begin() + Step, Edges.end()}; }
  constexpr Span<const Edge> MoveRecord() const { return {Edges.begin(), Step}; }
  constexpr bool Gaming() const { return Step < Edge::Max; }
  constexpr Int RemainStep() const { return Edge::Max - Step; }
  constexpr Int NowStep() const { return Step; }
  constexpr uint64_t Hash() const { return this->HashValue; }
  constexpr uint8_t EdgeCount(Box box) const { return this->Counter.At(box); }
  constexpr uint8_t MaxEdgeCount(Edge edge) const;
  constexpr bool Scoreable(Edge edge) const { return MaxEdgeCount(edge) == 3; }
  constexpr Int RelativeScore() const { return this->Score; }
  constexpr Int GetTurn() const { return this->Turn; }
  constexpr bool IsPlayer1Turn() const { return this->Turn == this->Player1Turn; }
  constexpr bool IsPlayer2Turn() const { return this->Turn == this->Player2Turn; }
  constexpr Int Player1Score() const { return (this->TotalScore + this->Score) / 2; }
  constexpr Int Player2Score() const { return (this->TotalScore - this->Score) / 2; }
  constexpr double Player1MovingTimeSecond() const { return this->Player1MovingTime / 1000.0; }
  constexpr double Player2MovingTimeSecond() const { return this->Player2MovingTime / 1000.0; }
  constexpr Owner NowOwner() const { return IsPlayer1Turn() ? Owner::Player1 : Owner::Player2; }
  constexpr Owner GetOwner(Edge edge) const { return this->EdgeOwner.At(edge); }
  constexpr Owner GetOwner(Box box) const { return this->BoxOwner.At(box); }
  constexpr Edge FindNotContainsEdgeInBox(Box box) const;
  constexpr Int FindScoreableEdge();
  constexpr Int MaxObtainableScore(Int endScore);

  constexpr BoardImpl& operator=(const BoardImpl& other) = default;
  template <typename Other>
  constexpr BoardImpl& operator=(const Other& other);
  template <typename Other>
  constexpr bool operator==(const Other& other) const;
  constexpr operator nlohmann::ordered_json() const;
};

template <int Config>
constexpr void BoardImpl<Config>::Reset() {
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
    this->Turn = this->Player1Turn;
  }
  if constexpr (HasFlag(EnableAbsoluteScore)) {
    this->TotalScore = 0;
  }
  if constexpr (HasFlag(EnableLogging)) {
    this->LastUpdateTime = std::chrono::system_clock::now();
    this->Player1MovingTime = 0;
    this->Player2MovingTime = 0;
  }
  if constexpr (HasFlag(EnableHashValue)) {
    this->HashValue = 0;
  }
  if constexpr (HasFlag(EnableOwner)) {
    std::fill(this->EdgeOwner.begin(), this->EdgeOwner.end(), Owner::None);
    std::fill(this->BoxOwner.begin(), this->BoxOwner.end(), Owner::None);
  }
}

template <int Config>
constexpr Int BoardImpl<Config>::Add(Edge edge) {
  Assert(NotContains(edge), K(MoveRecord()), K(edge));
  const Edge nowEdge = Edges.At(Step);
  const Int edgeIndex = EdgeIndexes.At(edge);
  Assert(Edges.At(edgeIndex) == edge, K(Edges), K(edgeIndex), K(edge));
  Assert(edgeIndex >= Step, K(edgeIndex), K(Step));
  Edges.At(Step) = edge;
  Edges.At(edgeIndex) = nowEdge;
  EdgeIndexes.At(edge) = Step;
  EdgeIndexes.At(nowEdge) = edgeIndex;
  ++Step;
  if constexpr (HasFlag(EnableHashValue)) {
    this->HashValue += this->HashMapper.At(edge);
  }
  if constexpr (HasFlag(EnableOwner)) {
    this->EdgeOwner.At(edge) = NowOwner();
  }
  Int score = 0;
  if constexpr (HasFlag(EnableEdgeCount)) {
    for (const Box box : edge.NearBoxes()) {
      const uint8_t num = ++this->Counter.At(box);
      Assert(num <= 4, K(num), K(this->Counter));
      if (num == 4) {
        ++score;
        if constexpr (HasFlag(EnableOwner)) {
          this->BoxOwner.At(box) = NowOwner();
        }
      }
      if constexpr (HasFlag(EnableScoreableCount)) {
        if (num == 3) {
          this->ScoreableEdges.Append(FindNotContainsEdgeInBox(box));
        }
      }
    }
    if constexpr (HasFlag(EnableRelativeScore)) {
      const Int turn = this->Turn;
      if (score > 0) {
        this->Score += score * this->Turn;
      } else {
        this->Turn = -this->Turn;
      }
      if constexpr (HasFlag(EnableAbsoluteScore)) {
        this->TotalScore += score;
      }
      if constexpr (HasFlag(EnableLogging)) {
        const Int step = NowStep();
        const auto now = std::chrono::system_clock::now();
        const int64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->LastUpdateTime).count();
        if (turn == this->Player1Turn) {
          this->Player1MovingTime += time;
        } else {
          this->Player2MovingTime += time;
        }
        LogInfo({
            {"Step", step},
            {"Turn", turn == this->Player1Turn ? 1 : 2},
            {"Move", static_cast<Int>(edge)},
            {"Score", {{"Player1", Player1Score()}, {"Player2", Player2Score()}}},
            {"Time", static_cast<double>(time) / 1000.0},
        });
        if (!Gaming()) {
          if (RelativeScore() > 0) {
            LogInfo({{"Winner", "Player1"}, {"Score", {{"Player1", Player1Score()}, {"Player2", Player2Score()}}}});
          } else if (RelativeScore() < 0) {
            LogInfo({{"Winner", "Player2"}, {"Score", {{"Player1", Player1Score()}, {"Player2", Player2Score()}}}});
          } else {
            LogInfo({{"Winner", "Draw"}});
          }
        }
        this->LastUpdateTime = now;
      }
    }
  }
  return score;
}

template <int Config>
constexpr Edge BoardImpl<Config>::FindNotContainsEdgeInBox(Box box) const {
  Assert(this->Counter.At(box) == 3, K(box), K(this->Counter), K(this->Counter.At(box)));
  for (const Edge edge : box.NearEdges()) {
    if (NotContains(edge)) {
      return edge;
    }
  }
  std::unreachable();
}

template <int Config>
constexpr Int BoardImpl<Config>::FindScoreableEdge() {
  for (const Edge edge : EmptyEdges()) {
    if (Scoreable(edge)) {
      this->ScoreableEdges.Append(edge);
    }
  }
  return this->ScoreableEdges.Size();
}

template <int Config>
constexpr Int BoardImpl<Config>::MaxObtainableScore(Int endScore) {
  Int score = 0;
  while (Gaming() && score < endScore) {
    if (this->ScoreableEdges.Empty()) {
      Assert(FindScoreableEdge() == 0, K(FindScoreableEdge()));
      break;
    }
    const Edge edge = this->ScoreableEdges.Pop();
    if (Contains(edge)) {
      continue;
    }
    Assert(Scoreable(edge), K(edge), K(MaxEdgeCount(edge)));
    score += Add(edge);
  }
  return score;
}

template <int Config>
constexpr uint8_t BoardImpl<Config>::MaxEdgeCount(Edge edge) const {
  const List<Box, 2>& nearBoxes = edge.NearBoxes();
  return std::max(this->Counter.At(nearBoxes.Front()), this->Counter.At(nearBoxes.Back()));
}

template <int Config>
template <typename Other>
constexpr BoardImpl<Config>& BoardImpl<Config>::operator=(const Other& other) {
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
    this->Player1MovingTime = other.Player1MovingTime;
    this->Player2MovingTime = other.Player2MovingTime;
  }
  if constexpr (HasFlag(EnableHashValue)) {
    static_assert(Other::HasFlag(EnableHashValue));
    this->HashValue = other.HashValue;
  }
  if constexpr (HasFlag(EnableOwner)) {
    static_assert(Other::HasFlag(EnableOwner));
    this->EdgeOwner = other.EdgeOwner;
    this->BoxOwner = other.BoxOwner;
  }
  return *this;
}

template <int Config>
template <typename Other>
constexpr bool BoardImpl<Config>::operator==(const Other& other) const {
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
constexpr BoardImpl<Config>::operator nlohmann::ordered_json() const {
  List<Edge, Edge::Max> moveRecord(MoveRecord());
  std::ranges::sort(moveRecord);
  return moveRecord;
}

static_assert(sizeof(BoardImpl<0>) == sizeof(BasicMixin));

template <int Config>
using Board = BoardImpl<FixedConfig(Config)>;

}  // namespace __detail__::board

using namespace __detail__::board::config;
using __detail__::board::Board;
using __detail__::board::Owner;
using BasicBoard = Board<0>;
using HashValueBoard = Board<EnableHashValue>;
using EdgeCountBoard = Board<EnableEdgeCount | EnableHashValue>;
using RelativeScoreBoard = Board<EnableRelativeScore | EnableHashValue>;
using AbsoluteScoreBoard = Board<EnableAbsoluteScore | EnableHashValue>;
using GameBoard = Board<EnableLogging | EnableOwner | EnableHashValue>;
using ScoreableCountBoard = Board<EnableScoreableCount>;

}  // namespace dab

namespace std {

using namespace dab::__detail__::board;

template <int Config>
  requires(HasFlag(Config, EnableHashValue))
struct hash<BoardImpl<Config>> {
  constexpr uint64_t operator()(const BoardImpl<Config>& board) const { return board.Hash(); }
};

}  // namespace std