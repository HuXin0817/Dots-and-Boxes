#pragma once

#include <functional>

#include "BaseCanvas.h"

namespace dab::__detail__::frontend {

class EdgeCanvas : public BaseCanvas {
  Q_OBJECT
  Q_DISABLE_COPY(EdgeCanvas)

 public:
  static constexpr int Width = UnitSize * 2;
  static constexpr int Height = Width * 5;

  EdgeCanvas(Edge edge, std::function<void(Edge)> callback, QWidget* parent);

 protected:
  void mousePressEvent(QMouseEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

 private:
  Edge Value;
  std::function<void(Edge)> Callback;

  QColor Color() const;
};

}  // namespace dab::__detail__::frontend