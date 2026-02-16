// 関数型プログラミング - TypeScript 実装
//
// TypeScript は関数型パラダイムを強力にサポート
// 高階関数、クロージャ、イミュータブル操作

console.log("=== Functional Programming Demo ===\n");

// --- 第一級関数 ---
function firstClassFunctions(): void {
  console.log("--- 第一級関数 ---");

  // 関数を変数に代入
  const add = (a: number, b: number): number => a + b;
  console.log(`add(2, 3) = ${add(2, 3)}`);

  // 関数を引数として渡す
  const apply = (x: number, fn: (n: number) => number): number => fn(x);
  const double = (n: number): number => n * 2;
  console.log(`apply(5, double) = ${apply(5, double)}`);

  // 関数を返す
  const makeAdder = (n: number): ((x: number) => number) => {
    return (x: number) => x + n;
  };
  const addFive = makeAdder(5);
  console.log(`makeAdder(5)(10) = ${addFive(10)}`);

  // 関数型エイリアス
  type BinaryOp = (a: number, b: number) => number;
  const multiply: BinaryOp = (a, b) => a * b;
  console.log(`multiply(3, 4) = ${multiply(3, 4)}`);

  console.log();
}

// --- クロージャ ---
function closures(): void {
  console.log("--- クロージャ ---");

  // カウンター
  const makeCounter = (): (() => number) => {
    let count = 0;
    return () => {
      count++;
      return count;
    };
  };

  const counter = makeCounter();
  console.log(`counter() = ${counter()}`);
  console.log(`counter() = ${counter()}`);
  console.log(`counter() = ${counter()}`);

  // 独立したカウンター
  const counter2 = makeCounter();
  console.log(`counter2() = ${counter2()}`);

  // フィボナッチジェネレータ
  const fibonacci = (): (() => number) => {
    let a = 0;
    let b = 1;
    return () => {
      const result = a;
      [a, b] = [b, a + b];
      return result;
    };
  };

  const fib = fibonacci();
  const fibValues = Array.from({ length: 10 }, () => fib());
  console.log(`Fibonacci: ${fibValues.join(" ")}`);

  console.log();
}

// --- 高階関数 ---
function higherOrderFunctions(): void {
  console.log("--- 高階関数 ---");

  const numbers = [1, 2, 3, 4, 5];

  // map
  const doubled = numbers.map((x) => x * 2);
  console.log(`map (*2): ${doubled}`);

  // filter
  const evens = numbers.filter((x) => x % 2 === 0);
  console.log(`filter (even): ${evens}`);

  // reduce
  const sum = numbers.reduce((acc, x) => acc + x, 0);
  console.log(`reduce (sum): ${sum}`);

  // find
  const found = numbers.find((x) => x > 3);
  console.log(`find (>3): ${found}`);

  // some / every
  console.log(`some (>3): ${numbers.some((x) => x > 3)}`);
  console.log(`every (>0): ${numbers.every((x) => x > 0)}`);

  // flatMap
  const nested = [[1, 2], [3, 4], [5]];
  const flattened = nested.flatMap((x) => x);
  console.log(`flatMap: ${flattened}`);

  // チェーン
  const result = numbers
    .map((x) => x * 2)
    .filter((x) => x > 4)
    .reduce((acc, x) => acc + x, 0);
  console.log(`chain (map->filter->reduce): ${result}`);

  console.log();
}

