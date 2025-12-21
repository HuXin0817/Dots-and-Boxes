#pragma once

#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "BasicSearchRobot.hpp"

template <int BoardSize>
class ImprovedSearchRobot final : public Robot<BoardSize> {
  public:
  ImprovedSearchRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override {
    if (auto edges = SubRobot.BestCandidateEdges(board);
        !SubRobot.GetSubRobot().GetEnemyUnscoreableEdges().Empty()) {
      return edges;
    }

    SearchEdges.Clear();
    int maxScore = -Box<BoardSize>::Max;
    for (Edge emptyEdge : board.GetEdgeCountableBoard().GetBasicBoard().EmptyEdges()) {
      SimulationBoard.Reset(board.GetEdgeCountableBoard());
      SimulationBoard.Add(emptyEdge);
      while (SimulationBoard.Gaming()) {
        Edge edge = SubRobot.BestCandidateEdges(SimulationBoard).At(0);
        assert(board.MaxCount(edge) > 1);
        SimulationBoard.Add(edge);
      }
      if (int score = SimulationBoard.GetScoreMap().Score(); score > maxScore) {
        maxScore = score;
        SearchEdges.Reset(emptyEdge);
      } else if (score == maxScore) {
        SearchEdges.Append(emptyEdge);
      }
    }

    return Export(SearchEdges);
  }

  private:
  BasicSearchRobot<BoardSize> SubRobot;
  ScoreCountableBoard<BoardSize> SimulationBoard;
  List<Edge<BoardSize>, Edge<BoardSize>::Max> SearchEdges;
};
