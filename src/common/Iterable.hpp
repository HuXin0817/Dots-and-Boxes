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

#include "Int.hpp"

namespace dab::detail::common {

template <typename Derived>
class Iterable {
 public:
  constexpr Iterable() = default;
  constexpr Iterable(const Iterable& other) = default;
  constexpr Iterable(Iterable&& other) = default;
  constexpr Iterable& operator=(const Iterable& other) = default;
  constexpr Iterable& operator=(Iterable&& other) = default;

  constexpr auto begin() { return static_cast<Derived*>(this)->begin(); }
  constexpr auto begin() const { return static_cast<const Derived*>(this)->begin(); }
  constexpr auto end() { return static_cast<Derived*>(this)->end(); }
  constexpr auto end() const { return static_cast<const Derived*>(this)->end(); }
  constexpr Int Size() const { return static_cast<const Derived*>(this)->Size(); }
  constexpr bool Empty() const { return Size() == 0; }
  constexpr auto& Front() { return begin()[0]; }
  constexpr const auto& Front() const { return begin()[0]; }
  constexpr auto& Back() { return begin()[Size() - 1]; }
  constexpr const auto& Back() const { return begin()[Size() - 1]; }
  constexpr auto& operator[](Int i);
  constexpr const auto& operator[](Int i) const;

 protected:
  ~Iterable() = default;
};

template <typename Derived>
constexpr auto& Iterable<Derived>::operator[](Int i) {
  assert(0 <= i && i < Size());
  return begin()[i];
}

template <typename Derived>
constexpr const auto& Iterable<Derived>::operator[](Int i) const {
  assert(0 <= i && i < Size());
  return begin()[i];
}

}  // namespace dab::detail::common