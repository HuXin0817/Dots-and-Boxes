#include "BaseCanvas.h"

#include <QApplication>
#include <QPainter>
#include <QWidget>

#include "../src/Board.h"

namespace dab::__detail__::frontend {

const GameBoard& BaseCanvas::GetBoard() const {
  QObject* mainWindow = parent();
  Q_ASSERT(mainWindow != nullptr);
  const QVariant value = mainWindow->property("Board");
  Q_ASSERT(value.isValid());
  bool ok = false;
  const std::uintptr_t num = value.toULongLong(&ok);
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