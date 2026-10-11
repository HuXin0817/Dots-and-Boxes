import { InvalidEdge } from "./board";
import { Random } from "./random";
import { SearchBoard } from "./search-board";

export class PuctSearch {
  constructor(model, evaluator, exactSolver, options = {}) {
    this.model = model;
    this.evaluator = evaluator;
    this.exactSolver = exactSolver;
    this.iterations = options.iterations ?? 1024;
    this.exploration = options.exploration ?? 2;
    this.leafExactSearchThreshold = options.leafExactSearchThreshold ?? 16;
    this.leafExactNodeLimit = options.leafExactNodeLimit ?? 20000;
    this.maxTreeNodes = options.maxTreeNodes ?? 250000;
    this.random = new Random(options.seed);
    this.simulationBoard = new SearchBoard(model);
    this.nodes = new Map();
  }

  bestEdge(rootBoard) {
    // The graph persists across moves, but is discarded as a whole when its
    // memory cap is exceeded. Occupancy keys let transpositions share nodes.
    if (this.nodes.size > this.maxTreeNodes) {
      this.nodes.clear();
    }
    const root = this.getNode(rootBoard);
    for (let iteration = 0; iteration < this.iterations; iteration += 1) {
      this.simulate(root, rootBoard);
      if (root.solvedValue !== undefined) {
        break;
      }
    }

    if (root.expanded && root.solvedValue !== undefined) {
      // Once proof propagation solves the root, choose only edges whose exact
      // backed-up value matches the proven minimax value.
      const candidateEdges = [];
      for (let index = 0; index < root.edges.length; index += 1) {
        if (root.edgeSolvedValues[index] === root.solvedValue) {
          candidateEdges.push(root.edges[index]);
        }
      }
      if (candidateEdges.length > 0) {
        return this.choose(candidateEdges);
      }
    }

    if (!root.expanded && root.solvedValue !== undefined) {
      // A root can be solved before expansion by chain theory or the exact
      // leaf solver. Recover a concrete move with an unrestricted root solve.
      const edges = this.exactSolver.bestEdges(rootBoard);
      return edges[0] ?? InvalidEdge;
    }
    if (!root.expanded) {
      throw new Error("PUCT root was not expanded");
    }
    return this.choose(this.rootResult(root));
  }

  choose(edges) {
    if (edges.length === 0) {
      return InvalidEdge;
    }
    return edges[this.random.int(edges.length)];
  }

  createNode() {
    return {
      edges: undefined,
      priors: undefined,
      children: undefined,
      edgeVisits: undefined,
      edgeValueSums: undefined,

      // Statistical estimates and exact proofs stay separate: a noisy rollout
      // must never make a node appear mathematically solved.
      edgeSolvedValues: undefined,
      visits: 0,
      solvedValue: undefined,
      expanded: false,
    };
  }

  stateKey(board) {
    // Values are future margins, so historical scores do not affect the key.
    return Array.from(board.occupied).join(",");
  }

  getNode(board) {
    const key = this.stateKey(board);
    let node = this.nodes.get(key);
    if (node === undefined) {
      node = this.createNode();
      this.nodes.set(key, node);
    }
    return node;
  }

  backupValue(childValue, points) {
    // A capture grants another move; a non-capture changes perspective.
    return points > 0 ? points / this.model.boxCount + childValue : -childValue;
  }

  exactValue(board) {
    const future = this.exactSolver.value(board, this.leafExactNodeLimit);

    // PUCT stores normalized future margins, matching rollout values and
    // allowing the same node to be reused across different score histories.
    return future === undefined ? undefined : future / this.model.boxCount;
  }

  evaluateAndExpand(node, board) {
    if (!board.gaming) {
      node.solvedValue = 0;
      return node.solvedValue;
    }
    if (node.solvedValue !== undefined) {
      return node.solvedValue;
    }

    const loonyValue = board.simpleLoonyValue();
    if (loonyValue !== undefined) {
      // Pure chain-and-loop positions have an exact component-game value and
      // should enter the tree as proofs rather than rollout estimates.
      node.solvedValue = loonyValue / this.model.boxCount;
      return node.solvedValue;
    }
    if (board.remainingSteps <= this.leafExactSearchThreshold) {
      const value = this.exactValue(board);
      if (value !== undefined) {
        node.solvedValue = value;
        return value;
      }
    }

    if (!node.expanded) {
      const prediction = this.evaluator.evaluate(board);

      // Merge actions that are equivalent under symmetries preserving this
      // position; every omitted edge has the same value as its representative.
      const edges = board.legalEdgeRepresentatives();
      node.edges = Int16Array.from(edges);
      node.priors = new Float64Array(edges.length);
      node.children = new Array(edges.length);
      node.edgeVisits = new Uint32Array(edges.length);
      node.edgeValueSums = new Float64Array(edges.length);
      node.edgeSolvedValues = new Float64Array(edges.length);
      node.edgeSolvedValues.fill(Number.NaN);
      node.expanded = true;

      let total = 0;
      for (let index = 0; index < edges.length; index += 1) {
        const prior = Math.max(
          Number.EPSILON,
          Number(prediction.policy[edges[index]]) || 0,
        );
        node.priors[index] = prior;
        total += prior;
      }
      for (let index = 0; index < edges.length; index += 1) {
        node.priors[index] /= total;
      }

      // The first visit returns the rollout estimate. Later simulations descend
      // through the newly initialized edge statistics.
      return Math.max(-1, Math.min(1, prediction.value));
    }

    return 0;
  }

