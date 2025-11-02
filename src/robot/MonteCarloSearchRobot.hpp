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
    if (auto edges = SubRobot.BestCandidateEdges(board); edges.Size() == 1) {
      return edges;
    }

    SearchResult.Reset();
    int times = SearchTime / board.RemainStep() + 1;
    while (times--) {
      AuxBoard.Reset(board.GetBoardV1());
      Edge edge = RandomChoice(SubRobot.BestCandidateEdges(AuxBoard));
      AuxBoard.Add(edge);
      while (AuxBoard.Gaming()) {
        AuxBoard.Add(RandomChoice(SubRobot.BestCandidateEdges(AuxBoard)));
      }
      SearchResult.Add(edge, AuxBoard.Score());
    }

    return SearchResult.Export();
  }

  private:
  ImprovedSearchRobot SubRobot;
  BoardV2 AuxBoard;
  EdgeScoreMap SearchResult;
};
