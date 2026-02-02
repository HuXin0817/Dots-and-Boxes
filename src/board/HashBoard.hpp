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
  Hash1() const;
  size_t
  Hash2() const;

  private:
  static Array<size_t, Edge<BoardSize>::Max>
  HashValueTable();
  static inline Array<size_t, Edge<BoardSize>::Max> HashValueTable1 = HashValueTable();
  static inline Array<size_t, Edge<BoardSize>::Max> HashValueTable2 = HashValueTable();

  size_t HashValue1 = 0;
  size_t HashValue2 = 0;
};

template <int64_t BoardSize>
HashBoard<BoardSize>::HashBoard() {
  Reset();
}

template <int64_t BoardSize>
void
HashBoard<BoardSize>::Reset() {
  BasicBoard<BoardSize>::Reset();
  HashValue1 = 0;
  HashValue2 = 0;
}

template <int64_t BoardSize>
void
HashBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  BasicBoard<BoardSize>::Add(edge);
  HashValue1 += HashValueTable1[edge];
  HashValue2 += HashValueTable2[edge];
}

template <int64_t BoardSize>
size_t
HashBoard<BoardSize>::Hash1() const {
  return HashValue1;
}

template <int64_t BoardSize>
size_t
HashBoard<BoardSize>::Hash2() const {
  return HashValue2;
}

template <int64_t BoardSize>
Array<size_t, Edge<BoardSize>::Max>
HashBoard<BoardSize>::HashValueTable() {
  Random random;
  Array<size_t, Edge<BoardSize>::Max> table;
  for (size_t& v : table) {
    v = random.Range(0, std::numeric_limits<size_t>::max());
  }
  return table;
}

}  // namespace dab::detail::board