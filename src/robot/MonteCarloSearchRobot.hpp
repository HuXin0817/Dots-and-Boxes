#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/Random.hpp"
#include "../model/EdgeScoreMap.hpp"
#include "ImprovedSearchRobot.hpp"

template <int BoardSize, typename SizeType>
class ParallelSearchRobot;

template <int BoardSize, typename SizeType>
class MonteCarloSearchRobot final : public Robot<BoardSize, SizeType> {
  friend class ParallelSearchRobot<BoardSize, SizeType>;

  static constexpr int SearchTime = 1 << 14;

  public:
  MonteCarloSearchRobot() = default;

  Span<Edge<BoardSize, SizeType>, SizeType>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize, SizeType>& board) override {
    if (const Span<Edge<BoardSize, SizeType>, SizeType> edges = SubRobot.BestCandidateEdges(board); edges.Size() == 1) {
      return edges;
    }

    SearchResult.Reset();
    int times = SearchTime / board.GetEdgeCountableBoard().GetBasicBoard().GetStep().RemainStep() + 1;
    while (times--) {
      SimulationBoard.Reset(board.GetEdgeCountableBoard());
      const Edge<BoardSize, SizeType> edge = RandomChoice<Span<Edge<BoardSize, SizeType>, SizeType>, SizeType>(
          SubRobot.BestCandidateEdges(SimulationBoard));
      SimulationBoard.Add(edge);
      while (SimulationBoard.Gaming()) {
        SimulationBoard.Add(RandomChoice<Span<Edge<BoardSize, SizeType>, SizeType>, SizeType>(
            SubRobot.BestCandidateEdges(SimulationBoard)));
      }
      SearchResult.Add(edge, SimulationBoard.GetScoreMap().Score());
    }

    return SearchResult.Export();
  }

  private:
  ImprovedSearchRobot<BoardSize, SizeType> SubRobot;
  ScoreCountableBoard<BoardSize, SizeType> SimulationBoard;
  EdgeScoreMap<BoardSize, SizeType> SearchResult;
};
