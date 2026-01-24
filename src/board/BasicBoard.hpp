#pragma once

#include "../common/Array.hpp"
#include "../common/Span.hpp"
#include "../model/Edge.hpp"
#include "../model/Step.hpp"

template <int64_t BoardSize>
class BasicBoard {
  public:
  BasicBoard();

  void
  Add(const Edge<BoardSize> edge);
  bool
  Contains(const Edge<BoardSize> edge) const;
  Span<Edge<BoardSize>>
  EmptyEdges() const;
  Span<Edge<BoardSize>>
  MoveRecord() const;
  Step<BoardSize>
  GetStep() const;

  private:
  Step<BoardSize> Step;
  Array<Edge<BoardSize>, Edge<BoardSize>::Max> Edges;
  Array<SizeType<BoardSize>, Edge<BoardSize>::Max> EdgeIndexes;
};

template <int64_t BoardSize>
BasicBoard<BoardSize>::BasicBoard() {
  for (const Edge<BoardSize> edge : Iota(Edge<BoardSize>::Max)) {
    EdgeIndexes.At(edge.Value()) = edge.Value();
    Edges.At(edge.Value()) = edge;
  }
}

template <int64_t BoardSize>
void
BasicBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  assert(!Contains(edge));
  const Edge<BoardSize> nowEdge = Edges.At(Step.NowStep());
  const SizeType<BoardSize> edgeIndex = EdgeIndexes.At(edge.Value());
  std::swap(Edges.At(edgeIndex), Edges.At(Step.NowStep()));
  EdgeIndexes.At(edge.Value()) = Step.NowStep();
  EdgeIndexes.At(nowEdge.Value()) = edgeIndex;
  Step.Go();
}

template <int64_t BoardSize>
bool
BasicBoard<BoardSize>::Contains(const Edge<BoardSize> edge) const {
  return EdgeIndexes.At(edge.Value()) < Step.NowStep();
}

template <int64_t BoardSize>
Span<Edge<BoardSize>>
BasicBoard<BoardSize>::EmptyEdges() const {
  return {Edges.begin() + Step.NowStep(), Edges.begin() + Edge<BoardSize>::Max};
}

template <int64_t BoardSize>
Span<Edge<BoardSize>>
BasicBoard<BoardSize>::MoveRecord() const {
  return {Edges.begin(), Edges.begin() + Step.NowStep()};
}

template <int64_t BoardSize>
Step<BoardSize>
BasicBoard<BoardSize>::GetStep() const {
  return Step;
}
