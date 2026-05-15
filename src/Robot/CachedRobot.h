/*
Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2025 Xin Hu <huxin0817.hx@gmail.com>

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

#include <thread-safe-lru/lru-cache.h>

#include "../Board.h"

namespace dab::__detail__::robot {

template <typename SubRobotType>
class CachedRobot : public SubRobotType {
 public:
  CachedRobot() = default;

  template <typename Board>
  Span<const Edge> BestCandidateEdges(const Board& board);

 private:
  using CacheType = tstarling::ThreadSafeLRUCache<HashValueBoard, Vector<Edge>>;
  static inline CacheType Cache{static_cast<std::size_t>(Edge::Max) << 10};

  HashValueBoard Key;
};

template <typename SubRobotType>
template <typename Board>
Span<const Edge> CachedRobot<SubRobotType>::BestCandidateEdges(const Board& board) {
  Key = board;
  if (CacheType::ConstAccessor ac; Cache.find(ac, Key)) {
    return *ac;
  }

  const Span result = SubRobotType::BestCandidateEdges(board);
  assert(!result.Empty());
  Cache.insert(Key, result);
  return result;
}

}  // namespace dab::__detail__::robot