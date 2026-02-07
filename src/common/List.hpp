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
#include <cstddef>

#include "Array.hpp"

namespace dab::detail::common {

template <typename T, uint32_t Cap>
class List : public Iterable<List<T, Cap>> {
 public:
  constexpr List() = default;
  constexpr List(const List& other) = default;
  constexpr List(List&& other) = default;
  constexpr List& operator=(const List& other) = default;
  constexpr List& operator=(List&& other) = default;

  constexpr void ClearAndSet(const T item);
  constexpr void Clear() { Length = 0; }
  constexpr void Append(const T item);
  constexpr uint32_t Size() const { return Length; }
  constexpr T* begin() { return Data.begin(); }
  constexpr const T* begin() const { return Data.begin(); }
  constexpr T* end() { return Data.begin() + Length; }
  constexpr const T* end() const { return Data.begin() + Length; }

 private:
  Array<T, Cap> Data;
  uint32_t Length = 0;
};

template <typename T, uint32_t Cap>
constexpr void List<T, Cap>::ClearAndSet(const T item) {
  Data[0] = item;
  Length = 1;
}

template <typename T, uint32_t Cap>
constexpr void List<T, Cap>::Append(const T item) {
  assert(Length < Cap);
  Data[Length++] = item;
}

}  // namespace dab::detail::common