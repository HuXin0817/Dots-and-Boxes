import { Player1Turn } from "./board";

const LoonyComponent = Object.freeze({
  Chain: 0,
  Loop: 1,
  OpenChain: 2,
  OpenLoop: 3,
  Removed: 4,
});

// The memo key is order-independent because disconnected components form a
// disjoint sum; only each component's type and length affect the recurrence.
function loonyStateKey(lengths, states) {
  const components = [];
  for (let index = 0; index < states.length; index += 1) {
    if (states[index] !== LoonyComponent.Removed) {
      components.push(`${states[index]}:${lengths[index]}`);
    }
  }
  components.sort();
  return components.join(",");
}

function solveLoonyComponents(lengths, states, memo) {
  const key = loonyStateKey(lengths, states);
  if (key.length === 0) {
    return 0;
  }
  const cached = memo.get(key);
  if (cached !== undefined) {
    return cached;
  }

  let best = Number.NEGATIVE_INFINITY;
  let hasOpenComponent = false;
  const considered = new Set();
  for (let index = 0; index < lengths.length; index += 1) {
    const type = states[index];
    if (type !== LoonyComponent.OpenChain && type !== LoonyComponent.OpenLoop) {
      continue;
    }
    hasOpenComponent = true;
    const length = lengths[index];
    const signature = `${type}:${length}`;
    if (considered.has(signature)) {
      continue;
    }
    considered.add(signature);

    states[index] = LoonyComponent.Removed;
    const rest = solveLoonyComponents(lengths, states, memo);
    states[index] = type;

    // The controller either takes the whole opened component or makes the
    // standard hard-hearted handout (two boxes in a chain, four in a loop).
    best = Math.max(best, length + rest);

    const handout = type === LoonyComponent.OpenLoop ? 4 : 2;
    const minimumLength = type === LoonyComponent.OpenLoop ? 4 : 2;
    if (length >= minimumLength) {
      best = Math.max(best, length - 2 * handout - rest);
    }
  }

  if (!hasOpenComponent) {
    considered.clear();
    for (let index = 0; index < lengths.length; index += 1) {
      const type = states[index];
      if (type !== LoonyComponent.Chain && type !== LoonyComponent.Loop) {
        continue;
      }
      const signature = `${type}:${lengths[index]}`;
      if (considered.has(signature)) {
        continue;
      }
      considered.add(signature);
      states[index] =
        type === LoonyComponent.Chain
          ? LoonyComponent.OpenChain
          : LoonyComponent.OpenLoop;

      // With no component already open, the current player must open one and
      // becomes the opponent in the resulting subproblem.
      best = Math.max(best, -solveLoonyComponents(lengths, states, memo));
      states[index] = type;

      if (type === LoonyComponent.Chain && lengths[index] === 2) {
        // Opening the middle edge of a closed 2-chain creates two independent
        // one-box open chains, a case not represented by opening an endpoint.
        lengths[index] = 1;
        states[index] = LoonyComponent.OpenChain;
        lengths.push(1);
        states.push(LoonyComponent.OpenChain);
        best = Math.max(best, -solveLoonyComponents(lengths, states, memo));
        lengths.pop();
        states.pop();
        lengths[index] = 2;
        states[index] = type;
      }
    }
  }

  const result = best === 0 ? 0 : best;
  memo.set(key, result);
  return result;
}

export class SearchBoard {
  constructor(model, trackSymmetries = false) {
    this.model = model;
    this.occupied = new Uint32Array(Math.ceil(model.edgeCount / 32));

    // Exact search can maintain all D4 transforms incrementally. Rollout
    // boards skip this storage and compute a transform only when requested.
    this.symmetryOccupied = undefined;
    if (trackSymmetries) {
      this.symmetryOccupied = [
        this.occupied,
        ...Array.from(
          { length: 7 },
          () => new Uint32Array(this.occupied.length),
        ),
      ];
    }
    this.canonicalCandidate = new Uint32Array(this.occupied.length);
    this.counters = new Uint8Array(model.boxCount);
    this.scoreQueue = new Int8Array(Math.max(1, model.edgeCount * 4));
    this.chainStack = new Int8Array(Math.max(1, model.boxCount));
    this.chainRemaining = new Uint8Array(model.boxCount);
    this.componentLengths = new Uint8Array(model.boxCount);
    this.componentTypes = new Uint8Array(model.boxCount);
    this.step = 0;
    this.score = 0;
    this.turn = Player1Turn;
  }

