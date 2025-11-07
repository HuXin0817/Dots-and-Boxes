#pragma once

#include <cassert>

static constexpr int BoardSize = 6;

#define V(Class)         \
  public:                \
  Class() = default;     \
  Class(int v) : v(v) {  \
  }                      \
  void operator++(int) { \
    v++;                 \
  }                      \
  operator int() const { \
    return v;            \
  }                      \
                         \
  private:               \
  int v = 0;

#define I(_Begin, _End, _Size)     \
  public:                          \
  auto begin() {                   \
    return (_Begin);               \
  }                                \
  const auto begin() const {       \
    return (_Begin);               \
  }                                \
  auto end() {                     \
    return (_End);                 \
  }                                \
  const auto end() const {         \
    return (_End);                 \
  }                                \
  int Size() const {               \
    return (_Size);                \
  }                                \
  int Empty() const {              \
    return (_Size) == 0;           \
  }                                \
  auto& At(int i) {                \
    assert(i >= 0 && i < (_Size)); \
    return *((_Begin) + i);        \
  }                                \
  const auto& At(int i) const {    \
    assert(i >= 0 && i < (_Size)); \
    return *((_Begin) + i);        \
  }
