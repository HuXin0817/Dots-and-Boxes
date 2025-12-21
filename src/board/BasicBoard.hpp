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
    Edge nowEdge = Edges.At(Step.NowStep());
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

  operator std::string() const {
    constexpr int rows = BoardSize + 1;
    constexpr int cols = BoardSize + 1;
    constexpr int gridRows = rows * 2 - 1;
    constexpr int gridCols = cols * 2 - 1;

    Array<Array<char, gridCols>, gridRows> grid;
    for (int i = 0; i < gridRows; i++) {
      for (int j = 0; j < gridCols; j++) {
        grid.At(i).At(j) = ' ';
      }
    }

    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        grid.At(i * 2).At(j * 2) = '+';
      }
    }

    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < BoardSize; j++) {
        Dot<BoardSize> dot1(i, j);
        Dot<BoardSize> dot2(i, j + 1);
        Edge edge(dot1, dot2);
        char ch = Contains(edge) ? '-' : ' ';
        grid.At(i * 2).At(j * 2 + 1) = ch;
      }
    }

    for (int i = 0; i < BoardSize; i++) {
      for (int j = 0; j < cols; j++) {
        Dot<BoardSize> dot1(i, j);
        Dot<BoardSize> dot2(i + 1, j);
        Edge edge(dot1, dot2);
        char ch = Contains(edge) ? '|' : ' ';
        grid.At(i * 2 + 1).At(j * 2) = ch;
      }
    }

    std::string result;
    for (int i = 0; i < gridRows; i++) {
      for (int j = 0; j < gridCols; j++) {
        result += grid.At(i).At(j);
      }
      if (i < gridRows - 1) {
        result += '\n';
      }
    }

    return result;
  }

  private:
  Step<BoardSize> Step;
  Array<Edge<BoardSize>, Edge<BoardSize>::Max> Edges;
  Array<int, Edge<BoardSize>::Max> EdgeIndexes;
};
