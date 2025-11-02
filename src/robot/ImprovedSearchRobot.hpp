#pragma once

#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "BasicSearchRobot.hpp"

class ImprovedSearchRobot final : public Robot {
  public:
  ImprovedSearchRobot() = default;

  Span<Edge>
  BestCandidateEdges(const ScoreCountableBoard& board) override {
    if (auto edges = SubRobot.BestCandidateEdges(board);
        !SubRobot.SubRobot.EnemyUnscoreableEdges.Empty()) {
      return edges;
    }

    SearchEdges.Clear();
    int maxScore = -Box::Max;
    for (Edge emptyEdge : board.EmptyEdges()) {
      SimulationBoard.Reset(board.GetEdgeCountableBoard());
      SimulationBoard.Add(emptyEdge);
      while (SimulationBoard.Gaming()) {
        Edge edge = SubRobot.BestCandidateEdges(SimulationBoard).At(0);
        assert(board.MaxCount(edge) > 1);
        SimulationBoard.Add(edge);
      }
      if (int score = SimulationBoard.Score(); score > maxScore) {
        maxScore = score;
        SearchEdges.Reset(emptyEdge);
      } else if (score == maxScore) {
        SearchEdges.Append(emptyEdge);
      }
    }

    return Export(SearchEdges);
  }

  private:
  BasicSearchRobot SubRobot;
  ScoreCountableBoard SimulationBoard;
  List<Edge, Edge::Max> SearchEdges;
};
