// データ構造 - TypeScript 実装
//
// JavaScript 組み込みのデータ構造と
// TypeScript での型安全な実装

console.log("=== Data Structures Demo ===\n");

// --- 配列 ---
function arrays(): void {
  console.log("--- 配列 ---");

  // 配列リテラル
  const numbers: number[] = [1, 2, 3, 4, 5];
  const strings: Array<string> = ["a", "b", "c"];

  console.log(`numbers: ${numbers}`);
  console.log(`strings: ${strings}`);

  // 配列操作
  const arr = [1, 2, 3];
  console.log(`\n配列操作:`);
  console.log(`  push(4): ${[...arr, 4]}`);
  console.log(`  unshift(0): ${[0, ...arr]}`);
  console.log(`  pop(): ${arr.slice(0, -1)}`);
  console.log(`  shift(): ${arr.slice(1)}`);
  console.log(`  slice(1, 3): ${arr.slice(1, 3)}`);
  console.log(`  concat([4, 5]): ${arr.concat([4, 5])}`);

  // スプレッド
  const combined = [...numbers, ...strings.map(Number)];
  console.log(`\nspread: ${combined}`);

  // 分割代入
  const [first, second, ...rest] = numbers;
  console.log(`destructuring: first=${first}, second=${second}, rest=${rest}`);

  console.log();
}

// --- タプル ---
function tuples(): void {
  console.log("--- タプル ---");

  // 基本タプル
  const point: [number, number] = [10, 20];
  const person: [string, number, boolean] = ["Alice", 30, true];

  console.log(`point: (${point[0]}, ${point[1]})`);
  console.log(`person: ${person}`);

  // 名前付きタプル
  type NamedPoint = [x: number, y: number, z: number];
  const p3d: NamedPoint = [1, 2, 3];
  console.log(`named tuple: ${p3d}`);

  // オプショナル要素
  type OptionalTuple = [string, number?];
  const t1: OptionalTuple = ["hello"];
  const t2: OptionalTuple = ["world", 42];
  console.log(`optional tuple: ${t1}, ${t2}`);

  // Rest 要素
  type StringNumberBools = [string, number, ...boolean[]];
  const snb: StringNumberBools = ["test", 1, true, false, true];
  console.log(`rest tuple: ${snb}`);

  console.log();
}

// --- Map ---
function maps(): void {
  console.log("--- Map ---");

  // Map の作成
  const map = new Map<string, number>();
  map.set("one", 1);
  map.set("two", 2);
  map.set("three", 3);

  console.log(`Map size: ${map.size}`);
  console.log(`get("two"): ${map.get("two")}`);
  console.log(`has("four"): ${map.has("four")}`);

  // イテレーション
  console.log("\nイテレーション:");
  for (const [key, value] of map) {
    console.log(`  ${key}: ${value}`);
  }

  // 配列から作成
  const fromArray = new Map([
    ["a", 1],
    ["b", 2],
  ]);
  console.log(`\nfrom array: ${[...fromArray]}`);

  // オブジェクトキー
  const objKeyMap = new Map<object, string>();
  const key1 = { id: 1 };
  const key2 = { id: 2 };
  objKeyMap.set(key1, "first");
  objKeyMap.set(key2, "second");
  console.log(`object key: ${objKeyMap.get(key1)}`);

  console.log();
}

// --- Set ---
function sets(): void {
  console.log("--- Set ---");

  // Set の作成
  const set = new Set<number>([1, 2, 3, 2, 1]);
  console.log(`Set (deduped): ${[...set]}`);

  // 操作
  set.add(4);
  set.delete(1);
  console.log(`after add(4), delete(1): ${[...set]}`);
  console.log(`has(2): ${set.has(2)}`);

  // 集合演算
  const a = new Set([1, 2, 3, 4]);
  const b = new Set([3, 4, 5, 6]);

  const union = new Set([...a, ...b]);
  const intersection = new Set([...a].filter((x) => b.has(x)));
  const difference = new Set([...a].filter((x) => !b.has(x)));

  console.log(`\n集合演算:`);
  console.log(`  A: ${[...a]}`);
  console.log(`  B: ${[...b]}`);
  console.log(`  A ∪ B (union): ${[...union]}`);
  console.log(`  A ∩ B (intersection): ${[...intersection]}`);
  console.log(`  A - B (difference): ${[...difference]}`);

  console.log();
}

