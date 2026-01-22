#pragma once

#include "../common/Array.hpp"
#include "../common/Span.hpp"
#include "../model/Edge.hpp"
#include "../model/Step.hpp"
#include "../model/ValueIterator.hpp"

template <int BoardSize, typename SizeType>
class BasicBoard {
  public:
  BasicBoard() {
    for (const Edge<BoardSize, SizeType> edge : ValueIterator<Edge<BoardSize, SizeType>, SizeType>()) {
      EdgeIndexes.At(edge.Value()) = edge.Value();
      Edges.At(edge.Value()) = edge;
    }
  }

  void
  Add(const Edge<BoardSize, SizeType> edge) {
    assert(NotContains(edge));
    const Edge<BoardSize, SizeType> nowEdge = Edges.At(Step.NowStep());
    const SizeType edgeIndex = EdgeIndexes.At(edge.Value());
    std::swap(Edges.At(edgeIndex), Edges.At(Step.NowStep()));
    EdgeIndexes.At(edge.Value()) = Step.NowStep();
    EdgeIndexes.At(nowEdge.Value()) = edgeIndex;
    Step.Go();
  }

  bool
  Contains(const Edge<BoardSize, SizeType> edge) const {
    return EdgeIndexes.At(edge.Value()) < Step.NowStep();
  }

  bool
  NotContains(const Edge<BoardSize, SizeType> edge) const {
    return EdgeIndexes.At(edge.Value()) >= Step.NowStep();
  }

  Span<Edge<BoardSize, SizeType>, SizeType>
  EmptyEdges() const {
    return {Edges.begin() + Step.NowStep(), Edges.begin() + Edge<BoardSize, SizeType>::Max};
  }

  Span<Edge<BoardSize, SizeType>, SizeType>
  MoveRecord() const {
    return {Edges.begin(), Edges.begin() + Step.NowStep()};
  }

  Step<BoardSize, SizeType>
  GetStep() const {
    return Step;
  }

  private:
  Step<BoardSize, SizeType> Step;
  Array<Edge<BoardSize, SizeType>, Edge<BoardSize, SizeType>::Max, SizeType> Edges;
  Array<SizeType, Edge<BoardSize, SizeType>::Max, SizeType> EdgeIndexes;
};
