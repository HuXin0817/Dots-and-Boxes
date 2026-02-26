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

#include "extern.h"

#include <Dab/Frontend.h>
#include <Dab/PlayerType.h>

namespace dab {

namespace __detail__ {

void MockRunningGame(PlayerType player1Type, PlayerType player2Type) {
  std::unique_ptr<Robot> robot1 = Robot::Create(player1Type);
  std::unique_ptr<Robot> robot2 = Robot::Create(player2Type);
  Assert(robot1);
  Assert(robot2);

  Random random;
  LoggingBoard board;
  while (board.Gaming()) {
    if (board.IsPlayer1Turn()) {
      board.Add(random.Choice(robot1->BestCandidateEdges(board)));
    } else {
      board.Add(random.Choice(robot2->BestCandidateEdges(board)));
    }
  }
}

}  // namespace __detail__

template <>
QWidget* CreateMainWindowImpl<BoardSize>(PlayerType player1Type, PlayerType player2Type, bool backgroundMode) {
  LogInfo(R"({{"BoardSize":{},"Player1Type":"{}","Player2Type":"{}"}})",
          BoardSize,
          PlayerTypeOptionStrings[static_cast<int>(player1Type)],
          PlayerTypeOptionStrings[static_cast<int>(player2Type)]);
  if (backgroundMode) {
    __detail__::MockRunningGame(player1Type, player2Type);
    exit(0);
  }

  return new MainWindow(player1Type, player2Type);
}

}  // namespace dab