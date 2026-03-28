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

#include <QWidget>

#include "PlayerType.h"

namespace dab {

template <int64_t BoardSize>
QWidget* CreateMainWindowImpl(PlayerType player1Type, PlayerType player2Type, bool backgroundMode);

template <int64_t BoardSize>
QWidget* CreateMainWindow(int64_t boardSize, PlayerType player1Type, PlayerType player2Type, bool backgroundMode) {
  if constexpr (BoardSize == 0) {
    return nullptr;
  } else {
    if (boardSize < BoardSize) {
      return CreateMainWindow<BoardSize - 1>(boardSize, player1Type, player2Type, backgroundMode);
    } else {
      return CreateMainWindowImpl<BoardSize>(player1Type, player2Type, backgroundMode);
    }
  }
}

}  // namespace dab