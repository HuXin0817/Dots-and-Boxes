#pragma once

#include <QApplication>
#include <QPainter>
#include <QPalette>
#include <QPointer>
#include <QStyleHints>
#include <QWidget>

namespace dab::detail::frontend {

template <int64_t BoardSize>
static constexpr int UnitSize = 6 + 16 / BoardSize;

inline QColor
ThemeColor(const QColor& DarkThemeColor, const QColor& LightThemeColor) {
  if (QApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark) {
    return DarkThemeColor;
  } else {
    return LightThemeColor;
  }
}

}  // namespace dab::detail::frontend