#pragma once

#include <QApplication>
#include <QColor>
#include <QWidget>

#include "../../src/model/ScoreMap.hpp"

class BaseCanvas : public QWidget {
  public:
  static constexpr int UnitSize = 8;

  explicit BaseCanvas(QWidget* parent) : QWidget(parent) {
  }

  virtual QColor
  Color() const {
    return {};
  }

  enum class CanvasState {
    Free,
    Player1Occupy,
    Player2Occupy,
  };

  static CanvasState
  StateFromTurn(bool Turn) {
    return Turn == Player1Turn ? CanvasState::Player1Occupy : CanvasState::Player2Occupy;
  }

  static bool
  isDarkTheme() {
    QPalette palette = QApplication::palette();
    QColor windowColor = palette.color(QPalette::Window);

    return windowColor.lightness() < 128;
  }
};
