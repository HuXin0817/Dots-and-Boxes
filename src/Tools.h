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

#include <spdlog/spdlog.h>

#include <format>
#include <mutex>
#include <nlohmann/json.hpp>
#include <source_location>
#include <sstream>
#include <string>
#include <type_traits>

namespace dab {

#define STR(x) #x
#define XSTR(x) STR(x)

inline void LogInfo(const nlohmann::ordered_json& message) { spdlog::info(message.dump()); }

template <class... Args>
void LogError(std::format_string<Args...> fmt, Args&&... args) {
  spdlog::error(nlohmann::ordered_json{{"Error", std::format(fmt, std::forward<Args>(args)...)}}.dump());
}

template <class... Args>
void AssertHelper(const std::source_location& location, const std::string& expr, Args&&... details) {
  if constexpr (sizeof...(details) > 0) {
    std::ostringstream oss;
    size_t index = 0;
    ((oss << (index++ ? "," : "") << details), ...);
    LogError("ASSERT: '{}' in file {}, line {}. Detail: {}", expr, location.file_name(), location.line(), oss.str());
  } else {
    LogError("ASSERT: '{}' in file {}, line {}.", expr, location.file_name(), location.line());
  }
  std::abort();
}

#define K(expr) (std::format("{}={}", #expr, nlohmann::ordered_json(expr).dump()))

#ifdef NDEBUG
#define Assert(expr, ...) ((void)0)
#else
#define Assert(expr, ...)                                                  \
  do {                                                                     \
    if (!(expr)) {                                                         \
      AssertHelper(std::source_location::current(), #expr, ##__VA_ARGS__); \
    }                                                                      \
  } while (false)
#endif  // NDEBUG

template <bool Bp, typename T>
using Mixin = std::conditional_t<Bp, T, std::type_identity<T>>;

}  // namespace dab