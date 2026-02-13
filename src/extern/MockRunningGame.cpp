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

#include "MockRunningGame.h"

#include <Dab/Robot.h>

#include <QPointer>

namespace dab::internal {

template <>
void MockRunningGame::Call<BoardSize>(int player1Type, int player2Type) {
  QScopedPointer<Robot> robot1;
  QScopedPointer<Robot> robot2;
  robot1.reset(CreateRobot(static_cast<PlayerType>(player1Type)));
  robot2.reset(CreateRobot(static_cast<PlayerType>(player2Type)));
  assert(robot1);
  assert(robot2);

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

}  // namespace dab::internal