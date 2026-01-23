#pragma once

#include "../board/ScoreableEdgeBoard.hpp"
#include "../common/Span.hpp"
#include "Robot.hpp"
#include "SimpleStrategyRobot.hpp"

template <int64_t BoardSize>
class ImprovedSearchRobot;

template <int64_t BoardSize>
class BasicSearchRobot final : public Robot<BoardSize> {
  friend class ImprovedSearchRobot<BoardSize>;

  public:
  Span<Edge<BoardSize>, SizeType<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override {
    if (Span<Edge<BoardSize>, SizeType<BoardSize>> edges = SubRobot.BestCandidateEdges(board);
        !SubRobot.EnemyUnscoreableEdges.Empty() || !SubRobot.ScoreableEdges.Empty()) {
      return edges;
    }

    SizeType<BoardSize> minScore = Box<BoardSize>::Max + 1;
    List<Edge<BoardSize>, Edge<BoardSize>::Max, SizeType<BoardSize>>& candidateEdges = SubRobot.EnemyUnscoreableEdges;
    assert(candidateEdges.Empty());

    for (const Edge<BoardSize> edge : board.GetEdgeCountableBoard().GetBasicBoard().EmptyEdges()) {
      SimulationBoard.Reset(board.GetEdgeCountableBoard());
      SimulationBoard.Add(edge);
      if (const SizeType<BoardSize> score = SimulationBoard.MaxObtainableScore(minScore); score < minScore) {
        minScore = score;
        candidateEdges.Reset(edge);
      } else if (score == minScore) {
        candidateEdges.Append(edge);
      }
    }

    return Export<List<Edge<BoardSize>, Edge<BoardSize>::Max, SizeType<BoardSize>>, SizeType<BoardSize>>(
        candidateEdges);
  }

  private:
  SimpleStrategyRobot<BoardSize> SubRobot;
  ScoreableEdgeBoard<BoardSize> SimulationBoard;
};
