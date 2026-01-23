#pragma once

#include "../common/Array.hpp"
#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "Edge.hpp"

template <int64_t BoardSize>
class EdgeScoreMap {
  public:
  EdgeScoreMap() = default;

  void
  Reset() {
    std::memset(Time.begin(), 0, sizeof(Time));
    std::memset(Score.begin(), 0, sizeof(Score));
    BestEdges.Clear();
  }

  void
  Add(const Edge<BoardSize> edge, const SizeType<BoardSize> score) {
    Time.At(edge.Value())++;
    Score.At(edge.Value()) += score;
  }

  void
  Add(const EdgeScoreMap& other) {
    for (const SizeType<BoardSize> i : std::views::iota(0, Edge<BoardSize>::Max)) {
      Time.At(i) += other.Time.At(i);
      Score.At(i) += other.Score.At(i);
    }
  }

  Span<Edge<BoardSize>, SizeType<BoardSize>>
  Export() {
    float maxScore = 0.0;
    for (const Edge<BoardSize> edge : std::views::iota(0, Edge<BoardSize>::Max)) {
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
    return ::Export<List<Edge<BoardSize>, Edge<BoardSize>::Max, SizeType<BoardSize>>, SizeType<BoardSize>>(BestEdges);
  }

  private:
  Array<int, Edge<BoardSize>::Max, SizeType<BoardSize>> Time;
  Array<int, Edge<BoardSize>::Max, SizeType<BoardSize>> Score;
  List<Edge<BoardSize>, Edge<BoardSize>::Max, SizeType<BoardSize>> BestEdges;
};
