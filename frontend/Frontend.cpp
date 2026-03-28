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

#include "Frontend.h"

#include "MainWindow.h"
#include "PlayerType.h"
#include "Robot.h"

namespace dab {

template <>
QWidget* CreateMainWindowImpl<BoardSize>(PlayerType player1Type, PlayerType player2Type, bool backgroundMode) {
  LogInfo({
      {"BoardSize", BoardSize},
      {"Player1Type", PlayerTypeOptionStrings[static_cast<int>(player1Type)]},
      {"Player2Type", PlayerTypeOptionStrings[static_cast<int>(player2Type)]},
  });
  if (backgroundMode) {
    MockRunningGame(player1Type, player2Type);
    exit(0);
  }

  return new __detail__::frontend::MainWindow(player1Type, player2Type);
}

}  // namespace dab