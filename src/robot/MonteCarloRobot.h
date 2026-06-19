#pragma once

#include "CachedRobot.h"
#include "SimulationRobot.h"

namespace dab::__detail__::robot {

class MonteCarloRobot {
 public:
  static constexpr std::int64_t SearchTime = static_cast<std::int64_t>(model::Edge::Max) << 8;

  struct ScoreMap {
    ScoreMap() { Reset(); }

    void Reset();
    void Add(model::Edge edge, Int score);
    void Add(const ScoreMap& other);
    iterable::Span<const model::Edge> Export(iterable::List<model::Edge, model::Edge::Max>& edges);

    iterable::Array<std::int64_t, model::Edge::Max> Time;
    iterable::Array<std::int64_t, model::Edge::Max> Score;
  };

  MonteCarloRobot() = default;
  template <typename Board>
  void SearchOnce(const Board& board);
  template <typename Board>
  iterable::Span<const model::Edge> BestCandidateEdges(const Board& board);
  template <typename Board>
  bool CanEarlyExit(const Board& board, iterable::Span<const model::Edge>& result);
  ScoreMap& GetSearchResult() { return SearchResult; }
  iterable::List<model::Edge, model::Edge::Max>& GetSearchEdges() { return SubRobot.GetSearchEdges(); }

 private:
  CachedRobot<SimulationRobot> SubRobot;
  board::RelativeScoreBoard SimulationBoard;
  ScoreMap SearchResult;
};

}  // namespace dab::__detail__::robot