#pragma once

#include "Square.hpp"
#include "Turn.hpp"

template <int BoardSize>
class ScoreMap {
  static constexpr SizeType MinWinnerScore = Box<BoardSize>::Max / 2 + 1;

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
  Add(const SizeType score) {
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

  SizeType
  Score() const {
    return Player1Score - Player2Score;
  }

  bool
  Gaming() const {
    return Player1Score < MinWinnerScore && Player2Score < MinWinnerScore;
  }

  SizeType
  GetPlayer1Score() const {
    return Player1Score;
  }

  SizeType
  GetPlayer2Score() const {
    return Player2Score;
  }

  Turn
  GetTurn() const {
    return Turn;
  }

  private:
  SizeType Player1Score = 0;
  SizeType Player2Score = 0;
  Turn Turn = Player1Turn;
};
