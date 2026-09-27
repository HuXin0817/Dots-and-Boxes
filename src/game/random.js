const Uint32Range = 0x1_0000_0000;

function rotateLeft(value, shift) {
  return ((value << shift) | (value >>> (32 - shift))) >>> 0;
}

function createSeed() {
  const values = new Uint32Array(1);
  globalThis.crypto?.getRandomValues?.(values);
  if (values[0] !== 0) {
    return values[0];
  }

  const now = Date.now();
  return (now ^ Math.floor(now / Uint32Range)) >>> 0;
}

function splitMix32(seed) {
  let state = seed >>> 0;
  return () => {
    state = (state + 0x9e3779b9) >>> 0;
    let value = state;
    value = Math.imul(value ^ (value >>> 16), 0x21f0aaad);
    value = Math.imul(value ^ (value >>> 15), 0x735a2d97);
    return (value ^ (value >>> 15)) >>> 0;
  };
}

export class Random {
  constructor(seed = createSeed()) {
    const nextSeed = splitMix32(seed);
    this.state0 = nextSeed();
    this.state1 = nextSeed();
    this.state2 = nextSeed();
    this.state3 = nextSeed();
  }

  nextUint32() {
    const result = Math.imul(rotateLeft(Math.imul(this.state1, 5), 7), 9);
    const temporary = this.state1 << 9;

    this.state2 ^= this.state0;
    this.state3 ^= this.state1;
    this.state1 ^= this.state2;
    this.state0 ^= this.state3;
    this.state2 ^= temporary;
    this.state3 = rotateLeft(this.state3, 11);

    return result >>> 0;
  }

  int(maxExclusive) {
    const limit = Uint32Range - (Uint32Range % maxExclusive);
    let value;
    do {
      value = this.nextUint32();
    } while (value >= limit);
    return value % maxExclusive;
  }
}
