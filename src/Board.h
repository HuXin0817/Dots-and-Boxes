#pragma once

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <functional>
#include <numeric>
#include <print>

#include "Iterable.h"
#include "Model.h"
#include "Random.h"

namespace dab::__detail__::board {

static constexpr std::int64_t EnableEdgeCount = 1 << 0;
static constexpr std::int64_t EnableRelativeScore = 1 << 1;
static constexpr std::int64_t EnableAbsoluteScore = 1 << 2;
static constexpr std::int64_t EnableScoreableCount = 1 << 3;
static constexpr std::int64_t EnableHashValue = 1 << 5;
static constexpr std::int64_t EnableOwner = 1 << 6;
static constexpr std::int64_t EnableLogging = 1 << 7;
static constexpr std::int64_t MaxFlag = 1 << 8;

static constexpr Int Player1Turn = 1;
static constexpr Int Player2Turn = -Player1Turn;

static constexpr bool HasFlag(std::int64_t config, std::int64_t flag) { return (config & flag) != 0; }

static constexpr std::int64_t FixedConfig(std::int64_t config) {
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
  iterable::Array<model::Edge, model::Edge::Max> Edges;
  iterable::Array<Int, model::Edge::Max> EdgeIndexes;
};

struct EdgeCountMixin {
  iterable::Array<std::uint8_t, model::Box::Max> Counter;
};

struct ScoreableCountMixin {
  iterable::Queue<model::Edge, model::Edge::Max> ScoreableEdges;
};

struct RelativeScoreMixin {
  Int Score = 0;
  Int Turn = Player1Turn;
};

struct AbsoluteScoreMixin {
  Int TotalScore = 0;
};

struct HashValueMixin {
  std::uint64_t HashValue;
};

enum class Owner {
  None,
  Player1,
  Player2,
};

struct OwnerMixin {
  iterable::Array<Owner, model::Edge::Max> EdgeOwner;
  iterable::Array<Owner, model::Box::Max> BoxOwner;
};

static constexpr iterable::Array<std::uint64_t, model::Edge::Max> CreateHashMapper() {
  ZobristHash hash;
  iterable::Array<std::uint64_t, model::Edge::Max> result;
  for (std::uint64_t& i : result) {
    i = hash.Next();
  }
  return result;
}

static constexpr iterable::Array<std::uint64_t, model::Edge::Max> HashMapper = CreateHashMapper();

template <std::int64_t Config>
class BoardImpl : BasicMixin,
                  Mixin<HasFlag(Config, EnableEdgeCount), EdgeCountMixin>,
                  Mixin<HasFlag(Config, EnableScoreableCount), ScoreableCountMixin>,
                  Mixin<HasFlag(Config, EnableRelativeScore), RelativeScoreMixin>,
                  Mixin<HasFlag(Config, EnableAbsoluteScore), AbsoluteScoreMixin>,
                  Mixin<HasFlag(Config, EnableHashValue), HashValueMixin>,
                  Mixin<HasFlag(Config, EnableOwner), OwnerMixin> {
  template <std::int64_t>
  friend class BoardImpl;
  static constexpr bool HasFlag(std::int64_t flag) { return (Config & flag) != 0; }
  static_assert(Config == FixedConfig(Config));

 public:
  constexpr BoardImpl() { Reset(); }

  constexpr void Reset();
  constexpr Int Add(model::Edge edge);
  constexpr bool Contains(model::Edge edge) const { return EdgeIndexes.At(edge) < Step; }
  constexpr bool NotContains(model::Edge edge) const { return EdgeIndexes.At(edge) >= Step; }
  constexpr iterable::Span<const model::Edge> EmptyEdges() const { return {Edges.begin() + Step, Edges.end()}; }
  constexpr iterable::Span<const model::Edge> MoveRecord() const { return {Edges.begin(), Step}; }
  constexpr bool Gaming() const { return Step < model::Edge::Max; }
  constexpr Int RemainStep() const { return model::Edge::Max - Step; }
  constexpr Int NowStep() const { return Step; }
  constexpr std::uint64_t Hash() const { return this->HashValue; }
  constexpr std::uint8_t EdgeCount(model::Box box) const { return this->Counter.At(box); }
  constexpr std::uint8_t MaxEdgeCount(model::Edge edge) const;
  constexpr bool Scoreable(model::Edge edge) const { return MaxEdgeCount(edge) == 3; }
  constexpr Int RelativeScore() const { return this->Score; }
  constexpr Int GetTurn() const { return this->Turn; }
  constexpr bool IsPlayer1Turn() const { return this->Turn == Player1Turn; }
  constexpr bool IsPlayer2Turn() const { return this->Turn == Player2Turn; }
  constexpr Int Player1Score() const { return (this->TotalScore + this->Score) / 2; }
  constexpr Int Player2Score() const { return (this->TotalScore - this->Score) / 2; }
  constexpr Owner NowOwner() const { return IsPlayer1Turn() ? Owner::Player1 : Owner::Player2; }
  constexpr Owner GetOwner(model::Edge edge) const { return this->EdgeOwner.At(edge); }
  constexpr Owner GetOwner(model::Box box) const { return this->BoxOwner.At(box); }
  constexpr model::Edge FindNotContainsEdgeInBox(model::Box box) const;
  constexpr Int FindScoreableEdge();
  constexpr Int MaxObtainableScore(Int endScore);

  constexpr BoardImpl& operator=(const BoardImpl& other) = default;
  template <typename Other>
  constexpr BoardImpl& operator=(const Other& other);
  template <typename Other>
  constexpr bool operator==(const Other& other) const;
  template <typename Other>
  constexpr std::strong_ordering operator<=>(const Other& other) const;
};

template <std::int64_t Config>
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

template <std::int64_t Config>
constexpr Int BoardImpl<Config>::Add(model::Edge edge) {
  assert(NotContains(edge));
  const model::Edge nowEdge = Edges.At(Step);
  const Int edgeIndex = EdgeIndexes.At(edge);
  assert(Edges.At(edgeIndex) == edge);
  assert(edgeIndex >= Step);
  Edges.At(Step) = edge;
  Edges.At(edgeIndex) = nowEdge;
  EdgeIndexes.At(edge) = Step;
  EdgeIndexes.At(nowEdge) = edgeIndex;
  ++Step;
  if constexpr (HasFlag(EnableHashValue)) {
    this->HashValue += HashMapper.At(edge);
  }
  if constexpr (HasFlag(EnableOwner)) {
    this->EdgeOwner.At(edge) = NowOwner();
  }
  Int score = 0;
  if constexpr (HasFlag(EnableEdgeCount)) {
    for (const model::Box box : edge.NearBoxes()) {
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
  if constexpr (HasFlag(EnableLogging)) {
    Int step = Step - 1;
    Int turn = score == 0 ? -this->Turn : this->Turn;
    int player = turn == Player1Turn ? 1 : 2;
    std::println(R"({:%Y-%m-%d %H:%M:%S} {{"Step":{},"Player":{},"Move":{},"Score":{{"Player1":{},"Player2":{}}}}})",
                 std::chrono::system_clock::now(),
                 step,
                 player,
                 static_cast<Int>(edge),
                 Player1Score(),
                 Player2Score());
  }
  return score;
}

template <std::int64_t Config>
constexpr model::Edge BoardImpl<Config>::FindNotContainsEdgeInBox(model::Box box) const {
  assert(this->Counter.At(box) == 3);
  for (model::Edge edge : box.NearEdges()) {
    if (NotContains(edge)) {
      return edge;
    }
  }
  assert(false);
  return model::Edge::Invalid;
}

template <std::int64_t Config>
constexpr Int BoardImpl<Config>::FindScoreableEdge() {
  for (const model::Edge edge : EmptyEdges()) {
    if (Scoreable(edge)) {
      this->ScoreableEdges.Append(edge);
    }
  }
  return this->ScoreableEdges.Size();
}

template <std::int64_t Config>
constexpr Int BoardImpl<Config>::MaxObtainableScore(Int endScore) {
  Int score = 0;
  while (Gaming() && score < endScore) {
    if (this->ScoreableEdges.Empty()) {
      assert(FindScoreableEdge() == 0);
      break;
    }
    const model::Edge edge = this->ScoreableEdges.Pop();
    if (Contains(edge)) {
      continue;
    }
    assert(Scoreable(edge));
    score += Add(edge);
  }
  return score;
}

template <std::int64_t Config>
constexpr std::uint8_t BoardImpl<Config>::MaxEdgeCount(model::Edge edge) const {
  const iterable::List<model::Box, 2>& nearBoxes = edge.NearBoxes();
  return std::max(this->Counter.At(nearBoxes.Front()), this->Counter.At(nearBoxes.Back()));
}

template <std::int64_t Config>
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

template <std::int64_t Config>
template <typename Other>
constexpr bool BoardImpl<Config>::operator==(const Other& other) const {
  if constexpr (HasFlag(EnableHashValue) && Other::HasFlag(EnableHashValue)) {
    if (this->HashValue != other.HashValue) {
      return false;
    }
  }
  if (UNLIKELY(Step != other.Step)) {
    return false;
  }
  for (model::Edge edge : MoveRecord()) {
    if (UNLIKELY(other.NotContains(edge))) {
      return false;
    }
  }
  return true;
}

template <std::int64_t Config>
template <typename Other>
constexpr std::strong_ordering BoardImpl<Config>::operator<=>(const Other& other) const {
  if constexpr (HasFlag(EnableHashValue) && Other::HasFlag(EnableHashValue)) {
    if (std::strong_ordering cmp = this->HashValue <=> other.HashValue; cmp != std::strong_ordering::equal) {
      return cmp;
    }
  }
  if (std::strong_ordering cmp = Step <=> other.Step; cmp != std::strong_ordering::equal) {
    return cmp;
  }
  for (model::Edge edge : model::Iota<model::Edge>) {
    if (std::strong_ordering cmp = Contains(edge) <=> other.Contains(edge); cmp != std::strong_ordering::equal) {
      return cmp;
    }
  }
  return std::strong_ordering::equal;
}

static_assert(sizeof(BoardImpl<0>) == sizeof(BasicMixin));

template <std::int64_t Config>
using Board = BoardImpl<FixedConfig(Config)>;

using BasicBoard = Board<0>;
using HashValueBoard = Board<EnableHashValue>;
using EdgeCountBoard = Board<EnableEdgeCount | EnableHashValue>;
using RelativeScoreBoard = Board<EnableRelativeScore | EnableHashValue>;
using AbsoluteScoreBoard = Board<EnableAbsoluteScore | EnableHashValue>;
using GameBoard = Board<EnableAbsoluteScore | EnableOwner | EnableHashValue | EnableLogging>;
using ScoreableCountBoard = Board<EnableScoreableCount>;

}  // namespace dab::__detail__::board

namespace std {

template <std::int64_t Config>
struct hash<dab::__detail__::board::BoardImpl<Config>> {
  constexpr std::uint64_t operator()(const dab::__detail__::board::BoardImpl<Config>& board) const {
    return board.Hash();
  }
};

}  // namespace std