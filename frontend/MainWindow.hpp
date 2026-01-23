#pragma once

#include "../src/robot/PlayerType.hpp"
#include "../src/robot/Robot.hpp"
#include "BoxCanvas.hpp"
#include "DotCanvas.hpp"
#include "EdgeCanvas.hpp"

template <int64_t BoardSize>
class MainWindow final : public BaseCanvas<BoardSize> {
  using Base = BaseCanvas<BoardSize>;

  static constexpr int BoardWidth = Box<BoardSize>::Size * EdgeCanvas<BoardSize>::Height;
  static constexpr int WindowSize = BoardWidth + 2 * BoxCanvas<BoardSize>::Width;

  public:
  explicit MainWindow(const PlayerType player1Type, const PlayerType player2Type, QWidget* parent = nullptr)
      : Base(parent), Player1Type(player1Type), Player2Type(player2Type) {
    Base::resize(WindowSize, WindowSize);
    Base::setMinimumSize(WindowSize, WindowSize);

    for (const int i : std::views::iota(0, Box<BoardSize>::Max)) {
      BoxCanvases.emplace_back(new BoxCanvas<BoardSize>(this));
    }
    for (const Edge<BoardSize> edge : std::views::iota(0, Edge<BoardSize>::Max)) {
      EdgeCanvases.emplace_back(
          new EdgeCanvas<BoardSize>(edge.Rotate(), [edge, this]() -> void { setPlayerMoveEdge(edge); }, this));
    }
    for (const int i : std::views::iota(0, Dot<BoardSize>::Max)) {
      DotCanvases.emplace_back(new DotCanvas<BoardSize>(this));
    }

    if (PlayerTypeIsRobot(Player1Type)) {
      Robot1.reset(CreateRobot<BoardSize>(Player1Type));
    }
    if (PlayerTypeIsRobot(Player2Type)) {
      Robot2.reset(CreateRobot<BoardSize>(Player2Type));
    }
  }

  QColor
  Color() const {
    static QColor DarkThemeColor = {43, 43, 43, 255};
    static QColor LightThemeColor = {242, 242, 242, 255};

    return Base::isDarkTheme() ? DarkThemeColor : LightThemeColor;
  }

  void
  Add(const Edge<BoardSize> edge) {
    if (Board.GetEdgeCountableBoard().GetBasicBoard().GetStep().NowStep() > 0) {
      EdgeCanvases[LastEdge.Value()]->SetHighLight(false);
    }
    EdgeCanvases[edge.Value()]->SetState(Board.GetScoreMap().GetTurn());
    EdgeCanvases[edge.Value()]->raise();
    for (const int i : std::views::iota(0, Dot<BoardSize>::Max)) {
      DotCanvases[i]->raise();
    }

    for (const Box<BoardSize> box : NearBoxes(edge)) {
      int count = 0;
      for (const Edge<BoardSize> nearEdge : NearEdges(box)) {
        if (Board.GetEdgeCountableBoard().GetBasicBoard().Contains(nearEdge)) {
          count++;
        }
      }
      if (count == 3) {
        BoxCanvases[box.Value()]->SetState(Board.GetScoreMap().GetTurn());
      }
    }

    Board.Add(edge);
    LastEdge = edge;
    Base::update();
    QApplication::beep();
  }

  protected:
  void
  paintEvent(QPaintEvent* event) override {
    Base::paintEvent(event);

    QPainter painter(this);
    painter.fillRect(Base::rect(), Color());
  }

  void
  resizeEvent(QResizeEvent* event) override {
    Base::resizeEvent(event);

    const int x0 = (Base::width() - BoardWidth) / 2 - Base::UnitSize;
    const int y0 = (Base::height() - BoardWidth) / 2 - Base::UnitSize;

    for (const int i : std::views::iota(0, Box<BoardSize>::Size)) {
      for (const int j : std::views::iota(0, Box<BoardSize>::Size)) {
        const int x = x0 + i * EdgeCanvas<BoardSize>::Height + 2 * Base::UnitSize;
        const int y = y0 + j * EdgeCanvas<BoardSize>::Height + 2 * Base::UnitSize;
        BoxCanvases[Box<BoardSize>(i, j).Value()]->move(x, y);
      }
    }

    for (const Edge<BoardSize> edge : std::views::iota(0, Edge<BoardSize>::Max)) {
      int x = x0 + edge.Dot1().X() * EdgeCanvas<BoardSize>::Height;
      int y = y0 + edge.Dot1().Y() * EdgeCanvas<BoardSize>::Height;
      if (edge.Dot1().X() == edge.Dot2().X()) {
        y += Base::UnitSize;
      } else {
        x += Base::UnitSize;
      }
      EdgeCanvases[edge.Value()]->move(x, y);
    }

    for (const int i : std::views::iota(0, Dot<BoardSize>::Size)) {
      for (const int j : std::views::iota(0, Dot<BoardSize>::Size)) {
        const int x = x0 + i * EdgeCanvas<BoardSize>::Height;
        const int y = y0 + j * EdgeCanvas<BoardSize>::Height;
        DotCanvases[Dot<BoardSize>(i, j).Value()]->move(x, y);
      }
    }
  }

