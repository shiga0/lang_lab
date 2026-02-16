// 並行処理 - TypeScript 実装
//
// JavaScript はシングルスレッド + イベントループ
// 非同期処理は Promise と async/await で実現

console.log("=== Concurrency Demo ===\n");

// --- Promise 基礎 ---
async function promiseBasics(): Promise<void> {
  console.log("--- Promise 基礎 ---");

  // Promise の作成
  const promise = new Promise<string>((resolve, reject) => {
    setTimeout(() => {
      resolve("Hello, Promise!");
    }, 100);
  });

  const result = await promise;
  console.log(`Promise result: ${result}`);

  // Promise.resolve / Promise.reject
  const immediate = await Promise.resolve(42);
  console.log(`Promise.resolve: ${immediate}`);

  // Promise チェーン
  const chained = await Promise.resolve(1)
    .then((x) => x + 1)
    .then((x) => x * 2)
    .then((x) => `Result: ${x}`);
  console.log(`Promise chain: ${chained}`);

  console.log();
}

// --- async/await ---
async function asyncAwaitDemo(): Promise<void> {
  console.log("--- async/await ---");

  // async 関数
  async function fetchData(id: number): Promise<string> {
    await sleep(50);
    return `Data for ${id}`;
  }

  const data = await fetchData(1);
  console.log(`fetchData(1): ${data}`);

  // 並列実行
  console.log("\n並列実行:");
  const start = Date.now();

  const [r1, r2, r3] = await Promise.all([
    fetchData(1),
    fetchData(2),
    fetchData(3),
  ]);

  const elapsed = Date.now() - start;
  console.log(`  Results: ${r1}, ${r2}, ${r3}`);
  console.log(`  Time: ~${elapsed}ms (並列なので約50ms)`);

  // 順次実行
  console.log("\n順次実行:");
  const start2 = Date.now();

  const s1 = await fetchData(1);
  const s2 = await fetchData(2);
  const s3 = await fetchData(3);

  const elapsed2 = Date.now() - start2;
  console.log(`  Results: ${s1}, ${s2}, ${s3}`);
  console.log(`  Time: ~${elapsed2}ms (順次なので約150ms)`);

  console.log();
}

// --- Promise コンビネータ ---
async function promiseCombinators(): Promise<void> {
  console.log("--- Promise コンビネータ ---");

  const fast = () => sleep(50).then(() => "fast");
  const slow = () => sleep(100).then(() => "slow");
  const fail = () => sleep(75).then(() => Promise.reject(new Error("failed")));

  // Promise.all: 全て成功
  const allResults = await Promise.all([fast(), slow()]);
  console.log(`Promise.all: ${allResults}`);

  // Promise.allSettled: 全て完了 (成功・失敗問わず)
  const settledResults = await Promise.allSettled([fast(), fail()]);
  console.log(`Promise.allSettled:`);
  settledResults.forEach((r, i) => {
    if (r.status === "fulfilled") {
      console.log(`  [${i}] fulfilled: ${r.value}`);
    } else {
      console.log(`  [${i}] rejected: ${r.reason.message}`);
    }
  });

  // Promise.race: 最初に完了
  const raceResult = await Promise.race([slow(), fast()]);
  console.log(`Promise.race: ${raceResult}`);

  // Promise.any: 最初に成功
  const anyResult = await Promise.any([fail().catch(() => sleep(200).then(() => "recovered")), fast()]);
  console.log(`Promise.any: ${anyResult}`);

  console.log();
}

// --- エラーハンドリング ---
async function errorHandling(): Promise<void> {
  console.log("--- エラーハンドリング ---");

  // try-catch
  async function mayFail(shouldFail: boolean): Promise<string> {
    await sleep(50);
    if (shouldFail) {
      throw new Error("Something went wrong");
    }
    return "success";
  }

  try {
    await mayFail(true);
  } catch (error) {
    console.log(`try-catch: ${(error as Error).message}`);
  }

  // .catch()
  const result = await mayFail(true).catch((err) => `caught: ${err.message}`);
  console.log(`.catch(): ${result}`);

  // Promise.all でのエラー
  try {
    await Promise.all([
      Promise.resolve(1),
      Promise.reject(new Error("one failed")),
      Promise.resolve(3),
    ]);
  } catch (error) {
    console.log(`Promise.all error: ${(error as Error).message}`);
  }

  console.log();
}

// --- イベントループ ---
async function eventLoopDemo(): Promise<void> {
  console.log("--- イベントループ ---");

  console.log("1. 同期コード");

  setTimeout(() => console.log("4. setTimeout (マクロタスク)"), 0);

  Promise.resolve().then(() => console.log("3. Promise.then (マイクロタスク)"));

  console.log("2. 同期コード");

  // 全タスクが完了するまで待つ
  await sleep(10);

  console.log("\nタスク優先順位:");
  console.log("  1. 同期コード");
  console.log("  2. マイクロタスク (Promise, queueMicrotask)");
  console.log("  3. マクロタスク (setTimeout, setInterval, I/O)");

  console.log();
}

