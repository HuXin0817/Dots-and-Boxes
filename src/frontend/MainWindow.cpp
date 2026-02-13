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

#include "MainWindow.h"

#include <Dab/Robot.h>

#include <QJsonObject>
#include <QMessageBox>
#include <QPainter>
#include <QPointer>
#include <QPushButton>
#include <QTime>
#include <QTimer>
#include <atomic>
#include <ranges>

#include "BaseCanvas.h"
#include "BoxCanvas.h"
#include "DotCanvas.h"
#include "EdgeCanvas.h"

namespace dab::__detail__::frontend {

MainWindow::MainWindow(PlayerType player1Type, PlayerType player2Type, QWidget* parent)
    : BaseCanvas(parent), Player1Type(player1Type), Player2Type(player2Type), PlayerMoveEdge(Edge::Invalid) {
  if (PlayerTypeIsRobot(Player1Type)) {
    Robot1.reset(CreateRobot(Player1Type));
  }
  if (PlayerTypeIsRobot(Player2Type)) {
    Robot2.reset(CreateRobot(Player2Type));
  }

  resize(WindowSize, WindowSize);
  setMinimumSize(WindowSize, WindowSize);

  BoxCanvases.reserve(Box::Max);
  for (const Box box : std::views::iota(0, Box::Max)) {
    BoxCanvases.emplace_back(new BoxCanvas(this));
  }

  EdgeCanvases.reserve(Edge::Max);
  for (const Edge edge : std::views::iota(0, Edge::Max)) {
    EdgeCanvases.emplace_back(new EdgeCanvas(edge.Rotate(), SetPlayerMoveEdgeFunc(edge), this));
  }

  DotCanvases.reserve(Dot::Max);
  for (const Dot dot : std::views::iota(0, Dot::Max)) {
    DotCanvases.emplace_back(new DotCanvas(this));
  }
}

void MainWindow::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.fillRect(rect(), Color());
}

void MainWindow::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);

  const int x0 = (width() - BoardWidth) / 2 - UnitSize;
  const int y0 = (height() - BoardWidth) / 2 - UnitSize;

  for (const Box box : std::views::iota(0, Box::Max)) {
    const int x = x0 + box.X() * EdgeCanvas::Height + 2 * UnitSize;
    const int y = y0 + box.Y() * EdgeCanvas::Height + 2 * UnitSize;
    BoxCanvases[box]->move(x, y);
  }

  for (const Edge edge : std::views::iota(0, Edge::Max)) {
    int x = x0 + edge.Dot1().X() * EdgeCanvas::Height;
    int y = y0 + edge.Dot1().Y() * EdgeCanvas::Height;
    if (edge.Rotate()) {
      y += UnitSize;
    } else {
      x += UnitSize;
    }
    EdgeCanvases[edge]->move(x, y);
  }

  for (const Dot dot : std::views::iota(0, Dot::Max)) {
    const int x = x0 + dot.X() * EdgeCanvas::Height;
    const int y = y0 + dot.Y() * EdgeCanvas::Height;
    DotCanvases[dot]->move(x, y);
  }
}

void MainWindow::showEvent(QShowEvent* event) {
  QWidget::showEvent(event);
  AsyncRun();
}

QColor MainWindow::Color() const {
  static constexpr QColor DarkThemeColor = QColor(43, 43, 43, 255);
  static constexpr QColor LightThemeColor = QColor(242, 242, 242, 255);

  return ThemeColor(DarkThemeColor, LightThemeColor);
}

QRunnable* MainWindow::SetPlayerMoveEdgeFunc(Edge edge) {
  return QRunnable::create([edge, this]() -> void {
    if (Board.Contains(edge)) {
      return;
    }
    if (PlayerTypeIsRobot(Player1Type) && Board.IsPlayer1Turn()) {
      return;
    }
    if (PlayerTypeIsRobot(Player2Type) && Board.IsPlayer2Turn()) {
      return;
    }
    Edge expected = Edge::Invalid;
    PlayerMoveEdge.compare_exchange_strong(expected, edge);
  });
}

void MainWindow::Run() {
  Board.Reset();
  Random Random;
  while (Board.Gaming()) {
    if (PlayerTypeIsRobot(Player1Type) && Board.IsPlayer1Turn()) {
      PlayerMoveEdge = Random.Choice(Robot1->BestCandidateEdges(Board));
    } else if (PlayerTypeIsRobot(Player2Type) && Board.IsPlayer2Turn()) {
      PlayerMoveEdge = Random.Choice(Robot2->BestCandidateEdges(Board));
    } else {
      PlayerMoveEdge = Edge::Invalid;
      while (PlayerMoveEdge.load() == Edge::Invalid) {
        QThread::yieldCurrentThread();
      }
    }
    Assert(Board.NotContains(PlayerMoveEdge.load()));
    QMetaObject::invokeMethod(this, &MainWindow::Add, Qt::BlockingQueuedConnection);
    Assert(Board.Contains(PlayerMoveEdge.load()));
  }
  QMetaObject::invokeMethod(this, &MainWindow::HandleGameOver, Qt::BlockingQueuedConnection);
}

void MainWindow::Add() {
  const Edge edge = PlayerMoveEdge.load();
  if (Board.NowStep() > 0) {
    EdgeCanvases[LastEdge]->SetHighLight(false);
  }
  LastEdge = edge;
  EdgeCanvases[edge]->SetOwner(static_cast<Turn>(Board));
  EdgeCanvases[edge]->raise();
  DotCanvases[edge.Dot1()]->raise();
  DotCanvases[edge.Dot2()]->raise();
  for (const Box box : edge.NearBoxes()) {
    if (Board.EdgeCount(box) == 3) {
      BoxCanvases[box]->SetOwner(static_cast<Turn>(Board));
    }
  }
  const Turn turn = static_cast<Turn>(Board);
  Board.Add(edge);
  update();
  QApplication::beep();
}

void MainWindow::Restart() {
  Board.Reset();
  for (const Edge edge : std::views::iota(0, Edge::Max)) {
    EdgeCanvases[edge]->SetHighLight(false);
    EdgeCanvases[edge]->SetOwner(Owner::None);
  }
  for (const Box box : std::views::iota(0, Box::Max)) {
    BoxCanvases[box]->SetOwner(Owner::None);
  }
  update();
  AsyncRun();
}

void MainWindow::HandleGameOver() {
  const QPointer messagebox = new QMessageBox(this);
  if (Board.RelativeScore() > 0) {
    messagebox->setText(QString("Blue Team Win! (Score %1:%2)").arg(Board.Player1Score()).arg(Board.Player2Score()));
  } else if (Board.RelativeScore() < 0) {
    messagebox->setText(QString("Red Team Win! (Score %1:%2)").arg(Board.Player1Score()).arg(Board.Player2Score()));
  } else {
    messagebox->setText("Draw!");
  }
  messagebox->setIcon(QMessageBox::Information);
  const QPointer restartButton = messagebox->addButton(QMessageBox::Reset);
  restartButton->setText("Restart");
  connect(restartButton, &QPushButton::pressed, this, &MainWindow::Restart);
  const QPointer closeButton = messagebox->addButton(QMessageBox::Close);
  connect(closeButton, &QPushButton::pressed, this, &MainWindow::close);
  messagebox->exec();
}

}  // namespace dab::__detail__::frontend