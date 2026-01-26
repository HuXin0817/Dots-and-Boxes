#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/Random.hpp"
#include "../model/SearchScoreMap.hpp"
#include "ImprovedSearchRobot.hpp"

namespace dab {

template <int64_t BoardSize, int64_t SearchTime = static_cast<int64_t>(Edge<BoardSize>::Max) << 8>
class MonteCarloSearchRobot final : public Robot<BoardSize> {
  public:
  MonteCarloSearchRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;
  bool
  CanEarlyExit(const ScoreCountableBoard<BoardSize>& board, Span<Edge<BoardSize>>& result);
  const SearchScoreMap<BoardSize, Int<2 * SearchTime>>&
  GetSearchResult() const;

  private:
  ImprovedSearchRobot<BoardSize> SubRobot;
  ScoreCountableBoard<BoardSize> SimulationBoard;
  SearchScoreMap<BoardSize, Int<2 * SearchTime>> SearchResult;
};

template <int64_t BoardSize, int64_t SearchTime>
Span<Edge<BoardSize>>
MonteCarloSearchRobot<BoardSize, SearchTime>::BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) {
  if (Span<Edge<BoardSize>> edges; CanEarlyExit(board, edges)) {
    return edges;
  }

  SearchResult.Reset();
  int64_t times = SearchTime / board.RemainStep() + 1;
  while (times--) {
    SimulationBoard.Reset(static_cast<EdgeCountableBoard<BoardSize>>(board));
    const Edge<BoardSize> edge = RandomChoice(SubRobot.BestCandidateEdges(SimulationBoard));
    SimulationBoard.Add(edge);
    while (SimulationBoard.Gaming()) {
      SimulationBoard.Add(RandomChoice(SubRobot.BestCandidateEdges(SimulationBoard)));
    }
    SearchResult.Add(edge, SimulationBoard.Score());
  }

  return SearchResult.Export();
}

template <int64_t BoardSize, int64_t SearchTime>
bool
MonteCarloSearchRobot<BoardSize, SearchTime>::CanEarlyExit(const ScoreCountableBoard<BoardSize>& board,
                                                           Span<Edge<BoardSize>>& result) {
  result = SubRobot.BestCandidateEdges(board);
  return result.Size() == 1;
}

template <int64_t BoardSize, int64_t SearchTime>
const SearchScoreMap<BoardSize, Int<2 * SearchTime>>&
MonteCarloSearchRobot<BoardSize, SearchTime>::GetSearchResult() const {
  return SearchResult;
}

}  // namespace dab
