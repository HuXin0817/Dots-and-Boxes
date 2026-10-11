import { InvalidEdge } from "./board";
import { SearchBoard } from "./search-board";
import { SearchCache } from "./search-cache";

const ExactSearchAborted = Symbol("ExactSearchAborted");

const ExactBound = Object.freeze({
  Exact: 0,
  Lower: 1,
  Upper: 2,
});

export class ExactSolver {
  constructor(model, options = {}) {
    const cacheLimit = options.cacheLimit ?? 500000;

    this.model = model;
    this.chainBoard = new SearchBoard(model);
    this.chainProbeBoard = new SearchBoard(model);
    this.cache = new SearchCache(
      Math.ceil(model.edgeCount / 32),
      Math.max(1, cacheLimit),
    );
    this.canonicalOccupied = new Uint32Array(Math.ceil(model.edgeCount / 32));
    this.valueCount = 2 * model.boxCount + 1;
    this.boundBase = this.valueCount + 1;
    this.nodesVisited = 0;
    this.nodeLimit = Number.POSITIVE_INFINITY;
  }

  value(board, nodeLimit = Number.POSITIVE_INFINITY) {
    this.nodesVisited = 0;
    this.nodeLimit = nodeLimit;

    // Values are future score margins from the player-to-move perspective.
    // Historical scores are deliberately excluded so transpositions can share
    // one cached result regardless of the path used to reach the position.
    const value = this.exactFutureMargin(
      board,
      -this.model.boxCount,
      this.model.boxCount,
    );
    return typeof value === "number" ? value : undefined;
  }

  bestEdges(board, nodeLimit = Number.POSITIVE_INFINITY) {
    this.nodesVisited = 0;
    this.nodeLimit = nodeLimit;
    let bestScore = -this.model.boxCount - 1;
    const result = [];

    for (const edge of board.legalEdgeRepresentatives(false)) {
      const score = this.exactEdgeValue(
        board,
        edge,
        bestScore,
        this.model.boxCount,
      );
      if (score === ExactSearchAborted) {
        return [];
      }

      if (result.length === 0 || score > bestScore) {
        bestScore = score;
        result.length = 0;
        result.push(edge);
      } else if (score === bestScore) {
        result.push(edge);
      }
    }

    if (result.length === 1) {
      return result;
    }

    // A fail-low search may return bestScore as an upper bound for an
    // inferior move. Prove every apparent tie with a zero-width window.
    let verifiedCount = 0;
    for (const edge of result) {
      const score = this.exactEdgeValue(board, edge, bestScore - 1, bestScore);
      if (score === ExactSearchAborted) {
        return [];
      }
      if (score >= bestScore) {
        result[verifiedCount++] = edge;
      }
    }
    result.length = verifiedCount;
    return result;
  }

  canonicalizePosition(board) {
    return board.canonicalizeOccupied(this.canonicalOccupied);
  }

  decodeEntry(encoded) {
    // Pack both bounds and the preferred move into one exact integer. The
    // extra valueCount index represents an absent bound.
    const upperIndex = encoded % this.boundBase;
    let payload = (encoded - upperIndex) / this.boundBase;
    const lowerIndex = payload % this.boundBase;
    payload = (payload - lowerIndex) / this.boundBase;
    return {
      bestEdge: payload - 1,
      lower:
        lowerIndex === this.valueCount
          ? undefined
          : lowerIndex - this.model.boxCount,
      upper:
        upperIndex === this.valueCount
          ? undefined
          : upperIndex - this.model.boxCount,
    };
  }

  encodeEntry(lower, upper, bestEdge) {
    const lowerIndex =
      lower === undefined ? this.valueCount : lower + this.model.boxCount;
    const upperIndex =
      upper === undefined ? this.valueCount : upper + this.model.boxCount;
    return (
      ((bestEdge + 1) * this.boundBase + lowerIndex) * this.boundBase +
      upperIndex
    );
  }

  cacheEntry(board, value, bound, bestEdge) {
    // All D4-equivalent positions share one entry. Moves are stored in the
    // canonical orientation and transformed back when the entry is read.
    const symmetry = this.canonicalizePosition(board);
    const canonicalBestEdge =
      bestEdge === InvalidEdge
        ? InvalidEdge
        : this.model.edgeSymmetries[symmetry][bestEdge];
    const cached = this.cache.get(this.canonicalOccupied);
    const entry =
      cached === undefined
        ? {
            bestEdge: InvalidEdge,
            lower: undefined,
            upper: undefined,
          }
        : this.decodeEntry(cached);

    // Lower and upper bounds are merged independently because different
    // alpha-beta windows may tighten opposite sides of the same position.
    if (bound === ExactBound.Exact) {
      entry.lower = value;
      entry.upper = value;
    } else if (bound === ExactBound.Lower) {
      entry.lower =
        entry.lower === undefined ? value : Math.max(entry.lower, value);
    } else {
      entry.upper =
        entry.upper === undefined ? value : Math.min(entry.upper, value);
    }
    if (canonicalBestEdge !== InvalidEdge) {
      entry.bestEdge = canonicalBestEdge;
    }
    this.cache.set(
      this.canonicalOccupied,
      this.encodeEntry(entry.lower, entry.upper, entry.bestEdge),
    );
  }

