#pragma once

#include "../common/Array.hpp"
#include "../common/Span.hpp"
#include "../model/Edge.hpp"
#include "../model/Step.hpp"
#include "../model/Iota.hpp"

namespace dab {

template <int64_t BoardSize>
class BasicBoard : public Step<BoardSize> {
  public:
  BasicBoard();

  void
  Reset();
  void
  Add(const Edge<BoardSize> edge);
  bool
  Contains(const Edge<BoardSize> edge) const;
  Span<Edge<BoardSize>>
  EmptyEdges() const;
  Span<Edge<BoardSize>>
  MoveRecord() const;

  private:
  Array<Edge<BoardSize>, Limits<Edge<BoardSize>>::Max> Edges;
  Array<SizeType<BoardSize>, Limits<Edge<BoardSize>>::Max> EdgeIndexes;
};

template <int64_t BoardSize>
BasicBoard<BoardSize>::BasicBoard() {
  Reset();
}

template <int64_t BoardSize>
void
BasicBoard<BoardSize>::Reset() {
  Step<BoardSize>::Reset();
  for (const Edge<BoardSize> edge : Iota<Edge<BoardSize>>()) {
    EdgeIndexes[edge] = edge;
    Edges[edge] = edge;
  }
}

template <int64_t BoardSize>
void
BasicBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  assert(!Contains(edge));
  const Edge<BoardSize> nowEdge = Edges[Step<BoardSize>::NowStep()];
  const SizeType<BoardSize> edgeIndex = EdgeIndexes[edge];
  std::swap(Edges[edgeIndex], Edges[Step<BoardSize>::NowStep()]);
  EdgeIndexes[edge] = Step<BoardSize>::NowStep();
  EdgeIndexes[nowEdge] = edgeIndex;
  Step<BoardSize>::Add();
}

template <int64_t BoardSize>
bool
BasicBoard<BoardSize>::Contains(const Edge<BoardSize> edge) const {
  return EdgeIndexes[edge] < Step<BoardSize>::NowStep();
}

template <int64_t BoardSize>
Span<Edge<BoardSize>>
BasicBoard<BoardSize>::EmptyEdges() const {
  return Span(Edges.begin() + Step<BoardSize>::NowStep(), Edges.begin() + Limits<Edge<BoardSize>>::Max);
}

template <int64_t BoardSize>
Span<Edge<BoardSize>>
BasicBoard<BoardSize>::MoveRecord() const {
  return Span(Edges.begin(), Edges.begin() + Step<BoardSize>::NowStep());
}

}  // namespace dab
