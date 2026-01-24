#pragma once

#include "Square.hpp"
#include "Turn.hpp"

template <int64_t BoardSize>
class ScoreMap {
  static constexpr SizeType<BoardSize> MinWinnerScore = Box<BoardSize>::Max / 2 + 1;

  public:
  ScoreMap();

  void
  Reset();

  void
  Add(const SizeType<BoardSize> score);

  SizeType<BoardSize>
  Score() const;

  bool
  Gaming() const;

  SizeType<BoardSize>
  GetPlayer1Score() const;

  SizeType<BoardSize>
  GetPlayer2Score() const;

  Turn
  GetTurn() const;

  private:
  SizeType<BoardSize> Player1Score = 0;
  SizeType<BoardSize> Player2Score = 0;
  Turn Turn = Player1Turn;
};

template <int64_t BoardSize>
ScoreMap<BoardSize>::ScoreMap() {
  Reset();
}

template <int64_t BoardSize>
void
ScoreMap<BoardSize>::Reset() {
  Player1Score = 0;
  Player2Score = 0;
  Turn = Player1Turn;
}

template <int64_t BoardSize>
void
ScoreMap<BoardSize>::Add(const SizeType<BoardSize> score) {
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

template <int64_t BoardSize>
SizeType<BoardSize>
ScoreMap<BoardSize>::Score() const {
  return Player1Score - Player2Score;
}

template <int64_t BoardSize>
bool
ScoreMap<BoardSize>::Gaming() const {
  return Player1Score < MinWinnerScore && Player2Score < MinWinnerScore;
}

template <int64_t BoardSize>
SizeType<BoardSize>
ScoreMap<BoardSize>::GetPlayer1Score() const {
  return Player1Score;
}

template <int64_t BoardSize>
SizeType<BoardSize>
ScoreMap<BoardSize>::GetPlayer2Score() const {
  return Player2Score;
}

template <int64_t BoardSize>
Turn
ScoreMap<BoardSize>::GetTurn() const {
  return Turn;
}
