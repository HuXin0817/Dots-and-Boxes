function hashKey(key) {
  let hash = 0x811c9dc5;
  for (let index = 0; index < key.length; index += 1) {
    hash = Math.imul(hash ^ key[index], 0x01000193);
  }
  hash ^= hash >>> 16;
  hash = Math.imul(hash, 0x85ebca6b);
  hash ^= hash >>> 13;
  hash = Math.imul(hash, 0xc2b2ae35);
  return (hash ^ (hash >>> 16)) >>> 0;
}

function bucketCapacity(maxEntries) {
  let result = 1;
  while (result < maxEntries) {
    result *= 2;
  }
  return result;
}

export class SearchCache {
  constructor(keyWordCount, maxEntries) {
    this.keyWordCount = keyWordCount;
    this.maxEntries = maxEntries;
    this.keyBytes = new Uint8Array(
      maxEntries * keyWordCount * Uint32Array.BYTES_PER_ELEMENT,
    );
    this.keys = new Uint32Array(this.keyBytes.buffer);
    this.values = new Uint8Array(maxEntries);
    this.hashes = new Uint32Array(maxEntries);
    this.hashNext = new Int32Array(maxEntries);
    this.newer = new Int32Array(maxEntries);
    this.older = new Int32Array(maxEntries);

    const buckets = bucketCapacity(maxEntries);
    this.bucketHeads = new Int32Array(buckets);
    this.bucketMask = buckets - 1;
    this.entryCount = 0;
    this.head = 0;
    this.tail = 0;
  }

  get size() {
    return this.entryCount;
  }

  get(key) {
    const hash = hashKey(key);
    const index = this.find(key, hash);
    if (index < 0) {
      return undefined;
    }

    this.touch(index);
    return this.values[index];
  }

  set(key, value) {
    const hash = hashKey(key);
    let index = this.find(key, hash);
    if (index >= 0) {
      this.values[index] = value;
      this.touch(index);
      return;
    }

    if (this.entryCount < this.maxEntries) {
      index = this.entryCount;
      this.entryCount += 1;
    } else {
      index = this.tail - 1;
      this.removeFromBucket(index);
      this.detach(index);
    }

    const keyOffset = index * this.keyWordCount;
    this.keys.set(key, keyOffset);
    this.values[index] = value;
    this.hashes[index] = hash;
    this.insertIntoBucket(index);
    this.addToHead(index);
  }

  clear() {
    this.bucketHeads.fill(0);
    this.entryCount = 0;
    this.head = 0;
    this.tail = 0;
  }

  find(key, hash) {
    let node = this.bucketHeads[hash & this.bucketMask];
    while (node !== 0) {
      const index = node - 1;
      if (this.hashes[index] === hash && this.keyEquals(index, key)) {
        return index;
      }
      node = this.hashNext[index];
    }
    return -1;
  }

  keyEquals(entryIndex, key) {
    const offset = entryIndex * this.keyWordCount;
    for (let index = 0; index < this.keyWordCount; index += 1) {
      if (this.keys[offset + index] !== key[index]) {
        return false;
      }
    }
    return true;
  }

  insertIntoBucket(index) {
    const bucket = this.hashes[index] & this.bucketMask;
    this.hashNext[index] = this.bucketHeads[bucket];
    this.bucketHeads[bucket] = index + 1;
  }

  removeFromBucket(index) {
    const bucket = this.hashes[index] & this.bucketMask;
    const target = index + 1;
    let previous = 0;
    let node = this.bucketHeads[bucket];

    while (node !== target) {
      previous = node;
      node = this.hashNext[node - 1];
    }

    if (previous === 0) {
      this.bucketHeads[bucket] = this.hashNext[index];
    } else {
      this.hashNext[previous - 1] = this.hashNext[index];
    }
    this.hashNext[index] = 0;
  }

  touch(index) {
    if (index + 1 === this.head) {
      return;
    }
    this.detach(index);
    this.addToHead(index);
  }

  detach(index) {
    const previous = this.newer[index];
    const next = this.older[index];

    if (previous === 0) {
      this.head = next;
    } else {
      this.older[previous - 1] = next;
    }
    if (next === 0) {
      this.tail = previous;
    } else {
      this.newer[next - 1] = previous;
    }
  }

  addToHead(index) {
    const node = index + 1;
    this.newer[index] = 0;
    this.older[index] = this.head;
    if (this.head === 0) {
      this.tail = node;
    } else {
      this.newer[this.head - 1] = node;
    }
    this.head = node;
  }
}
