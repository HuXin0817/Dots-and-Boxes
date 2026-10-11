import { ExactSolver } from "./exact-solver";
import { PuctSearch } from "./puct-search";
import { RuleEvaluator } from "./rule-evaluator";
import { SearchBoard } from "./search-board";

const DefaultRobotOptions = Object.freeze({
  exactCacheLimit: 500000,
  exactNodeLimit: 2000000,
  exactSearchThreshold: 30,
  exploration: 2,
  iterations: 1024,
  leafExactNodeLimit: 20000,
  leafExactSearchThreshold: 16,
  maxTreeNodes: 250000,
  seed: undefined,
});

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
  constructor(model, options = {}) {
    const config = { ...DefaultRobotOptions, ...options };
    if (!Number.isInteger(config.iterations) || config.iterations < 2) {
      throw new RangeError("PUCT iterations must be an integer >= 2");
    }
    if (!Number.isFinite(config.exploration) || config.exploration <= 0) {
      throw new RangeError("PUCT exploration must be positive");
    }

    this.exactSearchThreshold = config.exactSearchThreshold;
    this.rootExactNodeLimit = config.exactNodeLimit;
    this.rootBoard = new SearchBoard(model, true);
    this.exactSolver = new ExactSolver(model, {
      cacheLimit: config.exactCacheLimit,
    });
    this.evaluator = new RuleEvaluator(model, { seed: config.seed });
    this.puctSearch = new PuctSearch(
      model,
      this.evaluator,
      this.exactSolver,
      config,
    );
  }

  move(board) {
    this.rootBoard.loadFrom(board);
    const rootLoonyValue = this.rootBoard.simpleLoonyValue();

    // Prefer a proven move near the endgame. Pure chain-and-loop positions
    // bypass the node cap because their component value is already exact.
    if (
      this.rootBoard.remainingSteps <= this.exactSearchThreshold ||
      rootLoonyValue !== undefined
    ) {
      const candidates = this.exactSolver.bestEdges(
        this.rootBoard,
        rootLoonyValue === undefined
          ? this.rootExactNodeLimit
          : Number.POSITIVE_INFINITY,
      );
      if (candidates.length > 0) {
        return this.puctSearch.choose(candidates);
      }
    }

    // Fall back to PUCT when a full root solve is outside the configured
    // horizon or exhausts its node budget.
    return this.puctSearch.bestEdge(this.rootBoard);
  }
}
