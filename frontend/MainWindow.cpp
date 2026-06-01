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
#include "DotCanvas.h"
#include "EdgeCanvas.h"
#include "GlobalEnv.h"

namespace dab::__detail__::frontend {

MainWindow::MainWindow(PlayerType player1Type, PlayerType player2Type) {
  Env.SetPlayer1Type(player1Type);
  Env.SetPlayer2Type(player2Type);

  Robot1 = Robot::Create(player1Type);
  Robot2 = Robot::Create(player2Type);

  for (const Box box : Iota<Box>()) {
    BoxCanvases.At(box) = new BoxCanvas(&Env, box, this);
  }
  for (const Edge edge : Iota<Edge>()) {
    EdgeCanvases.At(edge) = new EdgeCanvas(&Env, edge, this);
  }
  for (const Dot dot : Iota<Dot>()) {
    DotCanvases.At(dot) = new DotCanvas(&Env, dot, this);
  }
  Resize();

  QPointer<QShortcut> scZoomOut = new QShortcut(QKeySequence::ZoomOut, this);
  connect(scZoomOut, &QShortcut::activated, this, [&]() -> void {
    Env.ReduceUnitSize();
    Resize();
  });

  QPointer<QShortcut> scZoomIn = new QShortcut(QKeySequence(Qt::Modifier::CTRL | Qt::Key::Key_Equal), this);
  connect(scZoomIn, &QShortcut::activated, this, [&]() -> void {
    Env.AddUnitSize();
    Resize();
  });

  QPointer<QShortcut> scRefresh = new QShortcut(QKeySequence::Refresh, this);
  connect(scRefresh, &QShortcut::activated, this, [&]() -> void {
    Env.ResetUnitSize();
    Resize();
  });
}

void MainWindow::paintEvent(QPaintEvent* event) {
  QMainWindow::paintEvent(event);
  QPainter painter(this);
  painter.fillRect(rect(), Color());
}

void MainWindow::showEvent(QShowEvent* event) {
  QWidget::showEvent(event);
  AsyncRun();
}

void MainWindow::Run() {
  Env.GetBoard().Reset();
  while (Env.GetBoard().Gaming()) {
    if (PlayerTypeIsRobot(Env.GetPlayer1Type()) && Env.GetBoard().IsPlayer1Turn()) {
      CandidateEdge = Robot1->Move(Env.GetBoard());
    } else if (PlayerTypeIsRobot(Env.GetPlayer2Type()) && Env.GetBoard().IsPlayer2Turn()) {
      CandidateEdge = Robot2->Move(Env.GetBoard());
    } else {
      CandidateEdge = Env.GetHumanMoveEdgeSync();
    }
    QMetaObject::invokeMethod(this, &MainWindow::Add, Qt::ConnectionType::BlockingQueuedConnection);
  }
  QMetaObject::invokeMethod(this, &MainWindow::HandleGameOver, Qt::ConnectionType::BlockingQueuedConnection);
}

void MainWindow::AsyncRun() {
  QThreadPool::globalInstance()->start([&]() -> void { Run(); });
}

void MainWindow::Add() {
  Int step = Env.GetBoard().NowStep();
  int player = Env.GetBoard().IsPlayer1Turn() ? 1 : 2;
  Int move = CandidateEdge;
  EdgeCanvases.At(CandidateEdge)->raise();
  DotCanvases.At(CandidateEdge.Dot1())->raise();
  DotCanvases.At(CandidateEdge.Dot2())->raise();
  Env.GetBoard().Add(CandidateEdge);
  std::println(R"({:%Y-%m-%d %H:%M:%S} {{"Step":{},"Player":{},"Move":{},"Score":{{"Player1":{},"Player2":{}}}}})",
               std::chrono::system_clock::now(),
               step,
               player,
               move,
               Env.GetBoard().Player1Score(),
               Env.GetBoard().Player2Score());
  update();
  QApplication::beep();
}

template <typename Canvas>
QPointer<QPropertyAnimation> CreateSizeAnimation(Canvas canvas) {
  QPointer<QPropertyAnimation> sizeAnimation = new QPropertyAnimation(canvas, "size");
  sizeAnimation->setDuration(500);
  sizeAnimation->setStartValue(canvas->size());
  sizeAnimation->setEndValue(canvas->Size());
  sizeAnimation->setEasingCurve(QEasingCurve::OutQuad);
  return sizeAnimation;
}

template <typename Canvas>
QPointer<QPropertyAnimation> CreatePosAnimation(Canvas canvas) {
  QPointer<QPropertyAnimation> posAnimation = new QPropertyAnimation(canvas, "pos");
  posAnimation->setDuration(500);
  posAnimation->setStartValue(canvas->pos());
  posAnimation->setEndValue(canvas->Pos());
  posAnimation->setEasingCurve(QEasingCurve::OutQuad);
  return posAnimation;
}

void MainWindow::Resize() {
  QPointer<QParallelAnimationGroup> animationGroup = new QParallelAnimationGroup(this);
  for (QPointer<BoxCanvas> canvas : BoxCanvases) {
    animationGroup->addAnimation(CreateSizeAnimation(canvas));
    animationGroup->addAnimation(CreatePosAnimation(canvas));
  }
  for (QPointer<EdgeCanvas> canvas : EdgeCanvases) {
    animationGroup->addAnimation(CreateSizeAnimation(canvas));
    animationGroup->addAnimation(CreatePosAnimation(canvas));
  }
  for (QPointer<DotCanvas> canvas : DotCanvases) {
    animationGroup->addAnimation(CreateSizeAnimation(canvas));
    animationGroup->addAnimation(CreatePosAnimation(canvas));
  }
  setFixedSize(Size());
  animationGroup->start();
}

void MainWindow::HandleGameOver() {
  QPointer<QMessageBox> messagebox = new QMessageBox(this);
  if (Env.GetBoard().RelativeScore() > 0) {
    messagebox->setText(
        QString("Blue Team Win! (Score %1:%2)").arg(Env.GetBoard().Player1Score()).arg(Env.GetBoard().Player2Score()));
  } else if (Env.GetBoard().RelativeScore() < 0) {
    messagebox->setText(
        QString("Red Team Win! (Score %1:%2)").arg(Env.GetBoard().Player1Score()).arg(Env.GetBoard().Player2Score()));
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