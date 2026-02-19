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

#include <Dab/Board.h>
#include <Dab/PlayerType.h>

namespace dab::__detail__::robot {

class Robot {
 public:
  virtual ~Robot() = default;

  virtual Span<const Edge> BestCandidateEdges(const LoggingBoard& board) = 0;
};

template <typename Derived>
class RobotWapper : public Robot {
 public:
  template <typename Board>
  Span<const Edge> BestCandidateEdges(const Board& board) {
    return derived().BestCandidateEdges(board);
  }
  Span<const Edge> BestCandidateEdges(const LoggingBoard& board) override { return BestCandidateEdges<>(board); }

 protected:
  ~RobotWapper() = default;

 private:
  constexpr Derived& derived() & { return static_cast<Derived&>(*this); }
  constexpr const Derived& derived() const& { return static_cast<const Derived&>(*this); }
  constexpr Derived&& derived() && { return static_cast<Derived&&>(*this); }
};

Robot* CreateRobot(PlayerType playerType);

}  // namespace dab::__detail__::robot