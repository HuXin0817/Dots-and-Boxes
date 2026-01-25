#pragma once

#include "SizeType.hpp"
#include "Turn.hpp"

namespace dab {

template <int64_t BoardSize>
class ScoreMap : public Turn {
  public:
  ScoreMap();

  void
  Reset();
  void
  Add(const SizeType<BoardSize> score);
  SizeType<BoardSize>
  Score() const;
  SizeType<BoardSize>
  GetPlayer1Score() const;
  SizeType<BoardSize>
  GetPlayer2Score() const;

  private:
  SizeType<BoardSize> Player1Score = 0;
  SizeType<BoardSize> Player2Score = 0;
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
  Turn::Reset();
}

template <int64_t BoardSize>
void
ScoreMap<BoardSize>::Add(const SizeType<BoardSize> score) {
  if (score == 0) {
    Turn::Add();
    return;
  }
  if (Turn::IsPlayer1Turn()) {
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
SizeType<BoardSize>
ScoreMap<BoardSize>::GetPlayer1Score() const {
  return Player1Score;
}

template <int64_t BoardSize>
SizeType<BoardSize>
ScoreMap<BoardSize>::GetPlayer2Score() const {
  return Player2Score;
}

}  // namespace dab
