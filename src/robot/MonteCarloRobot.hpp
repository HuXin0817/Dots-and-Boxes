#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/Random.hpp"
#include "../model/SearchScoreMap.hpp"
#include "SimulationRobot.hpp"

namespace dab {

template <int64_t BoardSize, int64_t SearchTime = static_cast<int64_t>(Edge<BoardSize>::Max) << 8>
class MonteCarloRobot final : public Robot<BoardSize> {
  public:
  MonteCarloRobot() = default;

  Span<const Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;
  bool
  CanEarlyExit(const ScoreCountableBoard<BoardSize>& board, Span<const Edge<BoardSize>>& result);
  const SearchScoreMap<BoardSize, Int<2 * SearchTime>>&
  GetSearchResult() const;

  private:
  SimulationRobot<BoardSize> SubRobot;
  ScoreCountableBoard<BoardSize> SimulationBoard;
  SearchScoreMap<BoardSize, Int<2 * SearchTime>> SearchResult;
};

template <int64_t BoardSize, int64_t SearchTime>
Span<const Edge<BoardSize>>
MonteCarloRobot<BoardSize, SearchTime>::BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) {
  if (Span<const Edge<BoardSize>> edges; CanEarlyExit(board, edges)) {
    return edges;
  }

  Random Random;
  SearchResult.Reset();
  int64_t times = SearchTime / board.RemainStep() + 1;
  while (times--) {
    SimulationBoard.Reset(static_cast<EdgeCountableBoard<BoardSize>>(board));
    const Edge<BoardSize> edge = Random.Choice(SubRobot.BestCandidateEdges(SimulationBoard));
    SimulationBoard.Add(edge);
    while (SimulationBoard.Gaming()) {
      SimulationBoard.Add(Random.Choice(SubRobot.BestCandidateEdges(SimulationBoard)));
    }
    SearchResult.Add(edge, SimulationBoard.Score());
  }

  return SearchResult.Export();
}

template <int64_t BoardSize, int64_t SearchTime>
bool
MonteCarloRobot<BoardSize, SearchTime>::CanEarlyExit(const ScoreCountableBoard<BoardSize>& board,
                                                     Span<const Edge<BoardSize>>& result) {
  result = SubRobot.BestCandidateEdges(board);
  return result.Size() == 1;
}

template <int64_t BoardSize, int64_t SearchTime>
const SearchScoreMap<BoardSize, Int<2 * SearchTime>>&
MonteCarloRobot<BoardSize, SearchTime>::GetSearchResult() const {
  return SearchResult;
}

}  // namespace dab
