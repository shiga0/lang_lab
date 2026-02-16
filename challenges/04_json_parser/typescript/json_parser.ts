// JSON Parser - TypeScript 実装 (再帰下降パーサー)
//
// 標準の JSON.parse を使わずに実装

console.log("=== JSON Parser Demo ===\n");

// --- 型定義 ---
type JSONValue = null | boolean | number | string | JSONValue[] | { [key: string]: JSONValue };

class ParseError extends Error {
  constructor(
    message: string,
    public position: number
  ) {
    super(`Parse error at position ${position}: ${message}`);
    this.name = "ParseError";
  }
}

// --- Parser クラス ---
class JSONParser {
  private input: string;
  private position: number;

  constructor(input: string) {
    this.input = input;
    this.position = 0;
  }

  parse(): JSONValue {
    this.skipWhitespace();
    const value = this.parseValue();
    this.skipWhitespace();

    if (this.position < this.input.length) {
      throw new ParseError("Unexpected characters after JSON value", this.position);
    }

    return value;
  }

  private current(): string {
    return this.input[this.position] ?? "";
  }

  private advance(): string {
    return this.input[this.position++] ?? "";
  }

  private skipWhitespace(): void {
    while (/\s/.test(this.current())) {
      this.position++;
    }
  }

  private parseValue(): JSONValue {
    this.skipWhitespace();
    const ch = this.current();

    switch (ch) {
      case "n":
        return this.parseNull();
      case "t":
      case "f":
        return this.parseBoolean();
      case '"':
        return this.parseString();
      case "[":
        return this.parseArray();
      case "{":
        return this.parseObject();
      default:
        if (ch === "-" || /\d/.test(ch)) {
          return this.parseNumber();
        }
        throw new ParseError(`Unexpected character: ${ch}`, this.position);
    }
  }

  private parseNull(): null {
    this.expectKeyword("null");
    return null;
  }

  private parseBoolean(): boolean {
    if (this.current() === "t") {
      this.expectKeyword("true");
      return true;
    }
    this.expectKeyword("false");
    return false;
  }

  private expectKeyword(keyword: string): void {
    for (const ch of keyword) {
      if (this.current() !== ch) {
        throw new ParseError(`Expected '${ch}'`, this.position);
      }
      this.advance();
    }
  }

  private parseString(): string {
    this.advance(); // consume opening "
    let result = "";

    while (true) {
      const ch = this.advance();

      if (ch === "") {
        throw new ParseError("Unterminated string", this.position);
      }

      if (ch === '"') {
        return result;
      }

      if (ch === "\\") {
        result += this.parseEscape();
      } else {
        result += ch;
      }
    }
  }

  private parseEscape(): string {
    const ch = this.advance();
    switch (ch) {
      case '"':
        return '"';
      case "\\":
        return "\\";
      case "/":
        return "/";
      case "b":
        return "\b";
      case "f":
        return "\f";
      case "n":
        return "\n";
      case "r":
        return "\r";
      case "t":
        return "\t";
      case "u": {
        let hex = "";
        for (let i = 0; i < 4; i++) {
          hex += this.advance();
        }
        const code = parseInt(hex, 16);
        if (isNaN(code)) {
          throw new ParseError("Invalid unicode escape", this.position);
        }
        return String.fromCharCode(code);
      }
      default:
        throw new ParseError(`Invalid escape: \\${ch}`, this.position);
    }
  }

  private parseNumber(): number {
    const start = this.position;

    // 負号
    if (this.current() === "-") {
      this.advance();
    }

    // 整数部
    if (this.current() === "0") {
      this.advance();
    } else if (/[1-9]/.test(this.current())) {
      while (/\d/.test(this.current())) {
        this.advance();
      }
    } else {
      throw new ParseError("Expected digit", this.position);
    }

    // 小数部
    if (this.current() === ".") {
      this.advance();
      if (!/\d/.test(this.current())) {
        throw new ParseError("Expected digit after decimal point", this.position);
      }
      while (/\d/.test(this.current())) {
        this.advance();
      }
    }

    // 指数部
    if (this.current() === "e" || this.current() === "E") {
      this.advance();
      if (this.current() === "+" || this.current() === "-") {
        this.advance();
      }
      if (!/\d/.test(this.current())) {
        throw new ParseError("Expected digit in exponent", this.position);
      }
      while (/\d/.test(this.current())) {
        this.advance();
      }
    }

    const numStr = this.input.slice(start, this.position);
    return parseFloat(numStr);
  }

