#pragma once

#include "Square.h"

static constexpr bool Player1Turn = true;
static constexpr bool Player2Turn = !Player1Turn;

static constexpr int MinWinnerScore = Box::Max / 2 + 1;

class ScoreMap {
  public:
  ScoreMap() {
    Reset();
  }

  void
  Reset() {
    Player1Score = 0;
    Player2Score = 0;
    Turn = Player1Turn;
  }

  void
  Add(int score) {
    if (score == 0) {
      Turn = !Turn;
      return;
    }
    if (Turn == Player1Turn) {
      Player1Score += score;
    } else {
      Player2Score += score;
    }
  }

  int
  Score() const {
    return Player1Score - Player2Score;
  }

  int
  GetScore(int player) const {
    return player == 0 ? Player1Score : Player2Score;
  }

  bool
  Gaming() const {
    return Player1Score < MinWinnerScore && Player2Score < MinWinnerScore;
  }

  int Player1Score = 0;
  int Player2Score = 0;
  bool Turn = Player1Turn;
};
