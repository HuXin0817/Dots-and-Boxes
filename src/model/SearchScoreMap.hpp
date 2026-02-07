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

#include <Dab/Common.hpp>
#include <algorithm>

#include "Edge.hpp"

namespace dab::detail::model {

template <int64_t BoardSize>
class SearchScoreMap {
 public:
  SearchScoreMap() = default;

  void Reset();
  void Add(const Edge<BoardSize> edge, const Int<BoardSize> score);
  void Add(const SearchScoreMap& other);
  Span<Edge<BoardSize>> Export();

 private:
  Array<int, Edge<BoardSize>::Max> Time;
  Array<int, Edge<BoardSize>::Max> Score;
  List<Edge<BoardSize>, Edge<BoardSize>::Max> BestEdges;
};

template <int64_t BoardSize>
void SearchScoreMap<BoardSize>::Reset() {
  std::fill(Time.begin(), Time.end(), 0);
  std::fill(Score.begin(), Score.end(), 0);
  BestEdges.Clear();
}

template <int64_t BoardSize>
void SearchScoreMap<BoardSize>::Add(const Edge<BoardSize> edge, const Int<BoardSize> score) {
  ++Time[edge];
  Score[edge] += score;
}

template <int64_t BoardSize>
void SearchScoreMap<BoardSize>::Add(const SearchScoreMap& other) {
  for (Int<BoardSize> i = 0; i < Edge<BoardSize>::Max; ++i) {
    Time[i] += other.Time[i];
    Score[i] += other.Score[i];
  }
}

template <int64_t BoardSize>
Span<Edge<BoardSize>> SearchScoreMap<BoardSize>::Export() {
  float maxScore = 0.0;
  for (Edge<BoardSize> edge = 0; edge < Edge<BoardSize>::Max; ++edge) {
    if (Time[edge] > 0) {
      if (const float score = static_cast<float>(Score[edge]) / static_cast<float>(Time[edge]);
          score > maxScore || BestEdges.Empty()) {
        maxScore = score;
        BestEdges.ClearAndSet(edge);
      } else if (score == maxScore) {
        BestEdges.Append(edge);
      }
    }
  }
  return Span(BestEdges.begin(), BestEdges.end());
}

}  // namespace dab::detail::model