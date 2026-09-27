import { InvalidEdge, Player1Turn } from "./board";

export class SearchBoard {
  constructor(model) {
    this.model = model;
    this.occupied = new Uint32Array(Math.ceil(model.edgeCount / 32));
    this.counters = new Uint8Array(model.boxCount);
    this.scoreQueue = new Int8Array(Math.max(1, model.edgeCount * 4));
    this.chainStack = new Int8Array(Math.max(1, model.boxCount));
    this.chainRemaining = new Uint8Array(model.boxCount);
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
    this.counters.set(board.counters);

    for (let index = 0; index < board.step; index += 1) {
      const edge = board.edges[index];
      this.occupied[edge >>> 5] |= 1 << (edge & 31);
    }
  }

  copyFrom(other) {
    this.step = other.step;
    this.score = other.score;
    this.turn = other.turn;
    this.occupied.set(other.occupied);
    this.counters.set(other.counters);
  }

  contains(edge) {
    return (this.occupied[edge >>> 5] & (1 << (edge & 31))) !== 0;
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

  smallestChainEdge() {
    this.chainRemaining.fill(0);
    let remainingCount = 0;
    for (let box = 0; box < this.model.boxCount; box += 1) {
      const count = this.counters[box];
      if (count === 4) {
        continue;
      }
      if (count !== 2) {
        return InvalidEdge;
      }
      this.chainRemaining[box] = 1;
      remainingCount += 1;
    }
    if (remainingCount === 0) {
      return InvalidEdge;
    }

    let bestCount = this.model.boxCount + 1;
    let bestEdge = this.model.edgeCount;
    for (let firstBox = 0; firstBox < this.model.boxCount; firstBox += 1) {
      if (this.chainRemaining[firstBox] === 0) {
        continue;
      }

      let stackCount = 0;
      this.chainStack[stackCount++] = firstBox;
      this.chainRemaining[firstBox] = 0;
      let componentCount = 0;
      let componentEdge = this.model.edgeCount;

      while (stackCount > 0) {
        const box = this.chainStack[--stackCount];
        componentCount += 1;
        const offset = box * 4;
        for (let index = 0; index < 4; index += 1) {
          const edge = this.model.boxEdges[offset + index];
          if (this.contains(edge)) {
            continue;
          }
          componentEdge = Math.min(componentEdge, edge);

          const first = this.model.edgeBoxA[edge];
          const second = this.model.edgeBoxB[edge];
          const neighbor = first === box ? second : first;
          if (neighbor >= 0 && this.chainRemaining[neighbor] !== 0) {
            this.chainRemaining[neighbor] = 0;
            this.chainStack[stackCount++] = neighbor;
          }
        }
      }

      if (
        componentCount < bestCount ||
        (componentCount === bestCount && componentEdge < bestEdge)
      ) {
        bestCount = componentCount;
        bestEdge = componentEdge;
      }
    }
    return bestEdge < this.model.edgeCount ? bestEdge : InvalidEdge;
  }
}
