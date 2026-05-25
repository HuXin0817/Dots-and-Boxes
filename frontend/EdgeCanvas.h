#pragma once

#include <functional>

#include "Common.h"

namespace dab::__detail__::frontend {

class EdgeCanvas : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(EdgeCanvas)

 public:
  static constexpr QColor DarkThemeColor = QColor(65, 65, 65, 255);
  static constexpr QColor LightThemeColor = QColor(217, 217, 217, 255);
  static constexpr QColor DarkThemeHoveredColor = QColor(90, 90, 90, 255);
  static constexpr QColor LightThemeHoveredColor = QColor(202, 202, 202, 255);
  static constexpr QColor Player1OccupyColor = QColor(64, 64, 255, 255);
  static constexpr QColor Player2OccupyColor = QColor(255, 64, 64, 255);

  static int Width(int unitSize) { return unitSize * 2; }
  static int Height(int unitSize) { return Width(unitSize) * 5; }

  EdgeCanvas(const GlobalEnv* env, Edge edge, std::function<void(Edge)> callback, QWidget* parent)
      : Env(env), Value(edge), Callback(std::move(callback)), QWidget(parent) {}

  Edge GetValue() const { return Value; }

 public Q_SLOTS:
  void Resize() {
    setFixedSize(Value.Rotate() ? Width(Env->GetUnitSize()) : Height(Env->GetUnitSize()),
                 Value.Rotate() ? Height(Env->GetUnitSize()) : Width(Env->GetUnitSize()));
  }

 protected:
  void mousePressEvent(QMouseEvent* event) override;
  void paintEvent(QPaintEvent* event) override;
  void enterEvent(QEnterEvent* event) override;
  void leaveEvent(QEvent* event) override;

 private:
  const GlobalEnv* Env;
  Edge Value;
  std::function<void(Edge)> Callback;
  bool Hovered = false;

  QColor Color() const;
};

}  // namespace dab::__detail__::frontend