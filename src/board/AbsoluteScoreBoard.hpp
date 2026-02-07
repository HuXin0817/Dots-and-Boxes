#pragma once

#include <Dab/Model.hpp>

#include "RelativeScoreBoard.hpp"

namespace dab::detail::board {

template <int64_t BoardSize>
class AbsoluteScoreBoard : public RelativeScoreBoard<BoardSize> {
 public:
  AbsoluteScoreBoard();

  void
  Reset(const EdgeCountableBoard<BoardSize>& newBoard = EdgeCountableBoard<BoardSize>());
  Int<BoardSize>
  Add(const Edge<BoardSize> edge);
  Int<BoardSize>
  Player1Score() const;
  Int<BoardSize>
  Player2Score() const;

 private:
  Int<BoardSize> TotalScore;
};

template <int64_t BoardSize>
AbsoluteScoreBoard<BoardSize>::AbsoluteScoreBoard() {
  Reset();
}

template <int64_t BoardSize>
void
AbsoluteScoreBoard<BoardSize>::Reset(const EdgeCountableBoard<BoardSize>& newBoard) {
  RelativeScoreBoard<BoardSize>::Reset(newBoard);
  TotalScore = 0;
}

template <int64_t BoardSize>
Int<BoardSize>
AbsoluteScoreBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  Int<BoardSize> score = RelativeScoreBoard<BoardSize>::Add(edge);
  TotalScore += score;
  return score;
}

template <int64_t BoardSize>
Int<BoardSize>
AbsoluteScoreBoard<BoardSize>::Player1Score() const {
  return (TotalScore + RelativeScoreBoard<BoardSize>::RelativeScore()) / 2;
}

template <int64_t BoardSize>
Int<BoardSize>
AbsoluteScoreBoard<BoardSize>::Player2Score() const {
  return (TotalScore - RelativeScoreBoard<BoardSize>::RelativeScore()) / 2;
}

}  // namespace dab::detail::board