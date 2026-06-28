#pragma once

#include <QColor>
#include <QWidget>

#include "Env.h"

namespace dab::__detail__::frontend {

class BoxCanvas : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(BoxCanvas)

 public:
  BoxCanvas(Env* env, model::Box box, QWidget* parent) : Env(env), Value(box), QWidget(parent) {}

  QSize Size() const { return {Env->BoxWidth(), Env->BoxWidth()}; }
  QPoint Pos() const;

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  Env* Env;
  model::Box Value;
};

}  // namespace dab::__detail__::frontend