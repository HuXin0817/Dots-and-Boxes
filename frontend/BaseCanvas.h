#pragma once

#include <QApplication>
#include <QStyleHints>
#include <QWidget>

#include "../src/Board.h"

namespace dab::__detail__::frontend {

class BaseCanvas : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(BaseCanvas)

 public:
  using QWidget::QWidget;

  static bool IsDarkTheme() { return QApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark; }

  static QColor ThemeColor(const QColor& darkThemeColor, const QColor& lightThemeColor) {
    return IsDarkTheme() ? darkThemeColor : lightThemeColor;
  }

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