  get gaming() {
    return this.step < this.model.edgeCount;
  }

  get remainingSteps() {
    return this.model.edgeCount - this.step;
  }

  loadFrom(board) {
    this.step = board.step;
    this.score = board.relativeScore;
    this.turn = board.turn;
    this.occupied.fill(0);
    if (this.symmetryOccupied !== undefined) {
      for (let symmetry = 1; symmetry < 8; symmetry += 1) {
        this.symmetryOccupied[symmetry].fill(0);
      }
    }
    this.counters.set(board.counters);

    for (let index = 0; index < board.step; index += 1) {
      const edge = board.edges[index];
      this.occupied[edge >>> 5] |= 1 << (edge & 31);
      if (this.symmetryOccupied !== undefined) {
        for (let symmetry = 1; symmetry < 8; symmetry += 1) {
          const transformed = this.model.edgeSymmetries[symmetry][edge];
          this.symmetryOccupied[symmetry][transformed >>> 5] |=
            1 << (transformed & 31);
        }
      }
    }
  }

  copyFrom(other) {
    this.step = other.step;
    this.score = other.score;
    this.turn = other.turn;
    this.occupied.set(other.occupied);
    if (this.symmetryOccupied !== undefined) {
      if (other.symmetryOccupied !== undefined) {
        for (let symmetry = 1; symmetry < 8; symmetry += 1) {
          this.symmetryOccupied[symmetry].set(other.symmetryOccupied[symmetry]);
        }
      } else {
        for (let symmetry = 1; symmetry < 8; symmetry += 1) {
          this.model.transformOccupied(
            other.occupied,
            symmetry,
            this.symmetryOccupied[symmetry],
          );
        }
      }
    }
    this.counters.set(other.counters);
  }

  contains(edge) {
    return (this.occupied[edge >>> 5] & (1 << (edge & 31))) !== 0;
  }

  nonredundantCornerEdge(edge) {
    const mate = this.model.cornerEdgeMate[edge];
    if (mate < 0 || this.contains(mate)) {
      return edge;
    }
    return this.model.cornerEdgeRepresentative[edge];
  }

  isRedundantCornerEdge(edge) {
    return this.nonredundantCornerEdge(edge) !== edge;
  }

  canonicalizeOccupied(target) {
    if (this.symmetryOccupied === undefined) {
      return this.model.canonicalizeOccupied(
        this.occupied,
        target,
        this.canonicalCandidate,
      );
    }
    let bestSymmetry = 0;

    // Compare from the most significant word so the selected bitmap is the
    // lexicographically smallest representative of the D4 orbit.
    for (let symmetry = 1; symmetry < 8; symmetry += 1) {
      const candidate = this.symmetryOccupied[symmetry];
      const best = this.symmetryOccupied[bestSymmetry];
      for (let word = best.length - 1; word >= 0; word -= 1) {
        if (candidate[word] === best[word]) {
          continue;
        }
        if (candidate[word] < best[word]) {
          bestSymmetry = symmetry;
        }
        break;
      }
    }
    target.set(this.symmetryOccupied[bestSymmetry]);
    return bestSymmetry;
  }

