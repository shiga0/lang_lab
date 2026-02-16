#!/usr/bin/env python3
"""JSON Parser - Python 実装 (再帰下降パーサー)"""

from typing import Any, Optional
from dataclasses import dataclass

print("=== JSON Parser Demo ===\n")


# --- ParseError ---
class ParseError(Exception):
    def __init__(self, message: str, position: int):
        super().__init__(f"Parse error at position {position}: {message}")
        self.position = position


# --- Tokenizer ---
class Tokenizer:
    def __init__(self, input_str: str):
        self._input = input_str
        self._position = 0

    @property
    def position(self) -> int:
        return self._position

    @property
    def current_char(self) -> Optional[str]:
        if self._position >= len(self._input):
            return None
        return self._input[self._position]

    def peek(self, offset: int = 0) -> Optional[str]:
        pos = self._position + offset
        if pos >= len(self._input):
            return None
        return self._input[pos]

    def advance(self) -> Optional[str]:
        char = self.current_char
        self._position += 1
        return char

    def skip_whitespace(self) -> None:
        while self.current_char and self.current_char.isspace():
            self.advance()

    def eof(self) -> bool:
        return self._position >= len(self._input)

    def expect(self, char: str) -> None:
        if self.current_char != char:
            raise ParseError(
                f"Expected '{char}' but got '{self.current_char}'", self._position
            )
        self.advance()


# --- Parser ---
class JSONParser:
    def __init__(self, input_str: str):
        self._tokenizer = Tokenizer(input_str)

    def parse(self) -> Any:
        self._tokenizer.skip_whitespace()
        value = self._parse_value()
        self._tokenizer.skip_whitespace()

        if not self._tokenizer.eof():
            raise ParseError(
                "Unexpected characters after JSON value", self._tokenizer.position
            )

        return value

    def _parse_value(self) -> Any:
        self._tokenizer.skip_whitespace()
        char = self._tokenizer.current_char

        if char is None:
            raise ParseError("Unexpected end of input", self._tokenizer.position)
        elif char == "n":
            return self._parse_null()
        elif char == "t" or char == "f":
            return self._parse_bool()
        elif char == '"':
            return self._parse_string()
        elif char == "[":
            return self._parse_array()
        elif char == "{":
            return self._parse_object()
        elif char == "-" or char.isdigit():
            return self._parse_number()
        else:
            raise ParseError(f"Unexpected character: '{char}'", self._tokenizer.position)

    def _parse_null(self) -> None:
        self._expect_keyword("null")
        return None

    def _parse_bool(self) -> bool:
        if self._tokenizer.current_char == "t":
            self._expect_keyword("true")
            return True
        else:
            self._expect_keyword("false")
            return False

    def _expect_keyword(self, keyword: str) -> None:
        for expected in keyword:
            actual = self._tokenizer.advance()
            if actual != expected:
                raise ParseError(
                    f"Expected '{expected}' but got '{actual}'",
                    self._tokenizer.position,
                )

    def _parse_string(self) -> str:
        self._tokenizer.advance()  # consume opening "
        result = []

        while True:
            char = self._tokenizer.advance()

            if char is None:
                raise ParseError("Unterminated string", self._tokenizer.position)
            elif char == '"':
                break
            elif char == "\\":
                result.append(self._parse_escape_sequence())
            else:
                result.append(char)

        return "".join(result)

    def _parse_escape_sequence(self) -> str:
        char = self._tokenizer.advance()

        escape_map = {
            "n": "\n",
            "t": "\t",
            "r": "\r",
            '"': '"',
            "\\": "\\",
            "/": "/",
        }

        if char in escape_map:
            return escape_map[char]
        elif char == "u":
            # Unicode escape
            hex_chars = "".join(self._tokenizer.advance() or "" for _ in range(4))
            return chr(int(hex_chars, 16))
        else:
            raise ParseError(
                f"Invalid escape sequence: \\{char}", self._tokenizer.position
            )

    def _parse_number(self) -> float | int:
        num_str = []

        # 負号
        if self._tokenizer.current_char == "-":
            num_str.append(self._tokenizer.advance())

        # 整数部
        if self._tokenizer.current_char == "0":
            num_str.append(self._tokenizer.advance())
        elif self._tokenizer.current_char and self._tokenizer.current_char.isdigit():
            while self._tokenizer.current_char and self._tokenizer.current_char.isdigit():
                num_str.append(self._tokenizer.advance())
        else:
            raise ParseError("Expected digit", self._tokenizer.position)

        is_float = False

        # 小数部
        if self._tokenizer.current_char == ".":
            is_float = True
            num_str.append(self._tokenizer.advance())
            if not (
                self._tokenizer.current_char
                and self._tokenizer.current_char.isdigit()
            ):
                raise ParseError(
                    "Expected digit after decimal point", self._tokenizer.position
                )
            while self._tokenizer.current_char and self._tokenizer.current_char.isdigit():
                num_str.append(self._tokenizer.advance())

        # 指数部
        if self._tokenizer.current_char and self._tokenizer.current_char in "eE":
            is_float = True
            num_str.append(self._tokenizer.advance())
            if self._tokenizer.current_char and self._tokenizer.current_char in "+-":
                num_str.append(self._tokenizer.advance())
            if not (
                self._tokenizer.current_char
                and self._tokenizer.current_char.isdigit()
            ):
                raise ParseError(
                    "Expected digit in exponent", self._tokenizer.position
                )
            while self._tokenizer.current_char and self._tokenizer.current_char.isdigit():
                num_str.append(self._tokenizer.advance())

        num_value = "".join(num_str)
        return float(num_value) if is_float else int(num_value)

    def _parse_array(self) -> list:
        self._tokenizer.advance()  # consume [
        self._tokenizer.skip_whitespace()

        arr = []

        # 空配列
        if self._tokenizer.current_char == "]":
            self._tokenizer.advance()
            return arr

        while True:
            arr.append(self._parse_value())
            self._tokenizer.skip_whitespace()

            char = self._tokenizer.current_char
            if char == ",":
                self._tokenizer.advance()
                self._tokenizer.skip_whitespace()
            elif char == "]":
                self._tokenizer.advance()
                break
            else:
                raise ParseError("Expected ',' or ']'", self._tokenizer.position)

        return arr

    def _parse_object(self) -> dict:
        self._tokenizer.advance()  # consume {
        self._tokenizer.skip_whitespace()

        obj = {}

        # 空オブジェクト
        if self._tokenizer.current_char == "}":
            self._tokenizer.advance()
            return obj

        while True:
            self._tokenizer.skip_whitespace()

            # キー
            if self._tokenizer.current_char != '"':
                raise ParseError("Expected string key", self._tokenizer.position)
            key = self._parse_string()

            self._tokenizer.skip_whitespace()
            self._tokenizer.expect(":")

            # 値
            value = self._parse_value()
            obj[key] = value

            self._tokenizer.skip_whitespace()

            char = self._tokenizer.current_char
            if char == ",":
                self._tokenizer.advance()
            elif char == "}":
                self._tokenizer.advance()
                break
            else:
                raise ParseError("Expected ',' or '}'", self._tokenizer.position)

        return obj


