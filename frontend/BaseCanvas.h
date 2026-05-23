#pragma once

#include <QApplication>
#include <QWidget>

#include "../src/Board.h"

namespace dab::__detail__::frontend {

class BaseCanvas : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(BaseCanvas)

 public:
  static constexpr int UnitSize = 6 + 16 / BoardSize;

  using QWidget::QWidget;

  static QColor ThemeColor(const QColor& darkThemeColor, const QColor& lightThemeColor);
  bool Hovered() const { return HoverState; }

 protected:
  const GameBoard& GetBoard() const;
  void enterEvent(QEnterEvent* event) override;
  void leaveEvent(QEvent* event) override;

 private:
  bool HoverState = false;
};

}  // namespace dab::__detail__::frontend