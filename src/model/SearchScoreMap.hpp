#pragma once

#include <algorithm>

#include "../common/Array.hpp"
#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "Edge.hpp"

namespace dab {

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

  Array<ScoreType, Limits<Edge<BoardSize>>::Max> Time;
  Array<ScoreType, Limits<Edge<BoardSize>>::Max> Score;
  List<Edge<BoardSize>, Limits<Edge<BoardSize>>::Max> BestEdges;
};

template <int64_t BoardSize, typename ScoreType>
void
SearchScoreMap<BoardSize, ScoreType>::Reset() {
  std::fill(Time.begin(), Time.end(), 0);
  std::fill(Score.begin(), Score.end(), 0);
  BestEdges.Clear();
}

template <int64_t BoardSize, typename ScoreType>
void
SearchScoreMap<BoardSize, ScoreType>::Add(const Edge<BoardSize> edge, const SizeType<BoardSize> score) {
  Time[edge]++;
  Score[edge] += score;
}

template <int64_t BoardSize, typename ScoreType>
template <typename OtherSearchScoreMap>
void
SearchScoreMap<BoardSize, ScoreType>::Add(const OtherSearchScoreMap& other) {
  for (const SizeType<BoardSize> i : Iota<Edge<BoardSize>>()) {
    Time[i] += other.Time[i];
    Score[i] += other.Score[i];
  }
}

template <int64_t BoardSize, typename ScoreType>
Span<Edge<BoardSize>>
SearchScoreMap<BoardSize, ScoreType>::Export() {
  float maxScore = 0.0;
  for (const Edge<BoardSize> edge : Iota<Edge<BoardSize>>()) {
    if (Time[edge] > 0) {
      if (const float score = static_cast<float>(Score[edge]) / static_cast<float>(Time[edge]);
          score > maxScore || BestEdges.Empty()) {
        maxScore = score;
        BestEdges.ClearAndSet(edge);
      } else if (score == maxScore) {
        BestEdges.Append(edge);
      }
    }
  }
  return ::dab::Export(BestEdges);
}

}  // namespace dab
