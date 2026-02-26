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

#include <tbb/parallel_for_each.h>

#include "MonteCarloRobot.h"

namespace dab::__detail__::robot {

class ParallelSearchRobot : public RobotWapper<ParallelSearchRobot> {
 public:
  ParallelSearchRobot() = default;
  template <typename Board>
  Span<const Edge> BestCandidateEdges(const Board& board);

 private:
  Array<MonteCarloRobot, 32> Workers;
  ScoreMap SearchResult;
};

template <typename Board>
Span<const Edge> ParallelSearchRobot::BestCandidateEdges(const Board& board) {
  if (Span<const Edge> edges; Workers.Front().CanEarlyExit(board, edges)) {
    return edges;
  }

  SearchResult.Reset();
  tbb::parallel_for_each(Workers, [&](MonteCarloRobot& robot) -> void { robot.SearchCandidateEdges(board); });
  for (const MonteCarloRobot& model : Workers) {
    SearchResult.Add(model.GetSearchResult());
  }

  return SearchResult.Export(Workers.Front().GetSearchEdges());
}

}  // namespace dab::__detail__::robot