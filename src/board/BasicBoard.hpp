#pragma once

#include "../common/Array.hpp"
#include "../common/Span.hpp"
#include "../model/Edge.hpp"
#include "../model/Step.hpp"

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
  Span<const Edge<BoardSize>>
  EmptyEdges() const;
  Span<const Edge<BoardSize>>
  MoveRecord() const;

  private:
  Array<Edge<BoardSize>, Edge<BoardSize>::Max> Edges;
  Array<SizeType<BoardSize>, Edge<BoardSize>::Max> EdgeIndexes;
};

template <int64_t BoardSize>
BasicBoard<BoardSize>::BasicBoard() {
  Reset();
}

template <int64_t BoardSize>
void
BasicBoard<BoardSize>::Reset() {
  Step<BoardSize>::Reset();
  for (const Edge<BoardSize> edge : Iota(Edge<BoardSize>::Max)) {
    EdgeIndexes.At(edge) = edge;
    Edges.At(edge) = edge;
  }
}

template <int64_t BoardSize>
void
BasicBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  assert(!Contains(edge));
  const Edge<BoardSize> nowEdge = Edges.At(Step<BoardSize>::NowStep());
  const SizeType<BoardSize> edgeIndex = EdgeIndexes.At(edge);
  std::swap(Edges.At(edgeIndex), Edges.At(Step<BoardSize>::NowStep()));
  EdgeIndexes.At(edge) = Step<BoardSize>::NowStep();
  EdgeIndexes.At(nowEdge) = edgeIndex;
  Step<BoardSize>::Add();
}

template <int64_t BoardSize>
bool
BasicBoard<BoardSize>::Contains(const Edge<BoardSize> edge) const {
  return EdgeIndexes.At(edge) < Step<BoardSize>::NowStep();
}

template <int64_t BoardSize>
Span<const Edge<BoardSize>>
BasicBoard<BoardSize>::EmptyEdges() const {
  return Span<const Edge<BoardSize>>(Edges.begin() + Step<BoardSize>::NowStep(), Edges.begin() + Edge<BoardSize>::Max);
}

template <int64_t BoardSize>
Span<const Edge<BoardSize>>
BasicBoard<BoardSize>::MoveRecord() const {
  return Span<const Edge<BoardSize>>(Edges.begin(), Edges.begin() + Step<BoardSize>::NowStep());
}

}  // namespace dab
