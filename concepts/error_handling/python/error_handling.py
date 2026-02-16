#!/usr/bin/env python3
"""エラー処理 - Python 実装

Python は例外ベースのエラー処理。
Result パターンも実装可能。
"""

from typing import TypeVar, Generic, Union, Optional, Callable
from dataclasses import dataclass

print("=== Error Handling Demo ===\n")


# --- 例外の基本 ---
def exception_basics() -> None:
    print("--- 例外の基本 ---")

    # try-except
    def divide(a: int, b: int) -> float:
        if b == 0:
            raise ValueError("Division by zero")
        return a / b

    try:
        print(f"10 / 2 = {divide(10, 2)}")
        print(f"10 / 0 = {divide(10, 0)}")
    except ValueError as e:
        print(f"Error: {e}")

    # 複数の例外をキャッチ
    try:
        result = int("not a number")
    except (ValueError, TypeError) as e:
        print(f"Caught ValueError or TypeError: {e}")

    # else と finally
    print("\nelse と finally:")
    try:
        result = divide(10, 2)
    except ValueError:
        print("  except: error occurred")
    else:
        print(f"  else: success, result = {result}")
    finally:
        print("  finally: always runs")

    print()


# --- カスタム例外 ---
def custom_exceptions() -> None:
    print("--- カスタム例外 ---")

    class ValidationError(Exception):
        def __init__(self, message: str, field: str):
            super().__init__(message)
            self.field = field

    class NetworkError(Exception):
        def __init__(self, message: str, status_code: int):
            super().__init__(message)
            self.status_code = status_code

    def validate_user(data: dict) -> None:
        if "name" not in data:
            raise ValidationError("Name is required", "name")
        if "email" not in data or "@" not in data.get("email", ""):
            raise ValidationError("Invalid email", "email")

    try:
        validate_user({"name": "Alice"})
    except ValidationError as e:
        print(f"ValidationError: {e} (field: {e.field})")

    # 例外の継承階層
    class AppError(Exception):
        """アプリケーションの基底例外"""
        pass

    class DatabaseError(AppError):
        pass

    class AuthError(AppError):
        pass

    def handle_error(e: AppError) -> None:
        if isinstance(e, DatabaseError):
            print(f"  Database error: {e}")
        elif isinstance(e, AuthError):
            print(f"  Auth error: {e}")
        else:
            print(f"  App error: {e}")

    handle_error(DatabaseError("Connection failed"))
    handle_error(AuthError("Invalid token"))

    print()


# --- 例外チェーン ---
def exception_chaining() -> None:
    print("--- 例外チェーン ---")

    def parse_config(path: str) -> dict:
        try:
            # ファイル読み込みのシミュレーション
            raise FileNotFoundError(f"Config file not found: {path}")
        except FileNotFoundError as e:
            # 新しい例外でラップ
            raise RuntimeError(f"Failed to load config: {path}") from e

    try:
        parse_config("config.yaml")
    except RuntimeError as e:
        print(f"Error: {e}")
        print(f"Caused by: {e.__cause__}")

    # 例外を抑制 (from None)
    def suppress_cause() -> None:
        try:
            raise ValueError("original")
        except ValueError:
            raise RuntimeError("new error") from None

    try:
        suppress_cause()
    except RuntimeError as e:
        print(f"\nSuppressed cause: {e}")
        print(f"__cause__: {e.__cause__}")

    print()


# --- コンテキストマネージャ ---
def context_managers() -> None:
    print("--- コンテキストマネージャ ---")

    # クラスベース
    class ManagedResource:
        def __init__(self, name: str):
            self.name = name

        def __enter__(self):
            print(f"  Acquiring {self.name}")
            return self

        def __exit__(self, exc_type, exc_val, exc_tb):
            print(f"  Releasing {self.name}")
            # True を返すと例外を抑制
            return False

    with ManagedResource("resource1"):
        print("  Using resource")

    # contextlib
    from contextlib import contextmanager

    @contextmanager
    def managed_resource(name: str):
        print(f"  Acquiring {name}")
        try:
            yield name
        finally:
            print(f"  Releasing {name}")

    print("\ncontextlib.contextmanager:")
    with managed_resource("resource2") as r:
        print(f"  Using {r}")

    # suppress
    from contextlib import suppress

    print("\ncontextlib.suppress:")
    with suppress(FileNotFoundError):
        open("nonexistent.txt")
    print("  No error raised")

    print()


