/*
Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2025 Xin Hu <202219120810@stu.cdut.edu.cn>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#pragma once

#include "ParallelSearchRobot.hpp"

namespace dab::detail::robot {

enum class PlayerType {
  Human = 0,
  GreedyRobot,
  ImproveGreedyRobot,
  SimulationRobot,
  MonteCarloRobot,
  ParallelSearchRobot,
};

static constexpr const char* PlayerTypeString[] = {
    "Human",
    "GreedyRobot",
    "ImproveGreedyRobot",
    "SimulationRobot",
    "MonteCarloRobot",
    "ParallelSearchRobot",
};

inline bool PlayerTypeIsRobot(PlayerType playerType) { return playerType != PlayerType::Human; }

template <int64_t BoardSize>
Robot<BoardSize>* CreateRobot(PlayerType playerType) {
  switch (playerType) {
    case PlayerType::GreedyRobot:
      return new GreedyRobot<BoardSize>();
    case PlayerType::ImproveGreedyRobot:
      return new ImproveGreedyRobot<BoardSize>();
    case PlayerType::SimulationRobot:
      return new SimulationRobot<BoardSize>();
    case PlayerType::MonteCarloRobot:
      return new MonteCarloRobot<BoardSize>();
    case PlayerType::ParallelSearchRobot:
      return new ParallelSearchRobot<BoardSize>();
    case PlayerType::Human:
      break;
  }
  return nullptr;
}

}  // namespace dab::detail::robot