#pragma once

#include "../common/Array.hpp"
#include "../common/Iota.hpp"
#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "Edge.hpp"

template <int BoardSize>
class EdgeScoreMap {
  public:
  EdgeScoreMap() = default;

  void
  Reset() {
    memset(Time.begin(), 0, sizeof(Time));
    memset(Score.begin(), 0, sizeof(Score));
    BestEdges.Clear();
  }

  void
  Add(const Edge<BoardSize> edge, const SizeType score) {
    Time.At(edge.Value())++;
    Score.At(edge.Value()) += score;
  }

  void
  Add(const EdgeScoreMap& other) {
    for (const SizeType i : Iota(Edge<BoardSize>::Max)) {
      Time.At(i) += other.Time.At(i);
      Score.At(i) += other.Score.At(i);
    }
  }

  Span<Edge<BoardSize>>
  Export() {
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

  private:
  Array<int, Edge<BoardSize>::Max> Time;
  Array<int, Edge<BoardSize>::Max> Score;
  List<Edge<BoardSize>, Edge<BoardSize>::Max> BestEdges;
};
