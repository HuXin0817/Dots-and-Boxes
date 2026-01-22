#pragma once

#include <QApplication>
#include <QColor>
#include <QTimer>
#include <chrono>
#include <functional>
#include <thread>

#include "../src/robot/ParallelSearchRobot.hpp"
#include "PlayerConfig.hpp"
#include "layers/BoxCanvasLayer.hpp"
#include "layers/DotCanvasLayer.hpp"
#include "layers/EdgeButtonLayer.hpp"
#include "layers/EdgeCanvasLayer.hpp"

namespace dab::frontend {

template <int BoardSize, typename SizeType>
class MainWindow final : public layer::BaseCanvasLayer<BoardSize, SizeType> {
  public:
  explicit MainWindow(const PlayerType player1Type, const PlayerType player2Type, QWidget* parent = nullptr)
      : layer::BaseCanvasLayer<BoardSize, SizeType>(parent), Player1Type(player1Type), Player2Type(player2Type) {
    layer::BaseCanvasLayer<BoardSize, SizeType>::resize(layer::BaseCanvasLayer<BoardSize, SizeType>::WindowSize,
                                                        layer::BaseCanvasLayer<BoardSize, SizeType>::WindowSize);
    layer::BaseCanvasLayer<BoardSize, SizeType>::setMinimumSize(
        layer::BaseCanvasLayer<BoardSize, SizeType>::WindowSize,
        layer::BaseCanvasLayer<BoardSize, SizeType>::WindowSize);

    BoxCanvasLayer.New(this);
    EdgeCanvasLayer.New(this);
    DotCanvasLayer.New(this);
    std::function<std::function<void()>(const model::Edge<BoardSize, SizeType>)> CallBackFactory =
        [this](const model::Edge<BoardSize, SizeType> edge) -> std::function<void()> {
      return [edge, this]() -> void { setPlayerMoveEdge(edge); };
    };
    EdgeButtonLayer.New(CallBackFactory, this);
  }

  QColor
  Color() const override {
    static QColor DarkThemeColor = {43, 43, 43, 255};
    static QColor LightThemeColor = {242, 242, 242, 255};

    return layer::BaseCanvasLayer<BoardSize, SizeType>::isDarkTheme() ? DarkThemeColor : LightThemeColor;
  }

