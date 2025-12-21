#pragma once

#include <string>

#include "../common/Array.hpp"
#include "../common/Span.hpp"
#include "../model/Edge.hpp"
#include "../model/Step.hpp"

template <int BoardSize>
class BasicBoard {
  public:
  BasicBoard() {
    for (Edge<BoardSize> edge = 0; edge < Edge<BoardSize>::Max; edge++) {
      EdgeIndexes.At(edge) = edge;
      Edges.At(edge) = edge;
    }
  }

  void
  Add(Edge<BoardSize> edge) {
    assert(NotContains(edge));
    Edge<BoardSize> nowEdge = Edges.At(Step.NowStep());
    int edgeIndex = EdgeIndexes.At(edge);
    std::swap(Edges.At(edgeIndex), Edges.At(Step.NowStep()));
    EdgeIndexes.At(edge) = Step.NowStep();
    EdgeIndexes.At(nowEdge) = edgeIndex;
    Step.Go();
  }

  bool
  Contains(Edge<BoardSize> edge) const {
    return EdgeIndexes.At(edge) < Step.NowStep();
  }

  bool
  NotContains(Edge<BoardSize> edge) const {
    return EdgeIndexes.At(edge) >= Step.NowStep();
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
