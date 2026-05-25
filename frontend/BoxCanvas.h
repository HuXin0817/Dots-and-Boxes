#pragma once

#include "EdgeCanvas.h"

namespace dab::__detail__::frontend {

class BoxCanvas : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(BoxCanvas)

 public:
  static constexpr QColor Player1OccupyColor = QColor(64, 64, 255, 64);
  static constexpr QColor Player2OccupyColor = QColor(255, 64, 64, 64);

  static int Width(int unitSize) { return EdgeCanvas::Height(unitSize) - 2 * unitSize; }

  BoxCanvas(const GlobalEnv* env, Box box, QWidget* parent) : Env(env), Value(box), QWidget(parent) {}

  Box GetValue() const { return Value; }

 public Q_SLOTS:
  void Resize() { setFixedSize(Width(Env->GetUnitSize()), Width(Env->GetUnitSize())); }

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  const GlobalEnv* Env;
  Box Value;

  QColor Color() const;
};

}  // namespace dab::__detail__::frontend