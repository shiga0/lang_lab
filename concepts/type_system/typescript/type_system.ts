// 型システム - TypeScript 実装
//
// TypeScript は構造的部分型 (Structural Subtyping) を採用
// 名前ではなく構造で型の互換性を判定

console.log("=== Type System Demo ===\n");

// --- 基本型 ---
function basicTypes(): void {
  console.log("--- 基本型 ---");

  // プリミティブ型
  const str: string = "Hello, TypeScript!";
  const num: number = 42;
  const float: number = 3.14; // number は整数も小数も
  const bool: boolean = true;
  const n: null = null;
  const u: undefined = undefined;

  console.log(`string: ${str}`);
  console.log(`number (int): ${num}`);
  console.log(`number (float): ${float}`);
  console.log(`boolean: ${bool}`);
  console.log(`null: ${n}, undefined: ${u}`);

  // bigint
  const big: bigint = 9007199254740991n;
  console.log(`bigint: ${big}`);

  // symbol
  const sym: symbol = Symbol("unique");
  console.log(`symbol: ${sym.toString()}`);

  console.log();
}

// --- 配列とタプル ---
function arraysAndTuples(): void {
  console.log("--- 配列とタプル ---");

  // 配列
  const numbers: number[] = [1, 2, 3, 4, 5];
  const strings: Array<string> = ["a", "b", "c"];
  console.log(`number[]: ${numbers}`);
  console.log(`Array<string>: ${strings}`);

  // タプル (固定長・各要素の型が異なる)
  const tuple: [string, number, boolean] = ["hello", 42, true];
  console.log(`tuple: ${tuple}`);

  // 名前付きタプル
  type Point = [x: number, y: number];
  const point: Point = [10, 20];
  console.log(`named tuple: (${point[0]}, ${point[1]})`);

  // 可変長タプル
  type StringNumberPairs = [string, ...number[]];
  const pairs: StringNumberPairs = ["values", 1, 2, 3];
  console.log(`variadic tuple: ${pairs}`);

  console.log();
}

// --- オブジェクト型 ---
function objectTypes(): void {
  console.log("--- オブジェクト型 ---");

  // インラインオブジェクト型
  const user: { name: string; age: number } = {
    name: "Alice",
    age: 30,
  };
  console.log(`object: ${JSON.stringify(user)}`);

  // インターフェース
  interface Person {
    name: string;
    age: number;
    email?: string; // オプショナル
    readonly id: number; // 読み取り専用
  }

  const person: Person = { name: "Bob", age: 25, id: 1 };
  console.log(`interface: ${JSON.stringify(person)}`);

  // 型エイリアス
  type Coordinate = {
    x: number;
    y: number;
  };

  const coord: Coordinate = { x: 10, y: 20 };
  console.log(`type alias: ${JSON.stringify(coord)}`);

  console.log();
}

// --- ユニオン型と交差型 ---
function unionAndIntersection(): void {
  console.log("--- ユニオン型と交差型 ---");

  // ユニオン型 (OR)
  type StringOrNumber = string | number;
  const val1: StringOrNumber = "hello";
  const val2: StringOrNumber = 42;
  console.log(`union: ${val1}, ${val2}`);

  // リテラル型とユニオン
  type Direction = "north" | "south" | "east" | "west";
  const dir: Direction = "north";
  console.log(`literal union: ${dir}`);

  // 交差型 (AND)
  type Named = { name: string };
  type Aged = { age: number };
  type NamedAndAged = Named & Aged;

  const person: NamedAndAged = { name: "Alice", age: 30 };
  console.log(`intersection: ${JSON.stringify(person)}`);

  // Discriminated Union (判別可能なユニオン)
  type Shape =
    | { kind: "circle"; radius: number }
    | { kind: "rectangle"; width: number; height: number };

  function area(shape: Shape): number {
    switch (shape.kind) {
      case "circle":
        return Math.PI * shape.radius ** 2;
      case "rectangle":
        return shape.width * shape.height;
    }
  }

  console.log(`discriminated union: area = ${area({ kind: "circle", radius: 5 }).toFixed(2)}`);

  console.log();
}

