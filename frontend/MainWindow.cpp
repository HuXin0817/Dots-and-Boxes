#include "MainWindow.h"

#include <QJsonObject>
#include <QMessageBox>
#include <QPainter>
#include <QPointer>
#include <QPushButton>
#include <QShortcut>
#include <QThreadPool>
#include <chrono>
#include <print>

#include "../src/Robot.h"
#include "BaseCanvas.h"
#include "BoxCanvas.h"
#include "DotCanvas.h"
#include "EdgeCanvas.h"

namespace dab::__detail__::frontend {

MainWindow::MainWindow(PlayerType player1Type, PlayerType player2Type, QWidget* parent)
    : BaseCanvas(parent), Player1Type(player1Type), Player2Type(player2Type) {
  Robot1 = Robot::Create(Player1Type);
  Robot2 = Robot::Create(Player2Type);

  setProperty("Board", QVariant::fromValue(reinterpret_cast<std::uintptr_t>(&Board)));
  for (const Box box : Iota<Box>()) {
    BoxCanvases.At(box) = new BoxCanvas(box, this);
  }
  std::function<void(Edge)> callback = [this](Edge edge) -> void { SetPlayerMoveEdge(edge); };
  for (const Edge edge : Iota<Edge>()) {
    EdgeCanvases.At(edge) = new EdgeCanvas(edge, callback, this);
  }
  for (QPointer<DotCanvas>& dotCanvas : DotCanvases) {
    dotCanvas = new DotCanvas(this);
  }
  Resize();

  QPointer<QShortcut> scZoomOut = new QShortcut(QKeySequence::ZoomOut, this);
  connect(scZoomOut, &QShortcut::activated, this, [&]() -> void {
    UnitSize = std::min<int>(UnitSize - 1, static_cast<double>(UnitSize) * 0.9);
    if (UnitSize <= 0) {
      UnitSize = 1;
      QApplication::beep();
    }
    Resize();
  });

  QPointer<QShortcut> scZoomIn = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Equal), this);
  connect(scZoomIn, &QShortcut::activated, this, [&]() -> void {
    UnitSize = std::max<int>(UnitSize + 1, static_cast<double>(UnitSize) * 1.1);
    Resize();
  });

  QPointer<QShortcut> scRefresh = new QShortcut(QKeySequence::Refresh, this);
  connect(scRefresh, &QShortcut::activated, this, [&]() -> void {
    UnitSize = DefaultUnitSize;
    Resize();
  });
}

void MainWindow::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.fillRect(rect(), Color());
}

void MainWindow::resizeEvent(QResizeEvent* event) {
  BaseCanvas::resizeEvent(event);
  Move();
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
  PlayerMoveEdge = edge;
}

void MainWindow::Run() {
  Board.Reset();
  Random random;
  while (Board.Gaming()) {
    if (PlayerTypeIsRobot(Player1Type) && Board.IsPlayer1Turn()) {
      Span<const Edge> candidateEdges = Robot1->BestCandidateEdges(Board);
      PlayerMoveEdge = random.Choice(candidateEdges);
    } else if (PlayerTypeIsRobot(Player2Type) && Board.IsPlayer2Turn()) {
      Span<const Edge> candidateEdges = Robot2->BestCandidateEdges(Board);
      PlayerMoveEdge = random.Choice(candidateEdges);
    } else {
      PlayerMoveEdge = Edge::Invalid;
      while (PlayerMoveEdge == Edge::Invalid) {
        QThread::yieldCurrentThread();
      }
    }
    QMetaObject::invokeMethod(this, &MainWindow::Add, Qt::BlockingQueuedConnection);
  }
  QMetaObject::invokeMethod(this, &MainWindow::HandleGameOver, Qt::BlockingQueuedConnection);
}

void MainWindow::AsyncRun() {
  QThreadPool::globalInstance()->start([&]() -> void { Run(); });
}

void MainWindow::Add() {
  Int step = Board.NowStep();
  int player = Board.IsPlayer1Turn() ? 1 : 2;
  Int move = PlayerMoveEdge;
  EdgeCanvases.At(PlayerMoveEdge)->raise();
  DotCanvases.At(PlayerMoveEdge.Dot1())->raise();
  DotCanvases.At(PlayerMoveEdge.Dot2())->raise();
  Board.Add(PlayerMoveEdge);
  std::println(R"({:%Y-%m-%d %H:%M:%S} {{"Step":{},"Player":{},"Move":{},"Score":{{"Player1":{},"Player2":{}}}}})",
               std::chrono::system_clock::now(),
               step,
               player,
               move,
               Board.Player1Score(),
               Board.Player2Score());
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

  if ((windowState() & Qt::WindowFullScreen) == 0) {
    QSize size(WindowSize(), WindowSize());
    setMinimumSize(size);
    resize(size);
  }

  Move();
}

void MainWindow::Move() {
  const int x0 = (width() - BoardWidth()) / 2 - UnitSize;
  const int y0 = (height() - BoardWidth()) / 2 - UnitSize;

  for (const Box box : Iota<Box>()) {
    const int x = x0 + box.X() * EdgeCanvas::Height() + 2 * UnitSize;
    const int y = y0 + box.Y() * EdgeCanvas::Height() + 2 * UnitSize;
    BoxCanvases.At(box)->move(x, y);
  }

  for (const Edge edge : Iota<Edge>()) {
    int x = x0 + edge.Dot1().X() * EdgeCanvas::Height();
    int y = y0 + edge.Dot1().Y() * EdgeCanvas::Height();
    if (edge.Rotate()) {
      y += UnitSize;
    } else {
      x += UnitSize;
    }
    EdgeCanvases.At(edge)->move(x, y);
  }

  for (const Dot dot : Iota<Dot>()) {
    const int x = x0 + dot.X() * EdgeCanvas::Height();
    const int y = y0 + dot.Y() * EdgeCanvas::Height();
    DotCanvases.At(dot)->move(x, y);
  }
}

void MainWindow::HandleGameOver() {
  const QPointer<QMessageBox> messagebox = new QMessageBox(this);
  if (Board.RelativeScore() > 0) {
    messagebox->setText(QString("Blue Team Win! (Score %1:%2)").arg(Board.Player1Score()).arg(Board.Player2Score()));
  } else if (Board.RelativeScore() < 0) {
    messagebox->setText(QString("Red Team Win! (Score %1:%2)").arg(Board.Player1Score()).arg(Board.Player2Score()));
  } else {
    messagebox->setText("Draw!");
  }
  messagebox->setIcon(QMessageBox::Information);
  const QPointer<QPushButton> restartButton = messagebox->addButton(QMessageBox::Reset);
  restartButton->setText("Restart");
  connect(restartButton, &QPushButton::pressed, this, &MainWindow::AsyncRun);
  const QPointer<QPushButton> closeButton = messagebox->addButton(QMessageBox::Close);
  connect(closeButton, &QPushButton::pressed, this, &MainWindow::close);
  messagebox->exec();
}

}  // namespace dab::__detail__::frontend