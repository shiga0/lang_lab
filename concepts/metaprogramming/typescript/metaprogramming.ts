// メタプログラミング - TypeScript 実装
//
// TypeScript は型レベルでのメタプログラミングが強力
// デコレータ、Proxy、Reflect、条件型など

console.log("=== Metaprogramming Demo ===\n");

// --- 型レベルメタプログラミング ---
function typeLevelMeta(): void {
  console.log("--- 型レベルメタプログラミング ---");

  // Mapped Types
  type Readonly2<T> = {
    readonly [K in keyof T]: T[K];
  };

  type Optional<T> = {
    [K in keyof T]?: T[K];
  };

  type User = {
    name: string;
    age: number;
  };

  // 使用例 (コンパイル時のみ)
  const readonlyUser: Readonly2<User> = { name: "Alice", age: 30 };
  console.log(`Mapped Type (Readonly): ${JSON.stringify(readonlyUser)}`);

  // Conditional Types
  type IsString<T> = T extends string ? true : false;
  type A = IsString<string>; // true
  type B = IsString<number>; // false

  console.log(`Conditional Type: IsString<string> = true, IsString<number> = false`);

  // infer
  type UnpackPromise<T> = T extends Promise<infer U> ? U : T;
  type C = UnpackPromise<Promise<string>>; // string

  console.log(`infer: UnpackPromise<Promise<string>> = string`);

  // Template Literal Types
  type EventName = `on${Capitalize<"click" | "focus" | "blur">}`;
  // "onClick" | "onFocus" | "onBlur"

  const handler: { [K in EventName]?: () => void } = {
    onClick: () => console.log("clicked"),
  };
  console.log(`Template Literal Type: ${Object.keys(handler)}`);

  console.log();
}

// --- Reflect API ---
function reflectAPI(): void {
  console.log("--- Reflect API ---");

  const obj = {
    name: "Alice",
    age: 30,
    greet() {
      return `Hello, I'm ${this.name}`;
    },
  };

  // プロパティの取得・設定
  console.log(`Reflect.get: ${Reflect.get(obj, "name")}`);

  Reflect.set(obj, "age", 31);
  console.log(`Reflect.set: age = ${obj.age}`);

  // プロパティの存在確認
  console.log(`Reflect.has("name"): ${Reflect.has(obj, "name")}`);
  console.log(`Reflect.has("email"): ${Reflect.has(obj, "email")}`);

  // キー一覧
  console.log(`Reflect.ownKeys: ${Reflect.ownKeys(obj)}`);

  // メソッド呼び出し
  const result = Reflect.apply(obj.greet, obj, []);
  console.log(`Reflect.apply: ${result}`);

  // プロパティ定義
  Reflect.defineProperty(obj, "email", {
    value: "alice@example.com",
    writable: true,
    enumerable: true,
  });
  console.log(`Reflect.defineProperty: email = ${(obj as any).email}`);

  console.log();
}

// --- Proxy ---
function proxyDemo(): void {
  console.log("--- Proxy ---");

  // 基本的な Proxy
  const target = {
    name: "Alice",
    age: 30,
  };

  const handler: ProxyHandler<typeof target> = {
    get(target, prop, receiver) {
      console.log(`  [GET] ${String(prop)}`);
      return Reflect.get(target, prop, receiver);
    },
    set(target, prop, value, receiver) {
      console.log(`  [SET] ${String(prop)} = ${value}`);
      return Reflect.set(target, prop, value, receiver);
    },
  };

  const proxy = new Proxy(target, handler);

  console.log("Accessing properties:");
  const name = proxy.name;
  proxy.age = 31;
  console.log(`  Result: name=${name}, age=${proxy.age}`);

  // バリデーション Proxy
  console.log("\nValidation Proxy:");

  const validationHandler: ProxyHandler<Record<string, number>> = {
    set(target, prop, value) {
      if (typeof value !== "number") {
        throw new TypeError(`${String(prop)} must be a number`);
      }
      if (value < 0) {
        throw new RangeError(`${String(prop)} must be non-negative`);
      }
      return Reflect.set(target, prop, value);
    },
  };

  const scores = new Proxy<Record<string, number>>({}, validationHandler);
  scores.math = 90;
  scores.english = 85;
  console.log(`  scores: ${JSON.stringify(scores)}`);

  try {
    scores.science = -10;
  } catch (e) {
    console.log(`  Error: ${(e as Error).message}`);
  }

  // ロギング Proxy
  console.log("\nLogging Proxy:");

  function createLoggingProxy<T extends object>(target: T): T {
    return new Proxy(target, {
      get(target, prop) {
        const value = Reflect.get(target, prop);
        if (typeof value === "function") {
          return function (...args: any[]) {
            console.log(`  [CALL] ${String(prop)}(${args.join(", ")})`);
            return value.apply(target, args);
          };
        }
        return value;
      },
    });
  }

  const obj = createLoggingProxy({
    add: (a: number, b: number) => a + b,
    greet: (name: string) => `Hello, ${name}`,
  });

  obj.add(2, 3);
  obj.greet("World");

  console.log();
}

