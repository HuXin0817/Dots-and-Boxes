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

}  // namespace dab