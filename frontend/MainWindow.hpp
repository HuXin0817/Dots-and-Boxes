#pragma once

#include <Dab/Robot.hpp>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPointer>
#include <QThreadPool>
#include <QTime>
#include <QTimer>
#include <atomic>
#include <cassert>

#include "BoxCanvas.hpp"
#include "Dab/Model.hpp"
#include "DotCanvas.hpp"
#include "EdgeCanvas.hpp"

namespace dab::detail::frontend {

template <int64_t BoardSize>
class MainWindow final : public BaseCanvas<BoardSize> {
  static constexpr int BoardWidth = BoardSize * EdgeCanvas<BoardSize>::Height;
  static constexpr int WindowSize = BoardWidth + 2 * BoxCanvas<BoardSize>::Width;

  public:
  explicit MainWindow(const PlayerType player1Type,
                      const PlayerType player2Type,
                      const bool backgroundMode,
                      QPointer<QWidget> parent);
  void
  Run();

  protected:
  void
  paintEvent(QPaintEvent* event) override;
  void
  resizeEvent(QResizeEvent* event) override;
  void
  showEvent(QShowEvent* event) override;

  private:
  const PlayerType Player1Type;
  const PlayerType Player2Type;
  const bool BackgroundMode;
  QScopedPointer<Robot<BoardSize>> Robot1;
  QScopedPointer<Robot<BoardSize>> Robot2;
  std::atomic<Edge<BoardSize>> PlayerMoveEdge;
  Edge<BoardSize> LastEdge;
  AbsoluteScoreBoard<BoardSize> Board;
  QList<QPointer<BoxCanvas<BoardSize>>> BoxCanvases;
  QList<QPointer<DotCanvas<BoardSize>>> DotCanvases;
  QList<QPointer<EdgeCanvas<BoardSize>>> EdgeCanvases;

  void
  SetPlayerMoveEdge(const Edge<BoardSize> edge);
  QColor
  Color() const;
  void
  Add(const Edge<BoardSize> edge);
};

template <int64_t BoardSize>
MainWindow<BoardSize>::MainWindow(const PlayerType player1Type,
                                  const PlayerType player2Type,
                                  const bool backgroundMode,
                                  QPointer<QWidget> parent)
    : BaseCanvas<BoardSize>(parent),
      Player1Type(player1Type),
      Player2Type(player2Type),
      BackgroundMode(backgroundMode) {
  if (PlayerTypeIsRobot(Player1Type)) {
    Robot1.reset(CreateRobot<BoardSize>(Player1Type));
  }
  if (PlayerTypeIsRobot(Player2Type)) {
    Robot2.reset(CreateRobot<BoardSize>(Player2Type));
  }
  if (backgroundMode) {
    assert(Robot1);
    assert(Robot2);
    Run();
  } else {
    BaseCanvas<BoardSize>::resize(WindowSize, WindowSize);
    BaseCanvas<BoardSize>::setMinimumSize(WindowSize, WindowSize);

    BoxCanvases.reserve(Box<BoardSize>::Max);
    for (Box<BoardSize> box = 0; box < Box<BoardSize>::Max; ++box) {
      BoxCanvases.emplace_back(new BoxCanvas<BoardSize>(this));
    }

    EdgeCanvases.reserve(Edge<BoardSize>::Max);
    for (Edge<BoardSize> edge = 0; edge < Edge<BoardSize>::Max; ++edge) {
      EdgeCanvases.emplace_back(
          new EdgeCanvas<BoardSize>(edge.Rotate(), [edge, this]() -> void { SetPlayerMoveEdge(edge); }, this));
    }

    DotCanvases.reserve(Dot<BoardSize>::Max);
    for (Dot<BoardSize> dot = 0; dot < Dot<BoardSize>::Max; ++dot) {
      DotCanvases.emplace_back(new DotCanvas<BoardSize>(this));
    }
  }
}

template <int64_t BoardSize>
void
MainWindow<BoardSize>::Run() {
  Random Random;
  while (Board.Gaming()) {
    const QTime startTime = QTime::currentTime();
    const Turn turn = static_cast<Turn<BoardSize>>(Board);

    if (PlayerTypeIsRobot(Player1Type) && Board.IsPlayer1Turn()) {
      PlayerMoveEdge = Random.Choice(Robot1->BestCandidateEdges(Board));
    } else if (PlayerTypeIsRobot(Player2Type) && Board.IsPlayer2Turn()) {
      PlayerMoveEdge = Random.Choice(Robot2->BestCandidateEdges(Board));
    } else {
      PlayerMoveEdge = InvalidEdge<BoardSize>;
      while (PlayerMoveEdge.load() == InvalidEdge<BoardSize>) {
        QThread::yieldCurrentThread();
      }
    }

    assert(!Board.Contains(PlayerMoveEdge.load()));
    if (BackgroundMode) {
      Add(PlayerMoveEdge.load());
    } else {
      QMetaObject::invokeMethod(this, [this]() -> void { Add(PlayerMoveEdge.load()); }, Qt::BlockingQueuedConnection);
    }
    assert(Board.Contains(PlayerMoveEdge.load()));

    const double seconds = static_cast<double>(startTime.msecsTo(QTime::currentTime())) / 1000.0;

    QJsonObject playerScore;
    playerScore.insert("Player1", Board.Player1Score());
    playerScore.insert("Player2", Board.Player2Score());

    QJsonObject moveRecord;
    moveRecord.insert("Step", Board.NowStep());
    moveRecord.insert("Turn", turn.IsPlayer1Turn() ? 1 : 2);
    moveRecord.insert("Move", Int<BoardSize>(PlayerMoveEdge.load()));
    moveRecord.insert("Score", playerScore);
    moveRecord.insert("Time", seconds);

    QJsonObject info;
    info.insert("Info", moveRecord);
    qInfo() << QJsonDocument(info).toJson(QJsonDocument::Compact).constData();
  }

  QJsonObject winner;
  if (Board.RelativeScore() > 0) {
    winner.insert("Winner", "Player1");
  } else if (Board.RelativeScore() < 0) {
    winner.insert("Winner", "Player2");
  } else {
    winner.insert("Winner", "Draw");
  }
  qInfo() << QJsonDocument(winner).toJson(QJsonDocument::Compact).constData();

  if (BackgroundMode) {
    exit(0);
  } else {
    QMetaObject::invokeMethod(
        this,
        [this]() -> void {
          QTimer::singleShot(2000, this, [this]() -> void {
            EdgeCanvases[LastEdge]->SetHighLight(false);
            BaseCanvas<BoardSize>::update();
            QTimer::singleShot(2000, this, &MainWindow::close);
          });
        },
        Qt::BlockingQueuedConnection);
  }
}

template <int64_t BoardSize>
void
MainWindow<BoardSize>::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.fillRect(BaseCanvas<BoardSize>::rect(), Color());
}

