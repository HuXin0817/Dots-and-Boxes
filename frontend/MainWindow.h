#pragma once

#include <QMainWindow>
#include <QPointer>
#include <QPropertyAnimation>

#include "../src/Robot/Robot.h"
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
  GlobalEnv Env;
  model::Edge CandidateEdge;
  iterable::Array<QPointer<BoxCanvas>, model::Box::Max> BoxCanvases;
  iterable::Array<QPointer<DotCanvas>, model::Dot::Max> DotCanvases;
  iterable::Array<QPointer<EdgeCanvas>, model::Edge::Max> EdgeCanvases;

  static constexpr int kAnimationDuration = 500;
  template <typename Canvas>
  QPointer<QPropertyAnimation> CreateSizeAnimation(Canvas canvas);
  template <typename Canvas>
  QPointer<QPropertyAnimation> CreatePosAnimation(Canvas canvas);
};

template <typename Canvas>
QPointer<QPropertyAnimation> MainWindow::CreateSizeAnimation(Canvas canvas) {
  QPointer<QPropertyAnimation> sizeAnimation = new QPropertyAnimation(canvas, "size");
  sizeAnimation->setDuration(kAnimationDuration);
  sizeAnimation->setStartValue(canvas->size());
  sizeAnimation->setEndValue(canvas->Size());
  sizeAnimation->setEasingCurve(QEasingCurve::OutQuad);
  return sizeAnimation;
}

template <typename Canvas>
QPointer<QPropertyAnimation> MainWindow::CreatePosAnimation(Canvas canvas) {
  QPointer<QPropertyAnimation> posAnimation = new QPropertyAnimation(canvas, "pos");
  posAnimation->setDuration(kAnimationDuration);
  posAnimation->setStartValue(canvas->pos());
  posAnimation->setEndValue(canvas->Pos());
  posAnimation->setEasingCurve(QEasingCurve::OutQuad);
  return posAnimation;
}

}  // namespace dab::__detail__::frontend