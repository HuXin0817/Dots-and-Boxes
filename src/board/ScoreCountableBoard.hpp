#pragma once

#include "../model/ScoreMap.hpp"
#include "EdgeCountableBoard.hpp"

template <int BoardSize>
class ScoreCountableBoard {
  public:
  ScoreCountableBoard() = default;

  void
  Reset(const EdgeCountableBoard<BoardSize>& newBoard) {
    EdgeCountableBoard.GetEdgeCountableBoard() = newBoard;
    ScoreMap.Reset();
  }

  int
  Add(Edge<BoardSize> edge) {
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

  EdgeCountableBoard<BoardSize>&
  GetEdgeCountableBoard() {
    return EdgeCountableBoard;
  }

  const ScoreMap<BoardSize>&
  GetScoreMap() const {
    return ScoreMap;
  }

  ScoreMap<BoardSize>&
  GetScoreMap() {
    return ScoreMap;
  }

  private:
  EdgeCountableBoard<BoardSize> EdgeCountableBoard;
  ScoreMap<BoardSize> ScoreMap;
};
