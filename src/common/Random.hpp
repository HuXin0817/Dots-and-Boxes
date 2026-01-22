#pragma once

#include <cassert>
#include <random>

namespace dab::common {

template <typename T, typename SizeType>
const auto&
RandomChoice(const T& data) {
  assert(!data.Empty());
  if (data.Size() == 1) {
    return data.At(0);
  }

  thread_local std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<SizeType> dist(0, data.Size() - 1);
  return data.At(dist(rng));
}

}  // namespace dab::common