  void
  Add(const model::Edge<BoardSize, SizeType> edge) {
    if (Board.GetEdgeCountableBoard().GetBasicBoard().GetStep().NowStep() > 0) {
      EdgeCanvasLayer->At(LastEdge)->SetHighLight(false);
    }
    EdgeCanvasLayer->At(edge)->SetState(Board.GetScoreMap().GetTurn());
    EdgeCanvasLayer->At(edge)->raise();

    for (const model::Box<BoardSize, SizeType> box : NearBoxes(edge)) {
      int count = 0;
      for (const model::Edge<BoardSize, SizeType> nearEdge : NearEdges(box)) {
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
    layer::BaseCanvasLayer<BoardSize, SizeType>::update();
    QApplication::beep();
  }

  protected:
  void
  paintEvent(QPaintEvent* event) override {
    layer::BaseCanvasLayer<BoardSize, SizeType>::paintEvent(event);

    QPainter painter(this);
    painter.fillRect(layer::BaseCanvasLayer<BoardSize, SizeType>::rect(), Color());
  }

  void
  resizeEvent(QResizeEvent* event) override {
    layer::BaseCanvasLayer<BoardSize, SizeType>::resizeEvent(event);

    const int x = (layer::BaseCanvasLayer<BoardSize, SizeType>::width() -
                   layer::BaseCanvasLayer<BoardSize, SizeType>::WindowSize) /
                  2;
    const int y = (layer::BaseCanvasLayer<BoardSize, SizeType>::height() -
                   layer::BaseCanvasLayer<BoardSize, SizeType>::WindowSize) /
                  2;

    BoxCanvasLayer->move(x, y);
    EdgeCanvasLayer->move(x, y);
    DotCanvasLayer->move(x, y);
  }

  void
  showEvent(QShowEvent* event) override {
    layer::BaseCanvasLayer<BoardSize, SizeType>::showEvent(event);

    std::thread([this] {
      while (Board.GetEdgeCountableBoard().GetBasicBoard().GetStep().Gaming()) {
        const std::chrono::time_point startTime = std::chrono::high_resolution_clock::now();

        if ((Player1Type == PlayerType::Robot && Board.GetScoreMap().GetTurn().Value() == model::Player1Turn.Value()) ||
            (Player2Type == PlayerType::Robot && Board.GetScoreMap().GetTurn().Value() == model::Player2Turn.Value())) {
          PlayerMoveEdge = common::RandomChoice<common::Span<model::Edge<BoardSize, SizeType>, SizeType>, SizeType>(
              Robot.BestCandidateEdges(Board));
        } else {
          PlayerMoveEdge = model::InvalidEdge<BoardSize, SizeType>();
          while (PlayerMoveEdge.Value() == model::InvalidEdge<BoardSize, SizeType>().Value()) {
            std::this_thread::yield();
          }
        }
        Add(PlayerMoveEdge);

        const std::chrono::time_point endTime = std::chrono::high_resolution_clock::now();
        const std::chrono::microseconds duration =
            std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        const double seconds = static_cast<double>(duration.count()) / 1000000.0;

        qDebug(
            "%s",
            QString(
                "Info: {\"Step\":%1,\"Player\":%2,\"Move\":%3,\"Score\":{\"Player1\":%4,\"Player2\":%5},\"Time\":%6}")
                .arg(Board.GetEdgeCountableBoard().GetBasicBoard().GetStep().NowStep())
                .arg(Board.GetScoreMap().GetTurn().Value() == model::Player1Turn.Value() ? 1 : 2)
                .arg(PlayerMoveEdge.Value())
                .arg(Board.GetScoreMap().GetPlayer1Score())
                .arg(Board.GetScoreMap().GetPlayer2Score())
                .arg(seconds, 0, 'f', 3)
                .toLocal8Bit()
                .constData());
      }

      if (Board.GetScoreMap().GetPlayer1Score() > Board.GetScoreMap().GetPlayer2Score()) {
        qDebug("Info: {\"Winner\":\"Player1\"}");
      } else if (Board.GetScoreMap().GetPlayer2Score() > Board.GetScoreMap().GetPlayer1Score()) {
        qDebug("Info: \"Winner\":\"Player2\"");
      } else {
        qDebug("Info: \"Winner\":\"Draw\"");
      }

      std::this_thread::sleep_for(std::chrono::seconds(2));
      EdgeCanvasLayer->At(LastEdge)->SetHighLight(false);
      layer::BaseCanvasLayer<BoardSize, SizeType>::update();

      std::this_thread::sleep_for(std::chrono::seconds(2));
      layer::BaseCanvasLayer<BoardSize, SizeType>::close();
    }).detach();
  }

  private:
  const PlayerType Player1Type;
  const PlayerType Player2Type;
  robot::ParallelSearchRobot<BoardSize, SizeType> Robot;
  model::Edge<BoardSize, SizeType> PlayerMoveEdge;
  model::Edge<BoardSize, SizeType> LastEdge;
  board::ScoreCountableBoard<BoardSize, SizeType> Board;
  common::Ptr<layer::BoxCanvasLayer<BoardSize, SizeType>> BoxCanvasLayer;
  common::Ptr<layer::EdgeCanvasLayer<BoardSize, SizeType>> EdgeCanvasLayer;
  common::Ptr<layer::DotCanvasLayer<BoardSize, SizeType>> DotCanvasLayer;
  common::Ptr<layer::EdgeButtonLayer<BoardSize, SizeType>> EdgeButtonLayer;

  void
  setPlayerMoveEdge(const model::Edge<BoardSize, SizeType> edge) {
    if (Board.GetEdgeCountableBoard().GetBasicBoard().Contains(edge)) {
      return;
    }
    if (Player1Type == PlayerType::Robot && Board.GetScoreMap().GetTurn().Value() == model::Player1Turn.Value()) {
      return;
    }
    if (Player2Type == PlayerType::Robot && Board.GetScoreMap().GetTurn().Value() == model::Player2Turn.Value()) {
      return;
    }
    PlayerMoveEdge = edge;
  }
};

}  // namespace dab::frontend
