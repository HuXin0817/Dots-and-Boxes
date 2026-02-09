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

#include "Models.h"

namespace dab::detail::frontend {

QJsonObject Config::ToJson() const {
  QJsonObject configData;
  configData.insert("BoardSize", BoardSize);
  configData.insert("Player1Type", PlayerTypeString(Player1Type));
  configData.insert("Player2Type", PlayerTypeString(Player2Type));
  configData.insert("BackgroundMode", BackgroundMode);
  QJsonObject config;
  config.insert("Config", configData);
  return config;
}

QJsonObject MoveRecord::ToJson() const {
  QJsonObject playerScore;
  playerScore.insert("Player1", Player1Score);
  playerScore.insert("Player2", Player2Score);

  QJsonObject moveRecord;
  moveRecord.insert("Step", Step);
  moveRecord.insert("Turn", Turn.IsPlayer1Turn() ? 1 : 2);
  moveRecord.insert("Move", Int(Move));
  moveRecord.insert("Score", playerScore);
  moveRecord.insert("Time", Time);

  QJsonObject info;
  info.insert("Info", moveRecord);
  return info;
}

QJsonObject Winner::ToJson() const {
  QJsonObject winner;
  winner.insert("Winner", Name);
  return winner;
}

}  // namespace dab::detail::frontend