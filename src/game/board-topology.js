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
    this.createSearchMetadata();
    this.createSymmetryMapper();
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

  createSearchMetadata() {
    this.cornerEdgeMate = new Int16Array(this.edgeCount);
    this.cornerEdgeRepresentative = new Int16Array(this.edgeCount);
    this.cornerEdgeMate.fill(-1);
    this.cornerEdgeRepresentative.fill(-1);

    const cornerBoxes = [
      0,
      this.size - 1,
      (this.size - 1) * this.size,
      this.boxCount - 1,
    ];
    for (const box of new Set(cornerBoxes)) {
      const boundaryEdges = [];
      const offset = box * 4;
      for (let index = 0; index < 4; index += 1) {
        const edge = this.boxEdges[offset + index];
        if (this.edgeBoxB[edge] < 0) {
          boundaryEdges.push(edge);
        }
      }
      if (boundaryEdges.length !== 2) {
        continue;
      }
      const [first, second] = boundaryEdges;
      const representative = Math.min(first, second);
      this.cornerEdgeMate[first] = second;
      this.cornerEdgeMate[second] = first;
      this.cornerEdgeRepresentative[first] = representative;
      this.cornerEdgeRepresentative[second] = representative;
    }

    const center = this.size / 2;
    const order = Array.from({ length: this.edgeCount }, (_, edge) => edge);
    order.sort((left, right) => {
      const leftX =
        (this.dotX(this.edgeDot1[left]) + this.dotX(this.edgeDot2[left])) / 2;
      const leftY =
        (this.dotY(this.edgeDot1[left]) + this.dotY(this.edgeDot2[left])) / 2;
      const rightX =
        (this.dotX(this.edgeDot1[right]) + this.dotX(this.edgeDot2[right])) / 2;
      const rightY =
        (this.dotY(this.edgeDot1[right]) + this.dotY(this.edgeDot2[right])) / 2;
      const leftDistance = (leftX - center) ** 2 + (leftY - center) ** 2;
      const rightDistance = (rightX - center) ** 2 + (rightY - center) ** 2;
      return leftDistance - rightDistance || left - right;
    });
    this.searchEdgeOrder = Int16Array.from(order);
  }

  transformDot(dot, symmetry) {
    const size = this.size;
    let x = this.dotX(dot);
    let y = this.dotY(dot);
    if (symmetry >= 4) {
      y = size - y;
    }
    for (let rotation = symmetry & 3; rotation > 0; rotation -= 1) {
      const nextX = y;
      y = size - x;
      x = nextX;
    }
    return x * (size + 1) + y;
  }

  createSymmetryMapper() {
    this.edgeSymmetries = Array.from(
      { length: 8 },
      () => new Int8Array(this.edgeCount),
    );
    this.inverseEdgeSymmetries = Array.from(
      { length: 8 },
      () => new Int8Array(this.edgeCount),
    );

    for (let symmetry = 0; symmetry < 8; symmetry += 1) {
      const mapping = this.edgeSymmetries[symmetry];
      const inverse = this.inverseEdgeSymmetries[symmetry];
      for (let edge = 0; edge < this.edgeCount; edge += 1) {
        const first = this.transformDot(this.edgeDot1[edge], symmetry);
        const second = this.transformDot(this.edgeDot2[edge], symmetry);
        const transformed = this.edgeFromDots(
          Math.min(first, second),
          Math.max(first, second),
        );
        mapping[edge] = transformed;
        inverse[transformed] = edge;
      }
    }
  }

  transformOccupied(occupied, symmetry, target) {
    target.fill(0);
    const mapping = this.edgeSymmetries[symmetry];
    for (let edge = 0; edge < this.edgeCount; edge += 1) {
      if ((occupied[edge >>> 5] & (1 << (edge & 31))) === 0) {
        continue;
      }
      const transformed = mapping[edge];
      target[transformed >>> 5] |= 1 << (transformed & 31);
    }
  }

  canonicalizeOccupied(occupied, target, candidate) {
    target.set(occupied);
    let bestSymmetry = 0;
    for (let symmetry = 1; symmetry < 8; symmetry += 1) {
      this.transformOccupied(occupied, symmetry, candidate);
      let comparison = 0;
      for (let word = target.length - 1; word >= 0; word -= 1) {
        if (candidate[word] === target[word]) {
          continue;
        }
        comparison = candidate[word] < target[word] ? -1 : 1;
        break;
      }
      if (comparison < 0) {
        target.set(candidate);
        bestSymmetry = symmetry;
      }
    }
    return bestSymmetry;
  }
}
