#pragma once

#include "../model/ScoreMap.h"
#include "BoardV1.h"

class BoardV2 : public BoardV1, public ScoreMap {
  public:
  BoardV2() = default;

  void
  Reset(const BoardV1& newBoard) {
    GetBoardV1() = newBoard;
    ScoreMap::Reset();
  }

  int
  Add(Edge edge) {
    int score = BoardV1::Add(edge);
    ScoreMap::Add(score);
    return score;
  }

  bool
  Gaming() const {
    return ScoreMap::Gaming() && BoardV1::Gaming();
  }
};
