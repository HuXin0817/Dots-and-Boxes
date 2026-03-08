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
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

namespace dab {

#define STR(x) #x
#define XSTR(x) STR(x)

static constexpr std::string_view ColorReset = "\033[0m";
static constexpr std::string_view ColorInfo = "\033[32m";   // Green
static constexpr std::string_view ColorDebug = "\033[34m";  // Blue
static constexpr std::string_view ColorError = "\033[31m";  // Red

namespace __detail__::tools {

class LogHelper {
 private:
  static inline std::mutex LogMutex;

 public:
  template <class... Args>
  static void Log(std::ostream& os, std::string_view color, std::format_string<Args...> fmt, Args&&... args);

#define LogInfo(fmt, ...) __detail__::tools::LogHelper::Log(std::cout, ColorInfo, fmt, ##__VA_ARGS__)

#define LogError(fmt, ...) __detail__::tools::LogHelper::Log(std::cerr, ColorError, fmt, ##__VA_ARGS__)

#ifdef NDEBUG
#define LogDebug(fmt, ...) ((void)0)
#else
#define LogDebug(fmt, ...) __detail__::tools::LogHelper::Log(std::cout, ColorDebug, fmt, ##__VA_ARGS__)
#endif
};

template <class... Args>
void LogHelper::Log(std::ostream& os, std::string_view color, std::format_string<Args...> fmt, Args&&... args) {
  static constexpr std::string module = XSTR(__detail__);
  const auto now = std::chrono::system_clock::now();
  const auto timestamp = std::chrono::floor<std::chrono::seconds>(now);
  const std::string message = std::format(fmt, std::forward<Args>(args)...);
  std::unique_lock lock(LogMutex);
  os << color;
  std::println(os, "[{}] {:%Y-%m-%dT%H:%M:%S} {}", module, timestamp, message);
  os << ColorReset;
  os.flush();
}

class AssertHelper {
  static inline std::mutex AssertHelperMutex;

 public:
  template <class... Args>
  static inline void Info(const std::source_location& location, const std::string& expr, Args&&... details);
};

template <class... Args>
void AssertHelper::Info(const std::source_location& location, const std::string& expr, Args&&... details) {
  static constexpr std::size_t details_size = sizeof...(details);
  std::unique_lock lock(AssertHelperMutex);
  std::ostringstream oss;
  if constexpr (details_size > 0) {
    std::size_t index = 0;
    ((oss << (index++ ? "," : "") << details), ...);
    LogError("ASSERT: '{}' in file {}, line {}. Detail: {}", expr, location.file_name(), location.line(), oss.str());
  } else {
    LogError("ASSERT: '{}' in file {}, line {}.", expr, location.file_name(), location.line());
  }
  std::abort();
}

}  // namespace __detail__::tools

template <typename T>
std::string ToString(T value) {
  if constexpr (std::is_arithmetic_v<T>) {
    return std::to_string(value);
  } else {
    return static_cast<std::string>(value);
  }
}

#define K(expr) (std::format("{}={}", #expr, ToString(expr)))

#ifdef NDEBUG
#define Assert(expr, ...) ((void)0)
#else

#define Assert(expr, ...)                                                    \
  do {                                                                       \
    if (!(expr)) {                                                           \
      const auto location = std::source_location::current();                 \
      __detail__::tools::AssertHelper::Info(location, #expr, ##__VA_ARGS__); \
    }                                                                        \
  } while (false)

#endif  // NDEBUG

template <bool _Bp, typename T>
using Mixin = std::conditional_t<_Bp, T, std::type_identity<T>>;

}  // namespace dab