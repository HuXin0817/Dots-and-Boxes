#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/Random.hpp"
#include "../model/EdgeScoreMap.hpp"
#include "ImprovedSearchRobot.hpp"

template <int BoardSize>
class MonteCarloSearchRobot final : public Robot<BoardSize> {
  static constexpr int SearchTime = 1 << 14;

  public:
  MonteCarloSearchRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override {
    if (auto edges = SubRobot.BestCandidateEdges(board); edges.Size() == 1) {
      return edges;
    }

    SearchResult.Reset();
    int times =
        SearchTime / board.GetEdgeCountableBoard().GetBasicBoard().GetStep().RemainStep() + 1;
    while (times--) {
      SimulationBoard.Reset(board.GetEdgeCountableBoard());
      Edge<BoardSize> edge = RandomChoice(SubRobot.BestCandidateEdges(SimulationBoard));
      SimulationBoard.Add(edge);
      while (SimulationBoard.Gaming()) {
        SimulationBoard.Add(RandomChoice(SubRobot.BestCandidateEdges(SimulationBoard)));
      }
      SearchResult.Add(edge, SimulationBoard.GetScoreMap().Score());
    }

    return SearchResult.Export();
  }

  const ImprovedSearchRobot<BoardSize>&
  GetSubRobot() const {
    return SubRobot;
  }

  ImprovedSearchRobot<BoardSize>&
  GetSubRobot() {
    return SubRobot;
  }

  const EdgeScoreMap<BoardSize>&
  GetSearchResult() const {
    return SearchResult;
  }

  EdgeScoreMap<BoardSize>&
  GetSearchResult() {
    return SearchResult;
  }

  private:
  ImprovedSearchRobot<BoardSize> SubRobot;
  ScoreCountableBoard<BoardSize> SimulationBoard;
  EdgeScoreMap<BoardSize> SearchResult;
};
