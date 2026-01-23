#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/Random.hpp"
#include "../model/EdgeScoreMap.hpp"
#include "ImprovedSearchRobot.hpp"

template <int BoardSize>
class ParallelSearchRobot;

template <int BoardSize, int SearchTime = Edge<BoardSize>::Max << 8>
class MonteCarloSearchRobot final : public Robot<BoardSize> {
  friend class ParallelSearchRobot<BoardSize>;

  public:
  MonteCarloSearchRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override {
    if (const Span<Edge<BoardSize>> edges = SubRobot.BestCandidateEdges(board); edges.Size() == 1) {
      return edges;
    }

    SearchResult.Reset();
    int times = SearchTime / board.GetEdgeCountableBoard().GetBasicBoard().GetStep().RemainStep() + 1;
    while (times--) {
      SimulationBoard.Reset(board.GetEdgeCountableBoard());
      const Edge<BoardSize> edge = RandomChoice(SubRobot.BestCandidateEdges(SimulationBoard));
      SimulationBoard.Add(edge);
      while (SimulationBoard.Gaming()) {
        SimulationBoard.Add(RandomChoice(SubRobot.BestCandidateEdges(SimulationBoard)));
      }
      SearchResult.Add(edge, SimulationBoard.GetScoreMap().Score());
    }

    return SearchResult.Export();
  }

  private:
  ImprovedSearchRobot<BoardSize> SubRobot;
  ScoreCountableBoard<BoardSize> SimulationBoard;
  EdgeScoreMap<BoardSize> SearchResult;
};