  symmetryStabilizers() {
    const result = [0];
    for (let symmetry = 1; symmetry < 8; symmetry += 1) {
      let candidate;
      if (this.symmetryOccupied !== undefined) {
        candidate = this.symmetryOccupied[symmetry];
      } else {
        this.model.transformOccupied(
          this.occupied,
          symmetry,
          this.canonicalCandidate,
        );
        candidate = this.canonicalCandidate;
      }
      let equal = true;
      for (let word = 0; word < this.occupied.length; word += 1) {
        if (candidate[word] !== this.occupied[word]) {
          equal = false;
          break;
        }
      }
      if (equal) {
        result.push(symmetry);
      }
    }
    return result;
  }

  symmetryRepresentativeEdge(edge, stabilizers, reduceCorners = true) {
    let representative = reduceCorners
      ? this.nonredundantCornerEdge(edge)
      : edge;
    for (const symmetry of stabilizers ?? this.symmetryStabilizers()) {
      representative = Math.min(
        representative,
        reduceCorners
          ? this.nonredundantCornerEdge(
              this.model.edgeSymmetries[symmetry][edge],
            )
          : this.model.edgeSymmetries[symmetry][edge],
      );
    }
    return representative;
  }

  legalEdgeRepresentatives(reduceSymmetry = true, reduceCorners = true) {
    const stabilizers = reduceSymmetry ? this.symmetryStabilizers() : [0];

    // Only symmetries that preserve the current position may merge actions;
    // using the full D4 group here would incorrectly identify legal choices.
    const result = [];
    for (const edge of this.model.searchEdgeOrder) {
      if (
        this.contains(edge) ||
        (reduceCorners && this.isRedundantCornerEdge(edge))
      ) {
        continue;
      }
      if (
        this.symmetryRepresentativeEdge(edge, stabilizers, reduceCorners) ===
        edge
      ) {
        result.push(edge);
      }
    }
    return result;
  }

  maxEdgeCount(edge) {
    const firstBox = this.model.edgeBoxA[edge];
    const secondBox = this.model.edgeBoxB[edge];
    const first = this.counters[firstBox];
    return secondBox < 0 ? first : Math.max(first, this.counters[secondBox]);
  }

  add(edge) {
    const word = edge >>> 5;
    const bit = 1 << (edge & 31);
    this.occupied[word] |= bit;
    if (this.symmetryOccupied !== undefined) {
      for (let symmetry = 1; symmetry < 8; symmetry += 1) {
        const transformed = this.model.edgeSymmetries[symmetry][edge];
        this.symmetryOccupied[symmetry][transformed >>> 5] |=
          1 << (transformed & 31);
      }
    }
    this.step += 1;

    let points = 0;
    const firstBox = this.model.edgeBoxA[edge];
    const secondBox = this.model.edgeBoxB[edge];
    this.counters[firstBox] += 1;
    if (this.counters[firstBox] === 4) {
      points += 1;
    }
    if (secondBox >= 0) {
      this.counters[secondBox] += 1;
      if (this.counters[secondBox] === 4) {
        points += 1;
      }
    }

    if (points > 0) {
      // Capturing keeps the move, so both score and turn remain expressed in
      // the same player-relative frame used by exact-search backups.
      this.score += points * this.turn;
    } else {
      this.turn = -this.turn;
    }
    return points;
  }

  undo(edge, points) {
    const firstBox = this.model.edgeBoxA[edge];
    const secondBox = this.model.edgeBoxB[edge];
    this.counters[firstBox] -= 1;
    if (secondBox >= 0) {
      this.counters[secondBox] -= 1;
    }

    if (points > 0) {
      this.score -= points * this.turn;
    } else {
      this.turn = -this.turn;
    }
    this.step -= 1;

    const word = edge >>> 5;
    const bit = 1 << (edge & 31);
    this.occupied[word] &= ~bit;
    if (this.symmetryOccupied !== undefined) {
      for (let symmetry = 1; symmetry < 8; symmetry += 1) {
        const transformed = this.model.edgeSymmetries[symmetry][edge];
        this.symmetryOccupied[symmetry][transformed >>> 5] &= ~(
          1 <<
          (transformed & 31)
        );
      }
    }
  }

