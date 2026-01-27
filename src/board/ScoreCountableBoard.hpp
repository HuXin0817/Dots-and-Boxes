#pragma once

#include "../model/GameScoreMap.hpp"
#include "EdgeCountableBoard.hpp"

namespace dab {

template <int64_t BoardSize>
class ScoreCountableBoard : public EdgeCountableBoard<BoardSize>, public GameScoreMap<BoardSize> {
  public:
  ScoreCountableBoard();

  void
  Reset(const EdgeCountableBoard<BoardSize>& newBoard = EdgeCountableBoard<BoardSize>());
  SizeType<BoardSize>
  Add(const Edge<BoardSize> edge);
};

template <int64_t BoardSize>
ScoreCountableBoard<BoardSize>::ScoreCountableBoard() {
  Reset();
}

template <int64_t BoardSize>
void
ScoreCountableBoard<BoardSize>::Reset(const EdgeCountableBoard<BoardSize>& newBoard) {
  EdgeCountableBoard<BoardSize>::operator=(newBoard);
  GameScoreMap<BoardSize>::Reset();
}

template <int64_t BoardSize>
SizeType<BoardSize>
ScoreCountableBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  const SizeType<BoardSize> score = EdgeCountableBoard<BoardSize>::Add(edge);
  GameScoreMap<BoardSize>::Add(score);
  return score;
}

}  // namespace dab
