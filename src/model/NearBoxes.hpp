#pragma once

#include "../common/Array.hpp"
#include "../common/List.hpp"
#include "Edge.hpp"
#include "Square.hpp"

template <int64_t BoardSize>
const List<Box<BoardSize>, 2, SizeType<BoardSize>>&
NearBoxes(const Edge<BoardSize> edge);

template <int64_t BoardSize>
class NearBoxesMapper {
  public:
  NearBoxesMapper() {
    for (const Edge<BoardSize> edge : std::views::iota(0, Edge<BoardSize>::Max)) {
      EdgeNearBoxes.At(edge.Value()) = GetNearBoxes(edge);
    }
  }

  private:
  Array<List<Box<BoardSize>, 2, SizeType<BoardSize>>, Edge<BoardSize>::Max, SizeType<BoardSize>> EdgeNearBoxes;

  static List<Box<BoardSize>, 2, SizeType<BoardSize>>
  GetNearBoxes(const Edge<BoardSize> edge) {
    List<Box<BoardSize>, 2, SizeType<BoardSize>> result;

    SizeType<BoardSize> x = edge.Dot2().X() - 1;
    SizeType<BoardSize> y = edge.Dot2().Y() - 1;
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

  friend const List<Box<BoardSize>, 2, SizeType<BoardSize>>&
  NearBoxes<BoardSize>(const Edge<BoardSize> edge);
};

template <int64_t BoardSize>
const List<Box<BoardSize>, 2, SizeType<BoardSize>>&
NearBoxes(const Edge<BoardSize> edge) {
  static NearBoxesMapper<BoardSize> NearBoxesMapperInstance;

  return NearBoxesMapperInstance.EdgeNearBoxes.At(edge.Value());
}
