#pragma once

#include "../src/robot/PlayerType.hpp"
#include "../src/robot/Robot.hpp"
#include "layers/BoxCanvasLayer.hpp"
#include "layers/DotCanvasLayer.hpp"
#include "layers/EdgeButtonLayer.hpp"
#include "layers/EdgeCanvasLayer.hpp"

template <int BoardSize, typename SizeType>
class MainWindow final : public BaseCanvasLayer<BoardSize, SizeType> {
  using Base = BaseCanvasLayer<BoardSize, SizeType>;

  public:
  explicit MainWindow(const PlayerType player1Type, const PlayerType player2Type, QWidget* parent = nullptr)
      : Base(parent), Player1Type(player1Type), Player2Type(player2Type) {
    Base::resize(Base::WindowSize, Base::WindowSize);
    Base::setMinimumSize(Base::WindowSize, Base::WindowSize);

    BoxCanvasLayer = std::make_unique<typename ::BoxCanvasLayer<BoardSize, SizeType>>(this);
    EdgeCanvasLayer = std::make_unique<typename ::EdgeCanvasLayer<BoardSize, SizeType>>(this);
    DotCanvasLayer = std::make_unique<typename ::DotCanvasLayer<BoardSize, SizeType>>(this);
    std::function<std::function<void()>(const Edge<BoardSize, SizeType>)> CallBackFactory =
        [this](const Edge<BoardSize, SizeType> edge) -> std::function<void()> {
      return [edge, this]() -> void { setPlayerMoveEdge(edge); };
    };
    EdgeButtonLayer = std::make_unique<typename ::EdgeButtonLayer<BoardSize, SizeType>>(CallBackFactory, this);
    if (PlayerTypeIsRobot(Player1Type)) {
      Robot1 = CreateRobot<BoardSize, SizeType>(Player1Type);
    }
    if (PlayerTypeIsRobot(Player2Type)) {
      Robot2 = CreateRobot<BoardSize, SizeType>(Player2Type);
    }
  }

  QColor
  Color() const override {
    static QColor DarkThemeColor = {43, 43, 43, 255};
    static QColor LightThemeColor = {242, 242, 242, 255};

    return Base::isDarkTheme() ? DarkThemeColor : LightThemeColor;
  }

  void
  Add(const Edge<BoardSize, SizeType> edge) {
    if (Board.GetEdgeCountableBoard().GetBasicBoard().GetStep().NowStep() > 0) {
      EdgeCanvasLayer->At(LastEdge)->SetHighLight(false);
    }
    EdgeCanvasLayer->At(edge)->SetState(Board.GetScoreMap().GetTurn());
    EdgeCanvasLayer->At(edge)->raise();

    for (const Box<BoardSize, SizeType> box : NearBoxes(edge)) {
      int count = 0;
      for (const Edge<BoardSize, SizeType> nearEdge : NearEdges(box)) {
        if (Board.GetEdgeCountableBoard().GetBasicBoard().Contains(nearEdge)) {
          count++;
        }
      }
      if (count == 3) {
        BoxCanvasLayer->At(box)->SetState(Board.GetScoreMap().GetTurn());
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

    const int x = (Base::width() - Base::WindowSize) / 2;
    const int y = (Base::height() - Base::WindowSize) / 2;

    BoxCanvasLayer->move(x, y);
    EdgeCanvasLayer->move(x, y);
    DotCanvasLayer->move(x, y);
  }

  void
  showEvent(QShowEvent* event) override {
    Base::showEvent(event);

    QThreadPool::globalInstance()->start([this] {
      while (Board.GetEdgeCountableBoard().GetBasicBoard().GetStep().Gaming()) {
        const QTime startTime = QTime::currentTime();

        if ((PlayerTypeIsRobot(Player1Type) && Board.GetScoreMap().GetTurn().Value() == Player1Turn.Value())) {
          PlayerMoveEdge =
              RandomChoice<Span<Edge<BoardSize, SizeType>, SizeType>, SizeType>(Robot1->BestCandidateEdges(Board));
        } else if (PlayerTypeIsRobot(Player2Type) && Board.GetScoreMap().GetTurn().Value() == Player2Turn.Value()) {
          PlayerMoveEdge =
              RandomChoice<Span<Edge<BoardSize, SizeType>, SizeType>, SizeType>(Robot2->BestCandidateEdges(Board));
        } else {
          PlayerMoveEdge = InvalidEdge<BoardSize, SizeType>();
          while (PlayerMoveEdge.Value() == InvalidEdge<BoardSize, SizeType>().Value()) {
            std::this_thread::yield();
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
      EdgeCanvasLayer->At(LastEdge)->SetHighLight(false);
      Base::update();

      QThread::sleep(2);
      Base::close();
    });
  }

  private:
  const PlayerType Player1Type;
  const PlayerType Player2Type;
  std::unique_ptr<Robot<BoardSize, SizeType>> Robot1;
  std::unique_ptr<Robot<BoardSize, SizeType>> Robot2;
  Edge<BoardSize, SizeType> PlayerMoveEdge;
  Edge<BoardSize, SizeType> LastEdge;
  ScoreCountableBoard<BoardSize, SizeType> Board;
  std::unique_ptr<BoxCanvasLayer<BoardSize, SizeType>> BoxCanvasLayer;
  std::unique_ptr<EdgeCanvasLayer<BoardSize, SizeType>> EdgeCanvasLayer;
  std::unique_ptr<DotCanvasLayer<BoardSize, SizeType>> DotCanvasLayer;
  std::unique_ptr<EdgeButtonLayer<BoardSize, SizeType>> EdgeButtonLayer;

  void
  setPlayerMoveEdge(const Edge<BoardSize, SizeType> edge) {
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
