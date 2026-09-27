import { LRUCache } from "lru-cache";
import { uniformInt } from "pure-rand/distribution/uniformInt";
import { xoroshiro128plus } from "pure-rand/generator/xoroshiro128plus";

import { InvalidEdge } from "./board";
import { SearchBoard } from "./search-board";

const ExactSearchThreshold = 26;
const ExactCacheLimit = 500_000;
const RolloutBudgetMultiplier = 16;

const ExactBound = Object.freeze({
  Exact: 0,
  Lower: 1,
  Upper: 2,
});

function createRandomSeed() {
  const values = new Uint32Array(1);
  globalThis.crypto?.getRandomValues(values);
  if (values[0] !== 0) {
    return values[0];
  }

  const now = Date.now();
  return (now ^ Math.floor(now / 0x1_0000_0000)) >>> 0;
}

export const PlayerType = Object.freeze({
  Human: "Human",
  Robot: "Robot",
});

export function parsePlayerType(value) {
  return value?.toLowerCase() === PlayerType.Human.toLowerCase()
    ? PlayerType.Human
    : PlayerType.Robot;
}

export class Robot {
  constructor(model) {
    this.model = model;
    this.rootBoard = new SearchBoard(model, true);
    this.greedyBoard = new SearchBoard(model);
    this.evaluationBoard = new SearchBoard(model);
    this.rolloutBoard = new SearchBoard(model);
    this.edgeBuffer = new Int8Array(model.edgeCount);
    this.candidateEdges = new Int8Array(model.edgeCount);
    this.times = new Int32Array(model.edgeCount);
    this.scores = new Int32Array(model.edgeCount);
    this.exactCache = new LRUCache({ max: ExactCacheLimit });
    this.random = xoroshiro128plus(createRandomSeed());
    this.candidateCount = 0;
  }

  move(board) {
    this.rootBoard.loadFrom(board);
    const bestCount = this.findBestEdges(this.rootBoard);
    const index = uniformInt(this.random, 0, bestCount - 1);
    return this.edgeBuffer[index];
  }

  findGreedyCandidates(board) {
    let scoreableCount = 0;
    let safeStart = this.model.edgeCount;
    for (let edge = 0; edge < this.model.edgeCount; edge += 1) {
      if (board.contains(edge)) {
        continue;
      }
      const count = board.maxEdgeCount(edge);
      if (count === 3) {
        this.candidateEdges[scoreableCount++] = edge;
      } else if (count < 2) {
        this.candidateEdges[--safeStart] = edge;
      }
    }

    if (scoreableCount > 0) {
      this.candidateCount = scoreableCount;
      return false;
    }

    this.candidateCount = this.model.edgeCount - safeStart;
    this.candidateEdges.copyWithin(0, safeStart);
    return this.candidateCount > 0;
  }

  findImprovedGreedyCandidates(board) {
    const hasSafeCandidates = this.findGreedyCandidates(board);
    if (hasSafeCandidates || this.candidateCount > 0) {
      return hasSafeCandidates;
    }

    let minimumScore = this.model.boxCount + 1;
    let candidateCount = 0;
    for (let edge = 0; edge < this.model.edgeCount; edge += 1) {
      if (board.contains(edge)) {
        continue;
      }
      this.evaluationBoard.copyFrom(board);
      this.evaluationBoard.add(edge);
      const score = this.evaluationBoard.maxObtainableScore(minimumScore);
      if (score < minimumScore) {
        minimumScore = score;
        candidateCount = 1;
        this.candidateEdges[0] = edge;
      } else if (score === minimumScore) {
        this.candidateEdges[candidateCount++] = edge;
      }
    }
    this.candidateCount = candidateCount;
    return false;
  }

  improvedGreedyMove(board) {
    let result = InvalidEdge;
    for (let edge = 0; edge < this.model.edgeCount; edge += 1) {
      if (board.contains(edge)) {
        continue;
      }
      const count = board.maxEdgeCount(edge);
      if (count === 3) {
        return edge;
      }
      if (count < 2) {
        result = edge;
      }
    }
    if (result !== InvalidEdge) {
      return result;
    }

    const chainEdge = board.smallestChainEdge();
    if (chainEdge !== InvalidEdge) {
      return chainEdge;
    }

    let minimumScore = this.model.boxCount + 1;
    for (let edge = 0; edge < this.model.edgeCount; edge += 1) {
      if (board.contains(edge)) {
        continue;
      }
      const candidate = this.greedyBoard;
      candidate.copyFrom(board);
      candidate.add(edge);
      const score = candidate.maxObtainableScore(minimumScore);
      if (score < minimumScore) {
        minimumScore = score;
        result = edge;
      }
    }
    return result;
  }

