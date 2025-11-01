#pragma once

#include "../board/BoardV2.h"
#include "../common/Span.h"
#include "../model/Edge.h"

class SearchModel {
  public:
  virtual ~SearchModel() = default;

  virtual Span<Edge>
  BestCandidateEdges(const BoardV2& board) = 0;
};
