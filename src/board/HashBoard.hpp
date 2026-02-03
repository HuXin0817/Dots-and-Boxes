#pragma once

#include <cassert>
#include <cstddef>
#include <functional>
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
  uint32_t
  Hash() const;
  bool
  operator==(const HashBoard& other) const;

  private:
  static Array<uint32_t, Edge<BoardSize>::Max>
  CreateHashValueTable();
  static inline Array<uint32_t, Edge<BoardSize>::Max> HashValueTable = CreateHashValueTable();

  uint32_t HashValue = 0;
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
uint32_t
HashBoard<BoardSize>::Hash() const {
  return HashValue;
}

template <int64_t BoardSize>
bool
HashBoard<BoardSize>::operator==(const HashBoard& other) const {
  if (HashValue != other.HashValue) {
    return false;
  }
  return BasicBoard<BoardSize>::operator==(other);
}

template <int64_t BoardSize>
Array<uint32_t, Edge<BoardSize>::Max>
HashBoard<BoardSize>::CreateHashValueTable() {
  Random random;
  Array<uint32_t, Edge<BoardSize>::Max> table;
  for (uint32_t& v : table) {
    v = random.Range(0, std::numeric_limits<uint32_t>::max());
  }
  return table;
}

}  // namespace dab::detail::board

namespace std {

template <int64_t BoardSize>
struct hash<dab::detail::board::HashBoard<BoardSize>> {
  uint32_t
  operator()(const dab::detail::board::HashBoard<BoardSize>& board) const {
    return board.Hash();
  }
};

}  // namespace std