// --- 型推論 ---
function typeInference(): void {
  console.log("--- 型推論 ---");

  // 型推論
  const x = 42; // number と推論
  const y = "hello"; // string と推論
  const z = [1, 2, 3]; // number[] と推論

  console.log(`x: number = ${x}`);
  console.log(`y: string = ${y}`);
  console.log(`z: number[] = ${z}`);

  // const アサーション
  const config = {
    endpoint: "/api",
    timeout: 3000,
  } as const;
  // 型: { readonly endpoint: "/api"; readonly timeout: 3000 }

  console.log(`as const: ${JSON.stringify(config)}`);

  // 配列の const アサーション
  const directions = ["north", "south"] as const;
  // 型: readonly ["north", "south"]
  console.log(`const array: ${directions}`);

  console.log();
}

// --- ジェネリクス ---
function generics(): void {
  console.log("--- ジェネリクス ---");

  // ジェネリック関数
  function identity<T>(value: T): T {
    return value;
  }

  console.log(`identity<number>: ${identity(42)}`);
  console.log(`identity<string>: ${identity("hello")}`);

  // 制約付きジェネリクス
  function getLength<T extends { length: number }>(item: T): number {
    return item.length;
  }

  console.log(`getLength("hello"): ${getLength("hello")}`);
  console.log(`getLength([1,2,3]): ${getLength([1, 2, 3])}`);

  // ジェネリッククラス
  class Box<T> {
    constructor(private value: T) {}
    get(): T {
      return this.value;
    }
  }

  const numBox = new Box(42);
  const strBox = new Box("hello");
  console.log(`Box<number>: ${numBox.get()}`);
  console.log(`Box<string>: ${strBox.get()}`);

  // 複数の型パラメータ
  function map<T, U>(arr: T[], fn: (item: T) => U): U[] {
    return arr.map(fn);
  }

  const doubled = map([1, 2, 3], (x) => x * 2);
  console.log(`map: ${doubled}`);

  console.log();
}

// --- 型ガードとナローイング ---
function typeGuards(): void {
  console.log("--- 型ガードとナローイング ---");

  // typeof
  function process(value: string | number): string {
    if (typeof value === "string") {
      return value.toUpperCase(); // string と推論
    }
    return value.toFixed(2); // number と推論
  }

  console.log(`typeof guard (string): ${process("hello")}`);
  console.log(`typeof guard (number): ${process(3.14159)}`);

  // instanceof
  class Dog {
    bark() {
      return "Woof!";
    }
  }
  class Cat {
    meow() {
      return "Meow!";
    }
  }

  function speak(animal: Dog | Cat): string {
    if (animal instanceof Dog) {
      return animal.bark();
    }
    return animal.meow();
  }

  console.log(`instanceof guard: ${speak(new Dog())}`);

  // in 演算子
  type Fish = { swim: () => void };
  type Bird = { fly: () => void };

  function move(animal: Fish | Bird): string {
    if ("swim" in animal) {
      return "swimming";
    }
    return "flying";
  }

  console.log(`in guard: ${move({ swim: () => {} })}`);

  // カスタム型ガード
  function isString(value: unknown): value is string {
    return typeof value === "string";
  }

  const val: unknown = "hello";
  if (isString(val)) {
    console.log(`custom guard: ${val.toUpperCase()}`);
  }

  console.log();
}

// --- ユーティリティ型 ---
function utilityTypes(): void {
  console.log("--- ユーティリティ型 ---");

  interface User {
    name: string;
    age: number;
    email: string;
  }

  // Partial
  type PartialUser = Partial<User>;
  const partial: PartialUser = { name: "Alice" };
  console.log(`Partial<User>: ${JSON.stringify(partial)}`);

  // Required
  type RequiredUser = Required<PartialUser>;

  // Pick
  type UserName = Pick<User, "name" | "age">;
  const picked: UserName = { name: "Bob", age: 25 };
  console.log(`Pick<User, "name" | "age">: ${JSON.stringify(picked)}`);

  // Omit
  type UserWithoutEmail = Omit<User, "email">;
  const omitted: UserWithoutEmail = { name: "Charlie", age: 30 };
  console.log(`Omit<User, "email">: ${JSON.stringify(omitted)}`);

  // Record
  type UserMap = Record<string, number>;
  const scores: UserMap = { Alice: 100, Bob: 85 };
  console.log(`Record<string, number>: ${JSON.stringify(scores)}`);

  // Readonly
  type ReadonlyUser = Readonly<User>;

  // ReturnType
  function getUser(): User {
    return { name: "Alice", age: 30, email: "alice@example.com" };
  }
  type GetUserReturn = ReturnType<typeof getUser>;
  console.log(`ReturnType: User`);

  console.log();
}

// --- 実行 ---
basicTypes();
arraysAndTuples();
objectTypes();
unionAndIntersection();
typeInference();
generics();
typeGuards();
utilityTypes();
