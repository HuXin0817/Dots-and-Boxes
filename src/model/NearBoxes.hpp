#pragma once

#include <cassert>

#include "../common/Array.hpp"
#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "Edge.hpp"
#include "Square.hpp"

template <int BoardSize>
inline const Span<Box<BoardSize>>&
NearBoxes(Edge<BoardSize> edge);

template <int BoardSize>
class NearBoxesMapper {
  public:
  NearBoxesMapper() {
    Array<List<Box<BoardSize>, 2>, Edge<BoardSize>::Max> edgeNearBoxes;
    for (Edge<BoardSize> edge = 0; edge.Int() < Edge<BoardSize>::Max; edge.IntRef()++) {
      edgeNearBoxes.At(edge.Int()) = GetNearBoxes(edge);
    }

    Array<Array<int, Box<BoardSize>::Max>, Box<BoardSize>::Max> adjacencyMatrix = {};
    Array<int, Box<BoardSize>::Max> boxDegrees = {};

    for (Edge<BoardSize> edge = 0; edge.Int() < Edge<BoardSize>::Max; edge.IntRef()++) {
      if (edgeNearBoxes.At(edge.Int()).Size() == 2) {
        int firstBox = edgeNearBoxes.At(edge.Int()).At(0).Int();
        int secondBox = edgeNearBoxes.At(edge.Int()).At(1).Int();
        adjacencyMatrix.At(firstBox).At(secondBox)++;
        adjacencyMatrix.At(secondBox).At(firstBox)++;
        boxDegrees.At(firstBox)++;
        boxDegrees.At(secondBox)++;
      }
    }

    List<int, Box<BoardSize>::Max> oddDegreeBoxes;
    for (int box = 0; box < Box<BoardSize>::Max; box++) {
      if (boxDegrees.At(box) % 2 == 1) {
        oddDegreeBoxes.Append(box);
      }
    }

    auto findEulerianPath = [&](int startBox) -> List<int, Edge<BoardSize>::Max> {
      List<int, Edge<BoardSize>::Max> eulerianPath;
      Array<int, Edge<BoardSize>::Max> pathStack;
      int stackSize = 0;
      pathStack.At(stackSize++) = startBox;

      while (stackSize > 0) {
        int currentBox = pathStack.At(stackSize - 1);

        bool found = false;
        for (int nextBox = 0; nextBox < Box<BoardSize>::Max; nextBox++) {
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
    Array<List<int, Edge<BoardSize>::Max>, Box<BoardSize>::Max> boxToIndices;
    Array<Array<int, Box<BoardSize>::Max>, Box<BoardSize>::Max> boxPairToIndex;
    for (int i = 0; i < Box<BoardSize>::Max; i++) {
      for (int j = 0; j < Box<BoardSize>::Max; j++) {
        boxPairToIndex.At(i).At(j) = NearBoxesBufferSize;
      }
    }

    while (true) {
      int startBox = -1;
      for (int box = 0; box < Box<BoardSize>::Max; box++) {
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

      auto eulerianPath = findEulerianPath(startBox);
      if (eulerianPath.Empty()) {
        continue;
      }

      for (int i = 0; i < eulerianPath.Size(); i++) {
        Box<BoardSize> box = eulerianPath.At(i);
        NearBoxesBuffer.At(nearBoxesBufferIndex++) = box;
        boxToIndices.At(box.Int()).Append(nearBoxesBufferIndex - 1);

        if (i > 0) {
          int prevBox = eulerianPath.At(i - 1);
          int minBox = prevBox < box.Int() ? prevBox : box.Int();
          int maxBox = prevBox < box.Int() ? box.Int() : prevBox;
          if (boxPairToIndex.At(minBox).At(maxBox) == NearBoxesBufferSize) {
            boxPairToIndex.At(minBox).At(maxBox) = nearBoxesBufferIndex - 2;
          }
        }
      }
    }

    for (Edge<BoardSize> edge = 0; edge.Int() < Edge<BoardSize>::Max; edge.IntRef()++) {
      if (edgeNearBoxes.At(edge.Int()).Size() == 1) {
        int box = edgeNearBoxes.At(edge.Int()).At(0).Int();
        if (boxToIndices.At(box).Empty()) {
          NearBoxesBuffer.At(nearBoxesBufferIndex++) = box;
          boxToIndices.At(box).Append(nearBoxesBufferIndex - 1);
        }
      }
    }

    Array<bool, Edge<BoardSize>::Max> edgeProcessed = {};
    for (Edge<BoardSize> edge = 0; edge.Int() < Edge<BoardSize>::Max; edge.IntRef()++) {
      if (edgeNearBoxes.At(edge.Int()).Size() == 2) {
        int firstBox = edgeNearBoxes.At(edge.Int()).At(0).Int();
        int secondBox = edgeNearBoxes.At(edge.Int()).At(1).Int();
        int minBox = firstBox < secondBox ? firstBox : secondBox;
        int maxBox = firstBox < secondBox ? secondBox : firstBox;

        int foundIndex = boxPairToIndex.At(minBox).At(maxBox);
        if (foundIndex < NearBoxesBufferSize && foundIndex < nearBoxesBufferIndex - 1) {
          if ((NearBoxesBuffer.At(foundIndex).Int() == firstBox &&
               NearBoxesBuffer.At(foundIndex + 1).Int() == secondBox) ||
              (NearBoxesBuffer.At(foundIndex).Int() == secondBox &&
               NearBoxesBuffer.At(foundIndex + 1).Int() == firstBox)) {
            EdgeNearBoxes.At(edge.Int()) = {
                NearBoxesBuffer.begin() + foundIndex,
                NearBoxesBuffer.begin() + foundIndex + 2,
            };
            edgeProcessed.At(edge.Int()) = true;
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
              if ((NearBoxesBuffer.At(i).Int() == firstBox &&
                   NearBoxesBuffer.At(i + 1).Int() == secondBox) ||
                  (NearBoxesBuffer.At(i).Int() == secondBox &&
                   NearBoxesBuffer.At(i + 1).Int() == firstBox)) {
                EdgeNearBoxes.At(edge.Int()) = {
                    NearBoxesBuffer.begin() + i,
                    NearBoxesBuffer.begin() + i + 2,
                };
                edgeProcessed.At(edge.Int()) = true;
                boxPairToIndex.At(minBox).At(maxBox) = i;
                found = true;
                break;
              }
            }

            if (i > 0 && i - 1 < nearBoxesBufferIndex - 1) {
              if ((NearBoxesBuffer.At(i - 1).Int() == firstBox &&
                   NearBoxesBuffer.At(i).Int() == secondBox) ||
                  (NearBoxesBuffer.At(i - 1).Int() == secondBox &&
                   NearBoxesBuffer.At(i).Int() == firstBox)) {
                EdgeNearBoxes.At(edge.Int()) = {
                    NearBoxesBuffer.begin() + (i - 1),
                    NearBoxesBuffer.begin() + (i + 1),
                };
                edgeProcessed.At(edge.Int()) = true;
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
              if ((NearBoxesBuffer.At(i).Int() == firstBox &&
                   NearBoxesBuffer.At(i + 1).Int() == secondBox) ||
                  (NearBoxesBuffer.At(i).Int() == secondBox &&
                   NearBoxesBuffer.At(i + 1).Int() == firstBox)) {
                EdgeNearBoxes.At(edge.Int()) = {
                    NearBoxesBuffer.begin() + i,
                    NearBoxesBuffer.begin() + i + 2,
                };
                edgeProcessed.At(edge.Int()) = true;
                boxPairToIndex.At(minBox).At(maxBox) = i;
                found = true;
                break;
              }
            }
            if (i > 0 && i - 1 < nearBoxesBufferIndex - 1) {
              if ((NearBoxesBuffer.At(i - 1).Int() == firstBox &&
                   NearBoxesBuffer.At(i).Int() == secondBox) ||
                  (NearBoxesBuffer.At(i - 1).Int() == secondBox &&
                   NearBoxesBuffer.At(i).Int() == firstBox)) {
                EdgeNearBoxes.At(edge.Int()) = {
                    NearBoxesBuffer.begin() + (i - 1),
                    NearBoxesBuffer.begin() + (i + 1),
                };
                edgeProcessed.At(edge.Int()) = true;
                boxPairToIndex.At(minBox).At(maxBox) = i - 1;
                found = true;
                break;
              }
            }
          }
        }

        if (!found) {
          for (int i = 0; i < nearBoxesBufferIndex - 1; i++) {
            if ((NearBoxesBuffer.At(i).Int() == firstBox &&
                 NearBoxesBuffer.At(i + 1).Int() == secondBox) ||
                (NearBoxesBuffer.At(i).Int() == secondBox &&
                 NearBoxesBuffer.At(i + 1).Int() == firstBox)) {
              EdgeNearBoxes.At(edge.Int()) = {
                  NearBoxesBuffer.begin() + i,
                  NearBoxesBuffer.begin() + i + 2,
              };
              edgeProcessed.At(edge.Int()) = true;
              boxPairToIndex.At(minBox).At(maxBox) = i;
              break;
            }
          }
        }
      } else if (edgeNearBoxes.At(edge.Int()).Size() == 1) {
        int box = edgeNearBoxes.At(edge.Int()).At(0).Int();
        int index = boxToIndices.At(box).At(0);
        EdgeNearBoxes.At(edge.Int()) = {
            NearBoxesBuffer.begin() + index,
            NearBoxesBuffer.begin() + index + 1,
        };
        edgeProcessed.At(edge.Int()) = true;
      }
    }

    for (Edge<BoardSize> edge = 0; edge.Int() < Edge<BoardSize>::Max; edge.IntRef()++) {
      if (!edgeProcessed.At(edge.Int()) && edgeNearBoxes.At(edge.Int()).Size() == 2) {
        int firstBox = edgeNearBoxes.At(edge.Int()).At(0).Int();
        int secondBox = edgeNearBoxes.At(edge.Int()).At(1).Int();

        int startIndex = nearBoxesBufferIndex;
        NearBoxesBuffer.At(nearBoxesBufferIndex++) = firstBox;
        NearBoxesBuffer.At(nearBoxesBufferIndex++) = secondBox;
        int endIndex = nearBoxesBufferIndex;

        EdgeNearBoxes.At(edge.Int()) = {
            NearBoxesBuffer.begin() + startIndex,
            NearBoxesBuffer.begin() + endIndex,
        };
      }
    }

    assert(CheckBoxNearBoxes());
  }

  List<Box<BoardSize>, 2>
  GetNearBoxes(Edge<BoardSize> edge) {
    List<Box<BoardSize>, 2> result;

    int x = edge.Dot2().X() - 1;
    int y = edge.Dot2().Y() - 1;
    if (x >= 0 && y >= 0) {
      result.Append(Box<BoardSize>(x, y));
    }

    x = edge.Dot1().X();
    y = edge.Dot1().Y();
    if (x < BoardSize && y < BoardSize) {
      result.Append(Box<BoardSize>(x, y));
    }

    return result;
  }

  bool
  CheckBoxNearBoxes() {
    for (Edge<BoardSize> edge = 0; edge.Int() < Edge<BoardSize>::Max; edge.IntRef()++) {
      auto expectedNearBoxes = GetNearBoxes(edge);
      if (EdgeNearBoxes.At(edge.Int()).Size() != expectedNearBoxes.Size()) {
        return false;
      }

      if (EdgeNearBoxes.At(edge.Int()).Size() == 1) {
        if (EdgeNearBoxes.At(edge.Int()).At(0).Int() != expectedNearBoxes.At(0).Int()) {
          return false;
        }
      } else {
        bool isEqual = false;
        if (EdgeNearBoxes.At(edge.Int()).At(0).Int() == expectedNearBoxes.At(0).Int() &&
            EdgeNearBoxes.At(edge.Int()).At(1).Int() == expectedNearBoxes.At(1).Int()) {
          isEqual = true;
        }

        if (EdgeNearBoxes.At(edge.Int()).At(0).Int() == expectedNearBoxes.At(1).Int() &&
            EdgeNearBoxes.At(edge.Int()).At(1).Int() == expectedNearBoxes.At(0).Int()) {
          isEqual = true;
        }

        if (!isEqual) {
          return false;
        }
      }
    }

    return true;
  }

  private:
  static constexpr int NearBoxesBufferSize = 4 * BoardSize * BoardSize - 8 * BoardSize + 3;
  Array<Box<BoardSize>, NearBoxesBufferSize> NearBoxesBuffer;

  Array<Span<Box<BoardSize>>, Edge<BoardSize>::Max> EdgeNearBoxes;

  friend const Span<Box<BoardSize>>&
  NearBoxes<BoardSize>(Edge<BoardSize> edge);
};

template <int BoardSize>
inline const Span<Box<BoardSize>>&
NearBoxes(Edge<BoardSize> edge) {
  static NearBoxesMapper<BoardSize> NearBoxesMapperInstance;

  return NearBoxesMapperInstance.EdgeNearBoxes.At(edge.Int());
}
