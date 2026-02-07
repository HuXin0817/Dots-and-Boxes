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

#include "MainWindow.hpp"

#include <Dab/Robot.hpp>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPointer>
#include <QThreadPool>
#include <QTime>
#include <QTimer>
#include <atomic>
#include <cassert>

#include "BoxCanvas.hpp"
#include "Dab/Model.hpp"
#include "DotCanvas.hpp"
#include "EdgeCanvas.hpp"

namespace dab::detail::frontend {

MainWindow::MainWindow(PlayerType player1Type, PlayerType player2Type, bool backgroundMode, QWidget* parent)
    : BaseCanvas(parent), Player1Type(player1Type), Player2Type(player2Type), BackgroundMode(backgroundMode) {
  if (PlayerTypeIsRobot(Player1Type)) {
    Robot1.reset(CreateRobot(Player1Type));
  }
  if (PlayerTypeIsRobot(Player2Type)) {
    Robot2.reset(CreateRobot(Player2Type));
  }
  if (backgroundMode) {
    assert(Robot1);
    assert(Robot2);
    Run();
  } else {
    resize(WindowSize, WindowSize);
    setMinimumSize(WindowSize, WindowSize);

    BoxCanvases.reserve(Box::Max);
    for (Box box = 0; box < Box::Max; ++box) {
      BoxCanvases.emplace_back(new BoxCanvas(this));
    }

    EdgeCanvases.reserve(Edge::Max);
    for (Edge edge = 0; edge < Edge::Max; ++edge) {
      EdgeCanvases.emplace_back(
          new EdgeCanvas(edge.Rotate(), [edge, this]() -> void { SetPlayerMoveEdge(edge); }, this));
    }

    DotCanvases.reserve(Dot::Max);
    for (Dot dot = 0; dot < Dot::Max; ++dot) {
      DotCanvases.emplace_back(new DotCanvas(this));
    }
  }
}

void MainWindow::Run() {
  Random Random;
  while (Board.Gaming()) {
    const QTime startTime = QTime::currentTime();
    const Turn turn = static_cast<Turn>(Board);

    if (PlayerTypeIsRobot(Player1Type) && Board.IsPlayer1Turn()) {
      PlayerMoveEdge = Random.Choice(Robot1->BestCandidateEdges(Board));
    } else if (PlayerTypeIsRobot(Player2Type) && Board.IsPlayer2Turn()) {
      PlayerMoveEdge = Random.Choice(Robot2->BestCandidateEdges(Board));
    } else {
      PlayerMoveEdge = InvalidEdge;
      while (PlayerMoveEdge.load() == InvalidEdge) {
        QThread::yieldCurrentThread();
      }
    }

    assert(Board.NotContains(PlayerMoveEdge.load()));
    if (BackgroundMode) {
      Add(PlayerMoveEdge.load());
    } else {
      QMetaObject::invokeMethod(this, [this]() -> void { Add(PlayerMoveEdge.load()); }, Qt::BlockingQueuedConnection);
    }
    assert(Board.Contains(PlayerMoveEdge.load()));

    const double seconds = static_cast<double>(startTime.msecsTo(QTime::currentTime())) / 1000.0;

    QJsonObject playerScore;
    playerScore.insert("Player1", Board.Player1Score());
    playerScore.insert("Player2", Board.Player2Score());

    QJsonObject moveRecord;
    moveRecord.insert("Step", Board.NowStep());
    moveRecord.insert("Turn", turn.IsPlayer1Turn() ? 1 : 2);
    moveRecord.insert("Move", Int(PlayerMoveEdge.load()));
    moveRecord.insert("Score", playerScore);
    moveRecord.insert("Time", seconds);

    QJsonObject info;
    info.insert("Info", moveRecord);
    qInfo() << QJsonDocument(info).toJson(QJsonDocument::Compact).constData();
  }

  QJsonObject winner;
  if (Board.RelativeScore() > 0) {
    winner.insert("Winner", "Player1");
  } else if (Board.RelativeScore() < 0) {
    winner.insert("Winner", "Player2");
  } else {
    winner.insert("Winner", "Draw");
  }
  qInfo() << QJsonDocument(winner).toJson(QJsonDocument::Compact).constData();

  if (BackgroundMode) {
    exit(0);
  } else {
    QMetaObject::invokeMethod(
        this,
        [this]() -> void {
          QTimer::singleShot(2000, this, [this]() -> void {
            EdgeCanvases[LastEdge]->SetHighLight(false);
            update();
            QTimer::singleShot(2000, this, &MainWindow::close);
          });
        },
        Qt::BlockingQueuedConnection);
  }
}

void MainWindow::paintEvent(QPaintEvent* event) {
  paintEvent(event);

  QPainter painter(this);
  painter.fillRect(rect(), Color());
}

void MainWindow::resizeEvent(QResizeEvent* event) {
  resizeEvent(event);

  const int x0 = (width() - BoardWidth) / 2 - UnitSize;
  const int y0 = (height() - BoardWidth) / 2 - UnitSize;

  for (Box box = 0; box < Box::Max; ++box) {
    const int x = x0 + box.X() * EdgeCanvas::Height + 2 * UnitSize;
    const int y = y0 + box.Y() * EdgeCanvas::Height + 2 * UnitSize;
    BoxCanvases[box]->move(x, y);
  }

  for (Edge edge = 0; edge < Edge::Max; ++edge) {
    int x = x0 + edge.Dot1().X() * EdgeCanvas::Height;
    int y = y0 + edge.Dot1().Y() * EdgeCanvas::Height;
    if (edge.Rotate()) {
      y += UnitSize;
    } else {
      x += UnitSize;
    }
    EdgeCanvases[edge]->move(x, y);
  }

  for (Dot dot = 0; dot < Dot::Max; ++dot) {
    const int x = x0 + dot.X() * EdgeCanvas::Height;
    const int y = y0 + dot.Y() * EdgeCanvas::Height;
    DotCanvases[dot]->move(x, y);
  }
}

void MainWindow::showEvent(QShowEvent* event) {
  showEvent(event);

  QThreadPool::globalInstance()->start([this]() -> void { Run(); });
}

void MainWindow::SetPlayerMoveEdge(Edge edge) {
  if (Board.Contains(edge)) {
    return;
  }
  if (PlayerTypeIsRobot(Player1Type) && Board.IsPlayer1Turn()) {
    return;
  }
  if (PlayerTypeIsRobot(Player2Type) && Board.IsPlayer2Turn()) {
    return;
  }
  Edge expected = InvalidEdge;
  PlayerMoveEdge.compare_exchange_strong(expected, edge);
}

QColor MainWindow::Color() const {
  static constexpr QColor DarkThemeColor = QColor(43, 43, 43, 255);
  static constexpr QColor LightThemeColor = QColor(242, 242, 242, 255);

  return ThemeColor(DarkThemeColor, LightThemeColor);
}

void MainWindow::Add(Edge edge) {
  if (!BackgroundMode) {
    if (Board.NowStep() > 0) {
      EdgeCanvases[LastEdge]->SetHighLight(false);
    }
    EdgeCanvases[edge]->SetOwner(Board);
    EdgeCanvases[edge]->raise();
    for (Dot dot = 0; dot < Dot::Max; ++dot) {
      DotCanvases[dot]->raise();
    }

    for (const Box box : NearBoxes(edge)) {
      int count = 0;
      for (const Edge nearEdge : NearEdges(box)) {
        if (Board.Contains(nearEdge)) {
          count++;
        }
      }
      if (count == 3) {
        BoxCanvases[box]->SetOwner(Board);
      }
    }
    LastEdge = edge;
  }

  Board.Add(edge);
  if (!BackgroundMode) {
    update();
    QApplication::beep();
  }
}

}  // namespace dab::detail::frontend