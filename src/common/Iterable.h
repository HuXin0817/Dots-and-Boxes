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

#include "Int.h"

namespace dab::__detail__::common {

template <typename Derived>
class Iterable {
 public:
  constexpr Iterable() = default;
  constexpr Iterable(const Iterable&) = default;
  constexpr Iterable(Iterable&&) = default;
  constexpr Iterable& operator=(const Iterable&) = default;
  constexpr Iterable& operator=(Iterable&&) = default;

  constexpr auto begin(this auto&& self) { return self.derived().begin(); }
  constexpr auto end(this auto&& self) { return self.derived().end(); }
  constexpr Int Size(this const auto& self) { return self.derived().Size(); }
  constexpr bool Empty(this const auto& self) { return self.Size() == 0; }
  constexpr auto& Front(this auto& self) { return self.begin()[0]; }
  constexpr const auto& Front(this const auto& self) { return self.begin()[0]; }
  constexpr auto& Back(this auto& self) { return self.begin()[self.Size() - 1]; }
  constexpr const auto& Back(this const auto& self) { return self.begin()[self.Size() - 1]; }

  constexpr auto& At(this auto& self, Int i) {
    Assert(0 <= i && i < self.Size());
    return self.begin()[i];
  }
  constexpr const auto& At(this const auto& self, Int i) {
    Assert(0 <= i && i < self.Size());
    return self.begin()[i];
  }

 protected:
  ~Iterable() = default;

 private:
  constexpr Derived& derived() & { return static_cast<Derived&>(*this); }
  constexpr const Derived& derived() const& { return static_cast<const Derived&>(*this); }
  constexpr Derived&& derived() && { return static_cast<Derived&&>(*this); }
};

}  // namespace dab::__detail__::common