template <int64_t BoardSize>
void
MainWindow<BoardSize>::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);

  const int x0 = (BaseCanvas<BoardSize>::width() - BoardWidth) / 2 - BaseCanvas<BoardSize>::UnitSize;
  const int y0 = (BaseCanvas<BoardSize>::height() - BoardWidth) / 2 - BaseCanvas<BoardSize>::UnitSize;

  for (Box<BoardSize> box = 0; box < Box<BoardSize>::Max; ++box) {
    const int x = x0 + box.X() * EdgeCanvas<BoardSize>::Height + 2 * BaseCanvas<BoardSize>::UnitSize;
    const int y = y0 + box.Y() * EdgeCanvas<BoardSize>::Height + 2 * BaseCanvas<BoardSize>::UnitSize;
    BoxCanvases[box]->move(x, y);
  }

  for (Edge<BoardSize> edge = 0; edge < Edge<BoardSize>::Max; ++edge) {
    int x = x0 + edge.Dot1().X() * EdgeCanvas<BoardSize>::Height;
    int y = y0 + edge.Dot1().Y() * EdgeCanvas<BoardSize>::Height;
    if (edge.Rotate()) {
      y += BaseCanvas<BoardSize>::UnitSize;
    } else {
      x += BaseCanvas<BoardSize>::UnitSize;
    }
    EdgeCanvases[edge]->move(x, y);
  }

  for (Dot<BoardSize> dot = 0; dot < Dot<BoardSize>::Max; ++dot) {
    const int x = x0 + dot.X() * EdgeCanvas<BoardSize>::Height;
    const int y = y0 + dot.Y() * EdgeCanvas<BoardSize>::Height;
    DotCanvases[dot]->move(x, y);
  }
}

template <int64_t BoardSize>
void
MainWindow<BoardSize>::showEvent(QShowEvent* event) {
  QWidget::showEvent(event);

  QThreadPool::globalInstance()->start([this]() -> void { Run(); });
}

template <int64_t BoardSize>
void
MainWindow<BoardSize>::SetPlayerMoveEdge(const Edge<BoardSize> edge) {
  if (Board.Contains(edge)) {
    return;
  }
  if (PlayerTypeIsRobot(Player1Type) && Board.IsPlayer1Turn()) {
    return;
  }
  if (PlayerTypeIsRobot(Player2Type) && Board.IsPlayer2Turn()) {
    return;
  }
  Edge<BoardSize> expected = InvalidEdge<BoardSize>;
  PlayerMoveEdge.compare_exchange_strong(expected, edge);
}

template <int64_t BoardSize>
QColor
MainWindow<BoardSize>::Color() const {
  static constexpr QColor DarkThemeColor = QColor(43, 43, 43, 255);
  static constexpr QColor LightThemeColor = QColor(242, 242, 242, 255);

  return BaseCanvas<BoardSize>::ThemeColor(DarkThemeColor, LightThemeColor);
}

template <int64_t BoardSize>
void
MainWindow<BoardSize>::Add(const Edge<BoardSize> edge) {
  if (!BackgroundMode) {
    if (Board.NowStep() > 0) {
      EdgeCanvases[LastEdge]->SetHighLight(false);
    }
    EdgeCanvases[edge]->SetOwner(Board);
    EdgeCanvases[edge]->raise();
    for (Dot<BoardSize> dot = 0; dot < Dot<BoardSize>::Max; ++dot) {
      DotCanvases[dot]->raise();
    }

    for (const Box<BoardSize> box : NearBoxes(edge)) {
      int count = 0;
      for (const Edge<BoardSize> nearEdge : NearEdges(box)) {
        if (Board.Contains(nearEdge)) {
          count++;
        }
      }
      if (count == 3) {
        BoxCanvases[box]->SetOwner(Board);
      }
    }
    LastEdge = edge;
  }

  Board.Add(edge);
  if (!BackgroundMode) {
    BaseCanvas<BoardSize>::update();
    QApplication::beep();
  }
}

}  // namespace dab::detail::frontend