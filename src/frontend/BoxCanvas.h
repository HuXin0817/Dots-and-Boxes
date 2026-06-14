#pragma once

#include <QColor>
#include <QWidget>

#include "GlobalEnv.h"

namespace dab::__detail__::frontend {

class BoxCanvas : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(BoxCanvas)

 public:
  static constexpr QColor Player1OccupyColor = QColor(64, 64, 255, 64);
  static constexpr QColor Player2OccupyColor = QColor(255, 64, 64, 64);

  BoxCanvas(GlobalEnv* env, model::Box box, QWidget* parent) : Env(env), Value(box), QWidget(parent) {}

  QSize Size() const { return MakeSize(Env->BoxWidth()); }
  QPoint Pos() const { return MakePos(Value, Env->BoxPadding(), Env->EdgeWidth()); }
  QColor Color() const;

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  GlobalEnv* Env;
  model::Box Value;
};

}  // namespace dab::__detail__::frontend