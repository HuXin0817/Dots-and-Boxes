#pragma once

#include "../common/Array.hpp"
#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "Edge.hpp"

template <int BoardSize>
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
  Add(Edge<BoardSize> edge, int score) {
    Time.At(edge)++;
    Score.At(edge) += score;
  }

  void
  Add(const EdgeScoreMap& other) {
    for (int i = 0; i < Edge<BoardSize>::Max; i++) {
      Time.At(i) += other.Time.At(i);
      Score.At(i) += other.Score.At(i);
    }
  }

  Span<Edge<BoardSize>>
  Export() {
    float maxScore = 0.0;
    for (Edge<BoardSize> edge = 0; edge < Edge<BoardSize>::Max; edge++) {
      if (Time.At(edge) > 0) {
        float score = static_cast<float>(Score.At(edge)) / static_cast<float>(Time.At(edge));
        if (score > maxScore || BestEdges.Empty()) {
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
