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

#include "Iterable.hpp"

namespace dab::detail::common {

template <typename T>
class Span : public Iterable<Span<T>> {
 public:
  constexpr Span() = default;
  constexpr Span(T* begin, T* end);

  template <typename Other>
  constexpr
  operator Span<Other>() const;

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
  T* BeginPtr = nullptr;
  T* EndPtr = nullptr;
};

template <typename T>
constexpr Span<T>::Span(T* begin, T* end) : BeginPtr(begin), EndPtr(end) {
}

template <typename T>
template <typename Other>
constexpr Span<T>::
operator Span<Other>() const {
  return Span<Other>(const_cast<Other*>(reinterpret_cast<const Other*>(begin())),
                     const_cast<Other*>(reinterpret_cast<const Other*>(end())));
}

template <typename T>
constexpr uint32_t
Span<T>::Size() const {
  return EndPtr - BeginPtr;
}

template <typename T>
constexpr T*
Span<T>::begin() {
  return BeginPtr;
}

template <typename T>
constexpr const T*
Span<T>::begin() const {
  return BeginPtr;
}

template <typename T>
constexpr T*
Span<T>::end() {
  return EndPtr;
}

template <typename T>
constexpr const T*
Span<T>::end() const {
  return EndPtr;
}

}  // namespace dab::detail::common