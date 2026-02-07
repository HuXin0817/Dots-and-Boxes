#pragma once

#include <Dab/Common.hpp>
#include <algorithm>

#include "Edge.hpp"

namespace dab::detail::model {

template <int64_t BoardSize>
class SearchScoreMap {
 public:
  SearchScoreMap() = default;

  void
  Reset();
  void
  Add(const Edge<BoardSize> edge, const Int<BoardSize> score);
  void
  Add(const SearchScoreMap& other);
  Span<Edge<BoardSize>>
  Export();

 private:
  Array<int, Edge<BoardSize>::Max> Time;
  Array<int, Edge<BoardSize>::Max> Score;
  List<Edge<BoardSize>, Edge<BoardSize>::Max> BestEdges;
};

template <int64_t BoardSize>
void
SearchScoreMap<BoardSize>::Reset() {
  std::fill(Time.begin(), Time.end(), 0);
  std::fill(Score.begin(), Score.end(), 0);
  BestEdges.Clear();
}

template <int64_t BoardSize>
void
SearchScoreMap<BoardSize>::Add(const Edge<BoardSize> edge, const Int<BoardSize> score) {
  ++Time[edge];
  Score[edge] += score;
}

template <int64_t BoardSize>
void
SearchScoreMap<BoardSize>::Add(const SearchScoreMap& other) {
  for (Int<BoardSize> i = 0; i < Edge<BoardSize>::Max; ++i) {
    Time[i] += other.Time[i];
    Score[i] += other.Score[i];
  }
}

template <int64_t BoardSize>
Span<Edge<BoardSize>>
SearchScoreMap<BoardSize>::Export() {
  float maxScore = 0.0;
  for (Edge<BoardSize> edge = 0; edge < Edge<BoardSize>::Max; ++edge) {
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
  return Span(BestEdges.begin(), BestEdges.end());
}

}  // namespace dab::detail::model