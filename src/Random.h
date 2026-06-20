#pragma once

#include <chrono>
#include <cstdint>
#include <random>
#include <thread>

namespace dab {

class ZobristHash {
 public:
  constexpr ZobristHash() = default;
  constexpr ZobristHash(std::uint64_t custom_seed) : seed(custom_seed) {}
  constexpr std::uint64_t Next();

 private:
  static constexpr std::uint64_t DefaultSeed = 0x9F27C15E8D4A6B03ULL;
  static constexpr std::uint64_t MixMult = 0xA5648D32917BC0EFULL;
  static constexpr std::uint64_t MixShift1 = 13;
  static constexpr std::uint64_t MixShift2 = 17;
  static constexpr std::uint64_t MixShift3 = 5;

  std::uint64_t iter = 0;
  std::uint64_t seed = DefaultSeed;

  static constexpr std::uint64_t xorshift64(std::uint64_t state);
};

inline constexpr std::uint64_t ZobristHash::Next() {
  const std::uint64_t cur_iter = iter++;
  const std::uint64_t scramble = xorshift64(cur_iter);
  const std::uint64_t mix_val = scramble * MixMult;
  seed ^= mix_val;
  seed = xorshift64(seed);
  return seed;
}

constexpr std::uint64_t ZobristHash::xorshift64(std::uint64_t state) {
  state ^= state << MixShift1;
  state ^= state >> MixShift2;
  state ^= state << MixShift3;
  return state;
}

class Random {
 public:
  Random();
  constexpr Random(std::uint64_t seed) { generator.seed(seed); };

  template <typename T>
  constexpr T Range(T min, T max);
  template <typename Iterable>
  constexpr const auto& Choice(const Iterable& data);

 private:
  std::mt19937_64 generator;
};

inline Random::Random() {
  const std::uint64_t timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
  const std::uint64_t thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());
  generator.seed(timestamp ^ thread_id);
}

template <typename T>
constexpr T Random::Range(T min, T max) {
  return std::uniform_int_distribution<T>{min, max}(generator);
}

template <typename Iterable>
constexpr const auto& Random::Choice(const Iterable& data) {
  assert(!data.Empty());
  return data.At(Range(0, data.Size() - 1));
}

}  // namespace dab