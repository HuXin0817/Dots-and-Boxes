#pragma once

#include "../common/Array.hpp"
#include "../common/Span.hpp"
#include "../model/Edge.hpp"
#include "../model/Step.hpp"

class BoardV0 : public Step {
  public:
  BoardV0() {
    for (Edge edge = 0; edge < Edge::Max; edge++) {
      EdgeIndexes.At(edge) = edge;
      Edges.At(edge) = edge;
    }
  }

  void
  Add(Edge edge) {
    assert(NotContains(edge));
    Edge nowEdge = Edges.At(NowStep());
    int edgeIndex = EdgeIndexes.At(edge);
    std::swap(Edges.At(edgeIndex), Edges.At(NowStep()));
    EdgeIndexes.At(edge) = NowStep();
    EdgeIndexes.At(nowEdge) = edgeIndex;
    Go();
  }

  bool
  Contains(Edge edge) const {
    return EdgeIndexes.At(edge) < NowStep();
  }

  bool
  NotContains(Edge edge) const {
    return EdgeIndexes.At(edge) >= NowStep();
  }

  Span<Edge>
  EmptyEdges() const {
    return {Edges.begin() + NowStep(), Edges.begin() + Edge::Max};
  }

  Span<Edge>
  MoveRecord() const {
    return {Edges.begin(), Edges.begin() + NowStep()};
  }

  private:
  Array<Edge, Edge::Max> Edges;
  Array<int, Edge::Max> EdgeIndexes;
};
