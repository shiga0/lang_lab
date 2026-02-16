# TypeScript チートシート

## 基本型

```typescript
// プリミティブ型
let str: string = "hello";
let num: number = 42;
let bool: boolean = true;
let n: null = null;
let u: undefined = undefined;

// 配列
let arr1: number[] = [1, 2, 3];
let arr2: Array<number> = [1, 2, 3];

// タプル
let tuple: [string, number] = ["hello", 42];

// オブジェクト
let obj: { name: string; age: number } = { name: "Alice", age: 30 };

// any / unknown
let a: any = "anything";      // 型チェックを無効化
let u: unknown = "anything";  // 安全な any (使用前に型チェック必要)

// void / never
function log(): void { console.log("hi"); }
function fail(): never { throw new Error(); }
```

## 型エイリアスとインターフェース

```typescript
// 型エイリアス
type Point = {
  x: number;
  y: number;
};

type ID = string | number;

// インターフェース
interface User {
  name: string;
  age: number;
  email?: string;  // オプショナル
  readonly id: number;  // 読み取り専用
}

// 拡張
interface Employee extends User {
  department: string;
}

// 型エイリアスの交差型
type EmployeeType = User & { department: string };
```

## ユニオン型と交差型

```typescript
// ユニオン型 (OR)
type StringOrNumber = string | number;

// 交差型 (AND)
type Named = { name: string };
type Aged = { age: number };
type Person = Named & Aged;

// リテラル型
type Direction = "north" | "south" | "east" | "west";
type Digit = 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9;
```

## 関数

```typescript
// 基本
function add(a: number, b: number): number {
  return a + b;
}

// アロー関数
const multiply = (a: number, b: number): number => a * b;

// オプショナル引数とデフォルト値
function greet(name: string, greeting: string = "Hello"): string {
  return `${greeting}, ${name}!`;
}

// レストパラメータ
function sum(...numbers: number[]): number {
  return numbers.reduce((a, b) => a + b, 0);
}

// 関数型
type BinaryFn = (a: number, b: number) => number;
const subtract: BinaryFn = (a, b) => a - b;

// オーバーロード
function format(value: string): string;
function format(value: number): string;
function format(value: string | number): string {
  return String(value);
}
```

## ジェネリクス

```typescript
// 関数
function identity<T>(value: T): T {
  return value;
}

// 複数の型パラメータ
function map<T, U>(arr: T[], fn: (item: T) => U): U[] {
  return arr.map(fn);
}

// 制約
function getLength<T extends { length: number }>(item: T): number {
  return item.length;
}

// インターフェース
interface Container<T> {
  value: T;
  getValue(): T;
}

// クラス
class Box<T> {
  constructor(private value: T) {}
  get(): T { return this.value; }
}

// デフォルト型
type Result<T = string> = { data: T };
```

## 型ガードとナローイング

```typescript
// typeof
function process(value: string | number) {
  if (typeof value === "string") {
    return value.toUpperCase();  // string
  }
  return value.toFixed(2);  // number
}

// instanceof
function handle(err: Error | string) {
  if (err instanceof Error) {
    return err.message;
  }
  return err;
}

// in
interface Dog { bark(): void; }
interface Cat { meow(): void; }

function speak(animal: Dog | Cat) {
  if ("bark" in animal) {
    animal.bark();
  } else {
    animal.meow();
  }
}

// カスタム型ガード
function isString(value: unknown): value is string {
  return typeof value === "string";
}

// Discriminated Union
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
```

## ユーティリティ型

```typescript
interface User {
  name: string;
  age: number;
  email: string;
}

// Partial: 全プロパティをオプショナルに
type PartialUser = Partial<User>;

// Required: 全プロパティを必須に
type RequiredUser = Required<PartialUser>;

// Readonly: 全プロパティを読み取り専用に
type ReadonlyUser = Readonly<User>;

// Pick: 指定したプロパティのみ
type UserName = Pick<User, "name">;

// Omit: 指定したプロパティを除外
type UserWithoutEmail = Omit<User, "email">;

// Record: キーと値の型を指定
type UserMap = Record<string, User>;

// Exclude: ユニオンから除外
type T1 = Exclude<"a" | "b" | "c", "a">;  // "b" | "c"

// Extract: ユニオンから抽出
type T2 = Extract<"a" | "b" | "c", "a" | "f">;  // "a"

// NonNullable: null と undefined を除外
type T3 = NonNullable<string | null | undefined>;  // string

// ReturnType: 関数の戻り値の型
type T4 = ReturnType<() => string>;  // string

// Parameters: 関数の引数の型
type T5 = Parameters<(a: string, b: number) => void>;  // [string, number]

// Awaited: Promise の解決値の型
type T6 = Awaited<Promise<string>>;  // string
```

## クラス

