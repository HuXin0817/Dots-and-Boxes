#pragma once

#include "CachedRobot.h"
#include "SimulationRobot.h"

namespace dab::__detail__::robot {

class MonteCarloRobot {
  struct ScoreMap {
    ScoreMap() { Reset(); }

    void Reset();
    void Add(Edge edge, Int score);
    void Add(const ScoreMap& other);
    Span<const Edge> Export(List<Edge, Edge::Max>& edges);

    Array<int64_t, Edge::Max> Time;
    Array<int64_t, Edge::Max> Score;
  };

  static constexpr std::uint64_t SearchTime = static_cast<std::uint64_t>(Edge::Max) << 8;

 public:
  MonteCarloRobot() = default;
  template <typename Board>
  void SearchCandidateEdges(const Board& board);
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
  float maxScore = 0.0f;
  for (const Edge edge : Iota<Edge>()) {
    if (Time.At(edge) > 0) {
      if (const float score = static_cast<float>(Score.At(edge)) / static_cast<float>(Time.At(edge));
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
void MonteCarloRobot::SearchCandidateEdges(const Board& board) {
  Random random;
  SearchResult.Reset();
  const Int turn = board.GetTurn();
  for (std::uint64_t i = 0; i < SearchTime / board.RemainStep(); i++) {
    SimulationBoard = board;
    const Edge edge = random.Choice(SubRobot.BestCandidateEdges(SimulationBoard));
    SimulationBoard.Add(edge);
    while (SimulationBoard.Gaming()) {
      SimulationBoard.Add(random.Choice(SubRobot.BestCandidateEdges(SimulationBoard)));
    }
    SearchResult.Add(edge, turn * SimulationBoard.RelativeScore());
  }
}

template <typename Board>
Span<const Edge> MonteCarloRobot::BestCandidateEdges(const Board& board) {
  if (Span<const Edge> edges; CanEarlyExit(board, edges)) {
    return edges;
  }
  SearchCandidateEdges(board);
  return SearchResult.Export(GetSearchEdges());
}

template <typename Board>
bool MonteCarloRobot::CanEarlyExit(const Board& board, Span<const Edge>& result) {
  result = SubRobot.BestCandidateEdges(board);
  return result.Size() == 1;
}

}  // namespace dab::__detail__::robot