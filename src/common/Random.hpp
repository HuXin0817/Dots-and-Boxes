#pragma once

#include <cassert>
#include <random>

inline int
RandomN(int n) {
  assert(n > 0);
  if (n == 1) {
    return 0;
  }
  thread_local std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution dist(0, n - 1);
  return dist(rng);
}

template <class T>
const auto&
RandomChoice(const T& data) {
  assert(!data.Empty());

  return data.At(RandomN(data.Size()));
}
