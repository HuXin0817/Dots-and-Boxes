export const Player1Turn = 1;
const Player2Turn = -1;
export const InvalidEdge = -1;

export const Owner = Object.freeze({
  None: 0,
  Player1: 1,
  Player2: 2,
});

export class Board {
  constructor(model) {
    this.model = model;
    this.edges = new Int8Array(model.edgeCount);
    this.edgeIndexes = new Int8Array(model.edgeCount);
    this.counters = new Uint8Array(model.boxCount);
    this.edgeOwners = new Uint8Array(model.edgeCount);
    this.boxOwners = new Uint8Array(model.boxCount);
    this.step = 0;
    this.relativeScore = 0;
    this.totalScore = 0;
    this.turn = Player1Turn;
    this.reset();
  }

  reset() {
    this.step = 0;
    this.relativeScore = 0;
    this.totalScore = 0;
    this.turn = Player1Turn;
    this.counters.fill(0);
    this.edgeOwners.fill(Owner.None);
    this.boxOwners.fill(Owner.None);
    for (let edge = 0; edge < this.model.edgeCount; edge += 1) {
      this.edges[edge] = edge;
      this.edgeIndexes[edge] = edge;
    }
  }

  add(edge) {
    const edgeIndex = this.edgeIndexes[edge];
    const displacedEdge = this.edges[this.step];
    this.edges[this.step] = edge;
    this.edges[edgeIndex] = displacedEdge;
    this.edgeIndexes[edge] = this.step;
    this.edgeIndexes[displacedEdge] = edgeIndex;
    this.step += 1;

    const owner = this.turn === Player1Turn ? Owner.Player1 : Owner.Player2;
    this.edgeOwners[edge] = owner;

    let score = 0;
    const boxA = this.model.edgeBoxA[edge];
    const boxB = this.model.edgeBoxB[edge];
    score += this.incrementBox(boxA, owner);
    if (boxB !== -1) {
      score += this.incrementBox(boxB, owner);
    }

    if (score > 0) {
      this.relativeScore += score * this.turn;
    } else {
      this.turn = -this.turn;
    }
    this.totalScore += score;
    return score;
  }

  contains(edge) {
    return this.edgeIndexes[edge] < this.step;
  }

  gaming() {
    return this.step < this.model.edgeCount;
  }

  isPlayer1Turn() {
    return this.turn === Player1Turn;
  }

  isPlayer2Turn() {
    return this.turn === Player2Turn;
  }

  player1Score() {
    return (this.totalScore + this.relativeScore) / 2;
  }

  player2Score() {
    return (this.totalScore - this.relativeScore) / 2;
  }

  lastEdge() {
    return this.step === 0 ? InvalidEdge : this.edges[this.step - 1];
  }

  edgeOwner(edge) {
    return this.edgeOwners[edge];
  }

  boxOwner(box) {
    return this.boxOwners[box];
  }

  toSnapshot() {
    return {
      size: this.model.size,
      moves: Array.from(this.edges.subarray(0, this.step)),
    };
  }

  incrementBox(box, owner) {
    const count = ++this.counters[box];
    if (count !== 4) {
      return 0;
    }
    this.boxOwners[box] = owner;
    return 1;
  }
}
