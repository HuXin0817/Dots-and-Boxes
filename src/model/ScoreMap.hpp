#pragma once

#include "Square.hpp"
#include "Turn.hpp"

template <int64_t BoardSize>
class ScoreMap {
  static constexpr Int<BoardSize> MinWinnerScore = Box<BoardSize>::Max / 2 + 1;

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
  Add(const Int<BoardSize> score) {
    if (score == 0) {
      Turn.Change();
      return;
    }
    if (Turn.Value() == Player1Turn.Value()) {
      Player1Score += score;
    } else {
      Player2Score += score;
    }
  }

  Int<BoardSize>
  Score() const {
    return Player1Score - Player2Score;
  }

  bool
  Gaming() const {
    return Player1Score < MinWinnerScore && Player2Score < MinWinnerScore;
  }

  Int<BoardSize>
  GetPlayer1Score() const {
    return Player1Score;
  }

  Int<BoardSize>
  GetPlayer2Score() const {
    return Player2Score;
  }

  Turn
  GetTurn() const {
    return Turn;
  }

  private:
  Int<BoardSize> Player1Score = 0;
  Int<BoardSize> Player2Score = 0;
  Turn Turn = Player1Turn;
};
