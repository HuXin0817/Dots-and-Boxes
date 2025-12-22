#pragma once

#include "../common/Array.hpp"
#include "../common/Span.hpp"
#include "../model/Edge.hpp"
#include "../model/Step.hpp"

template <int BoardSize>
class BasicBoard {
  public:
  BasicBoard() {
    for (Edge<BoardSize> edge = 0; edge.Int() < Edge<BoardSize>::Max; edge.IntRef()++) {
      EdgeIndexes.At(edge.Int()) = edge.Int();
      Edges.At(edge.Int()) = edge;
    }
  }

  void
  Add(Edge<BoardSize> edge) {
    assert(NotContains(edge));
    auto nowEdge = Edges.At(Step.NowStep());
    int edgeIndex = EdgeIndexes.At(edge.Int());
    std::swap(Edges.At(edgeIndex), Edges.At(Step.NowStep()));
    EdgeIndexes.At(edge.Int()) = Step.NowStep();
    EdgeIndexes.At(nowEdge.Int()) = edgeIndex;
    Step.Go();
  }

  bool
  Contains(Edge<BoardSize> edge) const {
    return EdgeIndexes.At(edge.Int()) < Step.NowStep();
  }

  bool
  NotContains(Edge<BoardSize> edge) const {
    return EdgeIndexes.At(edge.Int()) >= Step.NowStep();
  }

  Span<Edge<BoardSize>>
  EmptyEdges() const {
    return {Edges.begin() + Step.NowStep(), Edges.begin() + Edge<BoardSize>::Max};
  }

  Span<Edge<BoardSize>>
  MoveRecord() const {
    return {Edges.begin(), Edges.begin() + Step.NowStep()};
  }

  const Step<BoardSize>&
  GetStep() const {
    return Step;
  }

  Step<BoardSize>&
  GetStep() {
    return Step;
  }

  private:
  Step<BoardSize> Step;
  Array<Edge<BoardSize>, Edge<BoardSize>::Max> Edges;
  Array<int, Edge<BoardSize>::Max> EdgeIndexes;
};
