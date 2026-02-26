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

#include "ScoreMap.h"

#include <algorithm>

namespace dab::__detail__::robot {

void ScoreMap::Reset() {
  std::ranges::fill(Time, 0);
  std::ranges::fill(Score, 0);
}

void ScoreMap::Add(Edge edge, Int score) {
  ++Time.At(edge);
  Score.At(edge) += score;
}

void ScoreMap::Add(const ScoreMap& other) {
  for (const Int i : Iota<Edge>()) {
    Time.At(i) += other.Time.At(i);
    Score.At(i) += other.Score.At(i);
  }
}

Span<const Edge> ScoreMap::Export(List<Edge, Edge::Max>& edges) {
  edges.Clear();
  float maxScore = 0.0;
  for (const Edge edge : Iota<Edge>()) {
    if (Time.At(edge) > 0) {
      if (const float score = static_cast<float>(Score.At(edge)) / static_cast<float>(Time.At(edge));
          score > maxScore || edges.Empty()) {
        maxScore = score;
        edges.ClearAndSet(edge);
      } else if (score == maxScore) {
        edges.Append(edge);
      }
    }
  }
  return {edges.begin(), edges.Size()};
}

}  // namespace dab::__detail__::robot