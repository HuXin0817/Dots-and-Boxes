import { InvalidEdge } from "./board";
import { Random } from "./random";
import { SearchBoard } from "./search-board";

const PolicySharpness = 1.5;
const PolicyUniformMix = 0.1;

export class RuleEvaluator {
  constructor(model, options = {}) {
    this.model = model;
    this.random = new Random(options.seed);
    this.rolloutBoard = new SearchBoard(model);
    this.policyBoard = new SearchBoard(model);
    this.evaluationBoard = new SearchBoard(model);
    this.chainProbeBoard = new SearchBoard(model);
    this.candidateEdges = new Int8Array(model.edgeCount);
    this.candidateCount = 0;
  }

  evaluate(board) {
    return {
      policy: this.policy(board),
      value: this.rollout(board),
    };
  }

  findGreedyCandidates(board) {
    const handoutEdge = this.findSimpleHandoutEdge(board);
    if (handoutEdge !== InvalidEdge) {
      this.candidateEdges[0] = handoutEdge;
      this.candidateCount = 1;
      return false;
    }

    let scoreableCount = 0;
    let safeStart = this.model.edgeCount;

    // Reuse one buffer from both ends: captures grow from the front and safe
    // moves from the back. Captures always take precedence when available.
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

  findSimpleHandoutEdge(board) {
    // Recognize the local two-box handout only when there is one forced capture.
    // More open captures require global chain analysis and are left to search.
    let captureEdge = InvalidEdge;
    let captureCount = 0;
    for (let edge = 0; edge < this.model.edgeCount; edge += 1) {
      if (!board.contains(edge) && board.maxEdgeCount(edge) === 3) {
        captureEdge = edge;
        captureCount += 1;
        if (captureCount > 1) {
          return InvalidEdge;
        }
      }
    }
    if (captureCount !== 1) {
      return InvalidEdge;
    }

    const firstBox = this.model.edgeBoxA[captureEdge];
    const secondBox = this.model.edgeBoxB[captureEdge];
    if (secondBox < 0) {
      return InvalidEdge;
    }
    const firstCount = board.counters[firstBox];
    const secondCount = board.counters[secondBox];
    if (
      !(
        (firstCount === 3 && secondCount === 2) ||
        (firstCount === 2 && secondCount === 3)
      )
    ) {
      return InvalidEdge;
    }
    const finalBox = firstCount === 2 ? firstBox : secondBox;
    const offset = finalBox * 4;
    for (let index = 0; index < 4; index += 1) {
      const edge = this.model.boxEdges[offset + index];
      if (edge === captureEdge || board.contains(edge)) {
        continue;
      }
      this.chainProbeBoard.copyFrom(board);
      if (
        this.chainProbeBoard.add(edge) !== 0 ||
        this.chainProbeBoard.add(captureEdge) !== 2
      ) {
        continue;
      }
      const restValue = this.chainProbeBoard.gaming
        ? this.chainProbeBoard.simpleLoonyValue()
        : 0;

      // The sacrifice is useful only when taking the two offered boxes leaves
      // the opponent worse off in the exact remaining component game.
      if (restValue !== undefined && restValue < -2) {
        return edge;
      }
    }
    return InvalidEdge;
  }

  findImprovedGreedyCandidates(board) {
    const hasSafeCandidates = this.findGreedyCandidates(board);
    if (hasSafeCandidates || this.candidateCount > 0) {
      return hasSafeCandidates;
    }

    let minimumScore = this.model.boxCount + 1;
    let candidateCount = 0;

    // If every move is unsafe, minimize the opponent's complete forced-capture
    // closure rather than considering only the first box conceded.
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

  randomChoice() {
    const index = this.random.int(this.candidateCount);
    return this.candidateEdges[index];
  }

  completedBoxes(board, edge) {
    let points = board.counters[board.model.edgeBoxA[edge]] === 3 ? 1 : 0;
    const second = board.model.edgeBoxB[edge];
    if (second >= 0 && board.counters[second] === 3) {
      points += 1;
    }
    return points;
  }

  edgeLogit(board, edge, handoutEdge) {
    if (edge === handoutEdge) {
      return 8;
    }
    const points = this.completedBoxes(board, edge);
    if (points > 0) {
      return 5 + points;
    }
    if (board.maxEdgeCount(edge) < 2) {
      return 2;
    }

    this.policyBoard.copyFrom(board);
    this.policyBoard.add(edge);

    // Estimate how many boxes this sacrifice immediately exposes.
    const forced = this.policyBoard.maxObtainableScore(this.model.boxCount + 1);
    return -forced;
  }

  policy(board) {
    const probabilities = new Float64Array(this.model.edgeCount);
    const handoutEdge = this.findSimpleHandoutEdge(board);
    let maximum = Number.NEGATIVE_INFINITY;
    for (let edge = 0; edge < this.model.edgeCount; edge += 1) {
      if (board.contains(edge)) {
        continue;
      }
      const logit = PolicySharpness * this.edgeLogit(board, edge, handoutEdge);
      probabilities[edge] = logit;
      maximum = Math.max(maximum, logit);
    }

    let total = 0;

    // Subtracting the maximum keeps softmax numerically stable.
    for (let edge = 0; edge < this.model.edgeCount; edge += 1) {
      if (board.contains(edge)) {
        continue;
      }
      const weight = Math.exp(probabilities[edge] - maximum);
      probabilities[edge] = weight;
      total += weight;
    }

    // Keep a uniform floor so no legal move becomes unreachable to PUCT.
    for (let edge = 0; edge < this.model.edgeCount; edge += 1) {
      if (board.contains(edge)) {
        continue;
      }
      probabilities[edge] =
        (1 - PolicyUniformMix) * (probabilities[edge] / total) +
        PolicyUniformMix / (this.model.edgeCount - board.step);
    }
    return probabilities;
  }

  rollout(source) {
    this.rolloutBoard.copyFrom(source);
    let finalScore;
    while (this.rolloutBoard.gaming) {
      const loonyValue = this.rolloutBoard.simpleLoonyValue();
      if (loonyValue !== undefined) {
        // Replace stochastic play with the exact component-game value as soon
        // as only recognized chains and loops remain.
        finalScore =
          this.rolloutBoard.score + this.rolloutBoard.turn * loonyValue;
        break;
      }
      this.findImprovedGreedyCandidates(this.rolloutBoard);
      this.rolloutBoard.add(this.randomChoice());
    }
    // Convert the absolute board score into a normalized future margin from
    // the source player-to-move perspective.
    return (
      (source.turn * ((finalScore ?? this.rolloutBoard.score) - source.score)) /
      this.model.boxCount
    );
  }
}
