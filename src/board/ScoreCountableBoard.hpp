#pragma once

#include "../model/ScoreMap.hpp"
#include "EdgeCountableBoard.hpp"

template <int64_t BoardSize>
class ScoreCountableBoard {
  public:
  ScoreCountableBoard() = default;

  void
  Reset(const EdgeCountableBoard<BoardSize>& newBoard);
  SizeType<BoardSize>
  Add(const Edge<BoardSize> edge);
  const EdgeCountableBoard<BoardSize>&
  GetEdgeCountableBoard() const;
  const ScoreMap<BoardSize>&
  GetScoreMap() const;

  private:
  EdgeCountableBoard<BoardSize> EdgeCountableBoard;
  ScoreMap<BoardSize> ScoreMap;
};

template <int64_t BoardSize>
void
ScoreCountableBoard<BoardSize>::Reset(const ::EdgeCountableBoard<BoardSize>& newBoard) {
  EdgeCountableBoard = newBoard;
  ScoreMap.Reset();
}

template <int64_t BoardSize>
SizeType<BoardSize>
ScoreCountableBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  SizeType<BoardSize> score = EdgeCountableBoard.Add(edge);
  ScoreMap.Add(score);
  return score;
}

template <int64_t BoardSize>
const EdgeCountableBoard<BoardSize>&
ScoreCountableBoard<BoardSize>::GetEdgeCountableBoard() const {
  return EdgeCountableBoard;
}

template <int64_t BoardSize>
const ScoreMap<BoardSize>&
ScoreCountableBoard<BoardSize>::GetScoreMap() const {
  return ScoreMap;
}
