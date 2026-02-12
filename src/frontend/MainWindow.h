/*
Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2025 Xin Hu <202219120810@stu.cdut.edu.cn>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#pragma once

#include <Dab/Robot.h>

#include <QPointer>
#include <QRunnable>
#include <QTime>
#include <atomic>

#include "BoxCanvas.h"
#include "DotCanvas.h"
#include "EdgeCanvas.h"

namespace dab::__detail__::frontend {

class MainWindow : public BaseCanvas {
  Q_OBJECT

  static constexpr int BoardWidth = BoardSize * EdgeCanvas::Height;
  static constexpr int WindowSize = BoardWidth + 2 * BoxCanvas::Width;

 public:
  explicit MainWindow(PlayerType player1Type, PlayerType player2Type, QWidget* parent);

 protected:
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
  void showEvent(QShowEvent* event) override;

 private:
  const PlayerType Player1Type;
  const PlayerType Player2Type;
  QScopedPointer<Robot> Robot1;
  QScopedPointer<Robot> Robot2;
  std::atomic<Edge> PlayerMoveEdge;
  Edge LastEdge;
  AbsoluteScoreBoard Board;
  QList<QPointer<BoxCanvas>> BoxCanvases;
  QList<QPointer<DotCanvas>> DotCanvases;
  QList<QPointer<EdgeCanvas>> EdgeCanvases;
  std::once_flag FirstRun;
  QTime LastUpdateTime;

  QColor Color() const;
  QRunnable* SetPlayerMoveEdgeFunc(Edge edge);

 public Q_SLOTS:
  void Run();
  void Add();
  void Restart();
  void HandleGameOver();
};

}  // namespace dab::__detail__::frontend