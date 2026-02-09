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

#include "CommandParser.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QJsonObject>
#include <cstdlib>

#include "MainWindow.h"
#include "Models.h"

namespace dab::detail::frontend {

static constexpr const char* PlayerTypeOptionStrings[] = {
    "human",
    "robot:easy",
    "robot:medium",
    "robot:hard",
    "robot:expert",
    "robot:master",
};

int CommandParser::Process(const QApplication& application) {
  const QCommandLineOption player1Option = PlayerTypeOption(1);
  const QCommandLineOption player2Option = PlayerTypeOption(2);
  const QCommandLineOption backgroundModeOption = BackgroundModeOption();

  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addOption(player1Option);
  parser.addOption(player2Option);
  parser.addOption(backgroundModeOption);
  parser.process(application);

  const Config config{
      .Player1Type = ParsePlayerType(parser.value(player1Option)),
      .Player2Type = ParsePlayerType(parser.value(player2Option)),
      .BackgroundMode = parser.isSet(backgroundModeOption),
  };
  if (config.BackgroundMode && (!PlayerTypeIsRobot(config.Player1Type) || !PlayerTypeIsRobot(config.Player2Type))) {
    qInfo("Error: player type 'human' is not allow in background mode.");
    exit(EXIT_FAILURE);
  }
  LogInfo(config);

  QWidget* mainWindow = new MainWindow(config.Player1Type, config.Player2Type, config.BackgroundMode, nullptr);
  mainWindow->show();
  return application.exec();
}

QCommandLineOption CommandParser::PlayerTypeOption(int player) {
  QStringList accepted;
  accepted << "human" << "robot";
  for (size_t i = 1; i < std::size(PlayerTypeOptionStrings); ++i) {
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

  return QCommandLineOption(
      QStringList() << QString("p%1").arg(player) << QString("player%1").arg(player),
      QString("Set type of player %1. Accepts: %2 (default: 'robot'). Note: 'robot' is equivalent to '%3'.")
          .arg(player)
          .arg(acceptedStr)
          .arg(QString::fromUtf8(PlayerTypeOptionStrings[static_cast<int>(DefaultPlayerType)])),
      "Type",
      "robot");
}

QCommandLineOption CommandParser::BackgroundModeOption() {
  return QCommandLineOption(QStringList() << "b" << "background", "Run in background mode.");
}

PlayerType CommandParser::ParsePlayerType(const QString& arg) {
  if (arg.compare("robot", Qt::CaseInsensitive) == 0) {
    return DefaultPlayerType;
  }
  for (size_t i = 0; i < std::size(PlayerTypeOptionStrings); ++i) {
    if (arg.compare(PlayerTypeOptionStrings[i], Qt::CaseInsensitive) == 0) {
      return static_cast<PlayerType>(i);
    }
  }
  qInfo("Error: Invalid player type '%s'.", arg.toLocal8Bit().constData());
  exit(EXIT_FAILURE);
}

}  // namespace dab::detail::frontend