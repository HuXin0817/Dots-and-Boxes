#pragma once

#include <QString>

#include "../src/model/SizeType.hpp"

namespace dab::frontend {

template <int64_t BoardSize>
class MoveRecord {
  public:
  MoveRecord(SizeType<BoardSize> step,
             SizeType<BoardSize> move,
             int8_t player,
             SizeType<BoardSize> player1Score,
             SizeType<BoardSize> player2Score,
             float time);

  QString
  ToString() const;

  private:
  SizeType<BoardSize> Step;
  SizeType<BoardSize> Move;
  int8_t Player;
  SizeType<BoardSize> Player1Score;
  SizeType<BoardSize> Player2Score;
  float Time;
};

template <int64_t BoardSize>
MoveRecord<BoardSize>::MoveRecord(SizeType<BoardSize> step,
                                  SizeType<BoardSize> move,
                                  int8_t player,
                                  SizeType<BoardSize> player1Score,
                                  SizeType<BoardSize> player2Score,
                                  float time)
    : Step(step), Move(move), Player(player), Player1Score(player1Score), Player2Score(player2Score), Time(time) {
}

template <int64_t BoardSize>
QString
MoveRecord<BoardSize>::ToString() const {
  return QString(R"({"Step":%1,"Player":%2,"Move":%3,"Score":{"Player1":%4,"Player2":%5},"Time":%6})")
      .arg(Step)
      .arg(Player)
      .arg(Move)
      .arg(Player1Score)
      .arg(Player2Score)
      .arg(Time, 0, 'f', 3);
}

}  // namespace dab::frontend
