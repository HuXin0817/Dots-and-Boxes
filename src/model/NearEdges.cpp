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

#include "NearEdges.h"

#include <Dab/Common.h>

#include "Edge.h"
#include "Square.h"

namespace dab::detail::model {

class NearEdgesMapper {
 public:
  constexpr NearEdgesMapper();
  static constexpr Array<Edge, 4> GetNearEdges(Box box);

  Array<Array<Edge, 4>, Box::Max> BoxNearEdges;
};

constexpr NearEdgesMapper::NearEdgesMapper() {
  for (Box box = 0; box < Box::Max; ++box) {
    BoxNearEdges[box] = GetNearEdges(box);
  }
}

constexpr Array<Edge, 4> NearEdgesMapper::GetNearEdges(Box box) {
  Array<Edge, 4> NearEdges;
  const Int x = box.X();
  const Int y = box.Y();
  const Dot topLeft(x, y);
  const Dot topRight(x + 1, y);
  const Dot bottomLeft(x, y + 1);
  const Dot bottomRight(x + 1, y + 1);
  NearEdges[0] = Edge(topLeft, topRight);
  NearEdges[1] = Edge(topLeft, bottomLeft);
  NearEdges[2] = Edge(bottomLeft, bottomRight);
  NearEdges[3] = Edge(topRight, bottomRight);
  return NearEdges;
}

static constexpr NearEdgesMapper Instance;

const Array<Edge, 4>& NearEdges(Box box) { return Instance.BoxNearEdges[box]; }

}  // namespace dab::detail::model