#pragma once

#include <QPointer>

#include "../src/Robot.h"
#include "BoxCanvas.h"
#include "DotCanvas.h"
#include "EdgeCanvas.h"

namespace dab::__detail__::frontend {

class MainWindow : public BaseCanvas {
  Q_OBJECT
  Q_DISABLE_COPY(MainWindow)

  static constexpr int BoardWidth = BoardSize * EdgeCanvas::Height;
  static constexpr int WindowSize = BoardWidth + 2 * BoxCanvas::Width;

 public:
  MainWindow(PlayerType player1Type, PlayerType player2Type, QWidget* parent = nullptr);

 protected:
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
  void showEvent(QShowEvent* event) override;

 private:
  const PlayerType Player1Type;
  const PlayerType Player2Type;
  std::unique_ptr<Robot> Robot1;
  std::unique_ptr<Robot> Robot2;
  GameBoard Board;
  Edge PlayerMoveEdge;
  Array<QPointer<BoxCanvas>, Box::Max> BoxCanvases;
  Array<QPointer<DotCanvas>, Dot::Max> DotCanvases;
  Array<QPointer<EdgeCanvas>, Edge::Max> EdgeCanvases;

  QColor Color() const;
  void SetPlayerMoveEdge(Edge edge);

 public Q_SLOTS:
  void Run();
  void AsyncRun();
  void Add();
  void HandleGameOver();
};

}  // namespace dab::__detail__::frontend