#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/Random.hpp"
#include "../model/EdgeScoreMap.hpp"
#include "ImprovedSearchRobot.hpp"

template <int64_t BoardSize>
class ParallelSearchRobot;

template <int64_t BoardSize, int SearchTime = Edge<BoardSize>::Max << 8>
class MonteCarloSearchRobot final : public Robot<BoardSize> {
  friend class ParallelSearchRobot<BoardSize>;

  public:
  MonteCarloSearchRobot() = default;

  Span<Edge<BoardSize>, SizeType<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override {
    if (const Span<Edge<BoardSize>, SizeType<BoardSize>> edges = SubRobot.BestCandidateEdges(board);
        edges.Size() == 1) {
      return edges;
    }

    SearchResult.Reset();
    int times = SearchTime / board.GetEdgeCountableBoard().GetBasicBoard().GetStep().RemainStep() + 1;
    while (times--) {
      SimulationBoard.Reset(board.GetEdgeCountableBoard());
      const Edge<BoardSize> edge = RandomChoice<Span<Edge<BoardSize>, SizeType<BoardSize>>, SizeType<BoardSize>>(
          SubRobot.BestCandidateEdges(SimulationBoard));
      SimulationBoard.Add(edge);
      while (SimulationBoard.Gaming()) {
        SimulationBoard.Add(RandomChoice<Span<Edge<BoardSize>, SizeType<BoardSize>>, SizeType<BoardSize>>(
            SubRobot.BestCandidateEdges(SimulationBoard)));
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