  private parseArray(): JSONValue[] {
    this.advance(); // consume [
    this.skipWhitespace();

    const arr: JSONValue[] = [];

    // 空配列
    if (this.current() === "]") {
      this.advance();
      return arr;
    }

    while (true) {
      arr.push(this.parseValue());
      this.skipWhitespace();

      if (this.current() === ",") {
        this.advance();
        this.skipWhitespace();
      } else if (this.current() === "]") {
        this.advance();
        return arr;
      } else {
        throw new ParseError("Expected ',' or ']'", this.position);
      }
    }
  }

  private parseObject(): { [key: string]: JSONValue } {
    this.advance(); // consume {
    this.skipWhitespace();

    const obj: { [key: string]: JSONValue } = {};

    // 空オブジェクト
    if (this.current() === "}") {
      this.advance();
      return obj;
    }

    while (true) {
      this.skipWhitespace();

      // キー
      if (this.current() !== '"') {
        throw new ParseError("Expected string key", this.position);
      }
      const key = this.parseString();

      this.skipWhitespace();
      if (this.current() !== ":") {
        throw new ParseError("Expected ':'", this.position);
      }
      this.advance();

      // 値
      obj[key] = this.parseValue();

      this.skipWhitespace();
      if (this.current() === ",") {
        this.advance();
      } else if (this.current() === "}") {
        this.advance();
        return obj;
      } else {
        throw new ParseError("Expected ',' or '}'", this.position);
      }
    }
  }
}

// --- パブリック API ---
function parse(input: string): JSONValue {
  return new JSONParser(input).parse();
}

// --- デモ ---
function demo(): void {
  const examples = [
    "null",
    "true",
    "false",
    "42",
    "3.14",
    "-123.456e+10",
    '"hello"',
    '"hello\\nworld"',
    '"unicode: \\u0041\\u0042\\u0043"',
    "[1, 2, 3]",
    '["a", "b", "c"]',
    "[]",
    '{"name": "TypeScript", "version": 5}',
    "{}",
    '{"nested": {"array": [1, true, null]}}',
    '  { "key" : "value" }  ',
  ];

  for (const json of examples) {
    console.log(`Input:  ${json}`);
    try {
      const result = parse(json);
      console.log(`Parsed: ${JSON.stringify(result)}`);
    } catch (error) {
      console.log(`Error:  ${(error as Error).message}`);
    }
    console.log();
  }
}

// --- テスト ---
function runTests(): void {
  console.log("--- Tests ---");

  // null
  console.assert(parse("null") === null, "null failed");

  // boolean
  console.assert(parse("true") === true, "true failed");
  console.assert(parse("false") === false, "false failed");

  // number
  console.assert(parse("42") === 42, "integer failed");
  console.assert(parse("-42") === -42, "negative integer failed");
  console.assert(Math.abs(parse("3.14") as number - 3.14) < 0.001, "float failed");
  console.assert(parse("1e10") === 1e10, "exponent failed");

  // string
  console.assert(parse('"hello"') === "hello", "string failed");
  console.assert(parse('"hello\\nworld"') === "hello\nworld", "escape failed");
  console.assert(parse('"\\u0041"') === "A", "unicode failed");

  // array
  let arr = parse("[]") as JSONValue[];
  console.assert(Array.isArray(arr) && arr.length === 0, "empty array failed");

  arr = parse("[1, 2, 3]") as JSONValue[];
  console.assert(JSON.stringify(arr) === "[1,2,3]", "array failed");

  // object
  let obj = parse("{}") as { [key: string]: JSONValue };
  console.assert(typeof obj === "object" && Object.keys(obj).length === 0, "empty object failed");

  obj = parse('{"name": "TypeScript"}') as { [key: string]: JSONValue };
  console.assert(obj.name === "TypeScript", "object failed");

  // nested
  const nested = parse('{"arr": [1, {"nested": true}]}') as { [key: string]: JSONValue };
  console.assert(Array.isArray(nested.arr), "nested array failed");

  // whitespace
  const withSpace = parse('  { "key" : "value" }  ') as { [key: string]: JSONValue };
  console.assert(withSpace.key === "value", "whitespace failed");

  // errors
  let errorThrown = false;
  try {
    parse("");
  } catch {
    errorThrown = true;
  }
  console.assert(errorThrown, "empty input should fail");

  errorThrown = false;
  try {
    parse("{");
  } catch {
    errorThrown = true;
  }
  console.assert(errorThrown, "unclosed object should fail");

  errorThrown = false;
  try {
    parse("[1, 2,]");
  } catch {
    errorThrown = true;
  }
  console.assert(errorThrown, "trailing comma should fail");

  console.log("All tests passed!");
}

// --- 実行 ---
demo();
runTests();
