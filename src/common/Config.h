#pragma once

static constexpr int BoardSize = 6;

#define V(Class)         \
  public:                \
  Class() = default;     \
                         \
  Class(int v) : v(v) {  \
  }                      \
                         \
  void operator++(int) { \
    v++;                 \
  }                      \
                         \
  operator int() const { \
    return v;            \
  }                      \
                         \
  private:               \
  int v = 0;
