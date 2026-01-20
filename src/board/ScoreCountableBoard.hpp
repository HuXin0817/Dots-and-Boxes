#pragma once

#include "../model/ScoreMap.hpp"
#include "EdgeCountableBoard.hpp"

namespace dab::board {

template <int BoardSize>
class ScoreCountableBoard {
  public:
  ScoreCountableBoard() = default;

  void
  Reset(const EdgeCountableBoard<BoardSize>& newBoard) {
    EdgeCountableBoard = newBoard;
    ScoreMap.Reset();
  }

  int
  Add(const model::Edge<BoardSize> edge) {
    int score = EdgeCountableBoard.Add(edge);
    ScoreMap.Add(score);
    return score;
  }

  bool
  Gaming() const {
    return ScoreMap.Gaming() && EdgeCountableBoard.GetBasicBoard().GetStep().Gaming();
  }

  const EdgeCountableBoard<BoardSize>&
  GetEdgeCountableBoard() const {
    return EdgeCountableBoard;
  }

  const model::ScoreMap<BoardSize>&
  GetScoreMap() const {
    return ScoreMap;
  }

  private:
  EdgeCountableBoard<BoardSize> EdgeCountableBoard;
  model::ScoreMap<BoardSize> ScoreMap;
};

}  // namespace dab::board
