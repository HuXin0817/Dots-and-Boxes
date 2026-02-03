#pragma once

#include <Dab/Model.hpp>
#include <QApplication>
#include <QPainter>
#include <QPalette>
#include <QPointer>
#include <QStyleHints>
#include <QWidget>

namespace dab::detail::frontend {

template <int64_t BoardSize>
class BaseCanvas : public QWidget {
  public:
  using QWidget::QWidget;

  static constexpr int UnitSize = 6 + 16 / BoardSize;

  static QColor
  ThemeColor(const QColor& DarkThemeColor, const QColor& LightThemeColor);
  bool
  Hovered() const;

  std::optional<Turn> Owner;

  protected:
  void
  enterEvent(QEnterEvent* event) override;
  void
  leaveEvent(QEvent* event) override;

  private:
  bool HoverState = false;
};

template <int64_t BoardSize>
QColor
BaseCanvas<BoardSize>::ThemeColor(const QColor& DarkThemeColor, const QColor& LightThemeColor) {
  if (QApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark) {
    return DarkThemeColor;
  } else {
    return LightThemeColor;
  }
}

template <int64_t BoardSize>
bool
BaseCanvas<BoardSize>::Hovered() const {
  return HoverState;
}

template <int64_t BoardSize>
void
BaseCanvas<BoardSize>::enterEvent(QEnterEvent* event) {
  QWidget::enterEvent(event);

  HoverState = true;
  update();
}

template <int64_t BoardSize>
void
BaseCanvas<BoardSize>::leaveEvent(QEvent* event) {
  QWidget::leaveEvent(event);

  HoverState = false;
  update();
}

}  // namespace dab::detail::frontend