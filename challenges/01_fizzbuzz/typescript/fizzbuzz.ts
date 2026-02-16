// FizzBuzz - TypeScript 実装
//
// 様々なアプローチで FizzBuzz を実装

console.log("=== FizzBuzz Demo ===\n");

// --- 基本実装 ---
function basicFizzBuzz(): void {
  console.log("--- Basic Implementation ---");

  function fizzBuzz(n: number): string {
    if (n % 15 === 0) return "FizzBuzz";
    if (n % 3 === 0) return "Fizz";
    if (n % 5 === 0) return "Buzz";
    return String(n);
  }

  for (let i = 1; i <= 15; i++) {
    console.log(fizzBuzz(i));
  }
  console.log();
}

// --- 配列とmap ---
function arrayFizzBuzz(): void {
  console.log("--- Array Map Implementation ---");

  const result = Array.from({ length: 15 }, (_, i) => i + 1).map((n) => {
    const fizz = n % 3 === 0 ? "Fizz" : "";
    const buzz = n % 5 === 0 ? "Buzz" : "";
    return fizz + buzz || String(n);
  });

  console.log(result.join(", "));
  console.log();
}

// --- ジェネレータ ---
function* fizzBuzzGenerator(limit: number): Generator<string> {
  for (let i = 1; i <= limit; i++) {
    const fizz = i % 3 === 0 ? "Fizz" : "";
    const buzz = i % 5 === 0 ? "Buzz" : "";
    yield fizz + buzz || String(i);
  }
}

function generatorFizzBuzz(): void {
  console.log("--- Generator Implementation ---");

  const gen = fizzBuzzGenerator(15);
  console.log([...gen].join(", "));
  console.log();
}

// --- 関数型 ---
function functionalFizzBuzz(): void {
  console.log("--- Functional Implementation ---");

  type Rule = [divisor: number, word: string];

  const rules: Rule[] = [
    [3, "Fizz"],
    [5, "Buzz"],
  ];

  const fizzBuzz = (n: number): string => {
    const result = rules
      .filter(([div]) => n % div === 0)
      .map(([, word]) => word)
      .join("");
    return result || String(n);
  };

  const output = Array.from({ length: 15 }, (_, i) => fizzBuzz(i + 1));
  console.log(output.join(", "));
  console.log();
}

// --- パターンマッチング風 ---
function patternMatchFizzBuzz(): void {
  console.log("--- Pattern Match Style ---");

  const fizzBuzz = (n: number): string => {
    const match: [boolean, boolean] = [n % 3 === 0, n % 5 === 0];

    switch (true) {
      case match[0] && match[1]:
        return "FizzBuzz";
      case match[0]:
        return "Fizz";
      case match[1]:
        return "Buzz";
      default:
        return String(n);
    }
  };

  for (let i = 1; i <= 15; i++) {
    process.stdout.write(fizzBuzz(i) + " ");
  }
  console.log("\n");
}

// --- 型安全な拡張版 ---
function typeSafeFizzBuzz(): void {
  console.log("--- Type-Safe Extended Version ---");

  interface FizzBuzzRule {
    divisor: number;
    word: string;
  }

  class FizzBuzzEngine {
    private rules: FizzBuzzRule[];

    constructor(rules: FizzBuzzRule[] = []) {
      this.rules = rules.sort((a, b) => a.divisor - b.divisor);
    }

    addRule(divisor: number, word: string): this {
      this.rules.push({ divisor, word });
      this.rules.sort((a, b) => a.divisor - b.divisor);
      return this;
    }

    generate(n: number): string {
      const result = this.rules
        .filter((rule) => n % rule.divisor === 0)
        .map((rule) => rule.word)
        .join("");
      return result || String(n);
    }

    *range(start: number, end: number): Generator<string> {
      for (let i = start; i <= end; i++) {
        yield this.generate(i);
      }
    }
  }

  const engine = new FizzBuzzEngine()
    .addRule(3, "Fizz")
    .addRule(5, "Buzz")
    .addRule(7, "Bazz");

  console.log("With Fizz(3), Buzz(5), Bazz(7):");
  const results = [...engine.range(1, 21)];
  console.log(results.join(", "));
  console.log();
}

// --- 非同期版 ---
async function asyncFizzBuzz(): Promise<void> {
  console.log("--- Async Implementation ---");

  async function* asyncFizzBuzzGen(limit: number): AsyncGenerator<string> {
    for (let i = 1; i <= limit; i++) {
      await new Promise((r) => setTimeout(r, 10));
      const fizz = i % 3 === 0 ? "Fizz" : "";
      const buzz = i % 5 === 0 ? "Buzz" : "";
      yield fizz + buzz || String(i);
    }
  }

  const results: string[] = [];
  for await (const value of asyncFizzBuzzGen(10)) {
    results.push(value);
  }
  console.log(results.join(", "));
  console.log();
}

// --- テスト ---
function runTests(): void {
  console.log("--- Tests ---");

  const fizzBuzz = (n: number): string => {
    if (n % 15 === 0) return "FizzBuzz";
    if (n % 3 === 0) return "Fizz";
    if (n % 5 === 0) return "Buzz";
    return String(n);
  };

  const tests: [number, string][] = [
    [1, "1"],
    [2, "2"],
    [3, "Fizz"],
    [4, "4"],
    [5, "Buzz"],
    [6, "Fizz"],
    [10, "Buzz"],
    [15, "FizzBuzz"],
    [30, "FizzBuzz"],
  ];

  let passed = 0;
  for (const [input, expected] of tests) {
    const result = fizzBuzz(input);
    if (result === expected) {
      passed++;
    } else {
      console.log(`FAIL: fizzBuzz(${input}) = "${result}", expected "${expected}"`);
    }
  }

  console.log(`${passed}/${tests.length} tests passed`);
}

// --- 実行 ---
async function main(): Promise<void> {
  basicFizzBuzz();
  arrayFizzBuzz();
  generatorFizzBuzz();
  functionalFizzBuzz();
  patternMatchFizzBuzz();
  typeSafeFizzBuzz();
  await asyncFizzBuzz();
  runTests();
}

main();
