#pragma once

#include "../common/Array.hpp"
#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "Edge.hpp"
#include "ValueIterator.hpp"

template <int BoardSize, typename SizeType>
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
  Add(const Edge<BoardSize, SizeType> edge, const SizeType score) {
    Time.At(edge.Value())++;
    Score.At(edge.Value()) += score;
  }

  void
  Add(const EdgeScoreMap& other) {
    for (SizeType i = 0; i < Edge<BoardSize, SizeType>::Max; i++) {
      Time.At(i) += other.Time.At(i);
      Score.At(i) += other.Score.At(i);
    }
  }

  Span<Edge<BoardSize, SizeType>, SizeType>
  Export() {
    float maxScore = 0.0;
    for (const Edge<BoardSize, SizeType> edge : ValueIterator<Edge<BoardSize, SizeType>, SizeType>()) {
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
    return Export<List<Edge<BoardSize, SizeType>, Edge<BoardSize, SizeType>::Max, SizeType>, SizeType>(BestEdges);
  }

  private:
  Array<int, Edge<BoardSize, SizeType>::Max, SizeType> Time;
  Array<int, Edge<BoardSize, SizeType>::Max, SizeType> Score;
  List<Edge<BoardSize, SizeType>, Edge<BoardSize, SizeType>::Max, SizeType> BestEdges;
};
