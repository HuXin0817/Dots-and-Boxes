#pragma once

#include <cassert>
#include <chrono>
#include <random>

namespace dab::detail::common {

class Random {
  public:
  explicit Random();

  uint32_t
  Range(uint32_t min, uint32_t max);

  template <typename T>
  const auto&
  Choice(const T& data);

  private:
  std::mt19937_64 Rng;
  std::uniform_int_distribution<uint32_t> dist;
};

inline Random::Random() {
  Rng.seed(static_cast<uint64_t>(std::chrono::steady_clock::now().time_since_epoch().count()));
}

inline uint32_t
Random::Range(uint32_t min, uint32_t max) {
  dist.param(std::uniform_int_distribution<uint32_t>::param_type(min, max));
  return dist(Rng);
}

template <typename T>
const auto&
Random::Choice(const T& data) {
  assert(!data.Empty());
  if (data.Size() == 1) {
    return data[0];
  }
  return data[Range(0, data.Size() - 1)];
}

}  // namespace dab::detail::common