  findSimulationCandidates(board) {
    if (this.findImprovedGreedyCandidates(board)) {
      return;
    }

    let candidateCount = 0;
    let maximumScore = -this.model.boxCount;
    const turn = board.turn;
    for (let edge = 0; edge < this.model.edgeCount; edge += 1) {
      if (board.contains(edge)) {
        continue;
      }
      this.evaluationBoard.copyFrom(board);
      this.evaluationBoard.add(edge);
      while (this.evaluationBoard.gaming) {
        const candidate = this.improvedGreedyMove(this.evaluationBoard);
        this.evaluationBoard.add(candidate);
      }

      const score = turn * this.evaluationBoard.score;
      if (score > maximumScore) {
        maximumScore = score;
        candidateCount = 1;
        this.candidateEdges[0] = edge;
      } else if (score === maximumScore) {
        this.candidateEdges[candidateCount++] = edge;
      }
    }
    this.candidateCount = candidateCount;
  }

  randomChoice() {
    const index = uniformInt(this.random, 0, this.candidateCount - 1);
    return this.candidateEdges[index];
  }

  searchOnce(source, rootEdgeCount) {
    this.rolloutBoard.copyFrom(source);
    const index = uniformInt(this.random, 0, rootEdgeCount - 1);
    const firstEdge = this.edgeBuffer[index];
    this.rolloutBoard.add(firstEdge);
    while (this.rolloutBoard.gaming) {
      this.findSimulationCandidates(this.rolloutBoard);
      this.rolloutBoard.add(this.randomChoice());
    }

    const finalScore = source.turn * this.rolloutBoard.score;
    this.times[firstEdge] += 1;
    this.scores[firstEdge] += Math.sign(finalScore);
  }

  exactFutureMargin(board, initialAlpha, initialBeta) {
    if (!board.gaming) {
      return 0;
    }

    const key = board.key;
    let alpha = initialAlpha;
    let beta = initialBeta;
    const cached = this.exactCache.get(key);
    if (cached) {
      if (cached.bound === ExactBound.Exact) {
        return cached.value;
      }
      if (cached.bound === ExactBound.Lower) {
        alpha = Math.max(alpha, cached.value);
      } else {
        beta = Math.min(beta, cached.value);
      }
      if (alpha >= beta) {
        return cached.value;
      }
    }

    let best = -this.model.boxCount - 1;
    for (const scoreable of [true, false]) {
      for (let edge = 0; edge < this.model.edgeCount; edge += 1) {
        if (
          board.contains(edge) ||
          (board.maxEdgeCount(edge) === 3) !== scoreable
        ) {
          continue;
        }

        const points = board.add(edge);
        const value =
          points > 0
            ? points +
              this.exactFutureMargin(board, alpha - points, beta - points)
            : -this.exactFutureMargin(board, -beta, -alpha);
        board.undo(edge, points);
        best = Math.max(best, value);
        alpha = Math.max(alpha, best);
        if (alpha >= beta) {
          this.exactCache.set(key, {
            value: best,
            bound: ExactBound.Lower,
          });
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
    this.exactCache.set(key, { value: best, bound });
    return best;
  }

  exactBestEdges(board) {
    let bestScore = -this.model.boxCount - 1;
    let resultCount = 0;
    for (let edge = 0; edge < this.model.edgeCount; edge += 1) {
      if (board.contains(edge)) {
        continue;
      }

      const points = board.add(edge);
      const score =
        points > 0
          ? points +
            this.exactFutureMargin(
              board,
              -this.model.boxCount,
              this.model.boxCount,
            )
          : -this.exactFutureMargin(
              board,
              -this.model.boxCount,
              this.model.boxCount,
            );
      board.undo(edge, points);

      if (resultCount === 0 || score > bestScore) {
        bestScore = score;
        resultCount = 1;
        this.edgeBuffer[0] = edge;
      } else if (score === bestScore) {
        this.edgeBuffer[resultCount++] = edge;
      }
    }
    return resultCount;
  }

  findBestEdges(board) {
    if (board.remainingSteps <= ExactSearchThreshold) {
      return this.exactBestEdges(board);
    }

    this.findSimulationCandidates(board);
    if (this.candidateCount === 1) {
      this.edgeBuffer[0] = this.candidateEdges[0];
      return 1;
    }

    const rootEdgeCount = this.candidateCount;
    for (let index = 0; index < rootEdgeCount; index += 1) {
      this.edgeBuffer[index] = this.candidateEdges[index];
    }

    const rolloutCount = Math.floor(
      (this.model.edgeCount * RolloutBudgetMultiplier) / board.remainingSteps,
    );
    this.times.fill(0);
    this.scores.fill(0);
    for (let iteration = 0; iteration < rolloutCount; iteration += 1) {
      this.searchOnce(board, rootEdgeCount);
    }

    let resultCount = 0;
    let maximum = 0;
    for (let edge = 0; edge < this.model.edgeCount; edge += 1) {
      if (this.times[edge] === 0) {
        continue;
      }
      const average = this.scores[edge] / this.times[edge];
      if (resultCount === 0 || average > maximum) {
        maximum = average;
        resultCount = 1;
        this.edgeBuffer[0] = edge;
      } else if (average === maximum) {
        this.edgeBuffer[resultCount++] = edge;
      }
    }
    return resultCount;
  }
}
