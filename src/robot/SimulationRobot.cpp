#include "SimulationRobot.h"

#include "../Board.h"

namespace dab::__detail__::robot {

template <typename Board>
iterable::Span<const model::Edge> SimulationRobot::BestCandidateEdges(const Board& board) {
  if (const iterable::Span<const model::Edge> edges = SubRobot.BestCandidateEdges(board); SubRobot.EnemyUnscoreable()) {
    return edges;
  }

  SearchEdges.Clear();
  Int maxScore = -model::Box::Max;
  const Int turn = board.GetTurn();
  for (const model::Edge emptyEdge : board.EmptyEdges()) {
    SimulationBoard = board;
    SimulationBoard.Add(emptyEdge);
    while (SimulationBoard.Gaming()) {
      const model::Edge edge = SubRobot.SearchOne(SimulationBoard);
      assert(board.MaxEdgeCount(edge) > 1);
      SimulationBoard.Add(edge);
    }
    if (const Int score = turn * SimulationBoard.RelativeScore(); score > maxScore) {
      maxScore = score;
      SearchEdges.ClearAndSet(emptyEdge);
    } else if (score == maxScore) {
      SearchEdges.Append(emptyEdge);
    }
  }

  return SearchEdges;
}

template iterable::Span<const model::Edge> SimulationRobot::BestCandidateEdges<board::RelativeScoreBoard>(
    const board::RelativeScoreBoard& board);

template iterable::Span<const model::Edge> SimulationRobot::BestCandidateEdges<board::GameBoard>(
    const board::GameBoard& board);

}  // namespace dab::__detail__::robot