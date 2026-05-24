#pragma once

#include "BaseCanvas.h"

namespace dab::__detail__::frontend {

class DotCanvas : public BaseCanvas {
  Q_OBJECT
  Q_DISABLE_COPY(DotCanvas)

 public:
  static constexpr QColor DarkThemeColor = QColor(202, 202, 202, 255);
  static constexpr QColor LightThemeColor = QColor(255, 255, 255, 255);

  static int Width() { return 2 * UnitSize; }

  DotCanvas(QWidget* parent) : BaseCanvas(parent) { Resize(); }

 public Q_SLOTS:
  void Resize() { setFixedSize(Width(), Width()); }

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  QColor Color() const { return ThemeColor(DarkThemeColor, LightThemeColor); }
};

}  // namespace dab::__detail__::frontend