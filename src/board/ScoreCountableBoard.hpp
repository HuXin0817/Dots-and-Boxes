#pragma once

#include "../model/ScoreMap.hpp"
#include "EdgeCountableBoard.hpp"

template <int BoardSize>
class ScoreCountableBoard {
  public:
  ScoreCountableBoard() = default;

  void
  Reset(const EdgeCountableBoard<BoardSize>& newBoard) {
    EdgeCountableBoard = newBoard;
    ScoreMap.Reset();
  }

  SizeType
  Add(const Edge<BoardSize> edge) {
    SizeType score = EdgeCountableBoard.Add(edge);
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
