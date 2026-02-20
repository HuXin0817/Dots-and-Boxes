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
#include <tbb/tbb_allocator.h>

#include <algorithm>

#include "Iterable.h"

namespace dab::__detail__::common {

template <typename T>
class Vector : public Iterable<Vector<T>> {
  struct Buffer {
    Int Length;
    char Data[0];
  };

  static inline tbb::tbb_allocator<char> Allocator;

 public:
  constexpr Vector() noexcept;
  constexpr explicit Vector(Int length);
  constexpr Vector(T* begin, T* end);
  constexpr Vector(const T* begin, const T* end);

  constexpr Vector(const Vector& other);
  constexpr Vector& operator=(const Vector& other);

  constexpr Vector(Vector&& other) noexcept;
  constexpr Vector& operator=(Vector&& other) noexcept;

  constexpr void Reset(Int length);
  constexpr void Reset(T* begin, T* end);
  constexpr void Reset(const T* begin, const T* end);

  constexpr Int Size() const noexcept;
  constexpr T* begin() noexcept;
  constexpr const T* begin() const noexcept;
  constexpr T* end() noexcept;
  constexpr const T* end() const noexcept;

  ~Vector() noexcept;

 private:
  Buffer* Data = nullptr;
};

template <typename T>
constexpr Vector<T>::Vector() noexcept : Data(nullptr) {}

template <typename T>
constexpr Vector<T>::Vector(Int length) : Data(nullptr) {
  Reset(length);
}

template <typename T>
constexpr Vector<T>::Vector(T* begin, T* end) : Data(nullptr) {
  Reset(begin, end);
}

template <typename T>
constexpr Vector<T>::Vector(const T* begin, const T* end) : Data(nullptr) {
  Reset(begin, end);
}

template <typename T>
constexpr Vector<T>::Vector(const Vector& other) : Data(nullptr) {
  if (other.Data != nullptr) {
    Reset(other.Data->Length);
    std::uninitialized_copy(other.begin(), other.end(), begin());
  }
}

template <typename T>
constexpr Vector<T>& Vector<T>::operator=(const Vector& other) {
  if (this != &other) {
    Vector temp(other);
    std::swap(Data, temp.Data);
  }
  return *this;
}

template <typename T>
constexpr Vector<T>::Vector(Vector&& other) noexcept : Data(nullptr) {
  Data = other.Data;
  other.Data = nullptr;
}

template <typename T>
constexpr Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
  if (this != &other) {
    if (Data != nullptr) {
      std::destroy(begin(), end());
      Allocator.deallocate(reinterpret_cast<char*>(Data), sizeof(Buffer) + sizeof(T) * Data->Length);
    }
    Data = other.Data;
    other.Data = nullptr;
  }
  return *this;
}

template <typename T>
constexpr void Vector<T>::Reset(Int length) {
  Assert(length >= 0);

  if (Data != nullptr) {
    std::destroy(begin(), end());
    Allocator.deallocate(reinterpret_cast<char*>(Data), sizeof(Buffer) + sizeof(T) * Data->Length);
    Data = nullptr;
  }

  if (length > 0) {
    const size_t total_bytes = sizeof(Buffer) + sizeof(T) * length;
    Data = reinterpret_cast<Buffer*>(Allocator.allocate(total_bytes));
    Assert(Data != nullptr);
    Data->Length = length;
    std::uninitialized_value_construct(begin(), end());
  }
}

template <typename T>
constexpr void Vector<T>::Reset(T* begin, T* end) {
  Assert(begin <= end);
  const Int length = static_cast<Int>(end - begin);
  Reset(length);
  if (length > 0) {
    std::uninitialized_copy(begin, end, this->begin());
  }
}

template <typename T>
constexpr void Vector<T>::Reset(const T* begin, const T* end) {
  Assert(begin <= end);
  const Int length = static_cast<Int>(end - begin);
  Reset(length);
  if (length > 0) {
    std::uninitialized_copy(begin, end, this->begin());
  }
}

template <typename T>
constexpr Int Vector<T>::Size() const noexcept {
  return Data ? Data->Length : 0;
}

template <typename T>
constexpr T* Vector<T>::begin() noexcept {
  return Data ? reinterpret_cast<T*>(Data->Data) : nullptr;
}

template <typename T>
constexpr const T* Vector<T>::begin() const noexcept {
  return Data ? reinterpret_cast<const T*>(Data->Data) : nullptr;
}

template <typename T>
constexpr T* Vector<T>::end() noexcept {
  return begin() + Size();
}

template <typename T>
constexpr const T* Vector<T>::end() const noexcept {
  return begin() + Size();
}

template <typename T>
Vector<T>::~Vector() noexcept {
  if (Data != nullptr) {
    std::destroy(begin(), end());
    Allocator.deallocate(reinterpret_cast<char*>(Data), sizeof(Buffer) + sizeof(T) * Data->Length);
    Data = nullptr;
  }
}

}  // namespace dab::__detail__::common