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
    if (Span<Edge<BoardSize>> edges = SubRobot.BestCandidateEdges(board);
        !SubRobot.SubRobot.EnemyUnscoreableEdges.Empty()) {
      return edges;
    }

    SearchEdges.Clear();
    SizeType maxScore = -Box<BoardSize>::Max;
    for (const Edge<BoardSize> emptyEdge : board.GetEdgeCountableBoard().GetBasicBoard().EmptyEdges()) {
      SimulationBoard.Reset(board.GetEdgeCountableBoard());
      SimulationBoard.Add(emptyEdge);
      while (SimulationBoard.Gaming()) {
        const Edge<BoardSize> edge = SubRobot.BestCandidateEdges(SimulationBoard).At(0);
        assert(board.GetEdgeCountableBoard().GetEdgeCountOfBox().MaxCount(edge.Value()) > 1);
        SimulationBoard.Add(edge);
      }
      if (const SizeType score = SimulationBoard.GetScoreMap().Score(); score > maxScore) {
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
