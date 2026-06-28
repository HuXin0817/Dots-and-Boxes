#pragma once

#include "Env.h"

namespace dab::__detail__::frontend {

class DotCanvas : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(DotCanvas)

 public:
  DotCanvas(Env* env, model::Dot dot, QWidget* parent) : Env(env), Value(dot), QWidget(parent) {}

  QSize Size() const { return {Env->DotDiameter(), Env->DotDiameter()}; }
  QPoint Pos() const;

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  Env* Env;
  model::Dot Value;
};

}  // namespace dab::__detail__::frontend