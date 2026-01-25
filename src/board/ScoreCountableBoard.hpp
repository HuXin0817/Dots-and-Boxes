#pragma once

#include "../model/ScoreMap.hpp"
#include "EdgeCountableBoard.hpp"

namespace dab {

template <int64_t BoardSize>
class ScoreCountableBoard : public EdgeCountableBoard<BoardSize>, public ScoreMap<BoardSize> {
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
  ScoreMap<BoardSize>::Reset();
}

template <int64_t BoardSize>
SizeType<BoardSize>
ScoreCountableBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  SizeType<BoardSize> score = EdgeCountableBoard<BoardSize>::Add(edge);
  ScoreMap<BoardSize>::Add(score);
  return score;
}

}  // namespace dab
