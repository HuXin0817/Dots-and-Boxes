#pragma once

#include "../Common.h"
#include "GreedyRobot.h"

namespace dab::__detail__::robot {

class ImproveGreedyRobot : public GreedyRobot {
 public:
  ImproveGreedyRobot() = default;

  template <typename Board>
  Span<const Edge> BestCandidateEdges(const Board& board);
  template <typename Board>
  Edge SearchOne(const Board& board);

 private:
  Int CandidateEdgesSize = 0;
  ScoreableCountBoard SimulationBoardBackup;
  ScoreableCountBoard SimulationBoard;
};

template <typename Board>
Span<const Edge> ImproveGreedyRobot::BestCandidateEdges(const Board& board) {
  if (const Span<const Edge> edges = GreedyRobot::BestCandidateEdges(board); EnemyUnscoreable() || Scoreable()) {
    return edges;
  }

  Int minScore = Box::Max + 1;

  CandidateEdgesSize = 0;
  SimulationBoardBackup = board;
  for (const Edge edge : board.EmptyEdges()) {
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
Edge ImproveGreedyRobot::SearchOne(const Board& board) {
  Edge result = GreedyRobot::SearchOne(board);
  if (result != Edge::Invalid) {
    return result;
  }

  Int minScore = Box::Max + 1;
  SimulationBoardBackup = board;
  for (const Edge edge : board.EmptyEdges()) {
    SimulationBoard = SimulationBoardBackup;
    SimulationBoard.Add(edge);
    if (const Int score = SimulationBoard.MaxObtainableScore(minScore); score < minScore) {
      minScore = score;
      result = edge;
    }
  }

  return result;
}

}  // namespace dab::__detail__::robot