#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <unordered_set>

#include "Iterable.h"
#include "Model.h"

namespace dab::__detail__ {

namespace board {

static constexpr int EnableEdgeCount = 1 << 0;
static constexpr int EnableRelativeScore = 1 << 1;
static constexpr int EnableAbsoluteScore = 1 << 2;
static constexpr int EnableScoreableCount = 1 << 3;
static constexpr int EnableHashValue = 1 << 5;
static constexpr int EnableOwner = 1 << 6;
static constexpr int MaxFlag = 1 << 7;

static constexpr Int Player1Turn = 1;
static constexpr Int Player2Turn = -Player1Turn;

static constexpr bool HasFlag(int config, int flag) { return (config & flag) != 0; }

static constexpr int FixedConfig(int config) {
  config %= MaxFlag;
  if (HasFlag(config, EnableRelativeScore)) {
    config |= EnableEdgeCount;
  }
  if (HasFlag(config, EnableAbsoluteScore)) {
    config |= EnableEdgeCount | EnableRelativeScore;
  }
  if (HasFlag(config, EnableScoreableCount)) {
    config |= EnableEdgeCount;
  }
  if (HasFlag(config, EnableOwner)) {
    config |= EnableEdgeCount | EnableRelativeScore;
  }
  return config;
}

template <bool Bp, typename T>
using Mixin = std::conditional_t<Bp, T, std::type_identity<T>>;

struct BasicMixin {
  Int Step = 0;
  Array<Edge, Edge::Max> Edges;
  Array<Int, Edge::Max> EdgeIndexes;
};

struct EdgeCountMixin {
  Array<std::uint8_t, Box::Max> Counter;
};

struct ScoreableCountMixin {
  Queue<Edge, Edge::Max> ScoreableEdges;
};

struct RelativeScoreMixin {
  Int Score = 0;
  Int Turn = Player1Turn;
};

struct AbsoluteScoreMixin {
  Int TotalScore;
};

struct HashValueMixin {
  static Array<std::uint64_t, Edge::Max> HashMapper;

  std::uint64_t HashValue;
};

inline Array<std::uint64_t, Edge::Max> HashValueMixin::HashMapper = []() -> Array<std::uint64_t, Edge::Max> {
  Random random;
  Array<std::uint64_t, Edge::Max> result;
  std::unordered_set<std::uint64_t> visited;
  auto randv = [&]() -> std::uint64_t {
    return random.Range(static_cast<std::uint64_t>(1), std::numeric_limits<std::uint64_t>::max());
  };
  for (std::uint64_t& v : result) {
    v = randv();
    while (visited.contains(v)) {
      v = randv();
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
  constexpr std::uint64_t Hash() const { return this->HashValue; }
  constexpr std::uint8_t EdgeCount(Box box) const { return this->Counter.At(box); }
  constexpr std::uint8_t MaxEdgeCount(Edge edge) const;
  constexpr bool Scoreable(Edge edge) const { return MaxEdgeCount(edge) == 3; }
  constexpr Int RelativeScore() const { return this->Score; }
  constexpr Int GetTurn() const { return this->Turn; }
  constexpr bool IsPlayer1Turn() const { return this->Turn == Player1Turn; }
  constexpr bool IsPlayer2Turn() const { return this->Turn == Player2Turn; }
  constexpr Int Player1Score() const { return (this->TotalScore + this->Score) / 2; }
  constexpr Int Player2Score() const { return (this->TotalScore - this->Score) / 2; }
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
    this->Turn = Player1Turn;
  }
  if constexpr (HasFlag(EnableAbsoluteScore)) {
    this->TotalScore = 0;
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
  assert(NotContains(edge));
  const Edge nowEdge = Edges.At(Step);
  const Int edgeIndex = EdgeIndexes.At(edge);
  assert(Edges.At(edgeIndex) == edge);
  assert(edgeIndex >= Step);
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
      const std::uint8_t num = ++this->Counter.At(box);
      assert(num <= 4);
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
      if (score > 0) {
        this->Score += score * this->Turn;
      } else {
        this->Turn = -this->Turn;
      }
      if constexpr (HasFlag(EnableAbsoluteScore)) {
        this->TotalScore += score;
      }
    }
  }
  return score;
}

template <int Config>
constexpr Edge BoardImpl<Config>::FindNotContainsEdgeInBox(Box box) const {
  assert(this->Counter.At(box) == 3);
  return *std::ranges::find_if(box.NearEdges(), [&](Edge edge) -> bool { return NotContains(edge); });
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
      assert(FindScoreableEdge() == 0);
      break;
    }
    const Edge edge = this->ScoreableEdges.Pop();
    if (Contains(edge)) {
      continue;
    }
    assert(Scoreable(edge));
    score += Add(edge);
  }
  return score;
}

template <int Config>
constexpr std::uint8_t BoardImpl<Config>::MaxEdgeCount(Edge edge) const {
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
  return std::ranges::all_of(EmptyEdges(), [&](Edge edge) { return !other.Contains(edge); });
}

static_assert(sizeof(BoardImpl<0>) == sizeof(BasicMixin));

template <int Config>
using Board = BoardImpl<FixedConfig(Config)>;

}  // namespace board

using board::Owner;
using BasicBoard = board::Board<0>;
using HashValueBoard = board::Board<board::EnableHashValue>;
using EdgeCountBoard = board::Board<board::EnableEdgeCount | board::EnableHashValue>;
using RelativeScoreBoard = board::Board<board::EnableRelativeScore | board::EnableHashValue>;
using AbsoluteScoreBoard = board::Board<board::EnableAbsoluteScore | board::EnableHashValue>;
using GameBoard = board::Board<board::EnableAbsoluteScore | board::EnableOwner | board::EnableHashValue>;
using ScoreableCountBoard = board::Board<board::EnableScoreableCount>;

}  // namespace dab::__detail__

namespace std {

using namespace dab::__detail__::board;

template <int Config>
  requires(HasFlag(Config, EnableHashValue))
struct hash<BoardImpl<Config>> {
  constexpr std::uint64_t operator()(const BoardImpl<Config>& board) const { return board.Hash(); }
};

}  // namespace std