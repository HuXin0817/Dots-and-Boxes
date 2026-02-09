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

#include <QApplication>
#include <QCommandLineParser>
#include <QJsonObject>
#include <QLibrary>
#include <QWidget>
#include <cstdlib>

static constexpr int DefaultPlayerType = 5;

static constexpr const char* PlayerTypeOptionStrings[] = {
    "human",
    "robot:easy",
    "robot:medium",
    "robot:hard",
    "robot:expert",
    "robot:master",
};

bool PlayerTypeIsRobot(int playerType) { return playerType > 0; }

int ParsePlayerType(const QString& arg) {
  if (arg.compare("robot", Qt::CaseInsensitive) == 0) {
    return DefaultPlayerType;
  }
  for (size_t i = 0; i < std::size(PlayerTypeOptionStrings); ++i) {
    if (arg.compare(PlayerTypeOptionStrings[i], Qt::CaseInsensitive) == 0) {
      return static_cast<int>(i);
    }
  }
  qInfo("Error: Invalid player type '%s'.", arg.toLocal8Bit().constData());
  exit(EXIT_FAILURE);
}

int ParseBoardSize(const QString& arg) {
  bool ok = false;
  int boardSize = arg.toInt(&ok);
  if (!ok || boardSize < 1 || boardSize > MaxBoardSize) {
    qInfo("Error: Invalid board size '%s'. Must be between 1 and %d.", arg.toLocal8Bit().constData(), MaxBoardSize);
    exit(EXIT_FAILURE);
  }
  return boardSize;
}

int main(int argc, char* argv[]) {
  QApplication application(argc, argv);
  application.setApplicationName("Dots and Boxes");
  application.setApplicationVersion("1.0.0");
  application.setOrganizationName("Dots and Boxes");

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

  const QCommandLineOption boardSizeOption(
      QStringList() << "s" << "boardsize", QString("Set board size (1-%1).").arg(MaxBoardSize), "size", "6");

  const QCommandLineOption player1Option(
      QStringList() << "p1" << "player1",
      QString("Set type of player 1. Accepts: %1 (default: 'robot'). Note: 'robot' is equivalent to '%2'.")
          .arg(acceptedStr)
          .arg(QString::fromUtf8(PlayerTypeOptionStrings[DefaultPlayerType])),
      "Type",
      "robot");

  const QCommandLineOption player2Option(
      QStringList() << "p2" << "player2",
      QString("Set type of player 2. Accepts: %1 (default: 'robot'). Note: 'robot' is equivalent to '%2'.")
          .arg(acceptedStr)
          .arg(QString::fromUtf8(PlayerTypeOptionStrings[DefaultPlayerType])),
      "Type",
      "robot");

  const QCommandLineOption backgroundModeOption(QStringList() << "b" << "background", "Run in background mode.");

  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addOption(boardSizeOption);
  parser.addOption(player1Option);
  parser.addOption(player2Option);
  parser.addOption(backgroundModeOption);
  parser.process(application);

  int boardSize = ParseBoardSize(parser.value(boardSizeOption));
  int player1Type = ParsePlayerType(parser.value(player1Option));
  int player2Type = ParsePlayerType(parser.value(player2Option));
  bool backgroundMode = parser.isSet(backgroundModeOption);
  if (backgroundMode && (!PlayerTypeIsRobot(player1Type) || !PlayerTypeIsRobot(player2Type))) {
    qInfo("Error: player type 'human' is not allow in background mode.");
    exit(EXIT_FAILURE);
  }

  QLibrary library(QString("Dots_and_Boxes_%1x%1").arg(boardSize));
  if (!library.load()) {
    qInfo("Error: Failed to load library %s: %s",
          library.fileName().toLocal8Bit().constData(),
          library.errorString().toLocal8Bit().constData());
    exit(EXIT_FAILURE);
  }

  typedef QWidget* (*CreateMainWindowFunc)(int, int, bool, QWidget*);
  CreateMainWindowFunc createMainWindow = reinterpret_cast<CreateMainWindowFunc>(library.resolve("CreateMainWindow"));
  if (!createMainWindow) {
    qInfo("Error: Failed to resolve CreateMainWindow function in library %s: %s",
          library.fileName().toLocal8Bit().constData(),
          library.errorString().toLocal8Bit().constData());
    exit(EXIT_FAILURE);
  }

  QWidget* mainWindow = createMainWindow(player1Type, player2Type, backgroundMode, nullptr);
  mainWindow->show();
  return application.exec();
}