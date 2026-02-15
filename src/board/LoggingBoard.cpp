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

#include "LoggingBoard.h"

namespace dab::__detail__::board {

void LoggingBoard::Reset() {
  AbsoluteScoreBoard::Reset();
  LastUpdateTime = std::chrono::system_clock::now();
}

Int LoggingBoard::Add(Edge edge) {
  const int64_t step = NowStep();
  if (step == 0) {
    LogInfo(R"({{"BoardSize":{}}})", BoardSize);
  }
  const Turn turn = *this;
  const Int score = AbsoluteScoreBoard::Add(edge);
  const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  const int64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(now - LastUpdateTime).count();
  LogInfo(R"({{"Step":{},"Turn":{},"Move":{},"Score":{{"Player1":{},"Player2":{}}},"Time":{}}})",
          step,
          turn.IsPlayer1Turn() ? 1 : 2,
          static_cast<Int>(edge),
          Player1Score(),
          Player2Score(),
          static_cast<double>(time) / 1000.0);
  if (!Gaming()) {
    if (RelativeScore() > 0) {
      LogInfo(R"({{"Winner":"Player1"}})");
    } else if (RelativeScore() < 0) {
      LogInfo(R"({{"Winner":"Player2"}})");
    } else {
      LogInfo(R"({{"Winner":"Draw"}})");
    }
  }
  LastUpdateTime = now;
  return score;
}

}  // namespace dab::__detail__::board