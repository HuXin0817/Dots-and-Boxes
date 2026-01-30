#pragma once

#include "Board.hpp"
#include "Common.hpp"
#include "Model.hpp"
#include "SimulationRobot.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize, int64_t SearchTime = static_cast<int64_t>(Limits<Edge<BoardSize>>::Max) << 8>
class MonteCarloRobot final : public Robot<BoardSize> {
  public:
  MonteCarloRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;
  bool
  CanEarlyExit(const ScoreCountableBoard<BoardSize>& board, Span<Edge<BoardSize>>& result);
  const SearchScoreMap<BoardSize>&
  GetSearchResult() const;

  private:
  SimulationRobot<BoardSize> SubRobot;
  ScoreCountableBoard<BoardSize> SimulationBoard;
  SearchScoreMap<BoardSize> SearchResult;
};

template <int64_t BoardSize, int64_t SearchTime>
Span<Edge<BoardSize>>
MonteCarloRobot<BoardSize, SearchTime>::BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) {
  if (Span<Edge<BoardSize>> edges; CanEarlyExit(board, edges)) {
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
                                                     Span<Edge<BoardSize>>& result) {
  result = SubRobot.BestCandidateEdges(board);
  return result.Size() == 1;
}

template <int64_t BoardSize, int64_t SearchTime>
const SearchScoreMap<BoardSize>&
MonteCarloRobot<BoardSize, SearchTime>::GetSearchResult() const {
  return SearchResult;
}

}  // namespace dab::detail::robot