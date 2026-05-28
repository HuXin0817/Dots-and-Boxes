#pragma once

#include "GlobalEnv.h"

namespace dab::__detail__::frontend {

class DotCanvas : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(DotCanvas)

 public:
  static constexpr QColor DarkThemeColor = QColor(202, 202, 202, 255);
  static constexpr QColor LightThemeColor = QColor(255, 255, 255, 255);

  static int Width(int unitSize) { return 2 * unitSize; }

  DotCanvas(GlobalEnv* env, Dot dot, QWidget* parent) : Env(env), Value(dot), QWidget(parent) {}

  Dot GetValue() const { return Value; }

 public Q_SLOTS:
  void Resize() { setFixedSize(Width(Env->GetUnitSize()), Width(Env->GetUnitSize())); }

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  GlobalEnv* Env;
  Dot Value;

  QColor Color() const { return Env->ThemeColor(DarkThemeColor, LightThemeColor); }
};

}  // namespace dab::__detail__::frontend