// LinkedList - TypeScript 実装
//
// ジェネリックな単方向連結リスト

console.log("=== LinkedList Demo ===\n");

// --- Node クラス ---
class ListNode<T> {
  constructor(
    public value: T,
    public next: ListNode<T> | null = null
  ) {}
}

// --- LinkedList クラス ---
class LinkedList<T> {
  private head: ListNode<T> | null = null;
  private tail: ListNode<T> | null = null;
  private _size = 0;

  // 長さを返す
  get length(): number {
    return this._size;
  }

  // 空かどうか
  isEmpty(): boolean {
    return this._size === 0;
  }

  // 先頭に追加
  pushFront(value: T): void {
    const node = new ListNode(value, this.head);
    if (!this.head) {
      this.tail = node;
    }
    this.head = node;
    this._size++;
  }

  // 末尾に追加
  pushBack(value: T): void {
    const node = new ListNode(value);
    if (!this.tail) {
      this.head = this.tail = node;
    } else {
      this.tail.next = node;
      this.tail = node;
    }
    this._size++;
  }

  // 先頭を削除して返す
  popFront(): T | undefined {
    if (!this.head) return undefined;

    const value = this.head.value;
    this.head = this.head.next;
    if (!this.head) {
      this.tail = null;
    }
    this._size--;
    return value;
  }

  // 先頭の値を参照
  front(): T | undefined {
    return this.head?.value;
  }

  // 末尾の値を参照
  back(): T | undefined {
    return this.tail?.value;
  }

  // インデックスで取得
  get(index: number): T | undefined {
    if (index < 0 || index >= this._size) return undefined;

    let current = this.head;
    for (let i = 0; i < index && current; i++) {
      current = current.next;
    }
    return current?.value;
  }

  // 値を検索
  find(predicate: (value: T) => boolean): T | undefined {
    let current = this.head;
    while (current) {
      if (predicate(current.value)) {
        return current.value;
      }
      current = current.next;
    }
    return undefined;
  }

  // インデックスを検索
  indexOf(value: T): number {
    let current = this.head;
    let index = 0;
    while (current) {
      if (current.value === value) {
        return index;
      }
      current = current.next;
      index++;
    }
    return -1;
  }

  // 値が含まれているか
  contains(value: T): boolean {
    return this.indexOf(value) !== -1;
  }

  // インデックスで削除
  removeAt(index: number): T | undefined {
    if (index < 0 || index >= this._size) return undefined;

    if (index === 0) {
      return this.popFront();
    }

    let current = this.head;
    for (let i = 0; i < index - 1 && current; i++) {
      current = current.next;
    }

    if (!current?.next) return undefined;

    const value = current.next.value;
    current.next = current.next.next;

    if (!current.next) {
      this.tail = current;
    }

    this._size--;
    return value;
  }

  // 反転
  reverse(): void {
    if (this._size <= 1) return;

    let prev: ListNode<T> | null = null;
    let current = this.head;
    this.tail = this.head;

    while (current) {
      const next = current.next;
      current.next = prev;
      prev = current;
      current = next;
    }

    this.head = prev;
  }

  // forEach
  forEach(callback: (value: T, index: number) => void): void {
    let current = this.head;
    let index = 0;
    while (current) {
      callback(current.value, index);
      current = current.next;
      index++;
    }
  }

  // map
  map<U>(callback: (value: T, index: number) => U): LinkedList<U> {
    const result = new LinkedList<U>();
    this.forEach((value, index) => {
      result.pushBack(callback(value, index));
    });
    return result;
  }

  // filter
  filter(predicate: (value: T, index: number) => boolean): LinkedList<T> {
    const result = new LinkedList<T>();
    this.forEach((value, index) => {
      if (predicate(value, index)) {
        result.pushBack(value);
      }
    });
    return result;
  }

  // reduce
  reduce<U>(callback: (acc: U, value: T, index: number) => U, initial: U): U {
    let acc = initial;
    this.forEach((value, index) => {
      acc = callback(acc, value, index);
    });
    return acc;
  }

  // 配列に変換
  toArray(): T[] {
    const result: T[] = [];
    this.forEach((value) => result.push(value));
    return result;
  }

