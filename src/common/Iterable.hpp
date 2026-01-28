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
  At(size_t i);
  const auto&
  At(size_t i) const;

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
Iterable<Derived>::At(size_t i) {
  return static_cast<Derived*>(this)->Begin()[i];
}

template <typename Derived>
const auto&
Iterable<Derived>::At(size_t i) const {
  return static_cast<const Derived*>(this)->Begin()[i];
}

}  // namespace dab