  findHalfOpenChainPlan(board) {
    this.chainBoard.copyFrom(board);
    const captureEdges = [];

    // A half-open chain has exactly one forced capture at every step. Once
    // found, the entire forced sequence can be searched as one macro move.
    while (this.chainBoard.gaming) {
      let candidate = InvalidEdge;
      let candidateCount = 0;
      for (let edge = 0; edge < this.model.edgeCount; edge += 1) {
        if (
          !this.chainBoard.contains(edge) &&
          this.chainBoard.maxEdgeCount(edge) === 3
        ) {
          candidate = edge;
          candidateCount += 1;
          if (candidateCount > 1) {
            return undefined;
          }
        }
      }
      if (candidateCount === 0) {
        break;
      }
      if (this.chainBoard.add(candidate) !== 1) {
        return undefined;
      }
      captureEdges.push(candidate);
    }

    if (captureEdges.length < 3) {
      return undefined;
    }

    this.chainBoard.copyFrom(board);
    const handoutCaptureCount = captureEdges.length - 2;

    // Stop two boxes before the end and locate the non-scoring move that
    // performs the standard hard-hearted handout while preserving control.
    for (let index = 0; index < handoutCaptureCount; index += 1) {
      if (this.chainBoard.add(captureEdges[index]) !== 1) {
        return undefined;
      }
    }

    const frontier = captureEdges[handoutCaptureCount];
    const firstBox = this.model.edgeBoxA[frontier];
    const secondBox = this.model.edgeBoxB[frontier];
    if (secondBox < 0) {
      return undefined;
    }
    const nextBox =
      this.chainBoard.counters[firstBox] === 2
        ? firstBox
        : this.chainBoard.counters[secondBox] === 2
          ? secondBox
          : -1;
    if (nextBox < 0) {
      return undefined;
    }

    let handoutEdge = InvalidEdge;
    const offset = nextBox * 4;
    for (let index = 0; index < 4; index += 1) {
      const edge = this.model.boxEdges[offset + index];
      if (edge === frontier || this.chainBoard.contains(edge)) {
        continue;
      }
      this.chainProbeBoard.copyFrom(this.chainBoard);
      if (this.chainProbeBoard.add(edge) !== 0) {
        continue;
      }
      if (
        this.chainProbeBoard.counters[firstBox] === 3 &&
        this.chainProbeBoard.counters[secondBox] === 3
      ) {
        let scoringEdge = InvalidEdge;
        let scoringCount = 0;
        for (
          let candidate = 0;
          candidate < this.model.edgeCount;
          candidate += 1
        ) {
          if (
            !this.chainProbeBoard.contains(candidate) &&
            this.chainProbeBoard.maxEdgeCount(candidate) === 3
          ) {
            scoringEdge = candidate;
            scoringCount += 1;
          }
        }
        if (scoringCount !== 1 || scoringEdge !== frontier) {
          continue;
        }
        if (handoutEdge !== InvalidEdge) {
          return undefined;
        }
        handoutEdge = edge;
      }
    }
    if (handoutEdge === InvalidEdge) {
      return undefined;
    }
    return {
      captureEdges,
      handoutCaptureCount,
      handoutEdge,
    };
  }

  applySequence(board, edges) {
    const points = new Uint8Array(edges.length);
    let score = 0;
    for (let index = 0; index < edges.length; index += 1) {
      points[index] = board.add(edges[index]);
      score += points[index];
    }
    return { points, score };
  }

  undoSequence(board, edges, points) {
    for (let index = edges.length - 1; index >= 0; index -= 1) {
      board.undo(edges[index], points[index]);
    }
  }

  exactHalfOpenChainValue(board, plan, initialAlpha, beta) {
    let alpha = initialAlpha;

    // Chain theory reduces this position to two strategic choices: take the
    // whole chain, or give away the final two boxes to retain control.
    const takeAll = this.applySequence(board, plan.captureEdges);
    const takeFuture = this.exactFutureMargin(
      board,
      alpha - takeAll.score,
      beta - takeAll.score,
    );
    this.undoSequence(board, plan.captureEdges, takeAll.points);
    if (takeFuture === ExactSearchAborted) {
      return ExactSearchAborted;
    }
    let best = takeAll.score + takeFuture;
    alpha = Math.max(alpha, best);
    if (alpha >= beta) {
      return best;
    }

    const handoutEdges = [
      ...plan.captureEdges.slice(0, plan.handoutCaptureCount),
      plan.handoutEdge,
    ];
    const handout = this.applySequence(board, handoutEdges);
    const handoutFuture = this.exactFutureMargin(
      board,
      handout.score - beta,
      handout.score - alpha,
    );
    this.undoSequence(board, handoutEdges, handout.points);
    if (handoutFuture === ExactSearchAborted) {
      return ExactSearchAborted;
    }
    best = Math.max(best, handout.score - handoutFuture);
    return best === 0 ? 0 : best;
  }

