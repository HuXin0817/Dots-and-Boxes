#pragma once

#include "../model/ScoreMap.hpp"
#include "EdgeCountableBoard.hpp"

template <int BoardSize, typename SizeType>
class ScoreCountableBoard {
  public:
  ScoreCountableBoard() = default;

  void
  Reset(const EdgeCountableBoard<BoardSize, SizeType>& newBoard) {
    EdgeCountableBoard = newBoard;
    ScoreMap.Reset();
  }

  SizeType
  Add(const Edge<BoardSize, SizeType> edge) {
    SizeType score = EdgeCountableBoard.Add(edge);
    ScoreMap.Add(score);
    return score;
  }

  bool
  Gaming() const {
    return ScoreMap.Gaming() && EdgeCountableBoard.GetBasicBoard().GetStep().Gaming();
  }

  const EdgeCountableBoard<BoardSize, SizeType>&
  GetEdgeCountableBoard() const {
    return EdgeCountableBoard;
  }

  const ScoreMap<BoardSize, SizeType>&
  GetScoreMap() const {
    return ScoreMap;
  }

  private:
  EdgeCountableBoard<BoardSize, SizeType> EdgeCountableBoard;
  ScoreMap<BoardSize, SizeType> ScoreMap;
};
