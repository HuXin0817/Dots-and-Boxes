#pragma once

#include <cstring>

#include "../common/Array.hpp"
#include "../common/Iota.hpp"
#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "Edge.hpp"

template <int64_t BoardSize, typename ScoreType>
class SearchScoreMap {
  public:
  SearchScoreMap() = default;

  void
  Reset();
  void
  Add(const Edge<BoardSize> edge, const SizeType<BoardSize> score);
  template <typename OtherSearchScoreMap>
  void
  Add(const OtherSearchScoreMap& other);
  Span<Edge<BoardSize>>
  Export();

  Array<ScoreType, Edge<BoardSize>::Max> Time;
  Array<ScoreType, Edge<BoardSize>::Max> Score;
  List<Edge<BoardSize>, Edge<BoardSize>::Max> BestEdges;
};

template <int64_t BoardSize, typename ScoreType>
void
SearchScoreMap<BoardSize, ScoreType>::Reset() {
  memset(Time.begin(), 0, sizeof(Time));
  memset(Score.begin(), 0, sizeof(Score));
  BestEdges.Clear();
}

template <int64_t BoardSize, typename ScoreType>
void
SearchScoreMap<BoardSize, ScoreType>::Add(const Edge<BoardSize> edge, const SizeType<BoardSize> score) {
  Time.At(edge.Value())++;
  Score.At(edge.Value()) += score;
}

template <int64_t BoardSize, typename ScoreType>
template <typename OtherSearchScoreMap>
void
SearchScoreMap<BoardSize, ScoreType>::Add(const OtherSearchScoreMap& other) {
  for (const SizeType<BoardSize> i : Iota(Edge<BoardSize>::Max)) {
    Time.At(i) += other.Time.At(i);
    Score.At(i) += other.Score.At(i);
  }
}

template <int64_t BoardSize, typename ScoreType>
Span<Edge<BoardSize>>
SearchScoreMap<BoardSize, ScoreType>::Export() {
  float maxScore = 0.0;
  for (const Edge<BoardSize> edge : Iota(Edge<BoardSize>::Max)) {
    if (Time.At(edge.Value()) > 0) {
      if (float score = static_cast<float>(Score.At(edge.Value())) / static_cast<float>(Time.At(edge.Value()));
          score > maxScore || BestEdges.Empty()) {
        maxScore = score;
        BestEdges.Reset(edge);
      } else if (score == maxScore) {
        BestEdges.Append(edge);
      }
    }
  }
  return ::Export(BestEdges);
}
