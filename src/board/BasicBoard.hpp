#pragma once

#include <Dab/Model.hpp>

namespace dab::detail::board {

template <int64_t BoardSize>
class BasicBoard {
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
  bool
  Gaming() const;
  Int<BoardSize>
  RemainStep() const;
  Int<BoardSize>
  NowStep() const;
  bool
  operator==(const BasicBoard& other) const;

  private:
  Int<BoardSize> Step = 0;
  Array<Edge<BoardSize>, Edge<BoardSize>::Max> Edges;
  Array<Int<BoardSize>, Edge<BoardSize>::Max> EdgeIndexes;
};

template <int64_t BoardSize>
BasicBoard<BoardSize>::BasicBoard() {
  Reset();
}

template <int64_t BoardSize>
void
BasicBoard<BoardSize>::Reset() {
  Step = 0;
  for (Edge<BoardSize> edge = 0; edge < Edge<BoardSize>::Max; ++edge) {
    EdgeIndexes[edge] = edge;
    Edges[edge] = edge;
  }
}

template <int64_t BoardSize>
void
BasicBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  assert(!Contains(edge));
  const Edge<BoardSize> nowEdge = Edges[Step];
  const Int<BoardSize> edgeIndex = EdgeIndexes[edge];
  Edges[Step] = Edges[edgeIndex];
  Edges[edgeIndex] = nowEdge;
  EdgeIndexes[edge] = Step;
  EdgeIndexes[nowEdge] = edgeIndex;
  ++Step;
}

template <int64_t BoardSize>
bool
BasicBoard<BoardSize>::Contains(const Edge<BoardSize> edge) const {
  return EdgeIndexes[edge] < Step;
}

template <int64_t BoardSize>
Span<Edge<BoardSize>>
BasicBoard<BoardSize>::EmptyEdges() const {
  return Span(Edges.begin() + Step, Edges.begin() + Edge<BoardSize>::Max);
}

template <int64_t BoardSize>
Span<Edge<BoardSize>>
BasicBoard<BoardSize>::MoveRecord() const {
  return Span(Edges.begin(), Edges.begin() + Step);
}

template <int64_t BoardSize>
bool
BasicBoard<BoardSize>::Gaming() const {
  return Step < Edge<BoardSize>::Max;
}

template <int64_t BoardSize>
Int<BoardSize>
BasicBoard<BoardSize>::RemainStep() const {
  return Edge<BoardSize>::Max - Step;
}

template <int64_t BoardSize>
Int<BoardSize>
BasicBoard<BoardSize>::NowStep() const {
  return Step;
}

template <int64_t BoardSize>
bool
BasicBoard<BoardSize>::operator==(const BasicBoard& other) const {
  if (other.Step != Step) {
    return false;
  }
  if (Step < RemainStep()) {
    for (const Edge<BoardSize> edge : other.MoveRecord()) {
      if (!Contains(edge)) {
        return false;
      }
    }
  } else {
    for (const Edge<BoardSize> edge : other.EmptyEdges()) {
      if (Contains(edge)) {
        return false;
      }
    }
  }
  return true;
}

}  // namespace dab::detail::board