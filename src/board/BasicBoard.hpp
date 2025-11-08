#pragma once

#include <string>

#include "../common/Array.hpp"
#include "../common/Span.hpp"
#include "../model/Edge.hpp"
#include "../model/Step.hpp"

class BasicBoard : public Step {
  public:
  BasicBoard() {
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
        Dot dot1(i, j);
        Dot dot2(i, j + 1);
        Edge edge(dot1, dot2);
        char ch = Contains(edge) ? '-' : ' ';
        grid.At(i * 2).At(j * 2 + 1) = ch;
      }
    }

    for (int i = 0; i < BoardSize; i++) {
      for (int j = 0; j < cols; j++) {
        Dot dot1(i, j);
        Dot dot2(i + 1, j);
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
  Array<Edge, Edge::Max> Edges;
  Array<int, Edge::Max> EdgeIndexes;
};
