export class BoardTopology {
  constructor(size) {
    this.size = size;
    this.dotCount = (size + 1) * (size + 1);
    this.edgeCount = 2 * size * (size + 1);
    this.boxCount = size * size;
    this.edgeDot1 = new Int8Array(this.edgeCount);
    this.edgeDot2 = new Int8Array(this.edgeCount);
    this.edgeBoxA = new Int8Array(this.edgeCount);
    this.edgeBoxB = new Int8Array(this.edgeCount);
    this.boxEdges = new Int8Array(this.boxCount * 4);
    this.edgeBoxA.fill(-1);
    this.edgeBoxB.fill(-1);

    this.createEdgeMapper();
    this.createBoxMapper();
  }

  dotX(dot) {
    return Math.floor(dot / (this.size + 1));
  }

  dotY(dot) {
    return dot % (this.size + 1);
  }

  boxX(box) {
    return Math.floor(box / this.size);
  }

  boxY(box) {
    return box % this.size;
  }

  edgeRotated(edge) {
    return (edge & 1) !== 0;
  }

  edgeFromDots(dot1, dot2) {
    if (dot2 - dot1 === 1) {
      return 2 * (dot1 - Math.floor(dot1 / (this.size + 1))) + 1;
    }
    return 2 * dot1;
  }

  createEdgeMapper() {
    for (let edge = 0; edge < this.edgeCount; edge += 1) {
      const baseDot = edge >> 1;
      if ((edge & 1) !== 0) {
        const dot1 = baseDot + Math.floor(baseDot / this.size);
        this.edgeDot1[edge] = dot1;
        this.edgeDot2[edge] = dot1 + 1;
      } else {
        this.edgeDot1[edge] = baseDot;
        this.edgeDot2[edge] = baseDot + this.size + 1;
      }
    }
  }

  createBoxMapper() {
    for (let x = 0; x < this.size; x += 1) {
      for (let y = 0; y < this.size; y += 1) {
        const box = x * this.size + y;
        const topLeft = x * (this.size + 1) + y;
        const topRight = (x + 1) * (this.size + 1) + y;
        const bottomLeft = topLeft + 1;
        const bottomRight = topRight + 1;
        const offset = box * 4;
        const edges = [
          this.edgeFromDots(topLeft, topRight),
          this.edgeFromDots(topLeft, bottomLeft),
          this.edgeFromDots(bottomLeft, bottomRight),
          this.edgeFromDots(topRight, bottomRight),
        ].sort((left, right) => left - right);

        for (let index = 0; index < 4; index += 1) {
          const edge = edges[index];
          this.boxEdges[offset + index] = edge;
          if (this.edgeBoxA[edge] === -1) {
            this.edgeBoxA[edge] = box;
          } else {
            this.edgeBoxB[edge] = box;
          }
        }
      }
    }
  }
}