```typescript
class Animal {
  // フィールド
  private name: string;
  protected age: number;
  readonly species: string;

  // コンストラクタ省略形
  constructor(
    name: string,
    age: number,
    public sound: string  // 自動でフィールド化
  ) {
    this.name = name;
    this.age = age;
    this.species = "unknown";
  }

  // メソッド
  speak(): void {
    console.log(this.sound);
  }

  // getter/setter
  get info(): string {
    return `${this.name} (${this.age})`;
  }

  // 静的メンバ
  static create(name: string): Animal {
    return new Animal(name, 0, "...");
  }
}

// 継承
class Dog extends Animal {
  constructor(name: string, age: number) {
    super(name, age, "Woof!");
  }

  // オーバーライド
  override speak(): void {
    console.log(`${this.sound} ${this.sound}!`);
  }
}

// 抽象クラス
abstract class Shape {
  abstract area(): number;

  describe(): string {
    return `Area: ${this.area()}`;
  }
}

// インターフェースの実装
interface Serializable {
  serialize(): string;
}

class Point implements Serializable {
  constructor(public x: number, public y: number) {}

  serialize(): string {
    return JSON.stringify({ x: this.x, y: this.y });
  }
}
```

## 非同期処理

```typescript
// Promise
function fetchData(): Promise<string> {
  return new Promise((resolve, reject) => {
    setTimeout(() => resolve("data"), 1000);
  });
}

// async/await
async function getData(): Promise<string> {
  const data = await fetchData();
  return data.toUpperCase();
}

// Promise.all
async function fetchAll(): Promise<[string, number]> {
  const [a, b] = await Promise.all([
    fetchString(),
    fetchNumber(),
  ]);
  return [a, b];
}

// エラーハンドリング
async function safeGet(): Promise<string | null> {
  try {
    return await fetchData();
  } catch (error) {
    console.error(error);
    return null;
  }
}
```

## モジュール

```typescript
// エクスポート
export const PI = 3.14159;
export function add(a: number, b: number): number {
  return a + b;
}
export default class Calculator {}

// インポート
import Calculator, { PI, add } from "./math";
import * as math from "./math";
import type { User } from "./types";  // 型のみ

// 再エクスポート
export { add } from "./math";
export * from "./utils";
```

## 高度な型

```typescript
// Mapped Types
type Flags<T> = {
  [K in keyof T]: boolean;
};

// Conditional Types
type IsString<T> = T extends string ? true : false;

// infer
type ReturnOf<T> = T extends (...args: any[]) => infer R ? R : never;

// Template Literal Types
type EventName = `on${Capitalize<"click" | "focus">}`;  // "onClick" | "onFocus"

// Recursive Types
type JSONValue =
  | string
  | number
  | boolean
  | null
  | JSONValue[]
  | { [key: string]: JSONValue };

// Variadic Tuple Types
type Concat<T extends any[], U extends any[]> = [...T, ...U];

// const アサーション
const config = {
  endpoint: "/api",
  timeout: 3000,
} as const;
// 型: { readonly endpoint: "/api"; readonly timeout: 3000; }
```

## 宣言ファイル (.d.ts)

```typescript
// グローバル宣言
declare const VERSION: string;
declare function log(message: string): void;

// モジュール宣言
declare module "my-library" {
  export function doSomething(): void;
}

// 型のみのインポート/エクスポート
export type { User, Product };
import type { Config } from "./config";
```

## tsconfig.json

```json
{
  "compilerOptions": {
    "target": "ES2022",
    "module": "NodeNext",
    "moduleResolution": "NodeNext",
    "strict": true,
    "esModuleInterop": true,
    "skipLibCheck": true,
    "outDir": "./dist",
    "rootDir": "./src",
    "declaration": true,
    "sourceMap": true
  },
  "include": ["src/**/*"],
  "exclude": ["node_modules", "dist"]
}
```

## よく使うパターン

```typescript
// Result パターン
type Result<T, E = Error> =
  | { ok: true; value: T }
  | { ok: false; error: E };

function divide(a: number, b: number): Result<number, string> {
  if (b === 0) {
    return { ok: false, error: "Division by zero" };
  }
  return { ok: true, value: a / b };
}

// Builder パターン
class RequestBuilder {
  private url = "";
  private method = "GET";

  setUrl(url: string): this {
    this.url = url;
    return this;
  }

  setMethod(method: string): this {
    this.method = method;
    return this;
  }

  build(): Request {
    return new Request(this.url, { method: this.method });
  }
}

// Branded Types (名目的型)
type UserId = string & { readonly brand: unique symbol };
type OrderId = string & { readonly brand: unique symbol };

function createUserId(id: string): UserId {
  return id as UserId;
}

// satisfies 演算子 (TS 4.9+)
const palette = {
  red: [255, 0, 0],
  green: "#00ff00",
} satisfies Record<string, [number, number, number] | string>;
```
