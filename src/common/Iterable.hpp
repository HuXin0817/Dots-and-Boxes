/*
Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2025 Xin Hu <202219120810@stu.cdut.edu.cn>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#pragma once

#include <cassert>
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
  return static_cast<Derived*>(this)->begin();
}

template <typename Derived>
constexpr auto
Iterable<Derived>::begin() const {
  return static_cast<const Derived*>(this)->begin();
}

template <typename Derived>
constexpr auto
Iterable<Derived>::end() {
  return static_cast<Derived*>(this)->end();
}

template <typename Derived>
constexpr auto
Iterable<Derived>::end() const {
  return static_cast<const Derived*>(this)->end();
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