#pragma once

// clang-format off
#define SizeType                                                                                      \
  std::conditional_t<2 * BoardSize * (BoardSize + 1) <= std::numeric_limits<int8_t>::max(), int8_t,   \
  std::conditional_t<2 * BoardSize * (BoardSize + 1) <= std::numeric_limits<int16_t>::max(), int16_t, \
  std::conditional_t<2 * BoardSize * (BoardSize + 1) <= std::numeric_limits<int32_t>::max(), int32_t, int64_t>>>
// clang-format on

#define IntValueObject(Class) \
  public:                     \
  Class() = default;          \
  Class(SizeType v) : v(v) {  \
  }                           \
  SizeType Value() const {    \
    return v;                 \
  }                           \
                              \
  private:                    \
  SizeType v = 0;

#define IterableObject(Begin, End, Len) \
  public:                               \
  auto begin() {                        \
    return Begin;                       \
  }                                     \
  const auto begin() const {            \
    return Begin;                       \
  }                                     \
  auto end() {                          \
    return End;                         \
  }                                     \
  const auto end() const {              \
    return End;                         \
  }                                     \
  size_t Size() const {                 \
    return Len;                         \
  }                                     \
  bool Empty() const {                  \
    return (Len) == 0;                  \
  }                                     \
  auto& At(size_t i) {                  \
    assert(i >= 0 && i < (Len));        \
    return *((Begin) + i);              \
  }                                     \
  const auto& At(size_t i) const {      \
    assert(i >= 0 && i < (Len));        \
    return *((Begin) + i);              \
  }
