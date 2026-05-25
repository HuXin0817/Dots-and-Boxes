#include "MainWindow.h"

#include <QEasingCurve>
#include <QJsonObject>
#include <QKeySequence>
#include <QMessageBox>
#include <QPainter>
#include <QParallelAnimationGroup>
#include <QPointer>
#include <QPushButton>
#include <QShortcut>
#include <QThreadPool>
#include <chrono>
#include <print>

#include "../src/Robot.h"
#include "BoxCanvas.h"
#include "Common.h"
#include "DotCanvas.h"
#include "EdgeCanvas.h"

namespace dab::__detail__::frontend {

MainWindow::MainWindow(PlayerType player1Type, PlayerType player2Type, QWidget* parent)
    : QMainWindow(parent), Player1Type(player1Type), Player2Type(player2Type) {
  Robot1 = Robot::Create(Player1Type);
  Robot2 = Robot::Create(Player2Type);
  Env = std::unique_ptr<GlobalEnv>();

  for (const Box box : Iota<Box>()) {
    BoxCanvases.At(box) = new BoxCanvas(Env.get(), box, this);
  }
  std::function<void(Edge)> callback = [&](Edge edge) -> void { SetPlayerMoveEdge(edge); };
  for (const Edge edge : Iota<Edge>()) {
    EdgeCanvases.At(edge) = new EdgeCanvas(Env.get(), edge, callback, this);
  }
  for (const Dot dot : Iota<Dot>()) {
    DotCanvases.At(dot) = new DotCanvas(Env.get(), dot, this);
  }
  Resize();

  QPointer<QShortcut> scZoomOut = new QShortcut(QKeySequence::ZoomOut, this);
  connect(scZoomOut, &QShortcut::activated, this, [&]() -> void {
    Env->ReduceUnitSize();
    Resize();
  });

  QPointer<QShortcut> scZoomIn = new QShortcut(QKeySequence(Qt::Modifier::CTRL | Qt::Key::Key_Equal), this);
  connect(scZoomIn, &QShortcut::activated, this, [&]() -> void {
    Env->AddUnitSize();
    Resize();
  });

  QPointer<QShortcut> scRefresh = new QShortcut(QKeySequence::Refresh, this);
  connect(scRefresh, &QShortcut::activated, this, [&]() -> void {
    Env->ResetUnitSize();
    Resize();
  });
}

void MainWindow::paintEvent(QPaintEvent* event) {
  QMainWindow::paintEvent(event);

  QPainter painter(this);
  painter.fillRect(rect(), Color());
}

void MainWindow::resizeEvent(QResizeEvent* event) {
  QMainWindow::resizeEvent(event);
  Move();
}

void MainWindow::showEvent(QShowEvent* event) {
  QWidget::showEvent(event);
  AsyncRun();
}

void MainWindow::SetPlayerMoveEdge(Edge edge) {
  if (Env->GetBoard().Contains(edge)) {
    return;
  }
  if (PlayerTypeIsRobot(Player1Type) && Env->GetBoard().IsPlayer1Turn()) {
    return;
  }
  if (PlayerTypeIsRobot(Player2Type) && Env->GetBoard().IsPlayer2Turn()) {
    return;
  }
  PlayerMoveEdge = edge;
}

void MainWindow::Run() {
  Env->GetBoard().Reset();
  Random random;
  while (Env->GetBoard().Gaming()) {
    if (PlayerTypeIsRobot(Player1Type) && Env->GetBoard().IsPlayer1Turn()) {
      Span<const Edge> candidateEdges = Robot1->BestCandidateEdges(Env->GetBoard());
      PlayerMoveEdge = random.Choice(candidateEdges);
    } else if (PlayerTypeIsRobot(Player2Type) && Env->GetBoard().IsPlayer2Turn()) {
      Span<const Edge> candidateEdges = Robot2->BestCandidateEdges(Env->GetBoard());
      PlayerMoveEdge = random.Choice(candidateEdges);
    } else {
      PlayerMoveEdge = Edge::Invalid;
      while (PlayerMoveEdge == Edge::Invalid) {
        QThread::yieldCurrentThread();
      }
    }
    QMetaObject::invokeMethod(this, &MainWindow::Add, Qt::ConnectionType::BlockingQueuedConnection);
  }
  QMetaObject::invokeMethod(this, &MainWindow::HandleGameOver, Qt::ConnectionType::BlockingQueuedConnection);
}

void MainWindow::AsyncRun() {
  QThreadPool::globalInstance()->start([&]() -> void { Run(); });
}

void MainWindow::Add() {
  Int step = Env->GetBoard().NowStep();
  int player = Env->GetBoard().IsPlayer1Turn() ? 1 : 2;
  Int move = PlayerMoveEdge;
  EdgeCanvases.At(PlayerMoveEdge)->raise();
  DotCanvases.At(PlayerMoveEdge.Dot1())->raise();
  DotCanvases.At(PlayerMoveEdge.Dot2())->raise();
  Env->GetBoard().Add(PlayerMoveEdge);
  std::println(R"({:%Y-%m-%d %H:%M:%S} {{"Step":{},"Player":{},"Move":{},"Score":{{"Player1":{},"Player2":{}}}}})",
               std::chrono::system_clock::now(),
               step,
               player,
               move,
               Env->GetBoard().Player1Score(),
               Env->GetBoard().Player2Score());
  update();
  QApplication::beep();
}

void MainWindow::Resize() {
  for (const Box box : Iota<Box>()) {
    BoxCanvases.At(box)->Resize();
  }

  for (const Edge edge : Iota<Edge>()) {
    EdgeCanvases.At(edge)->Resize();
  }

  for (const Dot dot : Iota<Dot>()) {
    DotCanvases.At(dot)->Resize();
  }

  int windowSize = WindowSize(Env->GetUnitSize());
  setFixedSize(windowSize, windowSize);
}

QPointer<QPropertyAnimation> MainWindow::CreatePosAnimation(QWidget* widget, int x, int y) {
  QPointer<QPropertyAnimation> posAnimation = new QPropertyAnimation(widget, "pos");
  posAnimation->setDuration(500);
  posAnimation->setStartValue(widget->pos());
  posAnimation->setEndValue(QPoint(x, y));
  posAnimation->setEasingCurve(QEasingCurve::OutQuad);
  return posAnimation;
}

void MainWindow::Move() {
  QPointer<QParallelAnimationGroup> animationGroup = new QParallelAnimationGroup(this);

  const int x0 = (width() - BoardWidth(Env->GetUnitSize())) / 2 - Env->GetUnitSize();
  const int y0 = (height() - BoardWidth(Env->GetUnitSize())) / 2 - Env->GetUnitSize();

  for (QPointer<BoxCanvas> canvas : BoxCanvases) {
    const int x = x0 + canvas->GetValue().X() * EdgeCanvas::Height(Env->GetUnitSize()) + 2 * Env->GetUnitSize();
    const int y = y0 + canvas->GetValue().Y() * EdgeCanvas::Height(Env->GetUnitSize()) + 2 * Env->GetUnitSize();
    animationGroup->addAnimation(CreatePosAnimation(canvas, x, y));
  }

  for (QPointer<EdgeCanvas> canvas : EdgeCanvases) {
    int x = x0 + canvas->GetValue().Dot1().X() * EdgeCanvas::Height(Env->GetUnitSize());
    int y = y0 + canvas->GetValue().Dot1().Y() * EdgeCanvas::Height(Env->GetUnitSize());
    if (canvas->GetValue().Rotate()) {
      y += Env->GetUnitSize();
    } else {
      x += Env->GetUnitSize();
    }
    animationGroup->addAnimation(CreatePosAnimation(canvas, x, y));
  }

  for (QPointer<DotCanvas> canvas : DotCanvases) {
    const int x = x0 + canvas->GetValue().X() * EdgeCanvas::Height(Env->GetUnitSize());
    const int y = y0 + canvas->GetValue().Y() * EdgeCanvas::Height(Env->GetUnitSize());
    animationGroup->addAnimation(CreatePosAnimation(canvas, x, y));
  }

  animationGroup->start();
}

void MainWindow::HandleGameOver() {
  QPointer<QMessageBox> messagebox = new QMessageBox(this);
  if (Env->GetBoard().RelativeScore() > 0) {
    messagebox->setText(QString("Blue Team Win! (Score %1:%2)")
                            .arg(Env->GetBoard().Player1Score())
                            .arg(Env->GetBoard().Player2Score()));
  } else if (Env->GetBoard().RelativeScore() < 0) {
    messagebox->setText(
        QString("Red Team Win! (Score %1:%2)").arg(Env->GetBoard().Player1Score()).arg(Env->GetBoard().Player2Score()));
  } else {
    messagebox->setText("Draw!");
  }
  messagebox->setIcon(QMessageBox::Information);
  QPointer<QPushButton> restartButton = messagebox->addButton(QMessageBox::Reset);
  restartButton->setText("Restart");
  connect(restartButton, &QPushButton::pressed, this, &MainWindow::AsyncRun);
  QPointer<QPushButton> closeButton = messagebox->addButton(QMessageBox::Close);
  connect(closeButton, &QPushButton::pressed, this, &MainWindow::close);
  messagebox->exec();
}

}  // namespace dab::__detail__::frontend