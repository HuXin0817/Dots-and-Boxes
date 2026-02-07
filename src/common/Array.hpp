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

#include <cstdint>

#include "Iterable.hpp"

namespace dab::detail::common {

template <typename T, uint32_t Length>
class Array : public Iterable<Array<T, Length>> {
 public:
  constexpr Array() = default;
  constexpr Array(const Array& other) = default;
  constexpr Array(Array&& other) = default;
  constexpr Array&
  operator=(const Array& other) = default;
  constexpr Array&
  operator=(Array&& other) = default;

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
  T Data[Length];
};

template <typename T, uint32_t Length>
constexpr uint32_t
Array<T, Length>::Size() const {
  return Length;
}

template <typename T, uint32_t Length>
constexpr T*
Array<T, Length>::begin() {
  return Data;
}

template <typename T, uint32_t Length>
constexpr const T*
Array<T, Length>::begin() const {
  return Data;
}

template <typename T, uint32_t Length>
constexpr T*
Array<T, Length>::end() {
  return Data + Length;
}

template <typename T, uint32_t Length>
constexpr const T*
Array<T, Length>::end() const {
  return Data + Length;
}

}  // namespace dab::detail::common