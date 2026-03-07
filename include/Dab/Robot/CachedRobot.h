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

#include <Dab/Board.h>
#include <Dab/LRUCache.h>

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <thread>

#include "Dab/Tools.h"

namespace dab::__detail__::robot {

template <typename SubRobotType>
class CachedRobot : public SubRobotType {
 public:
  CachedRobot() { (void)doRecord; }

  template <typename Board>
  Span<const Edge> BestCandidateEdges(const Board& board);

 private:
  using Cache = tstarling::ThreadSafeLRUCache<HashValueBoard, Vector<Edge>>;
  static constexpr size_t CacheSize = static_cast<int64_t>(Edge::Max) << 10;
  static inline Cache Map{CacheSize};

  static inline std::atomic<uint64_t> CachedNumber = 0;
  static inline std::atomic<uint64_t> TotalNumber = 0;

  static int doRecord;

  HashValueBoard Key;
};

template <typename SubRobotType>
int CachedRobot<SubRobotType>::doRecord = []() {
  if constexpr (DebugMode) {
    std::thread([&]() {
      while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        if (TotalNumber.load() > 0) {
          uint64_t size = Map.size();
          uint64_t cached = CachedNumber.load();
          uint64_t total = TotalNumber.load();
          double percentage = 100.0 * cached / total;
          LogDebug(R"({{"CachedRobot":{{"Size":{},"CachedNumber":{},"TotalNumber":{},"CacheHitRate":{:.2f}%}}}})",
                   size,
                   cached,
                   total,
                   percentage);
        }
      }
    }).detach();
  }
  return 0;
}();

template <typename SubRobotType>
template <typename Board>
Span<const Edge> CachedRobot<SubRobotType>::BestCandidateEdges(const Board& board) {
  Key = board;
  if constexpr (DebugMode) {
    TotalNumber.fetch_add(1);
  }
  if (Cache::ConstAccessor ac; Map.find(ac, Key)) {
    if constexpr (DebugMode) {
      CachedNumber.fetch_add(1);
    }
    Assert(!ac->Empty());
    return {ac->begin(), ac->Size()};
  }

  Span result = SubRobotType::BestCandidateEdges(board);
  Assert(!result.Empty(), K(result));
  Map.insert(Key, Vector(result.begin(), result.Size()));
  return result;
}

}  // namespace dab::__detail__::robot