  selectAction(node, isRoot) {
    if (isRoot) {
      // Force one visit to every root representative before PUCT competition;
      // otherwise a weak prior can permanently hide a critical move.
      const unvisited = [];
      for (let index = 0; index < node.edges.length; index += 1) {
        if (node.edgeVisits[index] === 0) {
          unvisited.push(index);
        }
      }
      if (unvisited.length > 0) {
        return unvisited[this.random.int(unvisited.length)];
      }
    }

    const parentScale = Math.sqrt(Math.max(1, node.visits));
    let bestScore = Number.NEGATIVE_INFINITY;
    const best = [];
    for (let index = 0; index < node.edges.length; index += 1) {
      const visits = node.edgeVisits[index];
      const quality = visits === 0 ? 0 : node.edgeValueSums[index] / visits;
      const exploration =
        (this.exploration * node.priors[index] * parentScale) / (1 + visits);
      const score = quality + exploration;
      if (score > bestScore) {
        bestScore = score;
        best.length = 0;
        best.push(index);
      } else if (score === bestScore) {
        best.push(index);
      }
    }
    return best[this.random.int(best.length)];
  }

  updateSolvedValue(node) {
    if (!node.expanded) {
      return undefined;
    }
    let best = Number.NEGATIVE_INFINITY;
    for (const value of node.edgeSolvedValues) {
      if (Number.isNaN(value)) {
        // Maximization is proven only when every representative action is
        // solved; any unknown sibling could still improve the node value.
        return undefined;
      }
      best = Math.max(best, value);
    }
    node.solvedValue = best === 0 ? 0 : best;
    return node.solvedValue;
  }

  simulate(root, rootBoard) {
    this.simulationBoard.copyFrom(rootBoard);
    let node = root;
    const path = [];

    while (
      node.expanded &&
      node.solvedValue === undefined &&
      this.simulationBoard.gaming
    ) {
      const actionIndex = this.selectAction(node, node === root);
      const edge = node.edges[actionIndex];
      const points = this.simulationBoard.add(edge);
      let child = node.children[actionIndex];
      if (child === undefined) {
        child = this.getNode(this.simulationBoard);
        node.children[actionIndex] = child;
      }
      path.push({ actionIndex, node, points });
      node = child;
    }

    let value = this.evaluateAndExpand(node, this.simulationBoard);
    let solvedValue = node.solvedValue;
    node.visits += 1;

    // Back up rollout means on every visit, but propagate proofs only from
    // exact leaves. Captures preserve perspective; other moves negate it.
    for (let index = path.length - 1; index >= 0; index -= 1) {
      const step = path[index];
      value = this.backupValue(value, step.points);
      step.node.edgeVisits[step.actionIndex] += 1;
      step.node.edgeValueSums[step.actionIndex] += value;
      if (solvedValue !== undefined) {
        const parentSolvedValue = this.backupValue(solvedValue, step.points);
        step.node.edgeSolvedValues[step.actionIndex] = parentSolvedValue;
        solvedValue = this.updateSolvedValue(step.node);
      }
      step.node.visits += 1;
    }
  }

  rootResult(root) {
    let maximumVisits = -1;
    let maximumValue = Number.NEGATIVE_INFINITY;
    const bestEdges = [];

    // Visit count is the robust MCTS decision rule; mean value breaks ties.
    for (let index = 0; index < root.edges.length; index += 1) {
      const edge = root.edges[index];
      const visits = root.edgeVisits[index];
      const value =
        visits === 0
          ? Number.NEGATIVE_INFINITY
          : root.edgeValueSums[index] / visits;

      if (
        visits > maximumVisits ||
        (visits === maximumVisits && value > maximumValue)
      ) {
        maximumVisits = visits;
        maximumValue = value;
        bestEdges.length = 0;
        bestEdges.push(edge);
      } else if (visits === maximumVisits && value === maximumValue) {
        bestEdges.push(edge);
      }
    }
    return bestEdges;
  }
}
