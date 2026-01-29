#pragma once

#include "Common.hpp"
#include "Edge.hpp"
#include "Iota.hpp"
#include "Square.hpp"

namespace dab::detail::model {

template <int64_t BoardSize>
const List<Box<BoardSize>, 2>&
NearBoxes(const Edge<BoardSize> edge);

template <int64_t BoardSize>
class NearBoxesMapper {
  public:
  constexpr NearBoxesMapper();

  private:
  Array<List<Box<BoardSize>, 2>, Limits<Edge<BoardSize>>::Max> EdgeNearBoxes;

  static constexpr List<Box<BoardSize>, 2>
  GetNearBoxes(const Edge<BoardSize> edge);

  friend const List<Box<BoardSize>, 2>&
  NearBoxes<BoardSize>(const Edge<BoardSize> edge);
};

template <int64_t BoardSize>
constexpr NearBoxesMapper<BoardSize>::NearBoxesMapper() {
  for (const Edge<BoardSize> edge : Iota<Edge<BoardSize>>()) {
    EdgeNearBoxes[edge] = GetNearBoxes(edge);
  }
}

template <int64_t BoardSize>
constexpr List<Box<BoardSize>, 2>
NearBoxesMapper<BoardSize>::GetNearBoxes(const Edge<BoardSize> edge) {
  List<Box<BoardSize>, 2> result;

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

template <int64_t BoardSize>
const List<Box<BoardSize>, 2>&
NearBoxes(const Edge<BoardSize> edge) {
  static constexpr NearBoxesMapper<BoardSize> Instance;

  return Instance.EdgeNearBoxes[edge];
}

}  // namespace dab::detail::model