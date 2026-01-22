#pragma once

#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "BasicSearchRobot.hpp"

template <int BoardSize, typename SizeType>
class ImprovedSearchRobot final : public Robot<BoardSize, SizeType> {
  public:
  ImprovedSearchRobot() = default;

  Span<Edge<BoardSize, SizeType>, SizeType>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize, SizeType>& board) override {
    if (Span<Edge<BoardSize, SizeType>, SizeType> edges = SubRobot.BestCandidateEdges(board);
        !SubRobot.SubRobot.EnemyUnscoreableEdges.Empty()) {
      return edges;
    }

    SearchEdges.Clear();
    SizeType maxScore = -Box<BoardSize, SizeType>::Max;
    for (const Edge<BoardSize, SizeType> emptyEdge : board.GetEdgeCountableBoard().GetBasicBoard().EmptyEdges()) {
      SimulationBoard.Reset(board.GetEdgeCountableBoard());
      SimulationBoard.Add(emptyEdge);
      while (SimulationBoard.Gaming()) {
        const Edge<BoardSize, SizeType> edge = SubRobot.BestCandidateEdges(SimulationBoard).At(0);
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

    return Export<List<Edge<BoardSize, SizeType>, Edge<BoardSize, SizeType>::Max, SizeType>, SizeType>(SearchEdges);
  }

  private:
  BasicSearchRobot<BoardSize, SizeType> SubRobot;
  ScoreCountableBoard<BoardSize, SizeType> SimulationBoard;
  List<Edge<BoardSize, SizeType>, Edge<BoardSize, SizeType>::Max, SizeType> SearchEdges;
};
