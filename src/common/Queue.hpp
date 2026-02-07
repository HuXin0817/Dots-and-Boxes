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

#include "Array.hpp"

namespace dab::detail::common {

template <typename T, uint32_t Cap>
class Queue : public Iterable<Queue<T, Cap>> {
 public:
  constexpr Queue() = default;
  constexpr Queue(const Queue& other) = default;
  constexpr Queue(Queue&& other) = default;
  constexpr Queue&
  operator=(const Queue& other) = default;
  constexpr Queue&
  operator=(Queue&& other) = default;

  constexpr void
  Clear();
  constexpr void
  Append(const T item);
  constexpr T
  Pop();

  constexpr uint32_t
  Size() const;
  constexpr T*
  begin();
  constexpr const T*
  begin() const;
  constexpr T*
  end();
  constexpr const T*
  end() const;

 private:
  Array<T, Cap> Data;
  uint32_t BeginIndex = 0;
  uint32_t EndIndex = 0;
};

template <typename T, uint32_t Cap>
constexpr void
Queue<T, Cap>::Clear() {
  BeginIndex = 0;
  EndIndex = 0;
}

template <typename T, uint32_t Cap>
constexpr void
Queue<T, Cap>::Append(const T item) {
  assert(EndIndex < Cap);
  Data[EndIndex++] = item;
}

template <typename T, uint32_t Cap>
constexpr T
Queue<T, Cap>::Pop() {
  assert(Size() > 0);
  return Data[BeginIndex++];
}

template <typename T, uint32_t Cap>
constexpr uint32_t
Queue<T, Cap>::Size() const {
  return EndIndex - BeginIndex;
}

template <typename T, uint32_t Cap>
constexpr T*
Queue<T, Cap>::begin() {
  return Data.begin() + BeginIndex;
}

template <typename T, uint32_t Cap>
constexpr const T*
Queue<T, Cap>::begin() const {
  return Data.begin() + BeginIndex;
}

template <typename T, uint32_t Cap>
constexpr T*
Queue<T, Cap>::end() {
  return Data.begin() + EndIndex;
}

template <typename T, uint32_t Cap>
constexpr const T*
Queue<T, Cap>::end() const {
  return Data.begin() + EndIndex;
}

}  // namespace dab::detail::common