// --- WeakMap / WeakSet ---
function weakCollections(): void {
  console.log("--- WeakMap / WeakSet ---");

  // WeakMap: キーが弱参照
  const cache = new WeakMap<object, string>();
  let obj: { id: number } | null = { id: 1 };
  cache.set(obj, "cached value");
  console.log(`WeakMap.get(): ${cache.get(obj)}`);

  // WeakSet: 値が弱参照
  const seen = new WeakSet<object>();
  seen.add(obj);
  console.log(`WeakSet.has(): ${seen.has(obj)}`);

  // 参照を消すと GC 対象に
  obj = null;
  console.log(`After nullifying: entry will be GC'd`);

  console.log();
}

// --- スタック ---
function stacks(): void {
  console.log("--- スタック ---");

  class Stack<T> {
    private items: T[] = [];

    push(item: T): void {
      this.items.push(item);
    }

    pop(): T | undefined {
      return this.items.pop();
    }

    peek(): T | undefined {
      return this.items[this.items.length - 1];
    }

    isEmpty(): boolean {
      return this.items.length === 0;
    }

    get size(): number {
      return this.items.length;
    }

    toString(): string {
      return `Stack[${this.items.join(", ")}]`;
    }
  }

  const stack = new Stack<number>();
  stack.push(1);
  stack.push(2);
  stack.push(3);

  console.log(`Stack: ${stack}`);
  console.log(`pop(): ${stack.pop()}`);
  console.log(`peek(): ${stack.peek()}`);
  console.log(`After pop: ${stack}`);

  console.log();
}

// --- キュー ---
function queues(): void {
  console.log("--- キュー ---");

  class Queue<T> {
    private items: T[] = [];

    enqueue(item: T): void {
      this.items.push(item);
    }

    dequeue(): T | undefined {
      return this.items.shift();
    }

    front(): T | undefined {
      return this.items[0];
    }

    isEmpty(): boolean {
      return this.items.length === 0;
    }

    get size(): number {
      return this.items.length;
    }

    toString(): string {
      return `Queue[${this.items.join(" <- ")}]`;
    }
  }

  const queue = new Queue<string>();
  queue.enqueue("first");
  queue.enqueue("second");
  queue.enqueue("third");

  console.log(`Queue: ${queue}`);
  console.log(`dequeue(): ${queue.dequeue()}`);
  console.log(`front(): ${queue.front()}`);
  console.log(`After dequeue: ${queue}`);

  // 優先度付きキュー
  class PriorityQueue<T> {
    private items: Array<{ value: T; priority: number }> = [];

    enqueue(value: T, priority: number): void {
      const item = { value, priority };
      let added = false;
      for (let i = 0; i < this.items.length; i++) {
        if (priority < this.items[i].priority) {
          this.items.splice(i, 0, item);
          added = true;
          break;
        }
      }
      if (!added) {
        this.items.push(item);
      }
    }

    dequeue(): T | undefined {
      return this.items.shift()?.value;
    }

    toString(): string {
      return this.items.map((i) => `${i.value}(${i.priority})`).join(" -> ");
    }
  }

  console.log("\n優先度付きキュー:");
  const pq = new PriorityQueue<string>();
  pq.enqueue("low", 3);
  pq.enqueue("high", 1);
  pq.enqueue("medium", 2);
  console.log(`PriorityQueue: ${pq}`);
  console.log(`dequeue(): ${pq.dequeue()}`);

  console.log();
}

// --- 連結リスト ---
function linkedLists(): void {
  console.log("--- 連結リスト ---");

  class ListNode<T> {
    constructor(
      public value: T,
      public next: ListNode<T> | null = null
    ) {}
  }

  class LinkedList<T> {
    private head: ListNode<T> | null = null;
    private tail: ListNode<T> | null = null;
    private _size = 0;

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

    pushFront(value: T): void {
      const node = new ListNode(value, this.head);
      if (!this.head) {
        this.tail = node;
      }
      this.head = node;
      this._size++;
    }

    popFront(): T | undefined {
      if (!this.head) return undefined;
      const value = this.head.value;
      this.head = this.head.next;
      if (!this.head) this.tail = null;
      this._size--;
      return value;
    }

    get size(): number {
      return this._size;
    }

    *[Symbol.iterator](): Iterator<T> {
      let current = this.head;
      while (current) {
        yield current.value;
        current = current.next;
      }
    }

    toString(): string {
      return `[${[...this].join(" -> ")}]`;
    }
  }

  const list = new LinkedList<number>();
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  list.pushFront(0);

  console.log(`LinkedList: ${list}`);
  console.log(`popFront(): ${list.popFront()}`);
  console.log(`After pop: ${list}`);

  console.log();
}

