#pragma once

#include <cassert>

#include "../common/Array.hpp"
#include "../common/List.hpp"
#include "Edge.hpp"
#include "Square.hpp"
#include "ValueIterator.hpp"

namespace dab::model {

template <int BoardSize, typename SizeType>
const common::List<Box<BoardSize, SizeType>, 2, SizeType>&
NearBoxes(const Edge<BoardSize, SizeType> edge);

template <int BoardSize, typename SizeType>
class NearBoxesMapper {
  public:
  NearBoxesMapper() {
    for (const Edge<BoardSize, SizeType> edge : ValueIterator<Edge<BoardSize, SizeType>, SizeType>()) {
      EdgeNearBoxes.At(edge.Value()) = GetNearBoxes(edge);
    }
  }

  private:
  common::Array<common::List<Box<BoardSize, SizeType>, 2, SizeType>, Edge<BoardSize, SizeType>::Max, SizeType>
      EdgeNearBoxes;

  static common::List<Box<BoardSize, SizeType>, 2, SizeType>
  GetNearBoxes(const Edge<BoardSize, SizeType> edge) {
    common::List<Box<BoardSize, SizeType>, 2, SizeType> result;

    SizeType x = edge.Dot2().X() - 1;
    SizeType y = edge.Dot2().Y() - 1;
    if (x >= 0 && y >= 0) {
      result.Append(Box<BoardSize, SizeType>(x, y));
    }

    x = edge.Dot1().X();
    y = edge.Dot1().Y();
    if (x < BoardSize && y < BoardSize) {
      result.Append(Box<BoardSize, SizeType>(x, y));
    }

    return result;
  }

  friend const common::List<Box<BoardSize, SizeType>, 2, SizeType>&
  NearBoxes<BoardSize, SizeType>(const Edge<BoardSize, SizeType> edge);
};

template <int BoardSize, typename SizeType>
const common::List<Box<BoardSize, SizeType>, 2, SizeType>&
NearBoxes(const Edge<BoardSize, SizeType> edge) {
  static NearBoxesMapper<BoardSize, SizeType> NearBoxesMapperInstance;

  return NearBoxesMapperInstance.EdgeNearBoxes.At(edge.Value());
}

}  // namespace dab::model
