#pragma once

#include <cstddef>
#include <limits>

#include "BasicBoard.hpp"

namespace dab::detail::board {

template <int64_t BoardSize>
class HashBoard : public BasicBoard<BoardSize> {
  public:
  HashBoard();

  void
  Reset();
  void
  Add(const Edge<BoardSize> edge);
  size_t
  Hash() const;

  private:
  static Array<size_t, Edge<BoardSize>::Max>
  CreateHashValueTable();
  static inline Array<size_t, Edge<BoardSize>::Max> HashValueTable = CreateHashValueTable();

  size_t HashValue = 0;
};

template <int64_t BoardSize>
HashBoard<BoardSize>::HashBoard() {
  Reset();
}

template <int64_t BoardSize>
void
HashBoard<BoardSize>::Reset() {
  BasicBoard<BoardSize>::Reset();
  HashValue = 0;
}

template <int64_t BoardSize>
void
HashBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  BasicBoard<BoardSize>::Add(edge);
  HashValue += HashValueTable[edge];
}

template <int64_t BoardSize>
size_t
HashBoard<BoardSize>::Hash() const {
  return HashValue;
}

template <int64_t BoardSize>
Array<size_t, Edge<BoardSize>::Max>
HashBoard<BoardSize>::CreateHashValueTable() {
  Random random;
  Array<size_t, Edge<BoardSize>::Max> table;
  for (size_t& v : table) {
    v = random.Range(0, std::numeric_limits<size_t>::max());
  }
  return table;
}

}  // namespace dab::detail::board