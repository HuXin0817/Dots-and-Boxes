#pragma once

#include <cassert>

#include "../common/Array.hpp"
#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "Edge.hpp"
#include "Square.hpp"

class EdgeBoxMapper {
  Array<Array<Edge, 4>, Box::Max>
  GetBoxNearEdges() {
    Array<Array<Edge, 4>, Box::Max> boxNearEdges;

    for (int x = 0; x < Box::Size; x++) {
      for (int y = 0; y < Box::Size; y++) {
        Dot topLeft(x, y);
        Dot topRight(x + 1, y);
        Dot bottomLeft(x, y + 1);
        Dot bottomRight(x + 1, y + 1);
        boxNearEdges.At(Box(x, y)) = {
            Edge(topLeft, topRight),
            Edge(topLeft, bottomLeft),
            Edge(bottomLeft, bottomRight),
            Edge(topRight, bottomRight),
        };
      }
    }

    return boxNearEdges;
  }

  List<Box, 2>
  GetNearBoxes(Edge edge) {
    List<Box, 2> result;

    int x = edge.Dot2().X() - 1;
    int y = edge.Dot2().Y() - 1;
    if (x >= 0 && y >= 0) {
      result.Append(Box(x, y));
    }

    x = edge.Dot1().X();
    y = edge.Dot1().Y();
    if (x < BoardSize && y < BoardSize) {
      result.Append(Box(x, y));
    }

    return result;
  }