// --- デコレータ (実験的) ---
function decoratorsDemo(): void {
  console.log("--- デコレータパターン ---");

  // デコレータは TypeScript 5.0 で標準化
  // ここでは関数ベースのデコレータパターンを示す

  // メソッドデコレータ風
  function logged<T extends (...args: any[]) => any>(fn: T, name: string): T {
    return function (...args: Parameters<T>): ReturnType<T> {
      console.log(`  [LOG] ${name} called with: ${JSON.stringify(args)}`);
      const result = fn(...args);
      console.log(`  [LOG] ${name} returned: ${result}`);
      return result;
    } as T;
  }

  const add = logged((a: number, b: number) => a + b, "add");
  add(2, 3);

  // メモ化デコレータ風
  function memoized<T extends (...args: any[]) => any>(fn: T): T {
    const cache = new Map<string, ReturnType<T>>();
    return function (...args: Parameters<T>): ReturnType<T> {
      const key = JSON.stringify(args);
      if (cache.has(key)) {
        console.log(`  [CACHE HIT] ${key}`);
        return cache.get(key)!;
      }
      const result = fn(...args);
      cache.set(key, result);
      return result;
    } as T;
  }

  console.log("\nMemoization:");
  const fib = memoized((n: number): number => {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
  });

  console.log(`  fib(10) = ${fib(10)}`);
  console.log(`  fib(10) = ${fib(10)}`);

  // 時間計測デコレータ風
  function timed<T extends (...args: any[]) => any>(fn: T, name: string): T {
    return function (...args: Parameters<T>): ReturnType<T> {
      const start = performance.now();
      const result = fn(...args);
      const end = performance.now();
      console.log(`  [TIME] ${name}: ${(end - start).toFixed(2)}ms`);
      return result;
    } as T;
  }

  console.log("\nTiming:");
  const slowFn = timed(() => {
    let sum = 0;
    for (let i = 0; i < 1000000; i++) sum += i;
    return sum;
  }, "slowFn");
  slowFn();

  console.log();
}

// --- コード生成パターン ---
function codeGeneration(): void {
  console.log("--- コード生成パターン ---");

  // ファクトリ関数
  function createClass<T extends Record<string, any>>(
    name: string,
    defaults: T
  ): new (overrides?: Partial<T>) => T {
    return class {
      constructor(overrides?: Partial<T>) {
        Object.assign(this, defaults, overrides);
      }
    } as any;
  }

  const Person = createClass("Person", { name: "", age: 0, active: true });
  const person = new Person({ name: "Alice", age: 30 });
  console.log(`Factory: ${JSON.stringify(person)}`);

  // スキーマからの型生成
  const schema = {
    name: { type: "string" as const, required: true },
    age: { type: "number" as const, required: false },
    email: { type: "string" as const, required: true },
  } as const;

  function createValidator<T extends Record<string, { type: string; required: boolean }>>(
    schema: T
  ) {
    return (data: unknown): boolean => {
      if (typeof data !== "object" || data === null) return false;
      for (const [key, config] of Object.entries(schema)) {
        const value = (data as any)[key];
        if (config.required && value === undefined) return false;
        if (value !== undefined && typeof value !== config.type) return false;
      }
      return true;
    };
  }

  const validate = createValidator(schema);
  console.log(`\nValidator:`);
  console.log(`  valid: ${validate({ name: "Alice", email: "a@b.com" })}`);
  console.log(`  invalid (missing email): ${validate({ name: "Alice" })}`);

  console.log();
}

// --- Symbol の活用 ---
function symbolsDemo(): void {
  console.log("--- Symbol の活用 ---");

  // プライベートプロパティ
  const _private = Symbol("private");

  class MyClass {
    [_private] = "secret";
    public = "public";

    getPrivate(): string {
      return this[_private];
    }
  }

  const obj = new MyClass();
  console.log(`public: ${obj.public}`);
  console.log(`private (via method): ${obj.getPrivate()}`);

  // Well-known Symbols
  const iterable = {
    data: [1, 2, 3],
    *[Symbol.iterator]() {
      yield* this.data;
    },
  };
  console.log(`\nSymbol.iterator: ${[...iterable]}`);

  // Symbol.toStringTag
  const tagged = {
    [Symbol.toStringTag]: "MyObject",
  };
  console.log(`Symbol.toStringTag: ${Object.prototype.toString.call(tagged)}`);

  // Symbol.toPrimitive
  const custom = {
    [Symbol.toPrimitive](hint: string) {
      if (hint === "number") return 42;
      if (hint === "string") return "forty-two";
      return true;
    },
  };
  console.log(`Symbol.toPrimitive (number): ${+custom}`);
  console.log(`Symbol.toPrimitive (string): ${`${custom}`}`);

  console.log();
}

