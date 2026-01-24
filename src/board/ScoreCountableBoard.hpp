#pragma once

#include "../model/ScoreMap.hpp"
#include "EdgeCountableBoard.hpp"

template <int64_t BoardSize>
class ScoreCountableBoard {
  public:
  ScoreCountableBoard() = default;

  void
  Reset(const EdgeCountableBoard<BoardSize>& newBoard) {
    EdgeCountableBoard = newBoard;
    ScoreMap.Reset();
  }

  Int<BoardSize>
  Add(const Edge<BoardSize> edge) {
    Int<BoardSize> score = EdgeCountableBoard.Add(edge);
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

  const ScoreMap<BoardSize>&
  GetScoreMap() const {
    return ScoreMap;
  }

  private:
  EdgeCountableBoard<BoardSize> EdgeCountableBoard;
  ScoreMap<BoardSize> ScoreMap;
};
