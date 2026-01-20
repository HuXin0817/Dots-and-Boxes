#pragma once

#include <cassert>

#include "../common/Array.hpp"
#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "Edge.hpp"
#include "Square.hpp"

namespace dab::model {

template <int BoardSize>
const common::Span<Box<BoardSize>>&
NearBoxes(const Edge<BoardSize> edge);

template <int BoardSize>
class NearBoxesMapper {
  public:
  NearBoxesMapper() {
    common::Array<common::List<Box<BoardSize>, 2>, Edge<BoardSize>::Max> edgeNearBoxes;
    for (Edge<BoardSize> edge = 0; edge.Value() < Edge<BoardSize>::Max; ++edge) {
      edgeNearBoxes.At(edge.Value()) = GetNearBoxes(edge);
    }

    common::Array<common::Array<int, Box<BoardSize>::Max>, Box<BoardSize>::Max> adjacencyMatrix = {};
    common::Array<int, Box<BoardSize>::Max> boxDegrees = {};

    for (Edge<BoardSize> edge = 0; edge.Value() < Edge<BoardSize>::Max; ++edge) {
      if (edgeNearBoxes.At(edge.Value()).Size() == 2) {
        int firstBox = edgeNearBoxes.At(edge.Value()).At(0).Value();
        int secondBox = edgeNearBoxes.At(edge.Value()).At(1).Value();
        ++adjacencyMatrix.At(firstBox).At(secondBox);
        ++adjacencyMatrix.At(secondBox).At(firstBox);
        ++boxDegrees.At(firstBox);
        ++boxDegrees.At(secondBox);
      }
    }

    common::List<int, Box<BoardSize>::Max> oddDegreeBoxes;
    for (int box = 0; box < Box<BoardSize>::Max; ++box) {
      if (boxDegrees.At(box) % 2 == 1) {
        oddDegreeBoxes.Append(box);
      }
    }

    std::function<common::List<int, Edge<BoardSize>::Max>(int)> findEulerianPath =
        [&](int startBox) -> common::List<int, Edge<BoardSize>::Max> {
      common::List<int, Edge<BoardSize>::Max> eulerianPath;
      common::Array<int, Edge<BoardSize>::Max> pathStack;
      int stackSize = 0;
      pathStack.At(stackSize++) = startBox;

      while (stackSize > 0) {
        int currentBox = pathStack.At(stackSize - 1);

        bool found = false;
        for (int nextBox = 0; nextBox < Box<BoardSize>::Max; nextBox++) {
          if (adjacencyMatrix.At(currentBox).At(nextBox) > 0) {
            found = true;
            pathStack.At(stackSize++) = nextBox;
            --adjacencyMatrix.At(currentBox).At(nextBox);
            --adjacencyMatrix.At(nextBox).At(currentBox);
            --boxDegrees.At(currentBox);
            --boxDegrees.At(nextBox);
            break;
          }
        }

        if (!found) {
          eulerianPath.Append(currentBox);
          stackSize--;
        }
      }

      const int pathLength = eulerianPath.Size();
      for (int i = 0; i < pathLength / 2; i++) {
        int swappedBox = eulerianPath.At(i);
        eulerianPath.At(i) = eulerianPath.At(pathLength - 1 - i);
        eulerianPath.At(pathLength - 1 - i) = swappedBox;
      }
      return eulerianPath;
    };

    int nearBoxesBufferIndex = 0;
    common::Array<common::List<int, Edge<BoardSize>::Max>, Box<BoardSize>::Max> boxToIndices;
    common::Array<common::Array<int, Box<BoardSize>::Max>, Box<BoardSize>::Max> boxPairToIndex;
    for (int i = 0; i < Box<BoardSize>::Max; i++) {
      for (int j = 0; j < Box<BoardSize>::Max; j++) {
        boxPairToIndex.At(i).At(j) = NearBoxesBufferSize;
      }
    }

    while (true) {
      int startBox = -1;
      for (int box = 0; box < Box<BoardSize>::Max; ++box) {
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

      common::List<int, Edge<BoardSize>::Max> eulerianPath = findEulerianPath(startBox);
      if (eulerianPath.Empty()) {
        continue;
      }

      for (int i = 0; i < eulerianPath.Size(); i++) {
        Box<BoardSize> box = eulerianPath.At(i);
        NearBoxesBuffer.At(nearBoxesBufferIndex++) = box;
        boxToIndices.At(box.Value()).Append(nearBoxesBufferIndex - 1);

        if (i > 0) {
          const int prevBox = eulerianPath.At(i - 1);
          const int minBox = prevBox < box.Value() ? prevBox : box.Value();
          const int maxBox = prevBox < box.Value() ? box.Value() : prevBox;
          if (boxPairToIndex.At(minBox).At(maxBox) == NearBoxesBufferSize) {
            boxPairToIndex.At(minBox).At(maxBox) = nearBoxesBufferIndex - 2;
          }
        }
      }
    }

    for (Edge<BoardSize> edge = 0; edge.Value() < Edge<BoardSize>::Max; ++edge) {
      if (edgeNearBoxes.At(edge.Value()).Size() == 1) {
        int box = edgeNearBoxes.At(edge.Value()).At(0).Value();
        if (boxToIndices.At(box).Empty()) {
          NearBoxesBuffer.At(nearBoxesBufferIndex++) = box;
          boxToIndices.At(box).Append(nearBoxesBufferIndex - 1);
        }
      }
    }

    common::Array<bool, Edge<BoardSize>::Max> edgeProcessed = {};
    for (Edge<BoardSize> edge = 0; edge.Value() < Edge<BoardSize>::Max; ++edge) {
      if (edgeNearBoxes.At(edge.Value()).Size() == 2) {
        int firstBox = edgeNearBoxes.At(edge.Value()).At(0).Value();
        int secondBox = edgeNearBoxes.At(edge.Value()).At(1).Value();
        int minBox = firstBox < secondBox ? firstBox : secondBox;
        int maxBox = firstBox < secondBox ? secondBox : firstBox;

        int foundIndex = boxPairToIndex.At(minBox).At(maxBox);
        if (foundIndex < NearBoxesBufferSize && foundIndex < nearBoxesBufferIndex - 1) {
          if ((NearBoxesBuffer.At(foundIndex).Value() == firstBox &&
               NearBoxesBuffer.At(foundIndex + 1).Value() == secondBox) ||
              (NearBoxesBuffer.At(foundIndex).Value() == secondBox &&
               NearBoxesBuffer.At(foundIndex + 1).Value() == firstBox)) {
            EdgeNearBoxes.At(edge.Value()) = {
                NearBoxesBuffer.begin() + foundIndex,
                NearBoxesBuffer.begin() + foundIndex + 2,
            };
            edgeProcessed.At(edge.Value()) = true;
            continue;
          }
        }

        bool found = false;
        const int firstBoxIndicesSize = boxToIndices.At(firstBox).Size();
        const int secondBoxIndicesSize = boxToIndices.At(secondBox).Size();

        if (firstBoxIndicesSize <= secondBoxIndicesSize) {
          for (int idx = 0; idx < firstBoxIndicesSize; idx++) {
            int i = boxToIndices.At(firstBox).At(idx);
            if (i < nearBoxesBufferIndex - 1) {
              if ((NearBoxesBuffer.At(i).Value() == firstBox && NearBoxesBuffer.At(i + 1).Value() == secondBox) ||
                  (NearBoxesBuffer.At(i).Value() == secondBox && NearBoxesBuffer.At(i + 1).Value() == firstBox)) {
                EdgeNearBoxes.At(edge.Value()) = {
                    NearBoxesBuffer.begin() + i,
                    NearBoxesBuffer.begin() + i + 2,
                };
                edgeProcessed.At(edge.Value()) = true;
                boxPairToIndex.At(minBox).At(maxBox) = i;
                found = true;
                break;
              }
            }

            if (i > 0 && i - 1 < nearBoxesBufferIndex - 1) {
              if ((NearBoxesBuffer.At(i - 1).Value() == firstBox && NearBoxesBuffer.At(i).Value() == secondBox) ||
                  (NearBoxesBuffer.At(i - 1).Value() == secondBox && NearBoxesBuffer.At(i).Value() == firstBox)) {
                EdgeNearBoxes.At(edge.Value()) = {
                    NearBoxesBuffer.begin() + (i - 1),
                    NearBoxesBuffer.begin() + (i + 1),
                };
                edgeProcessed.At(edge.Value()) = true;
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
              if ((NearBoxesBuffer.At(i).Value() == firstBox && NearBoxesBuffer.At(i + 1).Value() == secondBox) ||
                  (NearBoxesBuffer.At(i).Value() == secondBox && NearBoxesBuffer.At(i + 1).Value() == firstBox)) {
                EdgeNearBoxes.At(edge.Value()) = {
                    NearBoxesBuffer.begin() + i,
                    NearBoxesBuffer.begin() + i + 2,
                };
                edgeProcessed.At(edge.Value()) = true;
                boxPairToIndex.At(minBox).At(maxBox) = i;
                found = true;
                break;
              }
            }
            if (i > 0 && i - 1 < nearBoxesBufferIndex - 1) {
              if ((NearBoxesBuffer.At(i - 1).Value() == firstBox && NearBoxesBuffer.At(i).Value() == secondBox) ||
                  (NearBoxesBuffer.At(i - 1).Value() == secondBox && NearBoxesBuffer.At(i).Value() == firstBox)) {
                EdgeNearBoxes.At(edge.Value()) = {
                    NearBoxesBuffer.begin() + (i - 1),
                    NearBoxesBuffer.begin() + (i + 1),
                };
                edgeProcessed.At(edge.Value()) = true;
                boxPairToIndex.At(minBox).At(maxBox) = i - 1;
                found = true;
                break;
              }
            }
          }
        }

        if (!found) {
          for (int i = 0; i < nearBoxesBufferIndex - 1; i++) {
            if ((NearBoxesBuffer.At(i).Value() == firstBox && NearBoxesBuffer.At(i + 1).Value() == secondBox) ||
                (NearBoxesBuffer.At(i).Value() == secondBox && NearBoxesBuffer.At(i + 1).Value() == firstBox)) {
              EdgeNearBoxes.At(edge.Value()) = {
                  NearBoxesBuffer.begin() + i,
                  NearBoxesBuffer.begin() + i + 2,
              };
              edgeProcessed.At(edge.Value()) = true;
              boxPairToIndex.At(minBox).At(maxBox) = i;
              break;
            }
          }
        }
      } else if (edgeNearBoxes.At(edge.Value()).Size() == 1) {
        int box = edgeNearBoxes.At(edge.Value()).At(0).Value();
        int index = boxToIndices.At(box).At(0);
        EdgeNearBoxes.At(edge.Value()) = {
            NearBoxesBuffer.begin() + index,
            NearBoxesBuffer.begin() + index + 1,
        };
        edgeProcessed.At(edge.Value()) = true;
      }
    }

    for (Edge<BoardSize> edge = 0; edge.Value() < Edge<BoardSize>::Max; ++edge) {
      if (!edgeProcessed.At(edge.Value()) && edgeNearBoxes.At(edge.Value()).Size() == 2) {
        int firstBox = edgeNearBoxes.At(edge.Value()).At(0).Value();
        int secondBox = edgeNearBoxes.At(edge.Value()).At(1).Value();

        int startIndex = nearBoxesBufferIndex;
        NearBoxesBuffer.At(nearBoxesBufferIndex++) = firstBox;
        NearBoxesBuffer.At(nearBoxesBufferIndex++) = secondBox;
        int endIndex = nearBoxesBufferIndex;

        EdgeNearBoxes.At(edge.Value()) = {
            NearBoxesBuffer.begin() + startIndex,
            NearBoxesBuffer.begin() + endIndex,
        };
      }
    }

    assert(CheckBoxNearBoxes());
  }

  common::List<Box<BoardSize>, 2>
  GetNearBoxes(const Edge<BoardSize> edge) {
    common::List<Box<BoardSize>, 2> result;

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
    for (Edge<BoardSize> edge = 0; edge.Value() < Edge<BoardSize>::Max; ++edge) {
      common::List<Box<BoardSize>, 2> expectedNearBoxes = GetNearBoxes(edge);
      if (EdgeNearBoxes.At(edge.Value()).Size() != expectedNearBoxes.Size()) {
        return false;
      }

      if (EdgeNearBoxes.At(edge.Value()).Size() == 1) {
        if (EdgeNearBoxes.At(edge.Value()).At(0).Value() != expectedNearBoxes.At(0).Value()) {
          return false;
        }
      } else {
        bool isEqual = false;
        if (EdgeNearBoxes.At(edge.Value()).At(0).Value() == expectedNearBoxes.At(0).Value() &&
            EdgeNearBoxes.At(edge.Value()).At(1).Value() == expectedNearBoxes.At(1).Value()) {
          isEqual = true;
        }

        if (EdgeNearBoxes.At(edge.Value()).At(0).Value() == expectedNearBoxes.At(1).Value() &&
            EdgeNearBoxes.At(edge.Value()).At(1).Value() == expectedNearBoxes.At(0).Value()) {
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
  common::Array<Box<BoardSize>, NearBoxesBufferSize> NearBoxesBuffer;

  common::Array<common::Span<Box<BoardSize>>, Edge<BoardSize>::Max> EdgeNearBoxes;

  friend const common::Span<Box<BoardSize>>&
  NearBoxes<BoardSize>(const Edge<BoardSize> edge);
};

template <int BoardSize>
const common::Span<Box<BoardSize>>&
NearBoxes(const Edge<BoardSize> edge) {
  static NearBoxesMapper<BoardSize> NearBoxesMapperInstance;

  return NearBoxesMapperInstance.EdgeNearBoxes.At(edge.Value());
}

}  // namespace dab::model
