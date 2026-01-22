#pragma once

#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "BasicSearchRobot.hpp"

namespace dab::robot {

template <int BoardSize, typename SizeType>
class ImprovedSearchRobot final : public Robot<BoardSize, SizeType> {
  public:
  ImprovedSearchRobot() = default;

  common::Span<model::Edge<BoardSize, SizeType>, SizeType>
  BestCandidateEdges(const board::ScoreCountableBoard<BoardSize, SizeType>& board) override {
    if (common::Span<model::Edge<BoardSize, SizeType>, SizeType> edges = SubRobot.BestCandidateEdges(board);
        !SubRobot.SubRobot.EnemyUnscoreableEdges.Empty()) {
      return edges;
    }

    SearchEdges.Clear();
    SizeType maxScore = -model::Box<BoardSize, SizeType>::Max;
    for (const model::Edge<BoardSize, SizeType> emptyEdge :
         board.GetEdgeCountableBoard().GetBasicBoard().EmptyEdges()) {
      SimulationBoard.Reset(board.GetEdgeCountableBoard());
      SimulationBoard.Add(emptyEdge);
      while (SimulationBoard.Gaming()) {
        const model::Edge<BoardSize, SizeType> edge = SubRobot.BestCandidateEdges(SimulationBoard).At(0);
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

    return common::Export<
        common::List<model::Edge<BoardSize, SizeType>, model::Edge<BoardSize, SizeType>::Max, SizeType>,
        SizeType>(SearchEdges);
  }

  private:
  BasicSearchRobot<BoardSize, SizeType> SubRobot;
  board::ScoreCountableBoard<BoardSize, SizeType> SimulationBoard;
  common::List<model::Edge<BoardSize, SizeType>, model::Edge<BoardSize, SizeType>::Max, SizeType> SearchEdges;
};

}  // namespace dab::robot
