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

#include <Dab/Tools.h>

#include <algorithm>
#include <memory>

#include "Iterable.h"

namespace dab::__detail__::common {

template <typename T>
class Vector : public Iterable<Vector<T>> {
  struct Buffer {
    Int Length;
    char Data[0];
  };

 public:
  constexpr Vector() = default;
  constexpr Vector(Int length) { Reset(length); }
  constexpr Vector(T* begin, T* end) { Reset(begin, end); }
  constexpr Vector(const T* begin, const T* end) { Reset(begin, end); }

  constexpr Vector(const Vector& other) = default;
  constexpr Vector(Vector&& other) = default;
  constexpr Vector& operator=(const Vector& other) = default;
  constexpr Vector& operator=(Vector&& other) = default;

  constexpr void Reset(Int length);
  constexpr void Reset(T* begin, T* end);
  constexpr void Reset(const T* begin, const T* end);

  constexpr Int Size() const { return Data->Length; }
  constexpr T* begin() { return reinterpret_cast<T*>(Data->Data); }
  constexpr const T* begin() const { return reinterpret_cast<const T*>(Data->Data); }
  constexpr T* end() { return reinterpret_cast<T*>(Data->Data) + Data->Length; }
  constexpr const T* end() const { return reinterpret_cast<const T*>(Data->Data) + Data->Length; }

  ~Vector() = default;

 private:
  std::unique_ptr<Buffer> Data;
};

template <typename T>
constexpr void Vector<T>::Reset(Int length) {
  Assert(Data == nullptr);
  char* const memory = new char[sizeof(Int) + sizeof(T) * length];
  Data.reset(reinterpret_cast<Buffer*>(memory));
  Data->Length = length;
}

template <typename T>
constexpr void Vector<T>::Reset(T* begin, T* end) {
  Reset(end - begin);
  std::ranges::copy(begin, end, reinterpret_cast<T*>(Data->Data));
}

template <typename T>
constexpr void Vector<T>::Reset(const T* begin, const T* end) {
  Reset(end - begin);
  std::ranges::copy(begin, end, reinterpret_cast<T*>(Data->Data));
}

}  // namespace dab::__detail__::common