// エラー処理 - TypeScript 実装
//
// JavaScript/TypeScript は例外ベースのエラー処理
// Result パターンも型安全に実装可能

console.log("=== Error Handling Demo ===\n");

// --- 例外の基本 ---
function exceptionBasics(): void {
  console.log("--- 例外の基本 ---");

  // throw と try-catch
  function divide(a: number, b: number): number {
    if (b === 0) {
      throw new Error("Division by zero");
    }
    return a / b;
  }

  try {
    console.log(`10 / 2 = ${divide(10, 2)}`);
    console.log(`10 / 0 = ${divide(10, 0)}`);
  } catch (error) {
    if (error instanceof Error) {
      console.log(`Error: ${error.message}`);
    }
  }

  // finally
  function withFinally(): void {
    try {
      console.log("try block");
      throw new Error("error in try");
    } catch (error) {
      console.log(`catch: ${(error as Error).message}`);
    } finally {
      console.log("finally: always runs");
    }
  }

  console.log();
  withFinally();

  console.log();
}

// --- カスタムエラー ---
function customErrors(): void {
  console.log("--- カスタムエラー ---");

  // カスタムエラークラス
  class ValidationError extends Error {
    constructor(
      message: string,
      public field: string
    ) {
      super(message);
      this.name = "ValidationError";
    }
  }

  class NetworkError extends Error {
    constructor(
      message: string,
      public statusCode: number
    ) {
      super(message);
      this.name = "NetworkError";
    }
  }

  function validateUser(user: { name?: string; email?: string }): void {
    if (!user.name) {
      throw new ValidationError("Name is required", "name");
    }
    if (!user.email?.includes("@")) {
      throw new ValidationError("Invalid email", "email");
    }
  }

  try {
    validateUser({ name: "Alice" });
  } catch (error) {
    if (error instanceof ValidationError) {
      console.log(`ValidationError: ${error.message} (field: ${error.field})`);
    }
  }

  // instanceof による分岐
  function handleError(error: unknown): void {
    if (error instanceof ValidationError) {
      console.log(`Validation failed on ${error.field}`);
    } else if (error instanceof NetworkError) {
      console.log(`Network error: ${error.statusCode}`);
    } else if (error instanceof Error) {
      console.log(`Unknown error: ${error.message}`);
    } else {
      console.log(`Non-error thrown: ${error}`);
    }
  }

  handleError(new ValidationError("test", "field"));
  handleError(new NetworkError("Not found", 404));

  console.log();
}

// --- Result パターン ---
function resultPattern(): void {
  console.log("--- Result パターン ---");

  // Result 型の定義
  type Result<T, E = Error> =
    | { ok: true; value: T }
    | { ok: false; error: E };

  // ヘルパー関数
  const Ok = <T>(value: T): Result<T, never> => ({ ok: true, value });
  const Err = <E>(error: E): Result<never, E> => ({ ok: false, error });

  // 使用例
  function safeDivide(a: number, b: number): Result<number, string> {
    if (b === 0) {
      return Err("Division by zero");
    }
    return Ok(a / b);
  }

  const result1 = safeDivide(10, 2);
  if (result1.ok) {
    console.log(`10 / 2 = ${result1.value}`);
  }

  const result2 = safeDivide(10, 0);
  if (!result2.ok) {
    console.log(`Error: ${result2.error}`);
  }

  // Result のチェーン
  function parseNumber(str: string): Result<number, string> {
    const n = Number(str);
    if (isNaN(n)) {
      return Err(`"${str}" is not a number`);
    }
    return Ok(n);
  }

  function compute(input: string): Result<number, string> {
    const parsed = parseNumber(input);
    if (!parsed.ok) return parsed;

    const divided = safeDivide(100, parsed.value);
    if (!divided.ok) return divided;

    return Ok(divided.value * 2);
  }

  console.log(`compute("5"): ${JSON.stringify(compute("5"))}`);
  console.log(`compute("0"): ${JSON.stringify(compute("0"))}`);
  console.log(`compute("abc"): ${JSON.stringify(compute("abc"))}`);

  console.log();
}

// --- Option パターン ---
function optionPattern(): void {
  console.log("--- Option パターン ---");

  // Option 型
  type Option<T> = { some: true; value: T } | { some: false };

  const Some = <T>(value: T): Option<T> => ({ some: true, value });
  const None = <T>(): Option<T> => ({ some: false });

  // 使用例
  function findUser(id: number): Option<{ id: number; name: string }> {
    const users = [
      { id: 1, name: "Alice" },
      { id: 2, name: "Bob" },
    ];
    const user = users.find((u) => u.id === id);
    return user ? Some(user) : None();
  }

  const user1 = findUser(1);
  if (user1.some) {
    console.log(`Found: ${user1.value.name}`);
  }

  const user3 = findUser(3);
  if (!user3.some) {
    console.log(`User 3 not found`);
  }

  // map, flatMap の実装
  function mapOption<T, U>(opt: Option<T>, fn: (v: T) => U): Option<U> {
    return opt.some ? Some(fn(opt.value)) : None();
  }

  const doubled = mapOption(Some(21), (x) => x * 2);
  console.log(`mapOption: ${doubled.some ? doubled.value : "None"}`);

  console.log();
}

