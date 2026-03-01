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

#include <Dab/PlayerType.h>
#include <Dab/Tools.h>

#include <QApplication>
#include <QCommandLineParser>
#include <cstdlib>
#include <ranges>

#include "frontend/Frontend.h"

using namespace dab;

static constexpr int64_t DefaultBoardSize = __DefaultBoardSize__;
static constexpr int64_t MaxBoardSize = __MaxBoardSize__;
static_assert(DefaultBoardSize <= MaxBoardSize);

QCommandLineOption CreateBoardSizeOption() {
  const QByteArray boardSizeEnv = qgetenv("BOARD_SIZE");
  QString defaultBoardSize = QString::number(DefaultBoardSize);
  if (!boardSizeEnv.isEmpty()) {
    defaultBoardSize = boardSizeEnv;
  }
  const QStringList names = {"s", "boardsize"};
  const QString description = QString("Set board size (1-%1).").arg(MaxBoardSize);
  return {names, description, "size", defaultBoardSize};
}

QCommandLineOption CreatePlayerTypeOption(int playerid) {
  const QByteArray playerTypeEnv = qgetenv(std::format("PLAYER{}", playerid).c_str());
  QString defaultPlayerType = "robot";
  if (!playerTypeEnv.isEmpty()) {
    defaultPlayerType = playerTypeEnv;
  }

  QStringList accepted;
  accepted << "human" << "robot";
  for (const size_t i : std::views::iota(0ull, std::size(PlayerTypeOptionStrings))) {
    accepted << QString::fromUtf8(PlayerTypeOptionStrings[i]);
  }

  QString acceptedStr;
  if (accepted.size() == 1) {
    acceptedStr = "'" + accepted[0] + "'";
  } else if (accepted.size() == 2) {
    acceptedStr = "'" + accepted[0] + "' or '" + accepted[1] + "'";
  } else {
    const QString allButLast = accepted.mid(0, accepted.size() - 1).join("', '");
    acceptedStr = "'" + allButLast + "' or '" + accepted.last() + "'";
  }

  const QStringList names = {QString("p%1").arg(playerid), QString("player%1").arg(playerid)};
  const QString defaultPlayerTypeString = PlayerTypeOptionStrings[static_cast<int>(DefaultPlayerType)];
  const QString discription =
      QString("Set type of player %1. Accepts: %2 (default: 'robot'). Note: 'robot' is equivalent to '%3'.")
          .arg(playerid)
          .arg(acceptedStr)
          .arg(defaultPlayerTypeString);
  return {names, discription, "type", defaultPlayerType};
}

PlayerType ParsePlayerType(const QString& arg) {
  if (arg.compare("robot", Qt::CaseInsensitive) == 0) {
    return DefaultPlayerType;
  }
  for (const size_t i : std::views::iota(0ull, std::size(PlayerTypeOptionStrings))) {
    if (arg.compare(PlayerTypeOptionStrings[i], Qt::CaseInsensitive) == 0) {
      return static_cast<PlayerType>(i);
    }
  }
  LogError("Invalid player type '{}'.", arg.toLocal8Bit().constData());
  exit(EXIT_FAILURE);
}

int64_t ParseBoardSize(const QString& arg) {
  bool ok = false;
  const int64_t boardSize = arg.toLongLong(&ok);
  if (!ok || boardSize < 1 || boardSize > MaxBoardSize) {
    LogError("Invalid board size '{}'. Must be between 1 and {}.", arg.toLocal8Bit().constData(), MaxBoardSize);
    exit(EXIT_FAILURE);
  }
  return boardSize;
}

int main(int argc, char* argv[]) {
  QApplication application(argc, argv);
  application.setApplicationName("Dots and Boxes");
  application.setApplicationVersion(__Version__);
  application.setOrganizationName("Dots and Boxes");

  const QCommandLineOption boardSizeOption = CreateBoardSizeOption();
  const QCommandLineOption player1Option = CreatePlayerTypeOption(1);
  const QCommandLineOption player2Option = CreatePlayerTypeOption(2);
  const QCommandLineOption backgroundModeOption(QStringList() << "b" << "background", "Running in background mode.");

  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addOption(boardSizeOption);
  parser.addOption(player1Option);
  parser.addOption(player2Option);
  parser.addOption(backgroundModeOption);
  parser.process(application);

  const int64_t boardSize = ParseBoardSize(parser.value(boardSizeOption));
  const PlayerType player1Type = ParsePlayerType(parser.value(player1Option));
  const PlayerType player2Type = ParsePlayerType(parser.value(player2Option));
  const bool backgroundMode = parser.isSet(backgroundModeOption);

  QWidget* mainWindow = CreateMainWindow<MaxBoardSize>(boardSize, player1Type, player2Type, backgroundMode);
  mainWindow->show();
  const int code = application.exec();
  if (code != 0) {
    LogError("Exit code {}.", code);
  }
  return code;
}