# --- 便利関数 ---
def parse_json(input_str: str) -> Any:
    return JSONParser(input_str).parse()


# --- デモ ---
examples = [
    "null",
    "true",
    "42",
    "3.14",
    '"hello"',
    "[1, 2, 3]",
    '{"name": "Python", "version": 3.12}',
    '{"nested": {"array": [1, true, null]}}',
]

for json_str in examples:
    print(f"Input:  {json_str}")
    try:
        result = parse_json(json_str)
        print(f"Parsed: {result!r}")
    except ParseError as e:
        print(f"Error:  {e}")
    print()


# --- テスト ---
print("--- Tests ---")


def run_tests() -> None:
    # null
    assert parse_json("null") is None, "null failed"

    # bool
    assert parse_json("true") is True, "true failed"
    assert parse_json("false") is False, "false failed"

    # number
    assert parse_json("42") == 42, "integer failed"
    assert parse_json("-17") == -17, "negative failed"
    assert parse_json("3.14") == 3.14, "float failed"
    assert parse_json("1e10") == 1e10, "exponent failed"

    # string
    assert parse_json('"hello"') == "hello", "string failed"
    assert parse_json('"hello\\nworld"') == "hello\nworld", "escape failed"

    # array
    assert parse_json("[]") == [], "empty array failed"
    assert parse_json("[1, 2, 3]") == [1, 2, 3], "array failed"
    assert parse_json("[true, null]") == [True, None], "mixed array failed"

    # object
    assert parse_json("{}") == {}, "empty object failed"
    assert parse_json('{"name": "Python"}') == {"name": "Python"}, "object failed"

    # nested
    nested = parse_json('{"arr": [1, {"nested": true}]}')
    assert nested["arr"][1]["nested"] is True, "nested failed"

    # whitespace
    assert parse_json('  { "key" : "value" }  ') == {"key": "value"}, "whitespace failed"

    # errors
    try:
        parse_json("")
        assert False, "empty input should fail"
    except ParseError:
        pass

    try:
        parse_json("{")
        assert False, "unclosed object should fail"
    except ParseError:
        pass

    print("All tests passed!")


run_tests()
