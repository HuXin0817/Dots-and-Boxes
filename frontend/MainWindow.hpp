#pragma once

#include <QApplication>
#include <QColor>
#include <QTimer>
#include <chrono>
#include <thread>

#include "../src/robot/RobotConfig.hpp"
#include "PlayerConfig.hpp"
#include "layers/BoxCanvasLayer.hpp"
#include "layers/DotCanvasLayer.hpp"
#include "layers/EdgeButtonLayer.hpp"
#include "layers/EdgeCanvasLayer.hpp"

template <int BoardSize>
class MainWindow final : public BaseCanvasLayer<BoardSize> {
  public:
  explicit MainWindow(PlayerType Player1Type,
                      PlayerType Player2Type,
                      RobotType Robot1Type,
                      RobotType Robot2Type,
                      QWidget* parent = nullptr)
      : BaseCanvasLayer<BoardSize>(parent),
        Player1Type(Player1Type),
        Player2Type(Player2Type),
        Robot1(RobotConfig<BoardSize>::CreateRobot(Robot1Type)),
        Robot2(RobotConfig<BoardSize>::CreateRobot(Robot2Type)) {
    BaseCanvasLayer<BoardSize>::resize(BaseCanvasLayer<BoardSize>::WindowSize,
                                       BaseCanvasLayer<BoardSize>::WindowSize);
    BaseCanvasLayer<BoardSize>::setMinimumSize(BaseCanvasLayer<BoardSize>::WindowSize,
                                               BaseCanvasLayer<BoardSize>::WindowSize);

    Board.New();
    BoxCanvases.New(this);
    EdgeCanvases.New(this);
    DotCanvases.New(this);
    auto CallBackFactory = [this](Edge<BoardSize> edge) {
      return [edge, this] { setPlayerMoveEdge(edge); };
    };
    EdgeButtons.New(CallBackFactory, this);
  }

  static inline QColor DarkThemeColor = {43, 43, 43, 255};
  static inline QColor LightThemeColor = {242, 242, 242, 255};

  QColor
  Color() const override {
    return BaseCanvasLayer<BoardSize>::isDarkTheme() ? DarkThemeColor : LightThemeColor;
  }

  void
  Add(Edge<BoardSize> edge) {
    if (Board->GetEdgeCountableBoard().GetBasicBoard().GetStep().NowStep() > 0) {
      EdgeCanvases->GetCanvases().At(LastEdge)->SetHighLight(false);
    }
    EdgeCanvases->GetCanvases().At(edge)->SetState(
        BaseCanvasLayer<BoardSize>::StateFromTurn(Board->GetScoreMap().GetTurn()));
    EdgeCanvases->GetCanvases().At(edge)->raise();

    for (Box box : NearBoxes(edge)) {
      int count = 0;
      for (Edge nearEdge : NearEdges(box)) {
        if (Board->GetEdgeCountableBoard().GetBasicBoard().Contains(nearEdge)) {
          count++;
        }
      }
      if (count == 3) {
        BoxCanvases->GetBoxCanvases().At(box)->SetState(
            BaseCanvasLayer<BoardSize>::StateFromTurn(Board->GetScoreMap().GetTurn()));
      }
    }

    Board->Add(edge);
    LastEdge = edge;
    BaseCanvasLayer<BoardSize>::update();
    QApplication::beep();
  }

  protected:
  void
  paintEvent(QPaintEvent* event) override {
    BaseCanvasLayer<BoardSize>::paintEvent(event);

    QPainter painter(this);
    painter.fillRect(BaseCanvasLayer<BoardSize>::rect(), Color());
  }

  void
  resizeEvent(QResizeEvent* event) override {
    BaseCanvasLayer<BoardSize>::resizeEvent(event);

    int x = (BaseCanvasLayer<BoardSize>::width() - BaseCanvasLayer<BoardSize>::WindowSize) / 2;
    int y = (BaseCanvasLayer<BoardSize>::height() - BaseCanvasLayer<BoardSize>::WindowSize) / 2;

    BoxCanvases->move(x, y);
    EdgeCanvases->move(x, y);
    DotCanvases->move(x, y);
  }

  void
  showEvent(QShowEvent* event) override {
    BaseCanvasLayer<BoardSize>::showEvent(event);

    std::thread([this] {
      while (Board->GetEdgeCountableBoard().GetBasicBoard().GetStep().Gaming()) {
        auto startTime = std::chrono::high_resolution_clock::now();

        if (Player1Type == PlayerType::Robot && Board->GetScoreMap().GetTurn() == Player1Turn) {
          PlayerMoveEdge = RandomChoice(Robot1->BestCandidateEdges(*Board));
        } else if (Player2Type == PlayerType::Robot &&
                   Board->GetScoreMap().GetTurn() == Player2Turn) {
          PlayerMoveEdge = RandomChoice(Robot2->BestCandidateEdges(*Board));
        } else {
          PlayerMoveEdge = -1;
          while (PlayerMoveEdge == -1) {
            std::this_thread::yield();
          }
        }
        Add(PlayerMoveEdge);

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        double seconds = static_cast<double>(duration.count()) / 1000000.0;

        printf("| Step %d | Player %d Move %s | Score %d : %d | Time: %.2fs |\n",
               Board->GetEdgeCountableBoard().GetBasicBoard().GetStep().NowStep(),
               Board->GetScoreMap().GetTurn() == Player1Turn ? 1 : 2,
               std::string(PlayerMoveEdge).c_str(),
               Board->GetScoreMap().GetPlayer1Score(),
               Board->GetScoreMap().GetPlayer2Score(),
               seconds);
      }

      if (Board->GetScoreMap().GetPlayer1Score() > Board->GetScoreMap().GetPlayer2Score()) {
        printf("| Player 1 Win! |\n");
      } else if (Board->GetScoreMap().GetPlayer2Score() > Board->GetScoreMap().GetPlayer1Score()) {
        printf("| Player 2 Win! |\n");
      } else {
        printf("| Draw! |\n");
      }

      std::this_thread::sleep_for(std::chrono::seconds(2));
      EdgeCanvases->GetCanvases().At(LastEdge)->SetHighLight(false);
      BaseCanvasLayer<BoardSize>::update();

      std::this_thread::sleep_for(std::chrono::seconds(2));
      BaseCanvasLayer<BoardSize>::close();
    }).detach();
  }

  private:
  PlayerType Player1Type;
  PlayerType Player2Type;
  Ptr<Robot<BoardSize>> Robot1;
  Ptr<Robot<BoardSize>> Robot2;
  Edge<BoardSize> PlayerMoveEdge;
  Ptr<ScoreCountableBoard<BoardSize>> Board;
  Ptr<BoxCanvasLayer<BoardSize>> BoxCanvases;
  Ptr<EdgeCanvasLayer<BoardSize>> EdgeCanvases;
  Ptr<DotCanvasLayer<BoardSize>> DotCanvases;
  Ptr<EdgeButtonLayer<BoardSize>> EdgeButtons;
  Edge<BoardSize> LastEdge;

  void
  setPlayerMoveEdge(Edge<BoardSize> edge) {
    if (Board->GetEdgeCountableBoard().GetBasicBoard().Contains(edge)) {
      return;
    }
    if (Player1Type == PlayerType::Robot && Board->GetScoreMap().GetTurn() == Player1Turn) {
      return;
    }
    if (Player2Type == PlayerType::Robot && Board->GetScoreMap().GetTurn() == Player2Turn) {
      return;
    }
    PlayerMoveEdge = edge;
  }
};
