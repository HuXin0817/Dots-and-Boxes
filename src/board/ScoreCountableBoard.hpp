#pragma once

#include <Dab/Model.hpp>

#include "EdgeCountableBoard.hpp"

namespace dab::detail::board {

template <int64_t BoardSize>
class ScoreCountableBoard : public EdgeCountableBoard<BoardSize>, public Turn<BoardSize> {
  public:
  ScoreCountableBoard();

  void
  Reset(const EdgeCountableBoard<BoardSize>& newBoard = EdgeCountableBoard<BoardSize>());
  Int<BoardSize>
  Add(const Edge<BoardSize> edge);
  Int<BoardSize>
  RelativeScore() const;
  Int<BoardSize>
  Player1Score() const;
  Int<BoardSize>
  Player2Score() const;

  private:
  Int<BoardSize> Score;
};

template <int64_t BoardSize>
ScoreCountableBoard<BoardSize>::ScoreCountableBoard() {
  Reset();
}

template <int64_t BoardSize>
void
ScoreCountableBoard<BoardSize>::Reset(const EdgeCountableBoard<BoardSize>& newBoard) {
  EdgeCountableBoard<BoardSize>::operator=(newBoard);
  Turn<BoardSize>::Reset();
  Score = 0;
}

template <int64_t BoardSize>
Int<BoardSize>
ScoreCountableBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  const Int<BoardSize> score = EdgeCountableBoard<BoardSize>::Add(edge);
  if (score > 0) {
    Score += score * Turn<BoardSize>::operator Int<BoardSize>();
  } else {
    Turn<BoardSize>::Add();
  }
  return score;
}

template <int64_t BoardSize>
Int<BoardSize>
ScoreCountableBoard<BoardSize>::RelativeScore() const {
  return Score;
}

template <int64_t BoardSize>
Int<BoardSize>
ScoreCountableBoard<BoardSize>::Player1Score() const {
  return (EdgeCountableBoard<BoardSize>::NowStep() + Score) / 2;
}

template <int64_t BoardSize>
Int<BoardSize>
ScoreCountableBoard<BoardSize>::Player2Score() const {
  return (EdgeCountableBoard<BoardSize>::NowStep() - Score) / 2;
}

}  // namespace dab::detail::board