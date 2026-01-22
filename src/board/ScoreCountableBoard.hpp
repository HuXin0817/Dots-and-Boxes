#pragma once

#include "../model/ScoreMap.hpp"
#include "EdgeCountableBoard.hpp"

namespace dab::board {

template <int BoardSize, typename SizeType>
class ScoreCountableBoard {
  public:
  ScoreCountableBoard() = default;

  void
  Reset(const EdgeCountableBoard<BoardSize, SizeType>& newBoard) {
    EdgeCountableBoard = newBoard;
    ScoreMap.Reset();
  }

  int
  Add(const model::Edge<BoardSize, SizeType> edge) {
    int score = EdgeCountableBoard.Add(edge);
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

  const model::ScoreMap<BoardSize, SizeType>&
  GetScoreMap() const {
    return ScoreMap;
  }

  private:
  EdgeCountableBoard<BoardSize, SizeType> EdgeCountableBoard;
  model::ScoreMap<BoardSize, SizeType> ScoreMap;
};

}  // namespace dab::board
