#pragma once

#include "../common/Array.h"
#include "../common/Span.h"
#include "Edge.h"
#include "Square.h"

class EdgeBoxMapper {
  static Array<Array<Edge, 4>, Box::Max>
  GetBoxNearEdges() {
    Array<Array<Edge, 4>, Box::Max> BoxNearEdges;

    for (int x = 0; x < Box::Size; x++) {
      for (int y = 0; y < Box::Size; y++) {
        Dot d00(x, y);
        Dot d01(x + 1, y);
        Dot d10(x, y + 1);
        Dot d11(x + 1, y + 1);
        BoxNearEdges.At(Box(x, y)) = {
            Edge(d00, d01),
            Edge(d00, d10),
            Edge(d10, d11),
            Edge(d01, d11),
        };
      }
    }

    return BoxNearEdges;
  }

  static Array<Span<Box>, Edge::Max>
  GetEdgeNearBoxes() {
    static Array<Box, 2 * Edge::Max - BoardSize * 4> NearBoxes;
    int index = 0;

    auto nearBoxes = [&index](Edge edge) -> Span<Box> {
      Box* start = NearBoxes.begin() + index;

      int x = edge.Dot2().X() - 1;
      int y = edge.Dot2().Y() - 1;
      if (x >= 0 && y >= 0) {
        NearBoxes.At(index++) = Box(x, y);
      }

      x = edge.Dot1().X();
      y = edge.Dot1().Y();
      if (x < BoardSize && y < BoardSize) {
        NearBoxes.At(index++) = Box(x, y);
      }

      Box* end = NearBoxes.begin() + index;

      return {start, end};
    };

    Array<Span<Box>, Edge::Max> mapper;
    for (Edge edge = 0; edge < Edge::Max; edge++) {
      mapper.At(edge) = nearBoxes(edge);
    }
    return mapper;
  }

  public:
  static inline Array<Array<Edge, 4>, Box::Max> BoxNearEdges = GetBoxNearEdges();
  static inline Array<Span<Box>, Edge::Max> EdgeNearBoxes = GetEdgeNearBoxes();
};
