// メモリ管理 - TypeScript 実装
//
// TypeScript/JavaScript は GC (Garbage Collection) でメモリ管理
// V8 エンジンは世代別 GC を使用

console.log("=== Memory Management Demo ===\n");

// --- 値型と参照型 ---
function valuesAndReferences(): void {
  console.log("--- 値型と参照型 ---");

  // プリミティブ: 値渡し (コピー)
  let a = 42;
  let b = a; // コピー
  b = 100;
  console.log(`プリミティブ: a = ${a}, b = ${b} (値はコピー)`);

  // オブジェクト: 参照渡し
  const obj1 = { x: 10, y: 20 };
  const obj2 = obj1; // 参照をコピー
  obj2.x = 100;
  console.log(`オブジェクト: obj1.x = ${obj1.x} (参照を共有)`);

  // 配列も参照型
  const arr1 = [1, 2, 3];
  const arr2 = arr1;
  arr2.push(4);
  console.log(`配列: arr1 = ${arr1} (参照を共有)`);

  // シャローコピー
  const original = { a: 1, b: { c: 2 } };
  const shallow = { ...original };
  shallow.a = 10;
  shallow.b.c = 20; // ネストしたオブジェクトは共有
  console.log(`シャローコピー: original.a = ${original.a}, original.b.c = ${original.b.c}`);

  // ディープコピー
  const deep = JSON.parse(JSON.stringify(original));
  deep.b.c = 100;
  console.log(`ディープコピー: original.b.c = ${original.b.c}`);

  // structuredClone (モダンな方法)
  const cloned = structuredClone(original);
  cloned.b.c = 200;
  console.log(`structuredClone: original.b.c = ${original.b.c}`);

  console.log();
}

// --- クロージャとメモリ ---
function closuresAndMemory(): void {
  console.log("--- クロージャとメモリ ---");

  // クロージャは外部変数を保持する
  function createCounter(): () => number {
    let count = 0; // クロージャによって保持される
    return () => {
      count++;
      return count;
    };
  }

  const counter = createCounter();
  console.log(`counter(): ${counter()}`);
  console.log(`counter(): ${counter()}`);
  console.log(`counter(): ${counter()}`);

  // メモリリークの例: クロージャが大きなデータを保持
  function potentialLeak(): () => number {
    const largeData = new Array(10000).fill("x");
    // largeData 全体がクロージャに保持される
    return () => largeData.length;
  }

  const getLength = potentialLeak();
  console.log(`closure holding large data: length = ${getLength()}`);

  console.log();
}

// --- WeakRef と FinalizationRegistry ---
function weakReferences(): void {
  console.log("--- WeakRef と FinalizationRegistry ---");

  // WeakRef: 弱い参照 (GC を妨げない)
  let obj: { name: string } | undefined = { name: "temporary" };
  const weakRef = new WeakRef(obj);

  console.log(`WeakRef.deref(): ${weakRef.deref()?.name}`);

  // obj への強参照を消す
  obj = undefined;
  // GC が実行されると weakRef.deref() は undefined を返す可能性

  // FinalizationRegistry: オブジェクトが GC されたときのコールバック
  const registry = new FinalizationRegistry((heldValue: string) => {
    console.log(`  [FinalizationRegistry] ${heldValue} was collected`);
  });

  function createTrackedObject(name: string): object {
    const tracked = { name };
    registry.register(tracked, name);
    return tracked;
  }

  let tracked: object | undefined = createTrackedObject("tracked-object");
  console.log(`Created tracked object`);
  tracked = undefined;
  console.log(`Removed reference (GC will collect eventually)`);

  console.log();
}

// --- WeakMap と WeakSet ---
function weakCollections(): void {
  console.log("--- WeakMap と WeakSet ---");

  // WeakMap: キーが弱参照
  const cache = new WeakMap<object, string>();

  let key: { id: number } | undefined = { id: 1 };
  cache.set(key, "cached value");
  console.log(`WeakMap.get(): ${cache.get(key)}`);

  // key への参照を消すと、エントリも GC 対象になる
  key = undefined;
  console.log(`key = undefined (entry will be GC'd)`);

  // WeakSet: 値が弱参照
  const seen = new WeakSet<object>();
  let obj: { name: string } | undefined = { name: "object" };
  seen.add(obj);
  console.log(`WeakSet.has(): ${seen.has(obj)}`);

  // 使用例: DOM 要素の追跡
  console.log(`\nWeakMap の使用例: プライベートデータ`);

  const privateData = new WeakMap<object, { secret: string }>();

  class MyClass {
    constructor(secret: string) {
      privateData.set(this, { secret });
    }

    getSecret(): string {
      return privateData.get(this)?.secret ?? "";
    }
  }

  const instance = new MyClass("my-secret");
  console.log(`instance.getSecret(): ${instance.getSecret()}`);
  // インスタンスが GC されると privateData のエントリも消える

  console.log();
}

