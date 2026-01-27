#pragma once

#include <cassert>
#include <chrono>
#include <random>
#include <type_traits>

namespace dab {

class Random {
  public:
  explicit Random();

  template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  T
  Range(T min, T max);

  template <typename T>
  const auto&
  Choice(const T& data);

  private:
  std::mt19937_64 Rng;
};

inline Random::Random() {
  Rng.seed(static_cast<uint64_t>(std::chrono::steady_clock::now().time_since_epoch().count()));
}

template <typename T, typename>
T
Random::Range(T min, T max) {
  std::uniform_int_distribution<T> dist(min, max);
  return dist(Rng);
}

template <typename T>
const auto&
Random::Choice(const T& data) {
  assert(!data.Empty());
  if (data.Size() == 1) {
    return data.At(0);
  }
  return data.At(Range<size_t>(0, static_cast<size_t>(data.Size() - 1)));
}

}  // namespace dab
