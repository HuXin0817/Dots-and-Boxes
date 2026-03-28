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

#include <chrono>
#include <cstdlib>
#include <format>
#include <mutex>
#include <nlohmann/json.hpp>
#include <random>
#include <source_location>
#include <sstream>
#include <string>
#include <type_traits>

namespace dab {

#ifdef NDEBUG
static constexpr bool DebugMode = false;
#else
static constexpr bool DebugMode = true;
#endif  // NDEBUG

#define STR(x) #x
#define XSTR(x) STR(x)

inline void LogInfo(const nlohmann::ordered_json& message) { spdlog::info(message.dump()); }

inline void LogDebug(const nlohmann::ordered_json& message) { spdlog::debug(message.dump()); }

template <class... Args>
void LogError(std::format_string<Args...> fmt, Args&&... args) {
  const std::string message = std::format(fmt, std::forward<Args>(args)...);
  spdlog::error(nlohmann::ordered_json{{"Error", message}}.dump());
}

static std::mutex AssertHelperMutex;

template <class... Args>
void AssertHelper(const std::source_location& location, const std::string& expr, Args&&... details) {
  static constexpr std::size_t details_size = sizeof...(details);
  std::unique_lock lock(AssertHelperMutex);
  if constexpr (details_size > 0) {
    std::ostringstream oss;
    std::size_t index = 0;
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

class Random {
 public:
  Random();

  template <typename T>
  T Range(T min, T max);
  template <typename T>
  const auto& Choice(const T& data);

 private:
  std::mt19937_64 Rng;
};

inline Random::Random() { Rng.seed(std::chrono::steady_clock::now().time_since_epoch().count()); }

template <typename T>
T Random::Range(T min, T max) {
  std::uniform_int_distribution<T> dist(min, max);
  return dist(Rng);
}

template <typename T>
const auto& Random::Choice(const T& data) {
  Assert(!data.Empty());
  if (data.Size() == 1) {
    return data.At(0);
  }
  return data.At(Range(0, data.Size() - 1));
}

}  // namespace dab