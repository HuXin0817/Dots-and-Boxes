#pragma once

#include <cstddef>

namespace dab {

template <typename Derived>
class Iterable {
  public:
  auto
  begin();
  auto
  begin() const;
  auto
  end();
  auto
  end() const;

  size_t
  Size() const;
  bool
  Empty() const;
  auto&
  operator[](size_t i);
  const auto&
  operator[](size_t i) const;
  auto&
  Front();
  const auto&
  Front() const;
  auto&
  Back();
  const auto&
  Back() const;

  protected:
  ~Iterable() = default;
};

template <typename Derived>
auto
Iterable<Derived>::begin() {
  return static_cast<Derived*>(this)->Begin();
}

template <typename Derived>
auto
Iterable<Derived>::begin() const {
  return static_cast<const Derived*>(this)->Begin();
}

template <typename Derived>
auto
Iterable<Derived>::end() {
  return static_cast<Derived*>(this)->End();
}

template <typename Derived>
auto
Iterable<Derived>::end() const {
  return static_cast<const Derived*>(this)->End();
}

template <typename Derived>
size_t
Iterable<Derived>::Size() const {
  return static_cast<const Derived*>(this)->Size();
}

template <typename Derived>
bool
Iterable<Derived>::Empty() const {
  return Size() == 0;
}

template <typename Derived>
auto&
Iterable<Derived>::operator[](size_t i) {
  return begin()[i];
}

template <typename Derived>
const auto&
Iterable<Derived>::operator[](size_t i) const {
  return begin()[i];
}

template <typename Derived>
auto&
Iterable<Derived>::Front() {
  return begin()[0];
}

template <typename Derived>
const auto&
Iterable<Derived>::Front() const {
  return begin()[0];
}

template <typename Derived>
auto&
Iterable<Derived>::Back() {
  return begin()[Size() - 1];
}

template <typename Derived>
const auto&
Iterable<Derived>::Back() const {
  return begin()[Size() - 1];
}

}  // namespace dab