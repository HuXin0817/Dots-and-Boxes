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

#include <QWidget>
#include <type_traits>
#include <utility>

namespace dab {

template <int64_t BoardSize, typename FuncNameTag, typename... Args>
constexpr auto Dispatch(int64_t boardSize, Args&&... args) {
  using ReturnType = decltype(FuncNameTag::template Call<BoardSize>(std::forward<Args>(args)...));
  static constexpr bool HaveReturnValue = !std::is_void_v<ReturnType>;

  if constexpr (BoardSize == 0) {
    std::unreachable();
    if constexpr (HaveReturnValue) {
      return ReturnType{};
    }
  } else {
    if (boardSize < BoardSize) {
      if constexpr (HaveReturnValue) {
        return Dispatch<BoardSize - 1, FuncNameTag>(boardSize, std::forward<Args>(args)...);
      } else {
        Dispatch<BoardSize - 1, FuncNameTag>(boardSize, std::forward<Args>(args)...);
      }
    } else {
      auto call = FuncNameTag::template Call<BoardSize>;
      if constexpr (HaveReturnValue) {
        return call(std::forward<Args>(args)...);
      } else {
        call(std::forward<Args>(args)...);
      }
    }
  }
}

class MockRunningGame {
 public:
  template <int64_t BoardSize>
  static void Call(int player1Type, int player2Type);
};

class CreateMainWindow {
 public:
  template <int64_t BoardSize>
  static QWidget* Call(int player1Type, int player2Type, QWidget* parent);
};

}  // namespace dab