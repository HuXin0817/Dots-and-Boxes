#pragma once

#include <QApplication>
#include <QStyleHints>
#include <QWidget>

#include "../src/Board.h"

namespace dab::__detail__::frontend {

inline bool IsDarkTheme() { return QApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark; }

inline QColor ThemeColor(const QColor& darkThemeColor, const QColor& lightThemeColor) {
  return IsDarkTheme() ? darkThemeColor : lightThemeColor;
}

class GlobalEnv {
 public:
  int GetUnitSize() const { return UnitSize; }

  void ResetUnitSize() { UnitSize = DefaultUnitSize; }

  void AddUnitSize() { UnitSize = std::max<int>(UnitSize + 1, static_cast<double>(UnitSize) * 1.1); }

  void ReduceUnitSize() {
    UnitSize = std::min<int>(UnitSize - 1, static_cast<double>(UnitSize) * 0.9);
    if (UnitSize <= 0) {
      UnitSize = 1;
      QApplication::beep();
    }
  }

  const GameBoard& GetBoard() const { return Board; }

  GameBoard& GetBoard() { return Board; }

 private:
  static constexpr int DefaultUnitSize = 6 + 16 / BoardSize;
  int UnitSize = DefaultUnitSize;

  GameBoard Board;
};

}  // namespace dab::__detail__::frontend