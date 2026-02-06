#pragma once

#include <Dab/Model.hpp>

#include "EdgeCountableBoard.hpp"

namespace dab::detail::board {

template <int64_t BoardSize>
class RelativeScoreBoard : public EdgeCountableBoard<BoardSize>, public Turn<BoardSize> {
  public:
  RelativeScoreBoard();

  void
  Reset(const EdgeCountableBoard<BoardSize>& newBoard = EdgeCountableBoard<BoardSize>());
  Int<BoardSize>
  Add(const Edge<BoardSize> edge);
  Int<BoardSize>
  RelativeScore() const;

  private:
  Int<BoardSize> Score;
};

template <int64_t BoardSize>
RelativeScoreBoard<BoardSize>::RelativeScoreBoard() {
  Reset();
}

template <int64_t BoardSize>
void
RelativeScoreBoard<BoardSize>::Reset(const EdgeCountableBoard<BoardSize>& newBoard) {
  EdgeCountableBoard<BoardSize>::operator=(newBoard);
  Turn<BoardSize>::Reset();
  Score = 0;
}

template <int64_t BoardSize>
Int<BoardSize>
RelativeScoreBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  const Int<BoardSize> score = EdgeCountableBoard<BoardSize>::Add(edge);
  Score += score * Turn<BoardSize>::v;
  const Int<BoardSize> neg = -(score == 0);
  Turn<BoardSize>::v = Turn<BoardSize>::v + neg ^ neg;
  return score;
}

template <int64_t BoardSize>
Int<BoardSize>
RelativeScoreBoard<BoardSize>::RelativeScore() const {
  return Score;
}

}  // namespace dab::detail::board