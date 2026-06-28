#pragma once

#include <QMainWindow>
#include <QPointer>
#include <QPropertyAnimation>

#include "../robot/Robot.h"
#include "BoxCanvas.h"
#include "DotCanvas.h"
#include "EdgeCanvas.h"
#include "Env.h"

namespace dab::__detail__::frontend {

class MainWindow : public QMainWindow {
  Q_OBJECT
  Q_DISABLE_COPY(MainWindow)

 public:
  static constexpr QColor DarkThemeColor = QColor(43, 43, 43, 255);
  static constexpr QColor LightThemeColor = QColor(242, 242, 242, 255);

  MainWindow(PlayerType player1Type, PlayerType player2Type);

  QSize Size() const { return {Env.WindowSize(), Env.WindowSize()}; }
  QColor Color() const { return Env.ThemeColor(DarkThemeColor, LightThemeColor); }

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
  std::unique_ptr<robot::Robot> Robot1 = nullptr;
  std::unique_ptr<robot::Robot> Robot2 = nullptr;
  Env Env;
  model::Edge CandidateEdge;
  iterable::Array<QPointer<BoxCanvas>, model::Box::Max> BoxCanvases;
  iterable::Array<QPointer<DotCanvas>, model::Dot::Max> DotCanvases;
  iterable::Array<QPointer<EdgeCanvas>, model::Edge::Max> EdgeCanvases;
};

}  // namespace dab::__detail__::frontend