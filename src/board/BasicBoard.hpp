#pragma once

#include "../common/Array.hpp"
#include "../common/Span.hpp"
#include "../model/Edge.hpp"
#include "../model/Step.hpp"

template <int BoardSize>
class BasicBoard {
  public:
  BasicBoard() {
    for (const Edge<BoardSize> edge : Iota(Edge<BoardSize>::Max)) {
      EdgeIndexes.At(edge.Value()) = edge.Value();
      Edges.At(edge.Value()) = edge;
    }
  }

  void
  Add(const Edge<BoardSize> edge) {
    assert(NotContains(edge));
    const Edge<BoardSize> nowEdge = Edges.At(Step.NowStep());
    const SizeType edgeIndex = EdgeIndexes.At(edge.Value());
    std::swap(Edges.At(edgeIndex), Edges.At(Step.NowStep()));
    EdgeIndexes.At(edge.Value()) = Step.NowStep();
    EdgeIndexes.At(nowEdge.Value()) = edgeIndex;
    Step.Go();
  }

  bool
  Contains(const Edge<BoardSize> edge) const {
    return EdgeIndexes.At(edge.Value()) < Step.NowStep();
  }

  bool
  NotContains(const Edge<BoardSize> edge) const {
    return EdgeIndexes.At(edge.Value()) >= Step.NowStep();
  }

  Span<Edge<BoardSize>>
  EmptyEdges() const {
    return {Edges.begin() + Step.NowStep(), Edges.begin() + Edge<BoardSize>::Max};
  }

  Span<Edge<BoardSize>>
  MoveRecord() const {
    return {Edges.begin(), Edges.begin() + Step.NowStep()};
  }

  Step<BoardSize>
  GetStep() const {
    return Step;
  }

  private:
  Step<BoardSize> Step;
  Array<Edge<BoardSize>, Edge<BoardSize>::Max> Edges;
  Array<SizeType, Edge<BoardSize>::Max> EdgeIndexes;
};
