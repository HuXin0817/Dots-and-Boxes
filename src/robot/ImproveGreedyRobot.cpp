#include "ImproveGreedyRobot.h"

namespace dab::__detail__::robot {

template <typename Board>
iterable::Span<const model::Edge> ImproveGreedyRobot::BestCandidateEdges(const Board& board) {
  if (const iterable::Span<const model::Edge> edges = GreedyRobot::BestCandidateEdges(board);
      EnemyUnscoreable() || Scoreable()) {
    return edges;
  }

  Int minScore = model::Box::Max + 1;

  CandidateEdgesSize = 0;
  SimulationBoardBackup = board;
  for (const model::Edge edge : board.EmptyEdges()) {
    SimulationBoard = SimulationBoardBackup;
    SimulationBoard.Add(edge);
    if (const Int score = SimulationBoard.MaxObtainableScore(minScore); score < minScore) {
      minScore = score;
      CandidateEdgesSize = 1;
      Edges.At(0) = edge;
    } else if (score == minScore) {
      Edges.At(CandidateEdgesSize++) = edge;
    }
  }

  return {Edges.begin(), CandidateEdgesSize};
}

template <typename Board>
model::Edge ImproveGreedyRobot::SearchOne(const Board& board) {
  model::Edge result = GreedyRobot::SearchOne(board);
  if (result != model::Edge::Invalid) {
    return result;
  }

  Int minScore = model::Box::Max + 1;
  SimulationBoardBackup = board;
  for (const model::Edge edge : board.EmptyEdges()) {
    SimulationBoard = SimulationBoardBackup;
    SimulationBoard.Add(edge);
    if (const Int score = SimulationBoard.MaxObtainableScore(minScore); score < minScore) {
      minScore = score;
      result = edge;
    }
  }

  return result;
}

template iterable::Span<const model::Edge> ImproveGreedyRobot::BestCandidateEdges<board::RelativeScoreBoard>(
    const board::RelativeScoreBoard& board);
template iterable::Span<const model::Edge> ImproveGreedyRobot::BestCandidateEdges<board::GameBoard>(
    const board::GameBoard& board);

template model::Edge ImproveGreedyRobot::SearchOne<board::RelativeScoreBoard>(const board::RelativeScoreBoard& board);

}  // namespace dab::__detail__::robot