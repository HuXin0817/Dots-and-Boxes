#pragma once

#include "../board/ScoreableEdgeBoard.hpp"
#include "../common/Span.hpp"
#include "SimpleStrategyRobot.hpp"

template <int64_t BoardSize>
class ImprovedSearchRobot;

template <int64_t BoardSize>
class BasicSearchRobot final : public Robot<BoardSize> {
  friend class ImprovedSearchRobot<BoardSize>;

  public:
  BasicSearchRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override {
    if (Span<Edge<BoardSize>> edges = SubRobot.BestCandidateEdges(board);
        !SubRobot.EnemyUnscoreableEdges.Empty() || !SubRobot.ScoreableEdges.Empty()) {
      return edges;
    }

    Int<BoardSize> minScore = Box<BoardSize>::Max + 1;
    List<Edge<BoardSize>, Edge<BoardSize>::Max>& candidateEdges = SubRobot.EnemyUnscoreableEdges;
    assert(candidateEdges.Empty());

    for (const Edge<BoardSize> edge : board.GetEdgeCountableBoard().GetBasicBoard().EmptyEdges()) {
      SimulationBoard.Reset(board.GetEdgeCountableBoard());
      SimulationBoard.Add(edge);
      if (const Int<BoardSize> score = SimulationBoard.MaxObtainableScore(minScore); score < minScore) {
        minScore = score;
        candidateEdges.Reset(edge);
      } else if (score == minScore) {
        candidateEdges.Append(edge);
      }
    }

    return Export(candidateEdges);
  }

  private:
  SimpleStrategyRobot<BoardSize> SubRobot;
  ScoreableEdgeBoard<BoardSize> SimulationBoard;
};
