#pragma once

#include <QMainWindow>
#include <QPointer>
#include <QPropertyAnimation>

#include "../src/Robot.h"
#include "BoxCanvas.h"
#include "DotCanvas.h"
#include "EdgeCanvas.h"

namespace dab::__detail__::frontend {

class MainWindow : public QMainWindow {
  Q_OBJECT
  Q_DISABLE_COPY(MainWindow)

 public:
  static constexpr QColor DarkThemeColor = QColor(43, 43, 43, 255);
  static constexpr QColor LightThemeColor = QColor(242, 242, 242, 255);

  static int BoardWidth(int unitSize) { return BoardSize * EdgeCanvas::Height(unitSize); }
  static int WindowSize(int unitSize) { return BoardWidth(unitSize) + 2 * BoxCanvas::Width(unitSize); }
  static int StartOffset(int unitSize) { return (WindowSize(unitSize) - BoardWidth(unitSize)) / 2 - unitSize; }

  QSize Size() const { return {WindowSize(Env.GetUnitSize()), WindowSize(Env.GetUnitSize())}; }
  QColor Color() const { return Env.ThemeColor(DarkThemeColor, LightThemeColor); }

  MainWindow(PlayerType player1Type, PlayerType player2Type);

 public Q_SLOTS:
  void Run();
  void AsyncRun();
  void Add();
  void Resize();
  void HandleGameOver();

 protected:
  void paintEvent(QPaintEvent* event) override;
  void showEvent(QShowEvent* event) override;

 private:
  std::unique_ptr<Robot> Robot1 = nullptr;
  std::unique_ptr<Robot> Robot2 = nullptr;
  GlobalEnv Env;
  Edge CandidateEdge;
  Array<QPointer<BoxCanvas>, Box::Max> BoxCanvases;
  Array<QPointer<DotCanvas>, Dot::Max> DotCanvases;
  Array<QPointer<EdgeCanvas>, Edge::Max> EdgeCanvases;
};

}  // namespace dab::__detail__::frontend