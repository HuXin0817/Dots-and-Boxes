#pragma once

template <typename T>
const auto&
RandomChoice(const T& data) {
  assert(!data.Empty());
  if (data.Size() == 1) {
    return data.At(0);
  }

  thread_local std::random_device rd;
  thread_local std::mt19937 rng(rd());
  std::uniform_int_distribution<size_t> dist(0, data.Size() - 1);
  return data.At(dist(rng));
}
