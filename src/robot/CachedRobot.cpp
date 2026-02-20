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

#include "CachedRobot.h"

#include <Dab/Board.h>

namespace dab::__detail__::robot {

template <typename Board>
Span<const Edge> CachedRobot::BestCandidateEdges(const Board& board) {
  Key = board;
  tstarling::ThreadSafeLRUCache<HashBoard, Vector<Edge>>::ConstAccessor ac;
  if (Map.find(ac, Key)) {
    return {ac->begin(), ac->end()};
  }

  Span result = SubRobot.BestCandidateEdges(board);
  Assert(!result.Empty());
  Map.insert(Key, Vector(result.begin(), result.end()));
  return result;
}

template Span<const Edge> CachedRobot::BestCandidateEdges<RelativeScoreBoard>(const RelativeScoreBoard& board);
template Span<const Edge> CachedRobot::BestCandidateEdges<LoggingBoard>(const LoggingBoard& board);

}  // namespace dab::__detail__::robot