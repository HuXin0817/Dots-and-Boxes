#pragma once

#include <QApplication>
#include <QWidget>

#include "../src/Board.h"

namespace dab::__detail__::frontend {

class BaseCanvas : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(BaseCanvas)

 public:
  using QWidget::QWidget;

  static QColor ThemeColor(const QColor& darkThemeColor, const QColor& lightThemeColor);
  bool Hovered() const { return HoverState; }

 protected:
  const GameBoard& GetBoard() const;
  void enterEvent(QEnterEvent* event) override;
  void leaveEvent(QEvent* event) override;

  static constexpr int DefaultUnitSize = 6 + 16 / BoardSize;
  static inline int UnitSize = DefaultUnitSize;

 private:
  bool HoverState = false;
};

}  // namespace dab::__detail__::frontend