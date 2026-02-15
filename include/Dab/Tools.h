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

#include <chrono>
#include <format>
#include <iostream>
#include <ostream>
#include <print>
#include <utility>

namespace dab {

template <class... Args>
void Log(std::ostream& os, std::format_string<Args...> fmt, Args&&... args) {
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  std::println(os,
               R"({:%Y-%m-%dT%H:%M:%S} {})",
               std::chrono::floor<std::chrono::seconds>(now),
               std::format(fmt, std::forward<Args>(args)...));
}

template <class... Args>
void LogInfo(std::format_string<Args...> fmt, Args&&... args) {
  Log(std::cout, fmt, std::forward<Args>(args)...);
}

template <class... Args>
void LogError(std::format_string<Args...> fmt, Args&&... args) {
  Log(std::cerr, R"({{"Error":"{}"}})", std::format(fmt, std::forward<Args>(args)...));
}

#ifdef NDEBUG
#define Assert(expr) ((void)0)
#else

#define Assert(expr)                                                                   \
  do {                                                                                 \
    if (!(expr)) {                                                                     \
      dab::LogError(R"(ASSERT: '{}' in file {}, line {})", #expr, __FILE__, __LINE__); \
      std::abort();                                                                    \
    }                                                                                  \
  } while (false)

#endif  // NDEBUG

}  // namespace dab