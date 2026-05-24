#pragma once

#include "BaseCanvas.h"
#include "EdgeCanvas.h"

namespace dab::__detail__::frontend {

class BoxCanvas : public BaseCanvas {
  Q_OBJECT
  Q_DISABLE_COPY(BoxCanvas)

 public:
  static constexpr QColor Player1OccupyColor = QColor(64, 64, 255, 64);
  static constexpr QColor Player2OccupyColor = QColor(255, 64, 64, 64);

  static int Width() { return EdgeCanvas::Height() - 2 * UnitSize; }

  BoxCanvas(Box box, QWidget* parent) : BaseCanvas(parent), Value(box) { Resize(); }

 public Q_SLOTS:
  void Resize() { setFixedSize(Width(), Width()); }

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  Box Value;

  QColor Color() const;
};

}  // namespace dab::__detail__::frontend