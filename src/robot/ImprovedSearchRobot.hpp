#pragma once

#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "BasicSearchRobot.hpp"

namespace dab::robot {

template <int BoardSize>
class ImprovedSearchRobot final : public Robot<BoardSize> {
  public:
  ImprovedSearchRobot() = default;

  common::Span<model::Edge<BoardSize>>
  BestCandidateEdges(const board::ScoreCountableBoard<BoardSize>& board) override {
    if (common::Span<model::Edge<BoardSize>> edges = SubRobot.BestCandidateEdges(board);
        !SubRobot.SubRobot.EnemyUnscoreableEdges.Empty()) {
      return edges;
    }

    SearchEdges.Clear();
    int maxScore = -model::Box<BoardSize>::Max;
    for (const model::Edge<BoardSize> emptyEdge : board.GetEdgeCountableBoard().GetBasicBoard().EmptyEdges()) {
      SimulationBoard.Reset(board.GetEdgeCountableBoard());
      SimulationBoard.Add(emptyEdge);
      while (SimulationBoard.Gaming()) {
        const model::Edge<BoardSize> edge = SubRobot.BestCandidateEdges(SimulationBoard).At(0);
        assert(board.GetEdgeCountableBoard().GetEdgeCountOfBox().MaxCount(edge.Value()) > 1);
        SimulationBoard.Add(edge);
      }
      if (const int score = SimulationBoard.GetScoreMap().Score(); score > maxScore) {
        maxScore = score;
        SearchEdges.Reset(emptyEdge);
      } else if (score == maxScore) {
        SearchEdges.Append(emptyEdge);
      }
    }

    return common::Export(SearchEdges);
  }

  private:
  BasicSearchRobot<BoardSize> SubRobot;
  board::ScoreCountableBoard<BoardSize> SimulationBoard;
  common::List<model::Edge<BoardSize>, model::Edge<BoardSize>::Max> SearchEdges;
};

}  // namespace dab::robot
