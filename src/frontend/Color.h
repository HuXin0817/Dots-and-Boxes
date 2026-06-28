#pragma once

#include <cstdint>
#include <initializer_list>

#include "../Board.h"

namespace dab::__detail__::frontend {

static constexpr std::initializer_list<bool> boolList = {false, true};
static constexpr std::initializer_list<board::Owner> ownerList = {
    board::Owner::None,
    board::Owner::Player1,
    board::Owner::Player2,
};

struct Color {
  std::uint8_t r, g, b, a;
};

static constexpr Color BackgroundColorDark = {43, 43, 43, 255};
static constexpr Color BackgroundColorLight = {242, 242, 242, 255};
static constexpr Color DotColorDark = {202, 202, 202, 255};
static constexpr Color DotColorLight = {255, 255, 255, 255};
static constexpr Color BoxPlayer1Color = {64, 64, 255, 64};
static constexpr Color BoxPlayer2Color = {255, 64, 64, 64};
static constexpr Color EdgeHoveredColorDark = {90, 90, 90, 255};
static constexpr Color EdgeHoveredColorLight = {202, 202, 202, 255};
static constexpr Color EdgeColorDark = {65, 65, 65, 255};
static constexpr Color EdgeColorLight = {217, 217, 217, 255};
static constexpr Color EdgePlayer1Color = {64, 64, 255, 255};
static constexpr Color EdgePlayer2Color = {255, 64, 64, 255};

static constexpr int EdgeHighlightAlpha = 255;
static constexpr int EdgeHoveredAlpha = 144;
static constexpr int EdgeDefaultAlpha = 128;

static constexpr Color BackgroundColor(bool isDark) { return isDark ? BackgroundColorDark : BackgroundColorLight; }

static constexpr iterable::Array<Color, 2> CreateBackgroundColorMap() {
  iterable::Array<Color, 2> mapper;
  for (bool isDark : boolList) {
    mapper.At(isDark) = BackgroundColor(isDark);
  }
  return mapper;
}

static constexpr Color DotColor(bool isDark) { return isDark ? DotColorDark : DotColorLight; }

static constexpr iterable::Array<Color, 2> CreateDotColorMap() {
  iterable::Array<Color, 2> mapper;
  for (bool isDark : boolList) {
    mapper.At(isDark) = DotColor(isDark);
  }
  return mapper;
}

static constexpr Color BoxColor(bool isDark, board::Owner owner) {
  if (owner == board::Owner::Player1) {
    return BoxPlayer1Color;
  } else if (owner == board::Owner::Player2) {
    return BoxPlayer2Color;
  } else {
    return BackgroundColor(isDark);
  }
}

static constexpr iterable::Array<iterable::Array<Color, 3>, 2> CreateBoxColorMap() {
  iterable::Array<iterable::Array<Color, 3>, 2> mapper;
  for (bool isDark : boolList) {
    for (board::Owner owner : ownerList) {
      mapper.At(isDark).At(static_cast<Int>(owner)) = BoxColor(isDark, owner);
    }
  }
  return mapper;
}

static constexpr Color EdgeColor(bool isDark, bool hovered, bool highlight, board::Owner owner) {
  if (owner == board::Owner::None) {
    if (hovered) {
      return isDark ? EdgeHoveredColorDark : EdgeHoveredColorLight;
    }

    return isDark ? EdgeColorDark : EdgeColorLight;
  }

  Color color;
  if (owner == board::Owner::Player1) {
    color = EdgePlayer1Color;
  } else if (owner == board::Owner::Player2) {
    color = EdgePlayer2Color;
  }

  if (highlight) {
    color.a = EdgeHighlightAlpha;
  } else if (hovered) {
    color.a = EdgeHoveredAlpha;
  } else {
    color.a = EdgeDefaultAlpha;
  }

  return color;
}

static constexpr iterable::Array<iterable::Array<iterable::Array<iterable::Array<Color, 3>, 2>, 2>, 2>
CreateEdgeColorMap() {
  iterable::Array<iterable::Array<iterable::Array<iterable::Array<Color, 3>, 2>, 2>, 2> mapper;
  for (bool isDark : boolList) {
    for (bool hovered : boolList) {
      for (bool highlight : boolList) {
        for (board::Owner owner : ownerList) {
          mapper.At(isDark).At(hovered).At(highlight).At(static_cast<Int>(owner)) =
              EdgeColor(isDark, hovered, highlight, owner);
        }
      }
    }
  }
  return mapper;
}

}  // namespace dab::__detail__::frontend
