#pragma once

#include "../board/BoardV2.hpp"
#include "../common/Span.hpp"
#include "../model/Edge.hpp"

class SearchModel {
  public:
  virtual ~SearchModel() = default;

  virtual Span<Edge>
  BestCandidateEdges(const BoardV2& board) = 0;
};
