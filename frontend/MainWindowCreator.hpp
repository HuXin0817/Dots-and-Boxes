#pragma once

#include <cstdint>

#include "Config.hpp"
#include "MainWindow.hpp"

namespace dab::detail::frontend {

template <int64_t MaxBoardSize>
class MainWindowCreator {
 public:
  QWidget*
  CreateMainWindow(const Config& config, QWidget* parent = nullptr);

 private:
  template <int64_t BoardSize>
  QWidget*
  CreateMainWindowImpl(const Config& config, QWidget* parent);
};

template <int64_t MaxBoardSize>
QWidget*
MainWindowCreator<MaxBoardSize>::CreateMainWindow(const Config& config, QWidget* parent) {
  assert(config.BoardSize > 0 && config.BoardSize <= MaxBoardSize);
  return CreateMainWindowImpl<MaxBoardSize>(config, parent);
}

template <int64_t MaxBoardSize>
template <int64_t BoardSize>
QWidget*
MainWindowCreator<MaxBoardSize>::CreateMainWindowImpl(const Config& config, QWidget* parent) {
  if (config.BoardSize == BoardSize) {
    return new MainWindow<BoardSize>(config.Player1Type, config.Player2Type, config.BackgroundMode, parent);
  }
  if constexpr (BoardSize > 1) {
    return CreateMainWindowImpl<BoardSize - 1>(config, parent);
  }
  return nullptr;
}

}  // namespace dab::detail::frontend