// --- ジェネレータと非同期イテレータ ---
async function generatorsAndAsyncIterators(): Promise<void> {
  console.log("--- ジェネレータと非同期イテレータ ---");

  // 同期ジェネレータ
  function* range(start: number, end: number): Generator<number> {
    for (let i = start; i <= end; i++) {
      yield i;
    }
  }

  console.log(`Generator: ${[...range(1, 5)]}`);

  // 非同期ジェネレータ
  async function* asyncRange(start: number, end: number): AsyncGenerator<number> {
    for (let i = start; i <= end; i++) {
      await sleep(10);
      yield i;
    }
  }

  process.stdout.write("AsyncGenerator: ");
  for await (const n of asyncRange(1, 5)) {
    process.stdout.write(`${n} `);
  }
  console.log();

  // 非同期イテラブル
  const asyncIterable = {
    [Symbol.asyncIterator](): AsyncIterator<string> {
      let i = 0;
      const items = ["a", "b", "c"];
      return {
        async next(): Promise<IteratorResult<string>> {
          await sleep(10);
          if (i < items.length) {
            return { value: items[i++], done: false };
          }
          return { value: undefined, done: true };
        },
      };
    },
  };

  process.stdout.write("AsyncIterable: ");
  for await (const item of asyncIterable) {
    process.stdout.write(`${item} `);
  }
  console.log();

  console.log();
}

// --- AbortController ---
async function abortControllerDemo(): Promise<void> {
  console.log("--- AbortController ---");

  async function cancellableOperation(signal: AbortSignal): Promise<string> {
    for (let i = 0; i < 10; i++) {
      if (signal.aborted) {
        throw new Error("Operation cancelled");
      }
      await sleep(20);
    }
    return "completed";
  }

  // タイムアウトでキャンセル
  const controller = new AbortController();
  setTimeout(() => controller.abort(), 50);

  try {
    await cancellableOperation(controller.signal);
  } catch (error) {
    console.log(`Cancelled: ${(error as Error).message}`);
  }

  // AbortSignal.timeout
  try {
    await Promise.race([
      sleep(200).then(() => "slow"),
      new Promise((_, reject) =>
        setTimeout(() => reject(new Error("timeout")), 50)
      ),
    ]);
  } catch (error) {
    console.log(`Timeout: ${(error as Error).message}`);
  }

  console.log();
}

// --- 並行制御パターン ---
async function concurrencyPatterns(): Promise<void> {
  console.log("--- 並行制御パターン ---");

  // セマフォ (同時実行数制限)
  class Semaphore {
    private queue: (() => void)[] = [];
    private running = 0;

    constructor(private limit: number) {}

    async acquire(): Promise<void> {
      if (this.running < this.limit) {
        this.running++;
        return;
      }
      await new Promise<void>((resolve) => this.queue.push(resolve));
      this.running++;
    }

    release(): void {
      this.running--;
      const next = this.queue.shift();
      if (next) next();
    }
  }

  const semaphore = new Semaphore(2);

  async function limitedTask(id: number): Promise<string> {
    await semaphore.acquire();
    try {
      console.log(`  Task ${id} started`);
      await sleep(50);
      console.log(`  Task ${id} completed`);
      return `result-${id}`;
    } finally {
      semaphore.release();
    }
  }

  console.log("Semaphore (limit=2):");
  const results = await Promise.all([
    limitedTask(1),
    limitedTask(2),
    limitedTask(3),
    limitedTask(4),
  ]);
  console.log(`Results: ${results}`);

  // キュー
  console.log("\nAsync Queue:");

  class AsyncQueue<T> {
    private queue: T[] = [];
    private waiters: ((value: T) => void)[] = [];

    enqueue(item: T): void {
      const waiter = this.waiters.shift();
      if (waiter) {
        waiter(item);
      } else {
        this.queue.push(item);
      }
    }

    async dequeue(): Promise<T> {
      if (this.queue.length > 0) {
        return this.queue.shift()!;
      }
      return new Promise((resolve) => this.waiters.push(resolve));
    }
  }

  const queue = new AsyncQueue<number>();

  // Producer
  (async () => {
    for (let i = 1; i <= 3; i++) {
      await sleep(20);
      queue.enqueue(i);
      console.log(`  Produced: ${i}`);
    }
  })();

  // Consumer
  for (let i = 0; i < 3; i++) {
    const item = await queue.dequeue();
    console.log(`  Consumed: ${item}`);
  }

  console.log();
}

// --- Worker Threads (Node.js) ---
function workerThreadsInfo(): void {
  console.log("--- Worker Threads (Node.js) ---");

  console.log("Worker Threads の使い方:");
  console.log(`
  // main.ts
  import { Worker } from "worker_threads";

  const worker = new Worker("./worker.ts");
  worker.postMessage({ task: "compute" });
  worker.on("message", (result) => console.log(result));

  // worker.ts
  import { parentPort } from "worker_threads";

  parentPort?.on("message", (msg) => {
    const result = heavyComputation(msg);
    parentPort?.postMessage(result);
  });
  `);

  console.log("注意点:");
  console.log("  - CPU バウンドタスクに有効");
  console.log("  - I/O バウンドは async/await で十分");
  console.log("  - SharedArrayBuffer でメモリ共有可能");

  console.log();
}

// --- ヘルパー関数 ---
function sleep(ms: number): Promise<void> {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

// --- 実行 ---
async function main(): Promise<void> {
  await promiseBasics();
  await asyncAwaitDemo();
  await promiseCombinators();
  await errorHandling();
  await eventLoopDemo();
  await generatorsAndAsyncIterators();
  await abortControllerDemo();
  await concurrencyPatterns();
  workerThreadsInfo();
}

main().catch(console.error);