// --- カリー化と部分適用 ---
function curryingAndPartialApplication(): void {
  console.log("--- カリー化と部分適用 ---");

  // カリー化された関数
  const curriedAdd = (a: number) => (b: number) => (c: number) => a + b + c;
  console.log(`curriedAdd(1)(2)(3) = ${curriedAdd(1)(2)(3)}`);

  // 部分適用
  const addOne = curriedAdd(1);
  const addOneAndTwo = addOne(2);
  console.log(`addOneAndTwo(3) = ${addOneAndTwo(3)}`);

  // 汎用 curry 関数
  function curry<A, B, C>(fn: (a: A, b: B) => C): (a: A) => (b: B) => C {
    return (a) => (b) => fn(a, b);
  }

  const add = (a: number, b: number): number => a + b;
  const curriedAddFn = curry(add);
  console.log(`curry(add)(10)(5) = ${curriedAddFn(10)(5)}`);

  // pipe / compose
  const pipe =
    <T>(...fns: Array<(x: T) => T>) =>
    (x: T): T =>
      fns.reduce((acc, fn) => fn(acc), x);

  const compose =
    <T>(...fns: Array<(x: T) => T>) =>
    (x: T): T =>
      fns.reduceRight((acc, fn) => fn(acc), x);

  const addTen = (x: number) => x + 10;
  const multiplyTwo = (x: number) => x * 2;
  const subtractFive = (x: number) => x - 5;

  const piped = pipe(addTen, multiplyTwo, subtractFive);
  console.log(`pipe(addTen, multiplyTwo, subtractFive)(5) = ${piped(5)}`);

  const composed = compose(subtractFive, multiplyTwo, addTen);
  console.log(`compose (same result) = ${composed(5)}`);

  console.log();
}

// --- 不変性 ---
function immutability(): void {
  console.log("--- 不変性 ---");

  // 配列の不変操作
  const arr = [1, 2, 3];
  const added = [...arr, 4];
  const removed = arr.filter((x) => x !== 2);
  const updated = arr.map((x) => (x === 2 ? 20 : x));

  console.log(`original: ${arr}`);
  console.log(`added 4: ${added}`);
  console.log(`removed 2: ${removed}`);
  console.log(`updated 2->20: ${updated}`);

  // オブジェクトの不変操作
  const obj = { a: 1, b: 2, c: 3 };
  const withD = { ...obj, d: 4 };
  const withoutB = (({ b, ...rest }) => rest)(obj);
  const updatedB = { ...obj, b: 20 };

  console.log(`\noriginal: ${JSON.stringify(obj)}`);
  console.log(`withD: ${JSON.stringify(withD)}`);
  console.log(`withoutB: ${JSON.stringify(withoutB)}`);
  console.log(`updatedB: ${JSON.stringify(updatedB)}`);

  // ネストした更新
  type State = {
    user: { name: string; address: { city: string } };
  };

  const state: State = {
    user: { name: "Alice", address: { city: "Tokyo" } },
  };

  const newState: State = {
    ...state,
    user: {
      ...state.user,
      address: {
        ...state.user.address,
        city: "Osaka",
      },
    },
  };

  console.log(`\nnested update:`);
  console.log(`  original city: ${state.user.address.city}`);
  console.log(`  new city: ${newState.user.address.city}`);

  // Object.freeze
  const frozen = Object.freeze({ x: 1, y: 2 });
  // frozen.x = 10;  // エラー (strict mode)
  console.log(`\nObject.freeze: ${JSON.stringify(frozen)}`);

  console.log();
}

// --- 関数型ユーティリティ ---
function functionalUtilities(): void {
  console.log("--- 関数型ユーティリティ ---");

  // identity
  const identity = <T>(x: T): T => x;
  console.log(`identity(42) = ${identity(42)}`);

  // constant
  const constant =
    <T>(x: T) =>
    (): T =>
      x;
  const alwaysFive = constant(5);
  console.log(`constant(5)() = ${alwaysFive()}`);

  // flip
  const flip =
    <A, B, C>(fn: (a: A, b: B) => C) =>
    (b: B, a: A): C =>
      fn(a, b);
  const subtract = (a: number, b: number) => a - b;
  const flippedSubtract = flip(subtract);
  console.log(`flip(subtract)(3, 10) = ${flippedSubtract(3, 10)}`);

  // memoize
  const memoize = <T extends (...args: any[]) => any>(fn: T): T => {
    const cache = new Map<string, ReturnType<T>>();
    return ((...args: Parameters<T>): ReturnType<T> => {
      const key = JSON.stringify(args);
      if (cache.has(key)) {
        return cache.get(key)!;
      }
      const result = fn(...args);
      cache.set(key, result);
      return result;
    }) as T;
  };

  let callCount = 0;
  const expensiveFn = (n: number): number => {
    callCount++;
    return n * n;
  };

  const memoizedFn = memoize(expensiveFn);
  memoizedFn(5);
  memoizedFn(5);
  memoizedFn(5);
  console.log(`memoize: 3 calls, actual executions = ${callCount}`);

  // debounce (簡易版)
  const debounce = <T extends (...args: any[]) => any>(
    fn: T,
    ms: number
  ): ((...args: Parameters<T>) => void) => {
    let timeoutId: ReturnType<typeof setTimeout>;
    return (...args) => {
      clearTimeout(timeoutId);
      timeoutId = setTimeout(() => fn(...args), ms);
    };
  };

  console.log(`debounce: created with 100ms delay`);

  console.log();
}

