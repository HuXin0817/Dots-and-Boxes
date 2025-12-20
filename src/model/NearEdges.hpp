#pragma once

#include <cassert>

#include "../common/Array.hpp"
#include "Edge.hpp"
#include "Square.hpp"

class NearEdgesMapper {
  NearEdgesMapper() {
    for (int x = 0; x < Box::Size; x++) {
      for (int y = 0; y < Box::Size; y++) {
        Dot topLeft(x, y);
        Dot topRight(x + 1, y);
        Dot bottomLeft(x, y + 1);
        Dot bottomRight(x + 1, y + 1);
        BoxNearEdges.At(Box(x, y)) = {
            Edge(topLeft, topRight),
            Edge(topLeft, bottomLeft),
            Edge(bottomLeft, bottomRight),
            Edge(topRight, bottomRight),
        };
      }
    }
  }

  Array<Array<Edge, 4>, Box::Max> BoxNearEdges;

  friend const Array<Edge, 4>&
  NearEdges(Box box);
};

inline const Array<Edge, 4>&
NearEdges(Box box) {
  static NearEdgesMapper NearEdgesMapperInstance;

  return NearEdgesMapperInstance.BoxNearEdges.At(box);
}
