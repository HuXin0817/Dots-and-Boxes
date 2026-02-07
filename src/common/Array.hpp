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