  exactEdgeValue(board, edge, alpha, beta) {
    const points = board.add(edge);

    // Captures keep the turn, so the child value stays in the same frame.
    // Non-captures pass the turn and require a negated alpha-beta window.
    const future =
      points > 0
        ? this.exactFutureMargin(board, alpha - points, beta - points)
        : this.exactFutureMargin(board, -beta, -alpha);
    board.undo(edge, points);
    if (future === ExactSearchAborted) {
      return ExactSearchAborted;
    }
    const result = points > 0 ? points + future : -future;
    return result === 0 ? 0 : result;
  }

  exactFutureMargin(board, initialAlpha, initialBeta) {
    if (!board.gaming) {
      return 0;
    }
    const loonyValue = board.simpleLoonyValue();
    if (loonyValue !== undefined) {
      return loonyValue;
    }

    let alpha = initialAlpha;
    let beta = initialBeta;

    // Probe the dual-bound transposition table before consuming the node
    // budget. A cached move is also valuable as alpha-beta move ordering.
    const symmetry = this.canonicalizePosition(board);
    const cached = this.cache.get(this.canonicalOccupied);
    let cachedBestEdge = InvalidEdge;
    if (cached !== undefined) {
      const entry = this.decodeEntry(cached);
      const canonicalBestEdge = entry.bestEdge;
      if (canonicalBestEdge !== InvalidEdge) {
        cachedBestEdge =
          this.model.inverseEdgeSymmetries[symmetry][canonicalBestEdge];
        cachedBestEdge = board.nonredundantCornerEdge(cachedBestEdge);
      }
      if (
        entry.lower !== undefined &&
        entry.upper !== undefined &&
        entry.lower === entry.upper
      ) {
        return entry.lower;
      }
      if (entry.lower !== undefined && entry.lower >= initialBeta) {
        return entry.lower;
      }
      if (entry.upper !== undefined && entry.upper <= initialAlpha) {
        return entry.upper;
      }
      if (entry.lower !== undefined) {
        alpha = Math.max(alpha, entry.lower);
      }
      if (entry.upper !== undefined) {
        beta = Math.min(beta, entry.upper);
      }
      if (alpha >= beta) {
        return entry.lower ?? entry.upper;
      }
    }

    if (this.nodesVisited >= this.nodeLimit) {
      return ExactSearchAborted;
    }
    this.nodesVisited += 1;

    const chainPlan = this.findHalfOpenChainPlan(board);
    if (chainPlan !== undefined) {
      const value = this.exactHalfOpenChainValue(board, chainPlan, alpha, beta);
      if (value === ExactSearchAborted) {
        return ExactSearchAborted;
      }
      const bound =
        value <= initialAlpha
          ? ExactBound.Upper
          : value >= initialBeta
            ? ExactBound.Lower
            : ExactBound.Exact;
      this.cacheEntry(board, value, bound, chainPlan.captureEdges[0]);
      return value;
    }

    let best = -this.model.boxCount - 1;
    let bestEdge = InvalidEdge;
    if (cachedBestEdge !== InvalidEdge && !board.contains(cachedBestEdge)) {
      const value = this.exactEdgeValue(board, cachedBestEdge, alpha, beta);
      if (value === ExactSearchAborted) {
        return ExactSearchAborted;
      }
      best = value;
      bestEdge = cachedBestEdge;
      alpha = Math.max(alpha, best);
      if (alpha >= beta) {
        this.cacheEntry(board, best, ExactBound.Lower, bestEdge);
        return best;
      }
    }

    const representativeEdges = board.legalEdgeRepresentatives(false);

    // Captures usually produce the fastest cutoffs, followed by safe moves;
    // sacrifices are searched last because they expose boxes to the opponent.
    for (let moveClass = 0; moveClass < 3; moveClass += 1) {
      for (const edge of representativeEdges) {
        const maximum = board.maxEdgeCount(edge);
        const candidateClass = maximum === 3 ? 0 : maximum < 2 ? 1 : 2;
        if (edge === cachedBestEdge || candidateClass !== moveClass) {
          continue;
        }

        const value = this.exactEdgeValue(board, edge, alpha, beta);
        if (value === ExactSearchAborted) {
          return ExactSearchAborted;
        }
        if (value > best) {
          best = value;
          bestEdge = edge;
        }
        alpha = Math.max(alpha, best);
        if (alpha >= beta) {
          this.cacheEntry(board, best, ExactBound.Lower, bestEdge);
          return best;
        }
      }
    }

    const bound =
      best <= initialAlpha
        ? ExactBound.Upper
        : best >= initialBeta
          ? ExactBound.Lower
          : ExactBound.Exact;
    this.cacheEntry(board, best, bound, bestEdge);
    return best;
  }
}
