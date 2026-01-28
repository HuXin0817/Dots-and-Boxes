#pragma once

#include <cstddef>

template <typename T>
class Iterable {
  public:
  virtual size_t
  Size() const = 0;
  virtual T*
  Begin() = 0;
  virtual const T*
  Begin() const = 0;
  virtual T*
  End() = 0;
  virtual const T*
  End() const = 0;
  virtual ~Iterable() = default;
};

template <typename T>
class IterableWapper : public T {
  public:
  using T::T;

  auto
  begin() {
    return T::Begin();
  }
  const auto
  begin() const {
    return T::Begin();
  }
  auto
  end() {
    return T::End();
  }
  const auto
  end() const {
    return T::End();
  }

  auto&
  operator[](const size_t i) {
    return begin()[i];
  }
  const auto&
  operator[](const size_t i) const {
    return begin()[i];
  }
  auto&
  At(const size_t i) {
    return begin()[i];
  }
  const auto&
  At(const size_t i) const {
    return begin()[i];
  }

  size_t
  Size() const {
    return T::Size();
  }
  bool
  Empty() const {
    return T::Size() == 0;
  }
};
