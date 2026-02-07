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

#include <Dab/Robot.hpp>
#include <QApplication>
#include <QCommandLineParser>

namespace dab::detail::frontend {

static constexpr const char* PlayerTypeOptionStrings[] = {
    "human",
    "robot:easy",
    "robot:medium",
    "robot:hard",
    "robot:expert",
    "robot:master",
};

class Config {
 public:
  QString ToString() const;

  PlayerType Player1Type;
  PlayerType Player2Type;
  bool BackgroundMode;
};

class CommandParser {
  static constexpr PlayerType DefaultPlayerType = PlayerType::ParallelSearchRobot;

 public:
  CommandParser() = default;
  int Process(const QApplication& application);

 private:
  QCommandLineOption PlayerTypeOption(int player);
  QCommandLineOption BackgroundModeOption();
  PlayerType ParsePlayerType(const QString& arg);
};

}  // namespace dab::detail::frontend