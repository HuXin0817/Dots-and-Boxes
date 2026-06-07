#pragma once

#include <functional>

#include "GlobalEnv.h"

namespace dab::__detail__::frontend {

class EdgeCanvas : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(EdgeCanvas)

 public:
  static constexpr QColor DarkThemeColor = QColor(65, 65, 65, 255);
  static constexpr QColor LightThemeColor = QColor(217, 217, 217, 255);
  static constexpr QColor DarkThemeHoveredColor = QColor(90, 90, 90, 255);
  static constexpr QColor LightThemeHoveredColor = QColor(202, 202, 202, 255);
  static constexpr QColor Player1OccupyColor = QColor(64, 64, 255, 255);
  static constexpr QColor Player2OccupyColor = QColor(255, 64, 64, 255);

  EdgeCanvas(GlobalEnv* env, model::Edge edge, QWidget* parent) : Env(env), Value(edge), QWidget(parent) {}

  QSize Size() const;
  QPoint Pos() const;
  QColor Color() const;

 protected:
  void mousePressEvent(QMouseEvent* event) override;
  void paintEvent(QPaintEvent* event) override;
  void enterEvent(QEnterEvent* event) override;
  void leaveEvent(QEvent* event) override;

 private:
  GlobalEnv* Env;
  model::Edge Value;
  bool Hovered = false;
};

}  // namespace dab::__detail__::frontend