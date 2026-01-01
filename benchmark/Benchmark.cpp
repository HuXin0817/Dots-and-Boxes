#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>

#include "../src/common/Random.hpp"
#include "../src/robot/BasicSearchRobot.hpp"
#include "../src/robot/ImprovedSearchRobot.hpp"
#include "../src/robot/MonteCarloSearchRobot.hpp"
#include "../src/robot/ParallelSearchRobot.hpp"
#include "../src/robot/Robot.hpp"

template <int BoardSize>
int
BenchMarkTwoRobot(Robot<BoardSize>* robot1, Robot<BoardSize>* robot2, int round) {
  int score = 0;

  for (int i = 0; i < round; i++) {
    ScoreCountableBoard<BoardSize> board;

    while (board.GetEdgeCountableBoard().GetBasicBoard().GetStep().Gaming()) {
      if (board.GetScoreMap().GetTurn().Bool() == Player1Turn.Bool()) {
        board.Add(RandomChoice(robot1->BestCandidateEdges(board)));
      } else {
        board.Add(RandomChoice(robot2->BestCandidateEdges(board)));
      }
    }

    int newScore = board.GetScoreMap().Score();

    score += newScore;
  }

  return score;
}

template <int BoardSize>
int
BenchMarkTwoRobot(std::function<Robot<BoardSize>*()> robot1Constructor,
                  std::function<Robot<BoardSize>*()> robot2Constructor,
                  int round,
                  int parallelNumber) {
  int roundPerThread = round / parallelNumber;
  std::vector<std::thread> threads(parallelNumber - 1);
  std::vector<int> scores(parallelNumber - 1);

  for (int i = 0; i < parallelNumber - 1; i++) {
    threads[i] = std::thread([&] {
      auto robot1 = robot1Constructor();
      auto robot2 = robot2Constructor();

      scores[i] = BenchMarkTwoRobot(robot1, robot2, roundPerThread);
    });
  }

  auto robot1 = robot1Constructor();
  auto robot2 = robot2Constructor();
  int score = BenchMarkTwoRobot(robot1, robot2, round - (parallelNumber - 1) * roundPerThread);
  for (int i = 0; i < parallelNumber - 1; i++) {
    threads[i].join();
  }

  for (int i = 0; i < parallelNumber - 1; i++) {
    score += scores[i];
  }

  return score;
}

int
main(int argc, char* argv[]) {
  static constexpr int BoardSize = 6;
  std::map<std::string, std::function<Robot<BoardSize>*()>> constructors = {
      {"SimpleStrategyRobot", [] { return new SimpleStrategyRobot<BoardSize>(); }},
      {"BasicSearchRobot", [] { return new BasicSearchRobot<BoardSize>(); }},
      {"ImprovedSearchRobot", [] { return new ImprovedSearchRobot<BoardSize>(); }},
      {"MonteCarloSearchRobot", [] { return new MonteCarloSearchRobot<BoardSize>(); }},
      {"ParallelSearchRobot", [] { return new ParallelSearchRobot<BoardSize>(); }},
  };

  std::string Robot1Name = "BasicSearchRobot";
  std::string Robot2Name = "SimpleStrategyRobot";
  int Round = 1000;

  if (argc > 2) {
    Robot1Name = argv[1];
    Robot2Name = argv[2];
    if (argc > 3) {
      Round = atoi(argv[3]);
    }
  }

  int ParallelNumber = CPU_COUNT;
  if (Robot1Name == "ParallelSearchRobot" || Robot2Name == "ParallelSearchRobot") {
    ParallelNumber = 1;
  }

  int score = BenchMarkTwoRobot(constructors[std::string(Robot1Name)],
                                constructors[std::string(Robot2Name)],
                                Round,
                                ParallelNumber);

  int sumScore = Box<BoardSize>::Max * Round;
  float robot1Score = (sumScore + score) / 2;
  float robot2Score = sumScore - robot1Score;

  std::cout << robot1Score / sumScore << " " << robot2Score / sumScore << std::endl;
}
