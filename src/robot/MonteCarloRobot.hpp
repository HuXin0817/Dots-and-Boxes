#pragma once

#include <Dab/Board.hpp>

#include "SimulationRobot.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize>
class MonteCarloRobot final : public Robot<BoardSize> {
  static constexpr int64_t SearchTime = static_cast<int64_t>(Edge<BoardSize>::Max) << 6;

 public:
  MonteCarloRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const RelativeScoreBoard<BoardSize>& board) override;
  bool
  CanEarlyExit(const RelativeScoreBoard<BoardSize>& board, Span<Edge<BoardSize>>& result);
  const SearchScoreMap<BoardSize>&
  GetSearchResult() const;

 private:
  SimulationRobot<BoardSize> SubRobot;
  RelativeScoreBoard<BoardSize> SimulationBoard;
  SearchScoreMap<BoardSize> SearchResult;
};

template <int64_t BoardSize>
Span<Edge<BoardSize>>
MonteCarloRobot<BoardSize>::BestCandidateEdges(const RelativeScoreBoard<BoardSize>& board) {
  if (Span<Edge<BoardSize>> edges; CanEarlyExit(board, edges)) {
    return edges;
  }

  Random Random;
  SearchResult.Reset();
  for (int64_t i = 0; i < SearchTime / board.RemainStep() + 1; ++i) {
    SimulationBoard.Reset(static_cast<EdgeCountableBoard<BoardSize>>(board));
    const Edge<BoardSize> edge = Random.Choice(SubRobot.BestCandidateEdges(SimulationBoard));
    SimulationBoard.Add(edge);
    while (SimulationBoard.Gaming()) {
      SimulationBoard.Add(Random.Choice(SubRobot.BestCandidateEdges(SimulationBoard)));
    }
    SearchResult.Add(edge, SimulationBoard.RelativeScore());
  }

  return SearchResult.Export();
}

template <int64_t BoardSize>
bool
MonteCarloRobot<BoardSize>::CanEarlyExit(const RelativeScoreBoard<BoardSize>& board, Span<Edge<BoardSize>>& result) {
  result = SubRobot.BestCandidateEdges(board);
  return result.Size() == 1;
}

template <int64_t BoardSize>
const SearchScoreMap<BoardSize>&
MonteCarloRobot<BoardSize>::GetSearchResult() const {
  return SearchResult;
}

}  // namespace dab::detail::robot