// --- ArrayBuffer と TypedArray ---
function binaryData(): void {
  console.log("--- ArrayBuffer と TypedArray ---");

  // ArrayBuffer: 固定長のバイナリデータ
  const buffer = new ArrayBuffer(16); // 16 バイト
  console.log(`ArrayBuffer: ${buffer.byteLength} bytes`);

  // TypedArray: ArrayBuffer のビュー
  const int32View = new Int32Array(buffer);
  int32View[0] = 42;
  int32View[1] = 100;
  console.log(`Int32Array: [${int32View[0]}, ${int32View[1]}]`);

  // 同じバッファを別の型で見る
  const uint8View = new Uint8Array(buffer);
  console.log(`Uint8Array (first 8 bytes): [${Array.from(uint8View.slice(0, 8))}]`);

  // DataView: 異なる型を混在させる
  const view = new DataView(buffer);
  view.setInt16(8, 1000, true); // リトルエンディアン
  console.log(`DataView.getInt16(8): ${view.getInt16(8, true)}`);

  // SharedArrayBuffer: スレッド間で共有 (Web Workers で使用)
  // const shared = new SharedArrayBuffer(16);

  console.log();
}

// --- メモリ効率のパターン ---
function memoryPatterns(): void {
  console.log("--- メモリ効率のパターン ---");

  // オブジェクトプール
  class ObjectPool<T> {
    private pool: T[] = [];
    private createFn: () => T;

    constructor(createFn: () => T, initialSize: number = 10) {
      this.createFn = createFn;
      for (let i = 0; i < initialSize; i++) {
        this.pool.push(createFn());
      }
    }

    acquire(): T {
      return this.pool.pop() ?? this.createFn();
    }

    release(obj: T): void {
      this.pool.push(obj);
    }

    get size(): number {
      return this.pool.length;
    }
  }

  const pool = new ObjectPool(() => ({ x: 0, y: 0 }), 5);
  console.log(`ObjectPool initial size: ${pool.size}`);

  const obj1 = pool.acquire();
  const obj2 = pool.acquire();
  console.log(`After acquire x2: ${pool.size}`);

  pool.release(obj1);
  pool.release(obj2);
  console.log(`After release x2: ${pool.size}`);

  // 文字列の intern (重複排除)
  const strings = new Map<string, string>();
  function intern(str: string): string {
    if (!strings.has(str)) {
      strings.set(str, str);
    }
    return strings.get(str)!;
  }

  const s1 = intern("hello");
  const s2 = intern("hello");
  console.log(`\nString interning: s1 === s2: ${s1 === s2}`);

  console.log();
}

// --- GC の挙動 ---
function garbageCollection(): void {
  console.log("--- GC の挙動 ---");

  console.log("JavaScript の GC:");
  console.log("  - V8: 世代別 GC (Young/Old generation)");
  console.log("  - Mark-and-Sweep + Scavenger");
  console.log("  - インクリメンタル & 並行 GC");

  // メモリ使用量 (Node.js)
  if (typeof process !== "undefined" && process.memoryUsage) {
    const usage = process.memoryUsage();
    console.log(`\nNode.js メモリ使用量:`);
    console.log(`  heapUsed: ${Math.round(usage.heapUsed / 1024 / 1024)} MB`);
    console.log(`  heapTotal: ${Math.round(usage.heapTotal / 1024 / 1024)} MB`);
    console.log(`  external: ${Math.round(usage.external / 1024 / 1024)} MB`);
    console.log(`  rss: ${Math.round(usage.rss / 1024 / 1024)} MB`);
  }

  // 手動 GC (--expose-gc フラグが必要)
  // if (global.gc) {
  //   global.gc();
  // }

  console.log();
}

// --- 実行 ---
valuesAndReferences();
closuresAndMemory();
weakReferences();
weakCollections();
binaryData();
memoryPatterns();
garbageCollection();
