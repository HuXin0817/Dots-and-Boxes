#pragma once

#include "../common/Array.hpp"
#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "Edge.hpp"

namespace dab::model {

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
  Add(const Edge<BoardSize> edge, const int score) {
    ++Time.At(edge.Value());
    Score.At(edge.Value()) += score;
  }

  void
  Add(const EdgeScoreMap& other) {
    for (int i = 0; i < Edge<BoardSize>::Max; i++) {
      Time.At(i) += other.Time.At(i);
      Score.At(i) += other.Score.At(i);
    }
  }

  common::Span<Edge<BoardSize>>
  Export() {
    float maxScore = 0.0;
    for (Edge<BoardSize> edge = 0; edge.Value() < Edge<BoardSize>::Max; ++edge) {
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
    return common::Export(BestEdges);
  }

  private:
  common::Array<int, Edge<BoardSize>::Max> Time;
  common::Array<int, Edge<BoardSize>::Max> Score;
  common::List<Edge<BoardSize>, Edge<BoardSize>::Max> BestEdges;
};

}  // namespace dab::model
