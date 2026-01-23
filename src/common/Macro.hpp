#pragma once

template <int64_t BoardSize, int64_t N = 2 * BoardSize * (BoardSize + 1) /* Edge<BoardSize>::Max */>
using SizeType = std::conditional_t<
    N != 2 * BoardSize*(BoardSize + 1),
    void,
    std::conditional_t<
        N <= std::numeric_limits<int8_t>::max(),
        int8_t,
        std::conditional_t<N <= std::numeric_limits<int16_t>::max(),
                           int16_t,
                           std::conditional_t<N <= std::numeric_limits<int32_t>::max(), int32_t, int64_t>>>>;

#define IntValueObject(Class, SizeType) \
  public:                               \
  Class() = default;                    \
  Class(SizeType v) : v(v) {            \
  }                                     \
  SizeType Value() const {              \
    return v;                           \
  }                                     \
                                        \
  private:                              \
  SizeType v = 0;

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
