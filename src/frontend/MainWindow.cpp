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

#include "../robot/Robot.h"
#include "BoxCanvas.h"
#include "DotCanvas.h"
#include "EdgeCanvas.h"
#include "GlobalEnv.h"

namespace dab::__detail__::frontend {

MainWindow::MainWindow(PlayerType player1Type, PlayerType player2Type) {
  Env.SetPlayer1Type(player1Type);
  Env.SetPlayer2Type(player2Type);

  Robot1 = robot::Robot::Create(player1Type);
  Robot2 = robot::Robot::Create(player2Type);

  for (const model::Box box : model::Iota<model::Box>()) {
    BoxCanvases.At(box) = new BoxCanvas(&Env, box, this);
  }
  for (const model::Edge edge : model::Iota<model::Edge>()) {
    EdgeCanvases.At(edge) = new EdgeCanvas(&Env, edge, this);
  }
  for (const model::Dot dot : model::Iota<model::Dot>()) {
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
  EdgeCanvases.At(CandidateEdge)->raise();
  DotCanvases.At(CandidateEdge.Dot1())->raise();
  DotCanvases.At(CandidateEdge.Dot2())->raise();
  Env.GetBoard().Add(CandidateEdge);
  update();
  QApplication::beep();
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