#pragma once

template <typename T>
const std::remove_const_t<std::remove_reference_t<decltype(T().At(0))>>&
RandomChoice(const T& data) {
  assert(!data.Empty());
  if (data.Size() == 1) {
    return data.At(0);
  }

  thread_local std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<size_t> dist(0, data.Size() - 1);
  return data.At(dist(rng));
}