// --- 二分木 ---
function binaryTrees(): void {
  console.log("--- 二分木 ---");

  class TreeNode<T> {
    constructor(
      public value: T,
      public left: TreeNode<T> | null = null,
      public right: TreeNode<T> | null = null
    ) {}
  }

  class BinarySearchTree<T> {
    private root: TreeNode<T> | null = null;

    insert(value: T): void {
      const node = new TreeNode(value);
      if (!this.root) {
        this.root = node;
        return;
      }
      this.insertNode(this.root, node);
    }

    private insertNode(root: TreeNode<T>, node: TreeNode<T>): void {
      if (node.value < root.value) {
        if (!root.left) {
          root.left = node;
        } else {
          this.insertNode(root.left, node);
        }
      } else {
        if (!root.right) {
          root.right = node;
        } else {
          this.insertNode(root.right, node);
        }
      }
    }

    search(value: T): boolean {
      return this.searchNode(this.root, value);
    }

    private searchNode(node: TreeNode<T> | null, value: T): boolean {
      if (!node) return false;
      if (value === node.value) return true;
      if (value < node.value) {
        return this.searchNode(node.left, value);
      }
      return this.searchNode(node.right, value);
    }

    inorder(): T[] {
      const result: T[] = [];
      this.inorderTraverse(this.root, result);
      return result;
    }

    private inorderTraverse(node: TreeNode<T> | null, result: T[]): void {
      if (node) {
        this.inorderTraverse(node.left, result);
        result.push(node.value);
        this.inorderTraverse(node.right, result);
      }
    }
  }

  const bst = new BinarySearchTree<number>();
  [5, 3, 7, 1, 4, 6, 8].forEach((n) => bst.insert(n));

  console.log(`Inorder traversal: ${bst.inorder()}`);
  console.log(`search(4): ${bst.search(4)}`);
  console.log(`search(9): ${bst.search(9)}`);

  console.log();
}

// --- ハッシュマップ (カスタム実装) ---
function customHashMap(): void {
  console.log("--- ハッシュマップ (カスタム実装) ---");

  class HashMap<K, V> {
    private buckets: Array<Array<[K, V]>>;
    private _size = 0;
    private capacity: number;

    constructor(capacity = 16) {
      this.capacity = capacity;
      this.buckets = new Array(capacity).fill(null).map(() => []);
    }

    private hash(key: K): number {
      const str = String(key);
      let hash = 0;
      for (let i = 0; i < str.length; i++) {
        hash = (hash * 31 + str.charCodeAt(i)) % this.capacity;
      }
      return hash;
    }

    set(key: K, value: V): void {
      const index = this.hash(key);
      const bucket = this.buckets[index];
      const existing = bucket.findIndex(([k]) => k === key);
      if (existing >= 0) {
        bucket[existing][1] = value;
      } else {
        bucket.push([key, value]);
        this._size++;
      }
    }

    get(key: K): V | undefined {
      const index = this.hash(key);
      const pair = this.buckets[index].find(([k]) => k === key);
      return pair?.[1];
    }

    has(key: K): boolean {
      return this.get(key) !== undefined;
    }

    get size(): number {
      return this._size;
    }
  }

  const map = new HashMap<string, number>();
  map.set("one", 1);
  map.set("two", 2);
  map.set("three", 3);

  console.log(`HashMap size: ${map.size}`);
  console.log(`get("two"): ${map.get("two")}`);
  console.log(`has("four"): ${map.has("four")}`);

  console.log();
}

// --- Record と ReadonlyArray ---
function readonlyStructures(): void {
  console.log("--- Record と ReadonlyArray ---");

  // ReadonlyArray
  const readonlyArr: readonly number[] = [1, 2, 3];
  // readonlyArr.push(4);  // エラー
  // readonlyArr[0] = 10;  // エラー
  console.log(`ReadonlyArray: ${readonlyArr}`);

  // Readonly
  type User = {
    name: string;
    age: number;
  };

  const readonlyUser: Readonly<User> = { name: "Alice", age: 30 };
  // readonlyUser.age = 31;  // エラー
  console.log(`Readonly<User>: ${JSON.stringify(readonlyUser)}`);

  // Record
  type ScoreMap = Record<string, number>;
  const scores: ScoreMap = {
    Alice: 100,
    Bob: 85,
    Charlie: 92,
  };
  console.log(`Record<string, number>: ${JSON.stringify(scores)}`);

  // as const
  const config = {
    apiUrl: "/api",
    timeout: 3000,
    retries: 3,
  } as const;
  // config.timeout = 5000;  // エラー
  console.log(`as const: ${JSON.stringify(config)}`);

  console.log();
}

// --- 実行 ---
arrays();
tuples();
maps();
sets();
weakCollections();
stacks();
queues();
linkedLists();
binaryTrees();
customHashMap();
readonlyStructures();
