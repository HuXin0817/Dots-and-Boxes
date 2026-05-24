#pragma once

#include <functional>

#include "BaseCanvas.h"

namespace dab::__detail__::frontend {

class EdgeCanvas : public BaseCanvas {
  Q_OBJECT
  Q_DISABLE_COPY(EdgeCanvas)

 public:
  static constexpr QColor DarkThemeColor = QColor(65, 65, 65, 255);
  static constexpr QColor LightThemeColor = QColor(217, 217, 217, 255);
  static constexpr QColor DarkThemeHoveredColor = QColor(90, 90, 90, 255);
  static constexpr QColor LightThemeHoveredColor = QColor(202, 202, 202, 255);
  static constexpr QColor Player1OccupyColor = QColor(64, 64, 255, 255);
  static constexpr QColor Player2OccupyColor = QColor(255, 64, 64, 255);

  static int Width() { return UnitSize * 2; }
  static int Height() { return Width() * 5; }

  EdgeCanvas(Edge edge, std::function<void(Edge)> callback, QWidget* parent)
      : BaseCanvas(parent), Value(edge), Callback(std::move(callback)) {
    Resize();
  }

 public Q_SLOTS:
  void Resize() { setFixedSize(Value.Rotate() ? Width() : Height(), Value.Rotate() ? Height() : Width()); }

 protected:
  void mousePressEvent(QMouseEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

 private:
  Edge Value;
  std::function<void(Edge)> Callback;

  QColor Color() const;
};

}  // namespace dab::__detail__::frontend