#pragma once

#include "Env.h"

namespace dab::__detail__::frontend {

class DotCanvas : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(DotCanvas)

 public:
  static constexpr QColor DarkThemeColor = QColor(202, 202, 202, 255);
  static constexpr QColor LightThemeColor = QColor(255, 255, 255, 255);

  DotCanvas(Env* env, model::Dot dot, QWidget* parent) : Env(env), Value(dot), QWidget(parent) {}

  QSize Size() const { return {Env->DotDiameter(), Env->DotDiameter()}; }
  QPoint Pos() const;
  QColor Color() const { return Env->ThemeColor(DarkThemeColor, LightThemeColor); }

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  Env* Env;
  model::Dot Value;
};

}  // namespace dab::__detail__::frontend