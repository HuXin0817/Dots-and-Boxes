#pragma once

#include <type_traits>

#include "Board.hpp"
#include "CachedRobot.hpp"
#include "Common.hpp"
#include "Model.hpp"
#include "SimulationRobot.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize,
          bool UseCachedRobot = false,
          int64_t SearchTime = static_cast<int64_t>(Edge<BoardSize>::Max) << 8>
class MonteCarloRobot final : public Robot<BoardSize> {
  using SubRobotType = std::conditional_t<UseCachedRobot,
                                          CachedRobot<BoardSize, SimulationRobot<BoardSize>>,
                                          class SimulationRobot<BoardSize>>;

  public:
  MonteCarloRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;
  bool
  CanEarlyExit(const ScoreCountableBoard<BoardSize>& board, Span<Edge<BoardSize>>& result);
  const SearchScoreMap<BoardSize>&
  GetSearchResult() const;

  private:
  SubRobotType SubRobot;
  ScoreCountableBoard<BoardSize> SimulationBoard;
  SearchScoreMap<BoardSize> SearchResult;
};

template <int64_t BoardSize, bool UseCachedRobot, int64_t SearchTime>
Span<Edge<BoardSize>>
MonteCarloRobot<BoardSize, UseCachedRobot, SearchTime>::BestCandidateEdges(
    const ScoreCountableBoard<BoardSize>& board) {
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
    SearchResult.Add(edge, SimulationBoard.Score());
  }

  return SearchResult.Export();
}

template <int64_t BoardSize, bool UseCachedRobot, int64_t SearchTime>
bool
MonteCarloRobot<BoardSize, UseCachedRobot, SearchTime>::CanEarlyExit(const ScoreCountableBoard<BoardSize>& board,
                                                                     Span<Edge<BoardSize>>& result) {
  result = SubRobot.BestCandidateEdges(board);
  return result.Size() == 1;
}

template <int64_t BoardSize, bool UseCachedRobot, int64_t SearchTime>
const SearchScoreMap<BoardSize>&
MonteCarloRobot<BoardSize, UseCachedRobot, SearchTime>::GetSearchResult() const {
  return SearchResult;
}

}  // namespace dab::detail::robot