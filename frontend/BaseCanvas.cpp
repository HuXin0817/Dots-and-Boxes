#include "BaseCanvas.h"

#include <QApplication>
#include <QPainter>
#include <QStyleHints>
#include <QWidget>

#include "../src/Board.h"

namespace dab::__detail__::frontend {

QColor BaseCanvas::ThemeColor(const QColor& DarkThemeColor, const QColor& LightThemeColor) {
  switch (QApplication::styleHints()->colorScheme()) {
    case Qt::ColorScheme::Light:
      return LightThemeColor;
    case Qt::ColorScheme::Dark:
      return DarkThemeColor;
    case Qt::ColorScheme::Unknown:
      break;
  }
  return {};
}

const GameBoard& BaseCanvas::GetBoard() const {
  QObject* mainWindow = parent();
  Q_ASSERT(mainWindow != nullptr);
  const QVariant value = mainWindow->property("Board");
  Q_ASSERT(value.isValid());
  bool ok;
  const qulonglong num = value.toULongLong(&ok);
  Q_ASSERT(ok);
  const GameBoard* board = reinterpret_cast<GameBoard*>(num);
  Q_ASSERT(board != nullptr);
  return *board;
}

void BaseCanvas::enterEvent(QEnterEvent* event) {
  QWidget::enterEvent(event);
  HoverState = true;
  update();
}

void BaseCanvas::leaveEvent(QEvent* event) {
  QWidget::leaveEvent(event);
  HoverState = false;
  update();
}

}  // namespace dab::__detail__::frontend