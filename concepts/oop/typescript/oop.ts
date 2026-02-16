// オブジェクト指向 - TypeScript 実装
//
// TypeScript は ES6 クラスに型を追加
// インターフェースは構造的部分型

console.log("=== OOP Demo ===\n");

// --- クラスの基本 ---
function classBasics(): void {
  console.log("--- クラスの基本 ---");

  class Person {
    // フィールド宣言
    private name: string;
    private age: number;

    // コンストラクタ
    constructor(name: string, age: number) {
      this.name = name;
      this.age = age;
    }

    // メソッド
    greet(): string {
      return `Hello, I'm ${this.name}`;
    }

    // getter
    get info(): string {
      return `${this.name} (${this.age})`;
    }

    // setter
    set setAge(age: number) {
      if (age >= 0) {
        this.age = age;
      }
    }
  }

  const person = new Person("Alice", 30);
  console.log(`greet(): ${person.greet()}`);
  console.log(`info: ${person.info}`);

  // コンストラクタ省略形
  class Point {
    constructor(
      public x: number,
      public y: number
    ) {}

    toString(): string {
      return `(${this.x}, ${this.y})`;
    }
  }

  const point = new Point(10, 20);
  console.log(`Point: ${point}`);

  console.log();
}

// --- アクセス修飾子 ---
function accessModifiers(): void {
  console.log("--- アクセス修飾子 ---");

  class Example {
    public publicField = "public"; // どこからでもアクセス可
    protected protectedField = "protected"; // 自クラスとサブクラスから
    private privateField = "private"; // 自クラスのみ
    readonly readonlyField = "readonly"; // 読み取り専用
    #truePrivate = "true private"; // ES2022 private field

    showFields(): void {
      console.log(`  public: ${this.publicField}`);
      console.log(`  protected: ${this.protectedField}`);
      console.log(`  private: ${this.privateField}`);
      console.log(`  readonly: ${this.readonlyField}`);
      console.log(`  #truePrivate: ${this.#truePrivate}`);
    }
  }

  const example = new Example();
  example.showFields();

  console.log(`\n外部からのアクセス:`);
  console.log(`  public: ${example.publicField}`);
  // console.log(example.protectedField);  // エラー
  // console.log(example.privateField);    // エラー
  // console.log(example.#truePrivate);    // エラー

  console.log();
}

// --- 継承 ---
function inheritance(): void {
  console.log("--- 継承 ---");

  class Animal {
    constructor(public name: string) {}

    speak(): string {
      return `${this.name} makes a sound`;
    }

    move(): string {
      return `${this.name} moves`;
    }
  }

  class Dog extends Animal {
    constructor(name: string, public breed: string) {
      super(name);
    }

    // オーバーライド
    override speak(): string {
      return `${this.name} says: Woof!`;
    }

    // 追加メソッド
    fetch(): string {
      return `${this.name} fetches the ball`;
    }
  }

  const dog = new Dog("Buddy", "Golden Retriever");
  console.log(`speak(): ${dog.speak()}`);
  console.log(`move(): ${dog.move()}`);
  console.log(`fetch(): ${dog.fetch()}`);
  console.log(`breed: ${dog.breed}`);

  console.log();
}

// --- インターフェース ---
function interfaces(): void {
  console.log("--- インターフェース ---");

  // インターフェース定義
  interface Shape {
    area(): number;
    perimeter(): number;
  }

  // 実装 (構造的部分型)
  class Rectangle implements Shape {
    constructor(
      public width: number,
      public height: number
    ) {}

    area(): number {
      return this.width * this.height;
    }

    perimeter(): number {
      return 2 * (this.width + this.height);
    }
  }

  class Circle implements Shape {
    constructor(public radius: number) {}

    area(): number {
      return Math.PI * this.radius ** 2;
    }

    perimeter(): number {
      return 2 * Math.PI * this.radius;
    }
  }

  // ポリモーフィズム
  const shapes: Shape[] = [new Rectangle(10, 5), new Circle(7)];

  shapes.forEach((shape) => {
    console.log(`${shape.constructor.name}:`);
    console.log(`  area: ${shape.area().toFixed(2)}`);
    console.log(`  perimeter: ${shape.perimeter().toFixed(2)}`);
  });

  // 構造的部分型: implements なしでも OK
  const plainObject: Shape = {
    area: () => 100,
    perimeter: () => 40,
  };
  console.log(`Plain object area: ${plainObject.area()}`);

  console.log();
}

// --- 抽象クラス ---
function abstractClasses(): void {
  console.log("--- 抽象クラス ---");

  abstract class Vehicle {
    constructor(public brand: string) {}

    // 抽象メソッド (サブクラスで実装必須)
    abstract start(): string;

    // 通常のメソッド
    describe(): string {
      return `This is a ${this.brand}`;
    }
  }

  class Car extends Vehicle {
    start(): string {
      return `${this.brand} car engine started`;
    }
  }

  class Motorcycle extends Vehicle {
    start(): string {
      return `${this.brand} motorcycle engine revved`;
    }
  }

  const vehicles: Vehicle[] = [new Car("Toyota"), new Motorcycle("Harley")];

  vehicles.forEach((v) => {
    console.log(`${v.describe()}`);
    console.log(`  start(): ${v.start()}`);
  });

  console.log();
}