# --- Result パターン ---
def result_pattern() -> None:
    print("--- Result パターン ---")

    T = TypeVar("T")
    E = TypeVar("E")

    @dataclass
    class Ok(Generic[T]):
        value: T

        def is_ok(self) -> bool:
            return True

        def is_err(self) -> bool:
            return False

    @dataclass
    class Err(Generic[E]):
        error: E

        def is_ok(self) -> bool:
            return False

        def is_err(self) -> bool:
            return True

    Result = Union[Ok[T], Err[E]]

    def safe_divide(a: int, b: int) -> Result[float, str]:
        if b == 0:
            return Err("Division by zero")
        return Ok(a / b)

    result1 = safe_divide(10, 2)
    if isinstance(result1, Ok):
        print(f"10 / 2 = {result1.value}")

    result2 = safe_divide(10, 0)
    if isinstance(result2, Err):
        print(f"Error: {result2.error}")

    # チェーン
    def parse_int(s: str) -> Result[int, str]:
        try:
            return Ok(int(s))
        except ValueError:
            return Err(f"Invalid integer: {s}")

    def compute(input_str: str) -> Result[float, str]:
        parsed = parse_int(input_str)
        if isinstance(parsed, Err):
            return parsed

        divided = safe_divide(100, parsed.value)
        if isinstance(divided, Err):
            return divided

        return Ok(divided.value * 2)

    print(f"\ncompute('5'): {compute('5')}")
    print(f"compute('0'): {compute('0')}")
    print(f"compute('abc'): {compute('abc')}")

    print()


# --- Optional パターン ---
def optional_pattern() -> None:
    print("--- Optional パターン ---")

    def find_user(user_id: int) -> Optional[dict]:
        users = {
            1: {"name": "Alice", "age": 30},
            2: {"name": "Bob", "age": 25},
        }
        return users.get(user_id)

    # None チェック
    user = find_user(1)
    if user is not None:
        print(f"Found: {user['name']}")

    user = find_user(99)
    if user is None:
        print("User not found")

    # Walrus operator (Python 3.8+)
    if (user := find_user(2)) is not None:
        print(f"Walrus: {user['name']}")

    # デフォルト値
    name = find_user(99) or {"name": "Unknown"}
    print(f"Default: {name}")

    print()


# --- assertions ---
def assertions_demo() -> None:
    print("--- assertions ---")

    def process_positive(n: int) -> int:
        assert n > 0, f"n must be positive, got {n}"
        return n * 2

    try:
        result = process_positive(5)
        print(f"process_positive(5) = {result}")

        result = process_positive(-1)
    except AssertionError as e:
        print(f"AssertionError: {e}")

    # 注意: assert は -O フラグで無効化される
    print("\n注意: python -O で assert は無効化される")

    print()


# --- warnings ---
def warnings_demo() -> None:
    print("--- warnings ---")

    import warnings

    def deprecated_function() -> None:
        warnings.warn(
            "deprecated_function is deprecated, use new_function instead",
            DeprecationWarning,
            stacklevel=2
        )
        print("  Running deprecated function")

    # 警告を表示
    warnings.filterwarnings("always", category=DeprecationWarning)
    deprecated_function()

    # 警告を例外に変換
    print("\nwarnings as errors:")
    try:
        with warnings.catch_warnings():
            warnings.simplefilter("error")
            deprecated_function()
    except DeprecationWarning as e:
        print(f"  Caught warning as error: {e}")

    print()


# --- エラーハンドリングパターン ---
def error_handling_patterns() -> None:
    print("--- エラーハンドリングパターン ---")

    # 1. EAFP (Easier to Ask Forgiveness than Permission)
    print("EAFP style:")
    data = {"key": "value"}
    try:
        value = data["nonexistent"]
    except KeyError:
        value = "default"
    print(f"  value: {value}")

    # 2. LBYL (Look Before You Leap)
    print("\nLBYL style:")
    if "nonexistent" in data:
        value = data["nonexistent"]
    else:
        value = "default"
    print(f"  value: {value}")

    # Python では EAFP が推奨される
    print("\nPython では EAFP スタイルが推奨される")

    print()


# --- 実行 ---
if __name__ == "__main__":
    exception_basics()
    custom_exceptions()
    exception_chaining()
    context_managers()
    result_pattern()
    optional_pattern()
    assertions_demo()
    warnings_demo()
    error_handling_patterns()
