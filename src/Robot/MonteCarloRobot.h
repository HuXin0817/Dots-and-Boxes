#pragma once

#include "CachedRobot.h"
#include "SimulationRobot.h"

namespace dab::__detail__::robot {

class MonteCarloRobot {
 public:
  static constexpr std::int64_t SearchTime = static_cast<std::int64_t>(Edge::Max) << 8;

  struct ScoreMap {
    ScoreMap() { Reset(); }

    void Reset();
    void Add(Edge edge, Int score);
    void Add(const ScoreMap& other);
    Span<const Edge> Export(List<Edge, Edge::Max>& edges);

    Array<std::int64_t, Edge::Max> Time;
    Array<std::int64_t, Edge::Max> Score;
  };

  MonteCarloRobot() = default;
  template <typename Board>
  void SearchOnce(const Board& board);
  template <typename Board>
  Span<const Edge> BestCandidateEdges(const Board& board);
  template <typename Board>
  bool CanEarlyExit(const Board& board, Span<const Edge>& result);
  ScoreMap& GetSearchResult() { return SearchResult; }
  List<Edge, Edge::Max>& GetSearchEdges() { return SubRobot.GetSearchEdges(); }

 private:
  CachedRobot<SimulationRobot> SubRobot;
  RelativeScoreBoard SimulationBoard;
  ScoreMap SearchResult;
};

inline void MonteCarloRobot::ScoreMap::Reset() {
  std::ranges::fill(Time, 0);
  std::ranges::fill(Score, 0);
}

inline void MonteCarloRobot::ScoreMap::Add(Edge edge, Int score) {
  ++Time.At(edge);
  Score.At(edge) += score;
}

inline void MonteCarloRobot::ScoreMap::Add(const ScoreMap& other) {
  for (const Int i : Iota<Edge>()) {
    Time.At(i) += other.Time.At(i);
    Score.At(i) += other.Score.At(i);
  }
}

inline Span<const Edge> MonteCarloRobot::ScoreMap::Export(List<Edge, Edge::Max>& edges) {
  edges.Clear();
  double maxScore = 0.0;
  for (const Edge edge : Iota<Edge>()) {
    if (Time.At(edge) > 0) {
      if (const double score = static_cast<double>(Score.At(edge)) / static_cast<double>(Time.At(edge));
          score > maxScore || edges.Empty()) {
        maxScore = score;
        edges.ClearAndSet(edge);
      } else if (score == maxScore) {
        edges.Append(edge);
      }
    }
  }
  return edges;
}

template <typename Board>
void MonteCarloRobot::SearchOnce(const Board& board) {
  Random random;
  SimulationBoard = board;
  const Edge edge = random.Choice(SubRobot.BestCandidateEdges(SimulationBoard));
  SimulationBoard.Add(edge);
  while (SimulationBoard.Gaming()) {
    SimulationBoard.Add(random.Choice(SubRobot.BestCandidateEdges(SimulationBoard)));
  }
  SearchResult.Add(edge, board.GetTurn() * SimulationBoard.RelativeScore());
}

template <typename Board>
Span<const Edge> MonteCarloRobot::BestCandidateEdges(const Board& board) {
  if (Span<const Edge> edges; CanEarlyExit(board, edges)) {
    return edges;
  }
  SearchResult.Reset();
  for (std::int64_t i = 0; i < SearchTime / board.RemainStep(); i++) {
    SearchOnce(board);
  }
  return SearchResult.Export(GetSearchEdges());
}

template <typename Board>
bool MonteCarloRobot::CanEarlyExit(const Board& board, Span<const Edge>& result) {
  return (result = SubRobot.BestCandidateEdges(board)).Size() == 1;
}

}  // namespace dab::__detail__::robot