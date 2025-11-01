#pragma once

#include "../common/Array.h"
#include "../common/List.h"
#include "../common/Span.h"
#include "Edge.h"

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
  Add(Edge edge, int score) {
    Time.At(edge)++;
    Score.At(edge) += score;
  }

  void
  Add(const EdgeScoreMap& other) {
    for (int i = 0; i < Edge::Max; i++) {
      Time.At(i) += other.Time.At(i);
      Score.At(i) += other.Score.At(i);
    }
  }

  Span<Edge>
  Export() {
    float maxScore = 0.0;
    for (Edge edge = 0; edge < Edge::Max; edge++) {
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
    return BestEdges.Export();
  }

  private:
  Array<int, Edge::Max> Time;
  Array<int, Edge::Max> Score;
  List<Edge, Edge::Max> BestEdges;
};
