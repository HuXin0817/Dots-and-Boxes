#include "MonteCarloRobot.h"

#include <ranges>

namespace dab::__detail__::robot {

void MonteCarloRobot::ScoreMap::Reset() {
  std::ranges::fill(Time, 0);
  std::ranges::fill(Score, 0);
}

void MonteCarloRobot::ScoreMap::Add(model::Edge edge, Int score) {
  ++Time.At(edge);
  Score.At(edge) += score;
}

void MonteCarloRobot::ScoreMap::Add(const ScoreMap& other) {
  for (const Int i : model::Iota<model::Edge>()) {
    Time.At(i) += other.Time.At(i);
    Score.At(i) += other.Score.At(i);
  }
}

iterable::Span<const model::Edge> MonteCarloRobot::ScoreMap::Export(
    iterable::List<model::Edge, model::Edge::Max>& edges) {
  edges.Clear();
  double maxScore = 0.0;
  for (const model::Edge edge : model::Iota<model::Edge>()) {
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
  iterable::Random random;
  SimulationBoard = board;
  const model::Edge edge = random.Choice(SubRobot.BestCandidateEdges(SimulationBoard));
  SimulationBoard.Add(edge);
  while (SimulationBoard.Gaming()) {
    SimulationBoard.Add(random.Choice(SubRobot.BestCandidateEdges(SimulationBoard)));
  }
  SearchResult.Add(edge, board.GetTurn() * SimulationBoard.RelativeScore());
}

template <typename Board>
iterable::Span<const model::Edge> MonteCarloRobot::BestCandidateEdges(const Board& board) {
  if (iterable::Span<const model::Edge> edges; CanEarlyExit(board, edges)) {
    return edges;
  }
  SearchResult.Reset();
  for (std::int64_t i = 0; i < SearchTime / board.RemainStep(); i++) {
    SearchOnce(board);
  }
  return SearchResult.Export(GetSearchEdges());
}

template <typename Board>
bool MonteCarloRobot::CanEarlyExit(const Board& board, iterable::Span<const model::Edge>& result) {
  return (result = SubRobot.BestCandidateEdges(board)).Size() == 1;
}

template iterable::Span<const model::Edge> MonteCarloRobot::BestCandidateEdges<board::GameBoard>(
    const board::GameBoard& board);

template bool MonteCarloRobot::CanEarlyExit(const board::GameBoard& board, iterable::Span<const model::Edge>& result);

}  // namespace dab::__detail__::robot