  Array<Span<Box>, Edge::Max>
  GetEdgeNearBoxes() {
    Array<List<Box, 2>, Edge::Max> edgeNearBoxes;
    for (Edge edge = 0; edge < Edge::Max; edge++) {
      edgeNearBoxes.At(edge) = GetNearBoxes(edge);
    }

    Array<Array<int, Box::Max>, Box::Max> adjacencyMatrix = {};
    Array<int, Box::Max> boxDegrees = {};

    for (Edge edge = 0; edge < Edge::Max; edge++) {
      if (edgeNearBoxes.At(edge).Size() == 2) {
        int firstBox = edgeNearBoxes.At(edge).At(0);
        int secondBox = edgeNearBoxes.At(edge).At(1);
        adjacencyMatrix.At(firstBox).At(secondBox)++;
        adjacencyMatrix.At(secondBox).At(firstBox)++;
        boxDegrees.At(firstBox)++;
        boxDegrees.At(secondBox)++;
      }
    }

    List<int, Box::Max> oddDegreeBoxes;
    for (int box = 0; box < Box::Max; box++) {
      if (boxDegrees.At(box) % 2 == 1) {
        oddDegreeBoxes.Append(box);
      }
    }

    auto findEulerianPath = [&](int startBox) -> List<int, Edge::Max> {
      List<int, Edge::Max> eulerianPath;
      Array<int, Edge::Max> pathStack;
      int stackSize = 0;
      pathStack.At(stackSize++) = startBox;

      while (stackSize > 0) {
        int currentBox = pathStack.At(stackSize - 1);

        bool found = false;
        for (int nextBox = 0; nextBox < Box::Max; nextBox++) {
          if (adjacencyMatrix.At(currentBox).At(nextBox) > 0) {
            found = true;
            pathStack.At(stackSize++) = nextBox;
            adjacencyMatrix.At(currentBox).At(nextBox)--;
            adjacencyMatrix.At(nextBox).At(currentBox)--;
            boxDegrees.At(currentBox)--;
            boxDegrees.At(nextBox)--;
            break;
          }
        }

        if (!found) {
          eulerianPath.Append(currentBox);
          stackSize--;
        }
      }

      int pathLength = eulerianPath.Size();
      for (int i = 0; i < pathLength / 2; i++) {
        int swappedBox = eulerianPath.At(i);
        eulerianPath.At(i) = eulerianPath.At(pathLength - 1 - i);
        eulerianPath.At(pathLength - 1 - i) = swappedBox;
      }
      return eulerianPath;
    };

    int nearBoxesBufferIndex = 0;
    Array<List<int, Edge::Max>, Box::Max> boxToIndices;
    Array<Array<int, Box::Max>, Box::Max> boxPairToIndex;
    for (int i = 0; i < Box::Max; i++) {
      for (int j = 0; j < Box::Max; j++) {
        boxPairToIndex.At(i).At(j) = NearBoxesBufferSize;
      }
    }

    while (true) {
      int startBox = -1;
      for (int box = 0; box < Box::Max; box++) {
        if (boxDegrees.At(box) > 0) {
          if (boxDegrees.At(box) % 2 == 1) {
            startBox = box;
            break;
          } else if (startBox == -1) {
            startBox = box;
          }
        }
      }

      if (startBox == -1) {
        break;
      }

      List<int, Edge::Max> eulerianPath = findEulerianPath(startBox);
      if (eulerianPath.Empty()) {
        continue;
      }

      for (int i = 0; i < eulerianPath.Size(); i++) {
        int box = eulerianPath.At(i);
        NearBoxesBuffer.At(nearBoxesBufferIndex++) = box;
        boxToIndices.At(box).Append(nearBoxesBufferIndex - 1);

        if (i > 0) {
          int prevBox = eulerianPath.At(i - 1);
          int minBox = prevBox < box ? prevBox : box;
          int maxBox = prevBox < box ? box : prevBox;
          if (boxPairToIndex.At(minBox).At(maxBox) == NearBoxesBufferSize) {
            boxPairToIndex.At(minBox).At(maxBox) = nearBoxesBufferIndex - 2;
          }
        }
      }
    }

    for (Edge edge = 0; edge < Edge::Max; edge++) {
      if (edgeNearBoxes.At(edge).Size() == 1) {
        int box = edgeNearBoxes.At(edge).At(0);
        if (boxToIndices.At(box).Empty()) {
          NearBoxesBuffer.At(nearBoxesBufferIndex++) = box;
          boxToIndices.At(box).Append(nearBoxesBufferIndex - 1);
        }
      }
    }

    Array<Span<Box>, Edge::Max> edgeToBoxSpans;

    Array<bool, Edge::Max> edgeProcessed = {};
    for (Edge edge = 0; edge < Edge::Max; edge++) {
      if (edgeNearBoxes.At(edge).Size() == 2) {
        int firstBox = edgeNearBoxes.At(edge).At(0);
        int secondBox = edgeNearBoxes.At(edge).At(1);
        int minBox = firstBox < secondBox ? firstBox : secondBox;
        int maxBox = firstBox < secondBox ? secondBox : firstBox;

        int foundIndex = boxPairToIndex.At(minBox).At(maxBox);
        if (foundIndex < NearBoxesBufferSize && foundIndex < nearBoxesBufferIndex - 1) {
          if ((NearBoxesBuffer.At(foundIndex) == firstBox &&
               NearBoxesBuffer.At(foundIndex + 1) == secondBox) ||
              (NearBoxesBuffer.At(foundIndex) == secondBox &&
               NearBoxesBuffer.At(foundIndex + 1) == firstBox)) {
            edgeToBoxSpans.At(edge) = {
                NearBoxesBuffer.begin() + foundIndex,
                NearBoxesBuffer.begin() + foundIndex + 2,
            };
            edgeProcessed.At(edge) = true;
            continue;
          }
        }

        bool found = false;
        int firstBoxIndicesSize = boxToIndices.At(firstBox).Size();
        int secondBoxIndicesSize = boxToIndices.At(secondBox).Size();

        if (firstBoxIndicesSize <= secondBoxIndicesSize) {
          for (int idx = 0; idx < firstBoxIndicesSize; idx++) {
            int i = boxToIndices.At(firstBox).At(idx);
            if (i < nearBoxesBufferIndex - 1) {
              if ((NearBoxesBuffer.At(i) == firstBox && NearBoxesBuffer.At(i + 1) == secondBox) ||
                  (NearBoxesBuffer.At(i) == secondBox && NearBoxesBuffer.At(i + 1) == firstBox)) {
                edgeToBoxSpans.At(edge) = {
                    NearBoxesBuffer.begin() + i,
                    NearBoxesBuffer.begin() + i + 2,
                };
                edgeProcessed.At(edge) = true;
                boxPairToIndex.At(minBox).At(maxBox) = i;
                found = true;
                break;
              }
            }

            if (i > 0 && i - 1 < nearBoxesBufferIndex - 1) {
              if ((NearBoxesBuffer.At(i - 1) == firstBox && NearBoxesBuffer.At(i) == secondBox) ||
                  (NearBoxesBuffer.At(i - 1) == secondBox && NearBoxesBuffer.At(i) == firstBox)) {
                edgeToBoxSpans.At(edge) = {
                    NearBoxesBuffer.begin() + (i - 1),
                    NearBoxesBuffer.begin() + (i + 1),
                };
                edgeProcessed.At(edge) = true;
                boxPairToIndex.At(minBox).At(maxBox) = i - 1;
                found = true;
                break;
              }
            }
          }
        } else {
          for (int idx = 0; idx < secondBoxIndicesSize; idx++) {
            int i = boxToIndices.At(secondBox).At(idx);
            if (i < nearBoxesBufferIndex - 1) {
              if ((NearBoxesBuffer.At(i) == firstBox && NearBoxesBuffer.At(i + 1) == secondBox) ||
                  (NearBoxesBuffer.At(i) == secondBox && NearBoxesBuffer.At(i + 1) == firstBox)) {
                edgeToBoxSpans.At(edge) = {
                    NearBoxesBuffer.begin() + i,
                    NearBoxesBuffer.begin() + i + 2,
                };
                edgeProcessed.At(edge) = true;
                boxPairToIndex.At(minBox).At(maxBox) = i;
                found = true;
                break;
              }
            }
            if (i > 0 && i - 1 < nearBoxesBufferIndex - 1) {
              if ((NearBoxesBuffer.At(i - 1) == firstBox && NearBoxesBuffer.At(i) == secondBox) ||
                  (NearBoxesBuffer.At(i - 1) == secondBox && NearBoxesBuffer.At(i) == firstBox)) {
                edgeToBoxSpans.At(edge) = {
                    NearBoxesBuffer.begin() + (i - 1),
                    NearBoxesBuffer.begin() + (i + 1),
                };
                edgeProcessed.At(edge) = true;
                boxPairToIndex.At(minBox).At(maxBox) = i - 1;
                found = true;
                break;
              }
            }
          }
        }

        if (!found) {
          for (int i = 0; i < nearBoxesBufferIndex - 1; i++) {
            if ((NearBoxesBuffer.At(i) == firstBox && NearBoxesBuffer.At(i + 1) == secondBox) ||
                (NearBoxesBuffer.At(i) == secondBox && NearBoxesBuffer.At(i + 1) == firstBox)) {
              edgeToBoxSpans.At(edge) = {
                  NearBoxesBuffer.begin() + i,
                  NearBoxesBuffer.begin() + i + 2,
              };
              edgeProcessed.At(edge) = true;
              boxPairToIndex.At(minBox).At(maxBox) = i;
              break;
            }
          }
        }
      } else if (edgeNearBoxes.At(edge).Size() == 1) {
        int box = edgeNearBoxes.At(edge).At(0);
        int index = boxToIndices.At(box).At(0);
        edgeToBoxSpans.At(edge) = {
            NearBoxesBuffer.begin() + index,
            NearBoxesBuffer.begin() + index + 1,
        };
        edgeProcessed.At(edge) = true;
      }
    }

    for (Edge edge = 0; edge < Edge::Max; edge++) {
      if (!edgeProcessed.At(edge) && edgeNearBoxes.At(edge).Size() == 2) {
        int firstBox = edgeNearBoxes.At(edge).At(0);
        int secondBox = edgeNearBoxes.At(edge).At(1);

        int startIndex = nearBoxesBufferIndex;
        NearBoxesBuffer.At(nearBoxesBufferIndex++) = firstBox;
        NearBoxesBuffer.At(nearBoxesBufferIndex++) = secondBox;
        int endIndex = nearBoxesBufferIndex;

        edgeToBoxSpans.At(edge) = {
            NearBoxesBuffer.begin() + startIndex,
            NearBoxesBuffer.begin() + endIndex,
        };
      }
    }

    assert(CheckBoxNearBoxes(edgeToBoxSpans));
    return edgeToBoxSpans;
  }

