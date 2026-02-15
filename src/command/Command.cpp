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

#include "Command.h"

#include <Dab/Tools.h>
#include <Dab/Version.h>

#include <QApplication>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QJsonObject>
#include <cstdlib>
#include <optional>
#include <ranges>
#include <type_traits>

#include "../extern/CreateMainWindow.h"
#include "../extern/MockRunningGame.h"

using namespace dab::internal;

namespace dab::command {

static constexpr int64_t DefaultBoardSize = __DefaultBoardSize__;
static constexpr int64_t MaxBoardSize = __MaxBoardSize__;
static_assert(DefaultBoardSize <= MaxBoardSize);

template <int64_t BoardSize, typename FuncNameTag, typename... Args>
  requires(BoardSize >= 0)
constexpr auto DispatchImpl(int64_t boardSize, Args&&... args) {
  using ReturnType = decltype(FuncNameTag::template Call<BoardSize>(std::forward<Args>(args)...));
  static constexpr bool HaveReturnValue = !std::is_void_v<ReturnType>;
  if constexpr (BoardSize == 0) {
    std::unreachable();
    if constexpr (HaveReturnValue) {
      return ReturnType{};
    }
  } else {
    if (boardSize < BoardSize) {
      if constexpr (HaveReturnValue) {
        return DispatchImpl<BoardSize - 1, FuncNameTag>(boardSize, std::forward<Args>(args)...);
      } else {
        DispatchImpl<BoardSize - 1, FuncNameTag>(boardSize, std::forward<Args>(args)...);
        return;
      }
    }
    auto call = FuncNameTag::template Call<BoardSize>;
    if constexpr (HaveReturnValue) {
      return call(std::forward<Args>(args)...);
    } else {
      call(std::forward<Args>(args)...);
      return;
    }
  }
}

template <typename FuncNameTag, typename... Args>
constexpr auto Dispatch(int64_t boardSize, Args&&... args) {
  return DispatchImpl<MaxBoardSize, FuncNameTag>(boardSize, std::forward<Args>(args)...);
}

QCommandLineOption CreateBoardSizeOption() {
  const QByteArray boardSizeEnv = qgetenv("BOARD_SIZE");
  QString defaultBoardSize = QString::number(DefaultBoardSize);
  if (!boardSizeEnv.isEmpty()) {
    defaultBoardSize = boardSizeEnv;
  }
  return QCommandLineOption(QStringList() << "s" << "boardsize",
                            QString("Set board size (1-%1).").arg(MaxBoardSize),
                            "size",
                            defaultBoardSize);
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

  return QCommandLineOption(
      QStringList() << QString("p%1").arg(playerid) << QString("player%1").arg(playerid),
      QString("Set type of player %1. Accepts: %2 (default: 'robot'). Note: 'robot' is equivalent to '%3'.")
          .arg(playerid)
          .arg(acceptedStr)
          .arg(QString::fromUtf8(PlayerTypeOptionStrings[DefaultPlayerType])),
      "type",
      defaultPlayerType);
}

std::optional<int> ParsePlayerType(const QString& arg) {
  if (arg.compare("robot", Qt::CaseInsensitive) == 0) {
    return DefaultPlayerType;
  }
  for (const size_t i : std::views::iota(0ull, std::size(PlayerTypeOptionStrings))) {
    if (arg.compare(PlayerTypeOptionStrings[i], Qt::CaseInsensitive) == 0) {
      return static_cast<int>(i);
    }
  }
  LogError("Invalid player type '{}'.", arg.toLocal8Bit().constData());
  return std::nullopt;
}

std::optional<int64_t> ParseBoardSize(const QString& arg) {
  bool ok = false;
  const int64_t boardSize = arg.toLongLong(&ok);
  if (!ok || boardSize < 1 || boardSize > MaxBoardSize) {
    LogError("Invalid board size '{}'. Must be between 1 and {}.", arg.toLocal8Bit().constData(), MaxBoardSize);
    return std::nullopt;
  }
  return boardSize;
}

int Process(int argc, char* argv[]) {
  QApplication application(argc, argv);
  application.setApplicationName("Dots and Boxes");
  application.setApplicationVersion(Version);
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

  const std::optional<int64_t> boardSize = ParseBoardSize(parser.value(boardSizeOption));
  if (!boardSize.has_value()) {
    return EXIT_FAILURE;
  }
  const std::optional<int> player1Type = ParsePlayerType(parser.value(player1Option));
  if (!player1Type.has_value()) {
    return EXIT_FAILURE;
  }
  const std::optional<int> player2Type = ParsePlayerType(parser.value(player2Option));
  if (!player2Type.has_value()) {
    return EXIT_FAILURE;
  }
  if (parser.isSet(backgroundModeOption)) {
    Dispatch<MockRunningGame>(boardSize.value(), player1Type.value(), player2Type.value());
    return 0;
  }

  QWidget* mainWindow =
      Dispatch<CreateMainWindow>(boardSize.value(), player1Type.value(), player2Type.value(), nullptr);
  mainWindow->show();
  return application.exec();
}

}  // namespace dab::command