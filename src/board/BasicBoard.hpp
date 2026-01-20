#pragma once

#include "../common/Array.hpp"
#include "../common/Span.hpp"
#include "../model/Edge.hpp"
#include "../model/Step.hpp"

namespace dab::board {

template <int BoardSize>
class BasicBoard {
  public:
  BasicBoard() {
    for (model::Edge<BoardSize> edge = 0; edge.Value() < model::Edge<BoardSize>::Max; ++edge) {
      EdgeIndexes.At(edge.Value()) = edge.Value();
      Edges.At(edge.Value()) = edge;
    }
  }

  void
  Add(const model::Edge<BoardSize> edge) {
    assert(NotContains(edge));
    const model::Edge<BoardSize> nowEdge = Edges.At(Step.NowStep());
    const int edgeIndex = EdgeIndexes.At(edge.Value());
    std::swap(Edges.At(edgeIndex), Edges.At(Step.NowStep()));
    EdgeIndexes.At(edge.Value()) = Step.NowStep();
    EdgeIndexes.At(nowEdge.Value()) = edgeIndex;
    Step.Go();
  }

  bool
  Contains(const model::Edge<BoardSize> edge) const {
    return EdgeIndexes.At(edge.Value()) < Step.NowStep();
  }

  bool
  NotContains(const model::Edge<BoardSize> edge) const {
    return EdgeIndexes.At(edge.Value()) >= Step.NowStep();
  }

  common::Span<model::Edge<BoardSize>>
  EmptyEdges() const {
    return {Edges.begin() + Step.NowStep(), Edges.begin() + model::Edge<BoardSize>::Max};
  }

  common::Span<model::Edge<BoardSize>>
  MoveRecord() const {
    return {Edges.begin(), Edges.begin() + Step.NowStep()};
  }

  model::Step<BoardSize>
  GetStep() const {
    return Step;
  }

  private:
  model::Step<BoardSize> Step;
  common::Array<model::Edge<BoardSize>, model::Edge<BoardSize>::Max> Edges;
  common::Array<int, model::Edge<BoardSize>::Max> EdgeIndexes;
};

}  // namespace dab::board
