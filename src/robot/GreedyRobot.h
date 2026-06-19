#pragma once

#include "../Model.h"

namespace dab::__detail__::robot {

class GreedyRobot {
 public:
  GreedyRobot() = default;

  bool EnemyUnscoreable() const { return EnemyUnscoreableIndex < model::Edge::Max; }
  bool Scoreable() const { return ScoreableIndex > 0; }
  template <typename Board>
  iterable::Span<const model::Edge> BestCandidateEdges(const Board& board);
  template <typename Board>
  model::Edge SearchOne(const Board& board);

 protected:
  iterable::Array<model::Edge, model::Edge::Max> Edges;

 private:
  Int EnemyUnscoreableIndex = 0;
  Int ScoreableIndex = 0;
};

}  // namespace dab::__detail__::robot