// --- 非同期エラー処理 ---
async function asyncErrorHandling(): Promise<void> {
  console.log("--- 非同期エラー処理 ---");

  async function fetchData(shouldFail: boolean): Promise<string> {
    await new Promise((r) => setTimeout(r, 50));
    if (shouldFail) {
      throw new Error("Fetch failed");
    }
    return "data";
  }

  // try-catch
  try {
    const data = await fetchData(false);
    console.log(`Success: ${data}`);
  } catch (error) {
    console.log(`Error: ${(error as Error).message}`);
  }

  // .catch()
  const result = await fetchData(true).catch((e) => `Caught: ${e.message}`);
  console.log(`With .catch(): ${result}`);

  // Promise.allSettled
  const results = await Promise.allSettled([
    fetchData(false),
    fetchData(true),
    fetchData(false),
  ]);

  results.forEach((r, i) => {
    if (r.status === "fulfilled") {
      console.log(`  [${i}] fulfilled: ${r.value}`);
    } else {
      console.log(`  [${i}] rejected: ${r.reason.message}`);
    }
  });

  console.log();
}

// --- 型安全なエラー処理 ---
function typeSafeErrors(): void {
  console.log("--- 型安全なエラー処理 ---");

  // Discriminated Union によるエラー
  type ApiResponse<T> =
    | { status: "success"; data: T }
    | { status: "error"; error: { code: number; message: string } }
    | { status: "loading" };

  function handleResponse<T>(response: ApiResponse<T>): string {
    switch (response.status) {
      case "success":
        return `Data: ${JSON.stringify(response.data)}`;
      case "error":
        return `Error ${response.code}: ${response.message}`;
      case "loading":
        return "Loading...";
    }
  }

  console.log(handleResponse({ status: "success", data: { id: 1 } }));
  console.log(handleResponse({ status: "error", error: { code: 404, message: "Not found" } }));

  // never 型を使った網羅性チェック
  function assertNever(x: never): never {
    throw new Error(`Unexpected value: ${x}`);
  }

  type Status = "pending" | "success" | "error";

  function processStatus(status: Status): string {
    switch (status) {
      case "pending":
        return "waiting";
      case "success":
        return "done";
      case "error":
        return "failed";
      default:
        return assertNever(status); // 網羅性をコンパイル時にチェック
    }
  }

  console.log(`processStatus: ${processStatus("success")}`);

  console.log();
}

// --- エラー境界パターン ---
function errorBoundary(): void {
  console.log("--- エラー境界パターン ---");

  // 安全なラッパー
  function tryCatch<T>(fn: () => T): [T, null] | [null, Error] {
    try {
      return [fn(), null];
    } catch (error) {
      return [null, error instanceof Error ? error : new Error(String(error))];
    }
  }

  const [result, error] = tryCatch(() => {
    return JSON.parse('{"name": "Alice"}');
  });

  if (error) {
    console.log(`Error: ${error.message}`);
  } else {
    console.log(`Parsed: ${JSON.stringify(result)}`);
  }

  // 非同期版
  async function asyncTryCatch<T>(
    fn: () => Promise<T>
  ): Promise<[T, null] | [null, Error]> {
    try {
      return [await fn(), null];
    } catch (error) {
      return [null, error instanceof Error ? error : new Error(String(error))];
    }
  }

  // フォールバック付き
  function withFallback<T>(fn: () => T, fallback: T): T {
    try {
      return fn();
    } catch {
      return fallback;
    }
  }

  const value = withFallback(() => JSON.parse("invalid"), {});
  console.log(`withFallback: ${JSON.stringify(value)}`);

  console.log();
}

// --- 検証とアサーション ---
function validationAndAssertions(): void {
  console.log("--- 検証とアサーション ---");

  // Type assertion function
  function assertString(value: unknown): asserts value is string {
    if (typeof value !== "string") {
      throw new Error(`Expected string, got ${typeof value}`);
    }
  }

  const maybeString: unknown = "hello";
  assertString(maybeString);
  // ここでは maybeString は string 型
  console.log(`assertString: ${maybeString.toUpperCase()}`);

  // 条件付き assert
  function assertDefined<T>(value: T | undefined | null): asserts value is T {
    if (value === undefined || value === null) {
      throw new Error("Value is not defined");
    }
  }

  // Branded types による検証済みの型
  type ValidEmail = string & { __brand: "ValidEmail" };

  function validateEmail(email: string): ValidEmail {
    if (!email.includes("@")) {
      throw new Error("Invalid email");
    }
    return email as ValidEmail;
  }

  const email = validateEmail("alice@example.com");
  console.log(`ValidEmail: ${email}`);

  // Zod-like バリデーション (簡易版)
  type Validator<T> = {
    parse: (value: unknown) => T;
    safeParse: (value: unknown) => { success: true; data: T } | { success: false; error: Error };
  };

  function string(): Validator<string> {
    return {
      parse(value) {
        if (typeof value !== "string") throw new Error("Expected string");
        return value;
      },
      safeParse(value) {
        try {
          return { success: true, data: this.parse(value) };
        } catch (error) {
          return { success: false, error: error as Error };
        }
      },
    };
  }

  const validator = string();
  console.log(`safeParse("hello"): ${JSON.stringify(validator.safeParse("hello"))}`);
  console.log(`safeParse(123): ${JSON.stringify(validator.safeParse(123))}`);

  console.log();
}

// --- 実行 ---
async function main(): Promise<void> {
  exceptionBasics();
  customErrors();
  resultPattern();
  optionPattern();
  await asyncErrorHandling();
  typeSafeErrors();
  errorBoundary();
  validationAndAssertions();
}

main().catch(console.error);
