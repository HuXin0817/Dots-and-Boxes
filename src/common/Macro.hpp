#pragma once

#include <cassert>

#define IntValueObject(Class) \
  public:                     \
  Class() = default;          \
  Class(int v) : v(v) {       \
  }                           \
  int Value() const {         \
    return v;                 \
  }                           \
                              \
  private:                    \
  int v = 0;

#define IterableObject(_Begin, _End, _Size) \
  public:                                   \
  auto begin() {                            \
    return (_Begin);                        \
  }                                         \
  const auto begin() const {                \
    return (_Begin);                        \
  }                                         \
  auto end() {                              \
    return (_End);                          \
  }                                         \
  const auto end() const {                  \
    return (_End);                          \
  }                                         \
  SizeType Size() const {                   \
    return (_Size);                         \
  }                                         \
  bool Empty() const {                      \
    return (_Size) == 0;                    \
  }                                         \
  auto& At(SizeType i) {                    \
    assert(i >= 0 && i < (_Size));          \
    return *((_Begin) + i);                 \
  }                                         \
  const auto& At(SizeType i) const {        \
    assert(i >= 0 && i < (_Size));          \
    return *((_Begin) + i);                 \
  }