  bool
  CheckBoxNearBoxes(Array<Span<Box>, Edge::Max> edgeToBoxSpans) {
    for (Edge edge = 0; edge < Edge::Max; edge++) {
      auto expectedNearBoxes = GetNearBoxes(edge);
      if (edgeToBoxSpans.At(edge).Size() != expectedNearBoxes.Size()) {
        return false;
      }

      if (edgeToBoxSpans.At(edge).Size() == 1) {
        if (edgeToBoxSpans.At(edge).At(0) != expectedNearBoxes.At(0)) {
          return false;
        }
      } else {
        bool isEqual = false;
        if (edgeToBoxSpans.At(edge).At(0) == expectedNearBoxes.At(0) &&
            edgeToBoxSpans.At(edge).At(1) == expectedNearBoxes.At(1)) {
          isEqual = true;
        }

        if (edgeToBoxSpans.At(edge).At(0) == expectedNearBoxes.At(1) &&
            edgeToBoxSpans.At(edge).At(1) == expectedNearBoxes.At(0)) {
          isEqual = true;
        }

        if (!isEqual) {
          return false;
        }
      }
    }

    return true;
  }

  static constexpr int NearBoxesBufferSize = 4 * BoardSize * BoardSize - 8 * BoardSize + 3;
  Array<Box, NearBoxesBufferSize> NearBoxesBuffer;

  Array<Array<Edge, 4>, Box::Max> BoxNearEdges = GetBoxNearEdges();
  Array<Span<Box>, Edge::Max> EdgeNearBoxes = GetEdgeNearBoxes();

  friend const Array<Edge, 4>&
  NearEdges(Box box);
  friend Span<Box>
  NearBoxes(Edge edge);
};

static EdgeBoxMapper EdgeBoxMapperInstance;

inline const Array<Edge, 4>&
NearEdges(Box box) {
  return EdgeBoxMapperInstance.BoxNearEdges.At(box);
}

inline Span<Box>
NearBoxes(Edge edge) {
  return EdgeBoxMapperInstance.EdgeNearBoxes.At(edge);
}
