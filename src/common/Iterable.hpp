#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>

namespace dab::detail::common {

template <typename Derived>
class Iterable {
  public:
  constexpr auto
  begin();
  constexpr auto
  begin() const;
  constexpr auto
  end();
  constexpr auto
  end() const;

  constexpr uint32_t
  Size() const;
  constexpr bool
  Empty() const;
  constexpr auto&
  operator[](uint32_t i);
  constexpr const auto&
  operator[](uint32_t i) const;
  constexpr auto&
  Front();
  constexpr const auto&
  Front() const;
  constexpr auto&
  Back();
  constexpr const auto&
  Back() const;

  protected:
  ~Iterable() = default;
};

template <typename Derived>
constexpr auto
Iterable<Derived>::begin() {
  return static_cast<Derived*>(this)->Begin();
}

template <typename Derived>
constexpr auto
Iterable<Derived>::begin() const {
  return static_cast<const Derived*>(this)->Begin();
}

template <typename Derived>
constexpr auto
Iterable<Derived>::end() {
  return static_cast<Derived*>(this)->End();
}

template <typename Derived>
constexpr auto
Iterable<Derived>::end() const {
  return static_cast<const Derived*>(this)->End();
}

template <typename Derived>
constexpr uint32_t
Iterable<Derived>::Size() const {
  return static_cast<const Derived*>(this)->Size();
}

template <typename Derived>
constexpr bool
Iterable<Derived>::Empty() const {
  return Size() == 0;
}

template <typename Derived>
constexpr auto&
Iterable<Derived>::operator[](uint32_t i) {
  assert(i < Size());
  return begin()[i];
}

template <typename Derived>
constexpr const auto&
Iterable<Derived>::operator[](uint32_t i) const {
  assert(i < Size());
  return begin()[i];
}

template <typename Derived>
constexpr auto&
Iterable<Derived>::Front() {
  return begin()[0];
}

template <typename Derived>
constexpr const auto&
Iterable<Derived>::Front() const {
  return begin()[0];
}

template <typename Derived>
constexpr auto&
Iterable<Derived>::Back() {
  return begin()[Size() - 1];
}

template <typename Derived>
constexpr const auto&
Iterable<Derived>::Back() const {
  return begin()[Size() - 1];
}

}  // namespace dab::detail::common