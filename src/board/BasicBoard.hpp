#pragma once

#include "../common/Array.hpp"
#include "../common/Span.hpp"
#include "../model/Edge.hpp"
#include "../model/Step.hpp"
#include "../model/ValueIterator.hpp"

namespace dab::board {

template <int BoardSize, typename SizeType>
class BasicBoard {
  public:
  BasicBoard() {
    for (const model::Edge<BoardSize, SizeType> edge :
         model::ValueIterator<model::Edge<BoardSize, SizeType>, SizeType>()) {
      EdgeIndexes.At(edge.Value()) = edge.Value();
      Edges.At(edge.Value()) = edge;
    }
  }

  void
  Add(const model::Edge<BoardSize, SizeType> edge) {
    assert(NotContains(edge));
    const model::Edge<BoardSize, SizeType> nowEdge = Edges.At(Step.NowStep());
    const SizeType edgeIndex = EdgeIndexes.At(edge.Value());
    std::swap(Edges.At(edgeIndex), Edges.At(Step.NowStep()));
    EdgeIndexes.At(edge.Value()) = Step.NowStep();
    EdgeIndexes.At(nowEdge.Value()) = edgeIndex;
    Step.Go();
  }

  bool
  Contains(const model::Edge<BoardSize, SizeType> edge) const {
    return EdgeIndexes.At(edge.Value()) < Step.NowStep();
  }

  bool
  NotContains(const model::Edge<BoardSize, SizeType> edge) const {
    return EdgeIndexes.At(edge.Value()) >= Step.NowStep();
  }

  common::Span<model::Edge<BoardSize, SizeType>, SizeType>
  EmptyEdges() const {
    return {Edges.begin() + Step.NowStep(), Edges.begin() + model::Edge<BoardSize, SizeType>::Max};
  }

  common::Span<model::Edge<BoardSize, SizeType>, SizeType>
  MoveRecord() const {
    return {Edges.begin(), Edges.begin() + Step.NowStep()};
  }

  model::Step<BoardSize, SizeType>
  GetStep() const {
    return Step;
  }

  private:
  model::Step<BoardSize, SizeType> Step;
  common::Array<model::Edge<BoardSize, SizeType>, model::Edge<BoardSize, SizeType>::Max, SizeType> Edges;
  common::Array<SizeType, model::Edge<BoardSize, SizeType>::Max, SizeType> EdgeIndexes;
};

}  // namespace dab::board
