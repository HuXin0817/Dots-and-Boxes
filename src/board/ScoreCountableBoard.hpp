#pragma once

#include "../model/ScoreMap.hpp"
#include "EdgeCountableBoard.hpp"

class ScoreCountableBoard : public EdgeCountableBoard, public ScoreMap {
  public:
  ScoreCountableBoard() = default;

  void
  Reset(const EdgeCountableBoard& newBoard) {
    GetBoardV1() = newBoard;
    ScoreMap::Reset();
  }

  int
  Add(Edge edge) {
    int score = EdgeCountableBoard::Add(edge);
    ScoreMap::Add(score);
    return score;
  }

  bool
  Gaming() const {
    return ScoreMap::Gaming() && EdgeCountableBoard::Gaming();
  }
};
