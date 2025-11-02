#pragma once

#include "../board/BoardV2.hpp"
#include "../common/Random.hpp"
#include "../model/EdgeScoreMap.hpp"
#include "ImprovedSearchRobot.hpp"

class MonteCarloSearchRobot final : public Robot {
  friend class ParallelSearchRobot;

  public:
  static constexpr int SearchTime = 10000;

  MonteCarloSearchRobot() = default;

  Span<Edge>
  BestCandidateEdges(const BoardV2& board) override {
    if (auto edges = SubModel.BestCandidateEdges(board); edges.Size() == 1) {
      return edges;
    }

    ScoreMap.Reset();
    int times = SearchTime / board.RemainStep() + 1;
    while (times--) {
      AuxBoard.Reset(board.GetBoardV1());
      Edge edge = RandomChoice(SubModel.BestCandidateEdges(AuxBoard));
      AuxBoard.Add(edge);
      while (AuxBoard.Gaming()) {
        AuxBoard.Add(RandomChoice(SubModel.BestCandidateEdges(AuxBoard)));
      }
      ScoreMap.Add(edge, AuxBoard.Score());
    }

    return ScoreMap.Export();
  }

  private:
  ImprovedSearchRobot SubModel;
  BoardV2 AuxBoard;
  EdgeScoreMap ScoreMap;
};
