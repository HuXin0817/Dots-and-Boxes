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
  explicit MainWindow(PlayerType player1Type,
                      PlayerType player2Type,
                      RobotType robot1Type,
                      RobotType robot2Type,
                      QWidget* parent = nullptr)
      : BaseCanvasLayer<BoardSize>(parent),
        Player1Type(player1Type),
        Player2Type(player2Type),
        Robot1(RobotConfig<BoardSize>::CreateRobot(robot1Type)),
        Robot2(RobotConfig<BoardSize>::CreateRobot(robot2Type)) {
    BaseCanvasLayer<BoardSize>::resize(BaseCanvasLayer<BoardSize>::WindowSize,
                                       BaseCanvasLayer<BoardSize>::WindowSize);
    BaseCanvasLayer<BoardSize>::setMinimumSize(BaseCanvasLayer<BoardSize>::WindowSize,
                                               BaseCanvasLayer<BoardSize>::WindowSize);

    Board.New();
    BoxCanvasLayer.New(this);
    EdgeCanvasLayer.New(this);
    DotCanvasLayer.New(this);
    auto CallBackFactory = [this](Edge<BoardSize> edge) {
      return [edge, this] { setPlayerMoveEdge(edge); };
    };
    EdgeButtonLayer.New(CallBackFactory, this);
  }

  QColor
  Color() const override {
    static QColor DarkThemeColor = {43, 43, 43, 255};
    static QColor LightThemeColor = {242, 242, 242, 255};

    return BaseCanvasLayer<BoardSize>::isDarkTheme() ? DarkThemeColor : LightThemeColor;
  }

  void
  Add(Edge<BoardSize> edge) {
    if (Board->GetEdgeCountableBoard().GetBasicBoard().GetStep().NowStep() > 0) {
      EdgeCanvasLayer->GetCanvases().At(LastEdge.Int())->SetHighLight(false);
    }
    EdgeCanvasLayer->GetCanvases()
        .At(edge.Int())
        ->SetState(BaseCanvasLayer<BoardSize>::StateFromTurn(Board->GetScoreMap().GetTurn()));
    EdgeCanvasLayer->GetCanvases().At(edge.Int())->raise();

    for (auto box : NearBoxes(edge)) {
      int count = 0;
      for (auto nearEdge : NearEdges(box)) {
        if (Board->GetEdgeCountableBoard().GetBasicBoard().Contains(nearEdge)) {
          count++;
        }
      }
      if (count == 3) {
        BoxCanvasLayer->GetBoxCanvases().At(box.Int())->SetState(
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

    BoxCanvasLayer->move(x, y);
    EdgeCanvasLayer->move(x, y);
    DotCanvasLayer->move(x, y);
  }

  void
  showEvent(QShowEvent* event) override {
    BaseCanvasLayer<BoardSize>::showEvent(event);

    std::thread([this] {
      while (Board->GetEdgeCountableBoard().GetBasicBoard().GetStep().Gaming()) {
        auto startTime = std::chrono::high_resolution_clock::now();

        if (Player1Type == PlayerType::Robot &&
            Board->GetScoreMap().GetTurn().Bool() == Player1Turn.Bool()) {
          PlayerMoveEdge = RandomChoice(Robot1->BestCandidateEdges(*Board));
        } else if (Player2Type == PlayerType::Robot &&
                   Board->GetScoreMap().GetTurn().Bool() == Player2Turn.Bool()) {
          PlayerMoveEdge = RandomChoice(Robot2->BestCandidateEdges(*Board));
        } else {
          PlayerMoveEdge = -1;
          while (PlayerMoveEdge.Int() == -1) {
            std::this_thread::yield();
          }
        }
        Add(PlayerMoveEdge);

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        double seconds = static_cast<double>(duration.count()) / 1000000.0;

        printf("| Step %d | Player %d Move %s | Score %d : %d | Time: %.2fs |\n",
               Board->GetEdgeCountableBoard().GetBasicBoard().GetStep().NowStep(),
               Board->GetScoreMap().GetTurn().Bool() == Player1Turn.Bool() ? 1 : 2,
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
      EdgeCanvasLayer->GetCanvases().At(LastEdge.Int())->SetHighLight(false);
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
  Ptr<BoxCanvasLayer<BoardSize>> BoxCanvasLayer;
  Ptr<EdgeCanvasLayer<BoardSize>> EdgeCanvasLayer;
  Ptr<DotCanvasLayer<BoardSize>> DotCanvasLayer;
  Ptr<EdgeButtonLayer<BoardSize>> EdgeButtonLayer;
  Edge<BoardSize> LastEdge;

  void
  setPlayerMoveEdge(Edge<BoardSize> edge) {
    if (Board->GetEdgeCountableBoard().GetBasicBoard().Contains(edge)) {
      return;
    }
    if (Player1Type == PlayerType::Robot &&
        Board->GetScoreMap().GetTurn().Bool() == Player1Turn.Bool()) {
      return;
    }
    if (Player2Type == PlayerType::Robot &&
        Board->GetScoreMap().GetTurn().Bool() == Player2Turn.Bool()) {
      return;
    }
    PlayerMoveEdge = edge;
  }
};
