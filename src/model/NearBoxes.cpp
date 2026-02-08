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

#include "NearBoxes.h"

#include <Dab/Common.h>

#include "Edge.h"
#include "Square.h"

namespace dab::detail::model {

class NearBoxesMapper {
 public:
  constexpr NearBoxesMapper();
  static constexpr List<Box, 2> GetNearBoxes(Edge edge);

  Array<List<Box, 2>, Edge::Max> EdgeNearBoxes;
};

constexpr NearBoxesMapper::NearBoxesMapper() {
  for (Edge edge = 0; edge < Edge::Max; ++edge) {
    EdgeNearBoxes[edge] = GetNearBoxes(edge);
  }
}

constexpr List<Box, 2> NearBoxesMapper::GetNearBoxes(Edge edge) {
  List<Box, 2> result;

  Int x = edge.Dot2().X() - 1;
  Int y = edge.Dot2().Y() - 1;
  if (x >= 0 && y >= 0) {
    result.Append(Box(x, y));
  }

  x = edge.Dot1().X();
  y = edge.Dot1().Y();
  if (x < BoardSize && y < BoardSize) {
    result.Append(Box(x, y));
  }

  return result;
}

static constexpr NearBoxesMapper Instance;

const List<Box, 2>& NearBoxes(Edge edge) { return Instance.EdgeNearBoxes[edge]; }

}  // namespace dab::detail::model