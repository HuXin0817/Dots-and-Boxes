/*
Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2025 Xin Hu <huxin0817.hx@gmail.com>

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
#include <cstdlib>
#include <format>
#include <iostream>
#include <mutex>
#include <ostream>
#include <print>
#include <source_location>
#include <utility>

namespace dab {

namespace __detail__::tools {

#define STR(x) #x
#define XSTR(x) STR(x)

template <class... Args>
void Log(std::ostream& os, std::format_string<Args...> fmt, Args&&... args) {
  static constexpr std::string name = XSTR(__detail__);
  const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  const std::chrono::system_clock::time_point timestamp = std::chrono::floor<std::chrono::seconds>(now);
  const std::string message = std::format(fmt, std::forward<Args>(args)...);
  std::println(os, R"([{}] {:%Y-%m-%dT%H:%M:%S} {})", name, timestamp, message);
}

template <class... Args>
void LogInfo(std::format_string<Args...> fmt, Args&&... args) {
  Log(std::cout, fmt, std::forward<Args>(args)...);
}

template <class... Args>
void LogError(std::format_string<Args...> fmt, Args&&... args) {
  Log(std::cerr, R"({{"Error":"{}"}})", std::format(fmt, std::forward<Args>(args)...));
}

static void AssertHelper(const std::string& expr,
                         const std::source_location& location = std::source_location::current()) {
  static std::mutex mu;
  {
    std::unique_lock lock(mu);
    LogError("ASSERT: '{}' in file {}, line {}", expr, location.file_name(), location.line());
    std::abort();
  }
};

#ifdef NDEBUG
#define Assert(expr) ((void)0)
#else

#define Assert(expr)                          \
  do {                                        \
    if (!(expr)) {                            \
      __detail__::tools::AssertHelper(#expr); \
    }                                         \
  } while (false)

#endif  // NDEBUG

}  // namespace __detail__::tools

using __detail__::tools::LogError;
using __detail__::tools::LogInfo;

}  // namespace dab