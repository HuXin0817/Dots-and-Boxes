#pragma once

#include "BaseCanvas.h"

namespace dab::__detail__::frontend {

class DotCanvas : public BaseCanvas {
  Q_OBJECT
  Q_DISABLE_COPY(DotCanvas)

 public:
  static constexpr int Width = 2 * UnitSize;

  DotCanvas(QWidget* parent) : BaseCanvas(parent) { setFixedSize(Width, Width); }

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  QColor Color() const;
};

}  // namespace dab::__detail__::frontend