  maxObtainableScore(endScore) {
    let front = 0;
    let end = 0;
    for (let edge = 0; edge < this.model.edgeCount; edge += 1) {
      if (!this.contains(edge) && this.maxEdgeCount(edge) === 3) {
        this.scoreQueue[end++] = edge;
      }
    }

    let result = 0;

    // This intentionally mutates the board while closing every forced
    // capture. Callers use a disposable board or undo the surrounding move.
    while (this.gaming && result < endScore && front < end) {
      const edge = this.scoreQueue[front++];
      if (this.contains(edge)) {
        continue;
      }
      result += this.add(edge);

      const firstBox = this.model.edgeBoxA[edge];
      const secondBox = this.model.edgeBoxB[edge];
      for (const box of [firstBox, secondBox]) {
        if (box < 0 || this.counters[box] !== 3) {
          continue;
        }
        const offset = box * 4;
        for (let index = 0; index < 4; index += 1) {
          const candidate = this.model.boxEdges[offset + index];
          if (!this.contains(candidate)) {
            this.scoreQueue[end++] = candidate;
            break;
          }
        }
      }
    }
    return result;
  }

  simpleLoonyValue() {
    this.chainRemaining.fill(0);
    let unfinishedCount = 0;
    for (let box = 0; box < this.model.boxCount; box += 1) {
      const count = this.counters[box];
      if (count === 4) {
        continue;
      }
      if (count < 2) {
        // A box with more than two free sides introduces branching, so the
        // chain/loop component recurrence is not exact for this position.
        return undefined;
      }
      this.chainRemaining[box] = 1;
      unfinishedCount += 1;
    }
    if (unfinishedCount === 0) {
      return undefined;
    }

    let componentCount = 0;
    for (let firstBox = 0; firstBox < this.model.boxCount; firstBox += 1) {
      if (this.chainRemaining[firstBox] === 0) {
        continue;
      }

      let stackCount = 0;
      this.chainStack[stackCount++] = firstBox;
      this.chainRemaining[firstBox] = 0;
      let length = 0;
      let boundaryEnds = 0;
      let openEnds = 0;

      while (stackCount > 0) {
        const box = this.chainStack[--stackCount];
        length += 1;
        if (this.counters[box] === 3) {
          openEnds += 1;
        }
        const offset = box * 4;
        for (let index = 0; index < 4; index += 1) {
          const edge = this.model.boxEdges[offset + index];
          if (this.contains(edge)) {
            continue;
          }

          const first = this.model.edgeBoxA[edge];
          const second = this.model.edgeBoxB[edge];
          const neighbor = first === box ? second : first;
          if (neighbor < 0) {
            boundaryEnds += 1;
          } else if (this.chainRemaining[neighbor] !== 0) {
            this.chainRemaining[neighbor] = 0;
            this.chainStack[stackCount++] = neighbor;
          }
        }
      }

      let type;

      // Boundary ends identify chains, while already capturable boxes identify
      // open ends. Any other topology falls back to the general search.
      if (openEnds === 0 && boundaryEnds === 2) {
        type = LoonyComponent.Chain;
      } else if (openEnds === 0 && boundaryEnds === 0 && length >= 4) {
        type = LoonyComponent.Loop;
      } else if (openEnds === 1 && boundaryEnds === 1) {
        type = LoonyComponent.OpenChain;
      } else if (openEnds === 2 && boundaryEnds === 0) {
        type = LoonyComponent.OpenLoop;
      } else {
        return undefined;
      }
      this.componentLengths[componentCount] = length;
      this.componentTypes[componentCount] = type;
      componentCount += 1;
    }

    const lengths = Array.from(
      this.componentLengths.subarray(0, componentCount),
    );
    const states = Array.from(this.componentTypes.subarray(0, componentCount));
    return solveLoonyComponents(lengths, states, new Map());
  }
}