// --- 動的プロパティアクセス ---
function dynamicAccess(): void {
  console.log("--- 動的プロパティアクセス ---");

  // Computed property names
  const prefix = "user";
  const obj = {
    [`${prefix}Name`]: "Alice",
    [`${prefix}Age`]: 30,
  };
  console.log(`Computed properties: ${JSON.stringify(obj)}`);

  // 動的メソッド呼び出し
  const calculator = {
    add: (a: number, b: number) => a + b,
    sub: (a: number, b: number) => a - b,
    mul: (a: number, b: number) => a * b,
    div: (a: number, b: number) => a / b,
  };

  function calculate(op: keyof typeof calculator, a: number, b: number): number {
    return calculator[op](a, b);
  }

  console.log(`calculate("add", 10, 5) = ${calculate("add", 10, 5)}`);
  console.log(`calculate("mul", 10, 5) = ${calculate("mul", 10, 5)}`);

  // Object.fromEntries と Object.entries
  const entries: [string, number][] = [
    ["a", 1],
    ["b", 2],
    ["c", 3],
  ];
  const fromEntries = Object.fromEntries(entries);
  console.log(`\nObject.fromEntries: ${JSON.stringify(fromEntries)}`);

  const doubled = Object.fromEntries(
    Object.entries(fromEntries).map(([k, v]) => [k, v * 2])
  );
  console.log(`Transformed: ${JSON.stringify(doubled)}`);

  console.log();
}

// --- eval と Function (注意: セキュリティリスク) ---
function dynamicExecution(): void {
  console.log("--- 動的コード実行 (参考) ---");

  // Function コンストラクタ
  const dynamicFn = new Function("a", "b", "return a + b");
  console.log(`new Function: ${dynamicFn(2, 3)}`);

  // eval は避けるべき
  console.log(`eval は XSS やインジェクション攻撃のリスクがあるため避ける`);

  // 代替: 安全な式評価ライブラリを使用
  // 例: mathjs, expr-eval など

  console.log();
}

// --- 型の実行時チェック ---
function runtimeTypeChecks(): void {
  console.log("--- 型の実行時チェック ---");

  // typeof
  function checkType(value: unknown): string {
    if (typeof value === "string") return "string";
    if (typeof value === "number") return "number";
    if (typeof value === "boolean") return "boolean";
    if (Array.isArray(value)) return "array";
    if (value === null) return "null";
    if (typeof value === "object") return "object";
    return "unknown";
  }

  console.log(`typeof "hello": ${checkType("hello")}`);
  console.log(`typeof 42: ${checkType(42)}`);
  console.log(`typeof [1, 2]: ${checkType([1, 2])}`);

  // Zod-like バリデータ (簡易版)
  type Schema<T> = {
    parse: (value: unknown) => T;
    safeParse: (value: unknown) => { success: true; data: T } | { success: false; error: string };
  };

  function string(): Schema<string> {
    return {
      parse(value) {
        if (typeof value !== "string") throw new Error("Expected string");
        return value;
      },
      safeParse(value) {
        try {
          return { success: true, data: this.parse(value) };
        } catch {
          return { success: false, error: "Expected string" };
        }
      },
    };
  }

  function object<T extends Record<string, Schema<any>>>(
    shape: T
  ): Schema<{ [K in keyof T]: T[K] extends Schema<infer U> ? U : never }> {
    return {
      parse(value) {
        if (typeof value !== "object" || value === null) {
          throw new Error("Expected object");
        }
        const result: any = {};
        for (const [key, schema] of Object.entries(shape)) {
          result[key] = schema.parse((value as any)[key]);
        }
        return result;
      },
      safeParse(value) {
        try {
          return { success: true, data: this.parse(value) };
        } catch (e) {
          return { success: false, error: (e as Error).message };
        }
      },
    };
  }

  const userSchema = object({
    name: string(),
  });

  console.log(`\nSchema validation:`);
  console.log(`  valid: ${JSON.stringify(userSchema.safeParse({ name: "Alice" }))}`);
  console.log(`  invalid: ${JSON.stringify(userSchema.safeParse({ name: 123 }))}`);

  console.log();
}

// --- 実行 ---
typeLevelMeta();
reflectAPI();
proxyDemo();
decoratorsDemo();
codeGeneration();
symbolsDemo();
dynamicAccess();
dynamicExecution();
runtimeTypeChecks();
