#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "Robot.hpp"

template <int BoardSize>
class SimpleStrategyRobot final : public Robot<BoardSize> {
  public:
  SimpleStrategyRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override {
    EnemyUnscoreableEdges.Clear();
    ScoreableEdges.Clear();
    auto emptyEdges = board.GetEdgeCountableBoard().GetBasicBoard().EmptyEdges();

    for (auto edge : emptyEdges) {
      if (int maxCount = board.GetEdgeCountableBoard().GetEdgeCountOfBox().MaxCount(edge);
          maxCount == 3) {
        ScoreableEdges.Append(edge);
      } else if (maxCount < 2) {
        EnemyUnscoreableEdges.Append(edge);
      }
    }

    if (!ScoreableEdges.Empty()) {
      return Export(ScoreableEdges);
    }
    if (!EnemyUnscoreableEdges.Empty()) {
      return Export(EnemyUnscoreableEdges);
    }

    return {emptyEdges.begin(), emptyEdges.end()};
  }

  const List<Edge<BoardSize>, Edge<BoardSize>::Max>&
  GetEnemyUnscoreableEdges() const {
    return EnemyUnscoreableEdges;
  }

  List<Edge<BoardSize>, Edge<BoardSize>::Max>&
  GetEnemyUnscoreableEdges() {
    return EnemyUnscoreableEdges;
  }

  const List<Edge<BoardSize>, Edge<BoardSize>::Max>&
  GetScoreableEdges() const {
    return ScoreableEdges;
  }

  List<Edge<BoardSize>, Edge<BoardSize>::Max>&
  GetScoreableEdges() {
    return ScoreableEdges;
  }

  private:
  List<Edge<BoardSize>, Edge<BoardSize>::Max> EnemyUnscoreableEdges;
  List<Edge<BoardSize>, Edge<BoardSize>::Max> ScoreableEdges;
};