// --- ジェネリッククラス ---
function genericClasses(): void {
  console.log("--- ジェネリッククラス ---");

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

    get length(): number {
      return this.items.length;
    }
  }

  const numStack = new Stack<number>();
  numStack.push(1);
  numStack.push(2);
  numStack.push(3);
  console.log(`Stack<number>: pop = ${numStack.pop()}, peek = ${numStack.peek()}`);

  const strStack = new Stack<string>();
  strStack.push("a");
  strStack.push("b");
  console.log(`Stack<string>: pop = ${strStack.pop()}`);

  // 制約付きジェネリクス
  interface HasId {
    id: number;
  }

  class Repository<T extends HasId> {
    private items: Map<number, T> = new Map();

    save(item: T): void {
      this.items.set(item.id, item);
    }

    find(id: number): T | undefined {
      return this.items.get(id);
    }
  }

  const repo = new Repository<{ id: number; name: string }>();
  repo.save({ id: 1, name: "Alice" });
  console.log(`Repository: find(1) = ${JSON.stringify(repo.find(1))}`);

  console.log();
}

// --- Mixin パターン ---
function mixins(): void {
  console.log("--- Mixin パターン ---");

  // Mixin 関数
  type Constructor<T = {}> = new (...args: any[]) => T;

  function Timestamped<TBase extends Constructor>(Base: TBase) {
    return class extends Base {
      createdAt = new Date();
      updatedAt = new Date();

      touch() {
        this.updatedAt = new Date();
      }
    };
  }

  function Tagged<TBase extends Constructor>(Base: TBase) {
    return class extends Base {
      tags: string[] = [];

      addTag(tag: string) {
        this.tags.push(tag);
      }
    };
  }

  // Mixin の適用
  class BaseEntity {
    constructor(public id: number) {}
  }

  class Entity extends Tagged(Timestamped(BaseEntity)) {
    constructor(id: number) {
      super(id);
    }
  }

  const entity = new Entity(1);
  entity.addTag("important");
  entity.touch();
  console.log(`Entity id: ${entity.id}`);
  console.log(`  tags: ${entity.tags}`);
  console.log(`  createdAt: ${entity.createdAt.toISOString()}`);

  console.log();
}

// --- デザインパターン ---
function designPatterns(): void {
  console.log("--- デザインパターン ---");

  // Singleton
  class Singleton {
    private static instance: Singleton;
    private constructor(public value: number) {}

    static getInstance(value: number): Singleton {
      if (!Singleton.instance) {
        Singleton.instance = new Singleton(value);
      }
      return Singleton.instance;
    }
  }

  const s1 = Singleton.getInstance(1);
  const s2 = Singleton.getInstance(2);
  console.log(`Singleton: s1.value = ${s1.value}, s2.value = ${s2.value}`);
  console.log(`  s1 === s2: ${s1 === s2}`);

  // Factory
  interface Product {
    name: string;
    price: number;
  }

  class ProductFactory {
    static create(type: "book" | "electronics"): Product {
      switch (type) {
        case "book":
          return { name: "Book", price: 20 };
        case "electronics":
          return { name: "Phone", price: 500 };
      }
    }
  }

  const book = ProductFactory.create("book");
  console.log(`\nFactory: ${JSON.stringify(book)}`);

  // Builder
  class RequestBuilder {
    private url = "";
    private method = "GET";
    private headers: Record<string, string> = {};

    setUrl(url: string): this {
      this.url = url;
      return this;
    }

    setMethod(method: string): this {
      this.method = method;
      return this;
    }

    addHeader(key: string, value: string): this {
      this.headers[key] = value;
      return this;
    }

    build(): { url: string; method: string; headers: Record<string, string> } {
      return { url: this.url, method: this.method, headers: this.headers };
    }
  }

  const request = new RequestBuilder()
    .setUrl("/api/users")
    .setMethod("POST")
    .addHeader("Content-Type", "application/json")
    .build();

  console.log(`\nBuilder: ${JSON.stringify(request)}`);

  console.log();
}

// --- 静的メンバ ---
function staticMembers(): void {
  console.log("--- 静的メンバ ---");

  class Counter {
    static count = 0;
    id: number;

    constructor() {
      Counter.count++;
      this.id = Counter.count;
    }

    static getCount(): number {
      return Counter.count;
    }

    static reset(): void {
      Counter.count = 0;
    }
  }

  const c1 = new Counter();
  const c2 = new Counter();
  const c3 = new Counter();

  console.log(`Counter.getCount(): ${Counter.getCount()}`);
  console.log(`c1.id: ${c1.id}, c2.id: ${c2.id}, c3.id: ${c3.id}`);

  // 静的ブロック (ES2022)
  class Config {
    static values: Map<string, string>;

    static {
      this.values = new Map();
      this.values.set("env", "development");
      this.values.set("version", "1.0.0");
    }
  }

  console.log(`\nConfig.values: ${JSON.stringify([...Config.values])}`);

  console.log();
}

// --- 実行 ---
classBasics();
accessModifiers();
inheritance();
interfaces();
abstractClasses();
genericClasses();
mixins();
designPatterns();
staticMembers();
