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
#include <type_traits>
#include <utility>

namespace dab {

template <int64_t BoardSize, typename FuncNameTag, typename ReturnType, typename... Args>
  requires(BoardSize >= 0)
constexpr auto DispatchImpl(int64_t boardSize, Args&&... args) {
  if constexpr (BoardSize == 0) {
    std::unreachable();
    if constexpr (!std::is_void_v<ReturnType>) {
      return ReturnType{};
    }
  } else {
    if (boardSize < BoardSize) {
      if constexpr (!std::is_void_v<ReturnType>) {
        return DispatchImpl<BoardSize - 1, FuncNameTag, ReturnType>(boardSize, std::forward<Args>(args)...);
      } else {
        DispatchImpl<BoardSize - 1, FuncNameTag, ReturnType>(boardSize, std::forward<Args>(args)...);
      }
    } else {
      if constexpr (!std::is_void_v<ReturnType>) {
        return FuncNameTag::template Call<BoardSize>(std::forward<Args>(args)...);
      } else {
        FuncNameTag::template Call<BoardSize>(std::forward<Args>(args)...);
      }
    }
  }
}

template <typename FuncNameTag, typename... Args>
constexpr auto Dispatch(int64_t boardSize, Args&&... args) {
  using ReturnType = decltype(FuncNameTag::template Call<__MaxBoardSize__>(std::forward<Args>(args)...));
  return DispatchImpl<__MaxBoardSize__, FuncNameTag, ReturnType>(boardSize, std::forward<Args>(args)...);
}

}  // namespace dab