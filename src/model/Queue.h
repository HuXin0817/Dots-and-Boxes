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

#include "Array.h"

namespace dab::__detail__::model {

template <typename T, Int Cap>
class Queue : public Iterable<Queue<T, Cap>> {
 public:
  constexpr Queue() = default;
  constexpr Queue(const Queue& other) = default;
  constexpr Queue(Queue&& other) = default;
  constexpr Queue& operator=(const Queue& other) = default;
  constexpr Queue& operator=(Queue&& other) = default;

  constexpr void Clear();
  constexpr void Append(T item);
  constexpr T Pop();
  constexpr Int Size() const { return EndIndex - BeginIndex; }
  constexpr T* begin() { return Data.begin() + BeginIndex; }
  constexpr const T* begin() const { return Data.begin() + BeginIndex; }
  constexpr T* end() { return Data.begin() + EndIndex; }
  constexpr const T* end() const { return Data.begin() + EndIndex; }

 private:
  Array<T, Cap> Data;
  Int BeginIndex = 0;
  Int EndIndex = 0;
};

template <typename T, Int Cap>
constexpr void Queue<T, Cap>::Clear() {
  BeginIndex = 0;
  EndIndex = 0;
}

template <typename T, Int Cap>
constexpr void Queue<T, Cap>::Append(T item) {
  assert(EndIndex < Cap);
  Data.At(EndIndex++) = item;
}

template <typename T, Int Cap>
constexpr T Queue<T, Cap>::Pop() {
  assert(Size() > 0);
  return Data.At(BeginIndex++);
}

}  // namespace dab::__detail__::model