  void
  showEvent(QShowEvent* event) override {
    Base::showEvent(event);

    QThreadPool::globalInstance()->start([this] {
      while (Board.GetEdgeCountableBoard().GetBasicBoard().GetStep().Gaming()) {
        const QTime startTime = QTime::currentTime();

        if ((PlayerTypeIsRobot(Player1Type) && Board.GetScoreMap().GetTurn().Value() == Player1Turn.Value())) {
          PlayerMoveEdge = RandomChoice<Span<Edge<BoardSize>, SizeType<BoardSize>>, SizeType<BoardSize>>(
              Robot1->BestCandidateEdges(Board));
        } else if (PlayerTypeIsRobot(Player2Type) && Board.GetScoreMap().GetTurn().Value() == Player2Turn.Value()) {
          PlayerMoveEdge = RandomChoice<Span<Edge<BoardSize>, SizeType<BoardSize>>, SizeType<BoardSize>>(
              Robot2->BestCandidateEdges(Board));
        } else {
          PlayerMoveEdge = InvalidEdge<BoardSize>();
          while (PlayerMoveEdge.Value() == InvalidEdge<BoardSize>().Value()) {
            QThread::yieldCurrentThread();
          }
        }
        Add(PlayerMoveEdge);

        const double seconds = static_cast<double>(startTime.msecsTo(QTime::currentTime())) / 1000.0;

        qInfo("%s",
              QString(
                  "Info: {\"Step\":%1,\"Player\":%2,\"Move\":%3,\"Score\":{\"Player1\":%4,\"Player2\":%5},\"Time\":%6}")
                  .arg(Board.GetEdgeCountableBoard().GetBasicBoard().GetStep().NowStep())
                  .arg(Board.GetScoreMap().GetTurn().Value() == Player1Turn.Value() ? 1 : 2)
                  .arg(PlayerMoveEdge.Value())
                  .arg(Board.GetScoreMap().GetPlayer1Score())
                  .arg(Board.GetScoreMap().GetPlayer2Score())
                  .arg(seconds, 0, 'f', 3)
                  .toLocal8Bit()
                  .constData());
      }

      if (Board.GetScoreMap().GetPlayer1Score() > Board.GetScoreMap().GetPlayer2Score()) {
        qInfo("Info: {\"Winner\":\"Player1\"}");
      } else if (Board.GetScoreMap().GetPlayer2Score() > Board.GetScoreMap().GetPlayer1Score()) {
        qInfo("Info: \"Winner\":\"Player2\"");
      } else {
        qInfo("Info: \"Winner\":\"Draw\"");
      }

      QThread::sleep(2);
      EdgeCanvases[LastEdge.Value()]->SetHighLight(false);
      Base::update();

      QThread::sleep(2);
      Base::close();
    });
  }

  private:
  const PlayerType Player1Type;
  const PlayerType Player2Type;
  QScopedPointer<Robot<BoardSize>> Robot1;
  QScopedPointer<Robot<BoardSize>> Robot2;
  Edge<BoardSize> PlayerMoveEdge;
  Edge<BoardSize> LastEdge;
  ScoreCountableBoard<BoardSize> Board;
  QList<QPointer<BoxCanvas<BoardSize>>> BoxCanvases;
  QList<QPointer<DotCanvas<BoardSize>>> DotCanvases;
  QList<QPointer<EdgeCanvas<BoardSize>>> EdgeCanvases;

  void
  setPlayerMoveEdge(const Edge<BoardSize> edge) {
    if (Board.GetEdgeCountableBoard().GetBasicBoard().Contains(edge)) {
      return;
    }
    if (PlayerTypeIsRobot(Player1Type) && Board.GetScoreMap().GetTurn().Value() == Player1Turn.Value()) {
      return;
    }
    if (PlayerTypeIsRobot(Player2Type) && Board.GetScoreMap().GetTurn().Value() == Player2Turn.Value()) {
      return;
    }
    PlayerMoveEdge = edge;
  }
};