  // 配列から作成
  static from<T>(iterable: Iterable<T>): LinkedList<T> {
    const list = new LinkedList<T>();
    for (const value of iterable) {
      list.pushBack(value);
    }
    return list;
  }

  // イテレータ
  *[Symbol.iterator](): Iterator<T> {
    let current = this.head;
    while (current) {
      yield current.value;
      current = current.next;
    }
  }

  // 文字列表現
  toString(): string {
    return `[${this.toArray().join(" -> ")}]`;
  }

  // クリア
  clear(): void {
    this.head = null;
    this.tail = null;
    this._size = 0;
  }
}

// --- デモ ---
function basicOperations(): void {
  console.log("--- Basic Operations ---");

  const list = new LinkedList<number>();
  console.log(`Empty list: ${list}`);

  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);
  console.log(`After pushFront 3, 2, 1: ${list}`);

  list.pushBack(4);
  list.pushBack(5);
  console.log(`After pushBack 4, 5: ${list}`);

  console.log(`Length: ${list.length}`);
  console.log(`Front: ${list.front()}`);
  console.log(`Back: ${list.back()}`);
  console.log(`Get(2): ${list.get(2)}`);

  console.log();

  console.log("--- Pop and Reverse ---");
  console.log(`PopFront: ${list.popFront()}`);
  console.log(`After popFront: ${list}`);

  list.reverse();
  console.log(`After reverse: ${list}`);

  console.log();
}

function iterationDemo(): void {
  console.log("--- Iteration ---");

  const list = LinkedList.from(["apple", "banana", "cherry"]);

  process.stdout.write("forEach: ");
  list.forEach((value) => process.stdout.write(`${value} `));
  console.log();

  console.log(`for...of: ${[...list].join(", ")}`);
  console.log(`toArray: ${list.toArray()}`);

  console.log();
}

function functionalDemo(): void {
  console.log("--- Functional Methods ---");

  const numbers = LinkedList.from([1, 2, 3, 4, 5]);

  const doubled = numbers.map((x) => x * 2);
  console.log(`map (*2): ${doubled}`);

  const evens = numbers.filter((x) => x % 2 === 0);
  console.log(`filter (even): ${evens}`);

  const sum = numbers.reduce((acc, x) => acc + x, 0);
  console.log(`reduce (sum): ${sum}`);

  console.log();
}

function runTests(): void {
  console.log("--- Tests ---");

  const list = new LinkedList<number>();

  // 空リスト
  console.assert(list.isEmpty(), "isEmpty failed");
  console.assert(list.length === 0, "length failed");

  // pushFront
  list.pushFront(1);
  list.pushFront(2);
  list.pushFront(3);
  console.assert(
    JSON.stringify(list.toArray()) === JSON.stringify([3, 2, 1]),
    "pushFront failed"
  );

  // pushBack
  list.pushBack(10);
  console.assert(
    JSON.stringify(list.toArray()) === JSON.stringify([3, 2, 1, 10]),
    "pushBack failed"
  );

  // front / back
  console.assert(list.front() === 3, "front failed");
  console.assert(list.back() === 10, "back failed");

  // popFront
  console.assert(list.popFront() === 3, "popFront failed");
  console.assert(
    JSON.stringify(list.toArray()) === JSON.stringify([2, 1, 10]),
    "after popFront failed"
  );

  // reverse
  list.reverse();
  console.assert(
    JSON.stringify(list.toArray()) === JSON.stringify([10, 1, 2]),
    "reverse failed"
  );

  // contains / indexOf
  console.assert(list.contains(1), "contains failed");
  console.assert(list.indexOf(1) === 1, "indexOf failed");
  console.assert(list.indexOf(99) === -1, "indexOf not found failed");

  // get
  console.assert(list.get(0) === 10, "get(0) failed");
  console.assert(list.get(2) === 2, "get(2) failed");
  console.assert(list.get(99) === undefined, "get out of bounds failed");

  // removeAt
  console.assert(list.removeAt(1) === 1, "removeAt failed");
  console.assert(
    JSON.stringify(list.toArray()) === JSON.stringify([10, 2]),
    "after removeAt failed"
  );

  // clear
  list.clear();
  console.assert(list.isEmpty(), "clear failed");

  console.log("All tests passed!");
}

// --- 実行 ---
basicOperations();
iterationDemo();
functionalDemo();
runTests();
