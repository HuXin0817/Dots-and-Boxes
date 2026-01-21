#pragma once

#include <cassert>

#include "../common/Array.hpp"
#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "Edge.hpp"
#include "Square.hpp"
#include "ValueIterator.hpp"

namespace dab::model {

template <int BoardSize>
const common::Span<Box<BoardSize>>&
NearBoxes(const Edge<BoardSize> edge);

template <int BoardSize>
class NearBoxesMapper {
  public:
  NearBoxesMapper() {
    int i = 0;
    for (const Edge<BoardSize> edge : ValueIterator<Edge<BoardSize>>()) {
      const int begin = i;
      for (const Box<BoardSize> box : GetNearBoxes(edge)) {
        NearBoxesBuffer.At(i++) = box;
      }
      EdgeNearBoxes.At(edge.Value()) = {NearBoxesBuffer.begin() + begin, NearBoxesBuffer.begin() + i};
    }
    assert(i == NearBoxesBuffer.Size());
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

  private:
  common::Array<Box<BoardSize>, 2 * Edge<BoardSize>::Max - 4 * BoardSize> NearBoxesBuffer;
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
