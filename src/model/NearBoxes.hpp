#pragma once

#include "../common/Array.hpp"
#include "../common/List.hpp"
#include "Edge.hpp"
#include "Square.hpp"

template <int BoardSize>
const List<Box<BoardSize>, 2>&
NearBoxes(const Edge<BoardSize> edge);

template <int BoardSize>
class NearBoxesMapper {
  public:
  NearBoxesMapper() {
    for (const Edge<BoardSize> edge : std::views::iota(0, Edge<BoardSize>::Max)) {
      EdgeNearBoxes.At(edge.Value()) = GetNearBoxes(edge);
    }
  }

  private:
  Array<List<Box<BoardSize>, 2>, Edge<BoardSize>::Max> EdgeNearBoxes;

  static List<Box<BoardSize>, 2>
  GetNearBoxes(const Edge<BoardSize> edge) {
    List<Box<BoardSize>, 2> result;

    SizeType x = edge.Dot2().X() - 1;
    SizeType y = edge.Dot2().Y() - 1;
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

  friend const List<Box<BoardSize>, 2>&
  NearBoxes<BoardSize>(const Edge<BoardSize> edge);
};

template <int BoardSize>
const List<Box<BoardSize>, 2>&
NearBoxes(const Edge<BoardSize> edge) {
  static NearBoxesMapper<BoardSize> Instance;

  return Instance.EdgeNearBoxes.At(edge.Value());
}
