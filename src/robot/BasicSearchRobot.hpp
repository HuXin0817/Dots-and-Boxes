#pragma once

#include "../board/ScoreableEdgeBoard.hpp"
#include "../common/Span.hpp"
#include "Robot.hpp"
#include "SimpleStrategyRobot.hpp"

template <int BoardSize, typename SizeType>
class ImprovedSearchRobot;

template <int BoardSize, typename SizeType>
class BasicSearchRobot final : public Robot<BoardSize, SizeType> {
  friend class ImprovedSearchRobot<BoardSize, SizeType>;

  public:
  Span<Edge<BoardSize, SizeType>, SizeType>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize, SizeType>& board) override {
    if (Span<Edge<BoardSize, SizeType>, SizeType> edges = SubRobot.BestCandidateEdges(board);
        !SubRobot.EnemyUnscoreableEdges.Empty() || !SubRobot.ScoreableEdges.Empty()) {
      return edges;
    }

    SizeType minScore = Box<BoardSize, SizeType>::Max + 1;
    List<Edge<BoardSize, SizeType>, Edge<BoardSize, SizeType>::Max, SizeType>& candidateEdges =
        SubRobot.EnemyUnscoreableEdges;
    assert(candidateEdges.Empty());

    for (const Edge<BoardSize, SizeType> edge : board.GetEdgeCountableBoard().GetBasicBoard().EmptyEdges()) {
      SimulationBoard.Reset(board.GetEdgeCountableBoard());
      SimulationBoard.Add(edge);
      if (const SizeType score = SimulationBoard.MaxObtainableScore(minScore); score < minScore) {
        minScore = score;
        candidateEdges.Reset(edge);
      } else if (score == minScore) {
        candidateEdges.Append(edge);
      }
    }

    return Export<List<Edge<BoardSize, SizeType>, Edge<BoardSize, SizeType>::Max, SizeType>, SizeType>(candidateEdges);
  }

  private:
  SimpleStrategyRobot<BoardSize, SizeType> SubRobot;
  ScoreableEdgeBoard<BoardSize, SizeType> SimulationBoard;
};
