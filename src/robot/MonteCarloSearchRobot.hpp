#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/Random.hpp"
#include "../model/EdgeScoreMap.hpp"
#include "ImprovedSearchRobot.hpp"

namespace dab::robot {

template <int BoardSize, typename SizeType>
class ParallelSearchRobot;

template <int BoardSize, typename SizeType>
class MonteCarloSearchRobot final : public Robot<BoardSize, SizeType> {
  friend class ParallelSearchRobot<BoardSize, SizeType>;

  static constexpr int SearchTime = 1 << 14;

  public:
  MonteCarloSearchRobot() = default;

  common::Span<model::Edge<BoardSize, SizeType>, int>
  BestCandidateEdges(const board::ScoreCountableBoard<BoardSize, SizeType>& board) override {
    if (const common::Span<model::Edge<BoardSize, SizeType>, int> edges = SubRobot.BestCandidateEdges(board);
        edges.Size() == 1) {
      return edges;
    }

    SearchResult.Reset();
    int times = SearchTime / board.GetEdgeCountableBoard().GetBasicBoard().GetStep().RemainStep() + 1;
    while (times--) {
      SimulationBoard.Reset(board.GetEdgeCountableBoard());
      const model::Edge<BoardSize, SizeType> edge =
          common::RandomChoice<common::Span<model::Edge<BoardSize, SizeType>, int>, int>(
              SubRobot.BestCandidateEdges(SimulationBoard));
      SimulationBoard.Add(edge);
      while (SimulationBoard.Gaming()) {
        SimulationBoard.Add(common::RandomChoice<common::Span<model::Edge<BoardSize, SizeType>, int>, int>(
            SubRobot.BestCandidateEdges(SimulationBoard)));
      }
      SearchResult.Add(edge, SimulationBoard.GetScoreMap().Score());
    }

    return SearchResult.Export();
  }

  private:
  ImprovedSearchRobot<BoardSize, SizeType> SubRobot;
  board::ScoreCountableBoard<BoardSize, SizeType> SimulationBoard;
  model::EdgeScoreMap<BoardSize, SizeType> SearchResult;
};

}  // namespace dab::robot
