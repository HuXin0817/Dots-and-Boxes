#pragma once

#include "../model/ScoreMap.hpp"
#include "EdgeCountableBoard.hpp"

template <int BoardSize>
class ScoreCountableBoard : public EdgeCountableBoard<BoardSize>, public ScoreMap<BoardSize> {
  public:
  ScoreCountableBoard() = default;

  void
  Reset(const EdgeCountableBoard<BoardSize>& newBoard) {
    EdgeCountableBoard<BoardSize>::GetEdgeCountableBoard() = newBoard;
    ScoreMap<BoardSize>::Reset();
  }

  int
  Add(Edge<BoardSize> edge) {
    int score = EdgeCountableBoard<BoardSize>::Add(edge);
    ScoreMap<BoardSize>::Add(score);
    return score;
  }

  bool
  Gaming() const {
    return ScoreMap<BoardSize>::Gaming() && EdgeCountableBoard<BoardSize>::Gaming();
  }
};