// --- Option/Maybe モナド ---
function optionMonad(): void {
  console.log("--- Option/Maybe モナド ---");

  class Option<T> {
    private constructor(private value: T | null) {}

    static some<T>(value: T): Option<T> {
      return new Option(value);
    }

    static none<T>(): Option<T> {
      return new Option<T>(null);
    }

    isSome(): boolean {
      return this.value !== null;
    }

    isNone(): boolean {
      return this.value === null;
    }

    map<U>(fn: (value: T) => U): Option<U> {
      return this.value !== null ? Option.some(fn(this.value)) : Option.none();
    }

    flatMap<U>(fn: (value: T) => Option<U>): Option<U> {
      return this.value !== null ? fn(this.value) : Option.none();
    }

    getOrElse(defaultValue: T): T {
      return this.value !== null ? this.value : defaultValue;
    }

    toString(): string {
      return this.value !== null ? `Some(${this.value})` : "None";
    }
  }

  const some = Option.some(5);
  const none = Option.none<number>();

  console.log(`some.map(x => x * 2): ${some.map((x) => x * 2)}`);
  console.log(`none.map(x => x * 2): ${none.map((x) => x * 2)}`);
  console.log(`some.getOrElse(0): ${some.getOrElse(0)}`);
  console.log(`none.getOrElse(0): ${none.getOrElse(0)}`);

  // チェーン
  const safeDivide = (a: number, b: number): Option<number> =>
    b === 0 ? Option.none() : Option.some(a / b);

  const result = Option.some(10)
    .flatMap((x) => safeDivide(x, 2))
    .flatMap((x) => safeDivide(x, 0))
    .map((x) => x * 100);

  console.log(`chained division with 0: ${result}`);

  console.log();
}

// --- 再帰と末尾再帰 ---
function recursion(): void {
  console.log("--- 再帰と末尾再帰 ---");

  // 通常の再帰
  const factorial = (n: number): number => {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
  };
  console.log(`factorial(5) = ${factorial(5)}`);

  // 末尾再帰 (TSは最適化しないが、パターンとして)
  const factorialTail = (n: number, acc: number = 1): number => {
    if (n <= 1) return acc;
    return factorialTail(n - 1, n * acc);
  };
  console.log(`factorialTail(5) = ${factorialTail(5)}`);

  // トランポリン (スタックオーバーフロー回避)
  type Thunk<T> = () => T | Thunk<T>;

  const trampoline = <T>(fn: Thunk<T>): T => {
    let result = fn();
    while (typeof result === "function") {
      result = (result as Thunk<T>)();
    }
    return result;
  };

  const factorialTrampoline = (n: number, acc: number = 1): number | Thunk<number> => {
    if (n <= 1) return acc;
    return () => factorialTrampoline(n - 1, n * acc);
  };

  console.log(`trampoline factorial(5) = ${trampoline(() => factorialTrampoline(5))}`);

  console.log();
}

// --- 実行 ---
firstClassFunctions();
closures();
higherOrderFunctions();
curryingAndPartialApplication();
immutability();
functionalUtilities();
optionMonad();
recursion();
