#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/Random.hpp"
#include "../model/EdgeScoreMap.hpp"
#include "ImprovedSearchRobot.hpp"

namespace dab::robot {

template <int BoardSize>
class ParallelSearchRobot;

template <int BoardSize>
class MonteCarloSearchRobot final : public Robot<BoardSize> {
  friend class ParallelSearchRobot<BoardSize>;

  static constexpr int SearchTime = 1 << 14;

  public:
  MonteCarloSearchRobot() = default;

  common::Span<model::Edge<BoardSize>, int>
  BestCandidateEdges(const board::ScoreCountableBoard<BoardSize>& board) override {
    if (const common::Span<model::Edge<BoardSize>, int> edges = SubRobot.BestCandidateEdges(board); edges.Size() == 1) {
      return edges;
    }

    SearchResult.Reset();
    int times = SearchTime / board.GetEdgeCountableBoard().GetBasicBoard().GetStep().RemainStep() + 1;
    while (times--) {
      SimulationBoard.Reset(board.GetEdgeCountableBoard());
      const model::Edge<BoardSize> edge = common::RandomChoice<common::Span<model::Edge<BoardSize>, int>, int>(SubRobot.BestCandidateEdges(SimulationBoard));
      SimulationBoard.Add(edge);
      while (SimulationBoard.Gaming()) {
        SimulationBoard.Add(common::RandomChoice<common::Span<model::Edge<BoardSize>, int>, int>(SubRobot.BestCandidateEdges(SimulationBoard)));
      }
      SearchResult.Add(edge, SimulationBoard.GetScoreMap().Score());
    }

    return SearchResult.Export();
  }

  private:
  ImprovedSearchRobot<BoardSize> SubRobot;
  board::ScoreCountableBoard<BoardSize> SimulationBoard;
  model::EdgeScoreMap<BoardSize> SearchResult;
};

}  // namespace dab::robot
