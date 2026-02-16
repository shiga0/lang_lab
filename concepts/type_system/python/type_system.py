#!/usr/bin/env python3
"""型システム - Python 実装

Python は動的型付け言語だが、型ヒントで静的解析が可能。
実行時には型ヒントは無視される。
"""

from typing import (
    List, Dict, Set, Tuple, Optional, Union,
    Callable, TypeVar, Generic, Any,
    Literal, Protocol, Final, ClassVar
)
from dataclasses import dataclass

print("=== Type System Demo ===\n")


# --- 基本型 ---
def basic_types() -> None:
    print("--- 基本型 ---")

    # 数値型
    x: int = 42
    y: float = 3.14
    z: complex = 1 + 2j
    print(f"int: {x}, float: {y}, complex: {z}")

    # 文字列
    s: str = "Hello, Python!"
    print(f"str: {s}")

    # ブール
    b: bool = True
    print(f"bool: {b}")

    # None
    n: None = None
    print(f"None: {n}")

    # 型の確認
    print(f"\ntype(42) = {type(42)}")
    print(f"isinstance(42, int) = {isinstance(42, int)}")

    print()


# --- コレクション型 ---
def collection_types() -> None:
    print("--- コレクション型 ---")

    # リスト
    numbers: List[int] = [1, 2, 3, 4, 5]
    print(f"List[int]: {numbers}")

    # タプル
    point: Tuple[int, int] = (10, 20)
    print(f"Tuple[int, int]: {point}")

    # 可変長タプル
    values: Tuple[int, ...] = (1, 2, 3, 4, 5)
    print(f"Tuple[int, ...]: {values}")

    # 辞書
    user: Dict[str, Any] = {"name": "Alice", "age": 30}
    print(f"Dict[str, Any]: {user}")

    # セット
    tags: Set[str] = {"python", "programming"}
    print(f"Set[str]: {tags}")

    print()


# --- Union と Optional ---
def union_and_optional() -> None:
    print("--- Union と Optional ---")

    # Union (複数の型を許容)
    def process(value: Union[int, str]) -> str:
        return str(value)

    print(f"Union[int, str]: {process(42)}, {process('hello')}")

    # Python 3.10+ の新構文
    def process_new(value: int | str) -> str:
        return str(value)

    # Optional (None を許容)
    def find_user(user_id: int) -> Optional[str]:
        users = {1: "Alice", 2: "Bob"}
        return users.get(user_id)

    print(f"Optional[str]: {find_user(1)}, {find_user(99)}")

    print()


# --- Literal 型 ---
def literal_types() -> None:
    print("--- Literal 型 ---")

    # 特定の値のみ許容
    Mode = Literal["read", "write", "append"]

    def open_file(path: str, mode: Mode) -> str:
        return f"Opening {path} in {mode} mode"

    print(f"Literal: {open_file('file.txt', 'read')}")

    # 数値リテラル
    Priority = Literal[1, 2, 3]

    def set_priority(p: Priority) -> str:
        return f"Priority set to {p}"

    print(f"Literal[1,2,3]: {set_priority(1)}")

    print()


# --- TypeVar とジェネリクス ---
def generics() -> None:
    print("--- TypeVar とジェネリクス ---")

    # TypeVar
    T = TypeVar("T")

    def first(items: List[T]) -> Optional[T]:
        return items[0] if items else None

    print(f"first([1, 2, 3]) = {first([1, 2, 3])}")
    print(f"first(['a', 'b']) = {first(['a', 'b'])}")

    # 制約付き TypeVar
    Number = TypeVar("Number", int, float)

    def add(a: Number, b: Number) -> Number:
        return a + b

    print(f"add(1, 2) = {add(1, 2)}")
    print(f"add(1.5, 2.5) = {add(1.5, 2.5)}")

    # Generic クラス
    class Stack(Generic[T]):
        def __init__(self) -> None:
            self._items: List[T] = []

        def push(self, item: T) -> None:
            self._items.append(item)

        def pop(self) -> T:
            return self._items.pop()

        def __repr__(self) -> str:
            return f"Stack({self._items})"

    stack: Stack[int] = Stack()
    stack.push(1)
    stack.push(2)
    print(f"Stack[int]: {stack}")
    print(f"pop() = {stack.pop()}")

    print()


# --- Protocol (構造的部分型) ---
def protocols() -> None:
    print("--- Protocol (構造的部分型) ---")

    # Protocol 定義
    class Drawable(Protocol):
        def draw(self) -> str: ...

    # 明示的な implements は不要
    class Circle:
        def __init__(self, radius: float) -> None:
            self.radius = radius

        def draw(self) -> str:
            return f"Circle(radius={self.radius})"

    class Rectangle:
        def __init__(self, width: float, height: float) -> None:
            self.width = width
            self.height = height

        def draw(self) -> str:
            return f"Rectangle({self.width}x{self.height})"

    def render(shape: Drawable) -> None:
        print(f"  Drawing: {shape.draw()}")

    render(Circle(5))
    render(Rectangle(10, 20))

    print()


# --- Callable ---
def callable_types() -> None:
    print("--- Callable ---")

    # 関数型
    BinaryOp = Callable[[int, int], int]

    def apply(a: int, b: int, op: BinaryOp) -> int:
        return op(a, b)

    add: BinaryOp = lambda a, b: a + b
    mul: BinaryOp = lambda a, b: a * b

    print(f"apply(2, 3, add) = {apply(2, 3, add)}")
    print(f"apply(2, 3, mul) = {apply(2, 3, mul)}")

    # 引数なし
    Factory = Callable[[], str]

    def greet_factory() -> Factory:
        return lambda: "Hello!"

    factory = greet_factory()
    print(f"Factory(): {factory()}")

    print()


# --- TypedDict ---
def typed_dict() -> None:
    print("--- TypedDict ---")

    from typing import TypedDict

    class UserDict(TypedDict):
        name: str
        age: int
        email: str

    user: UserDict = {
        "name": "Alice",
        "age": 30,
        "email": "alice@example.com"
    }
    print(f"TypedDict: {user}")

    # オプショナルフィールド
    class PartialUser(TypedDict, total=False):
        name: str
        age: int
        email: str

    partial: PartialUser = {"name": "Bob"}
    print(f"TypedDict (partial): {partial}")

    print()


# --- Final と ClassVar ---
def final_and_classvar() -> None:
    print("--- Final と ClassVar ---")

    class Config:
        # クラス変数
        VERSION: ClassVar[str] = "1.0.0"
        DEBUG: ClassVar[bool] = False

        def __init__(self, name: str) -> None:
            # Final (再代入不可)
            self.name: Final[str] = name

    config = Config("MyApp")
    print(f"ClassVar VERSION: {Config.VERSION}")
    print(f"Final name: {config.name}")

    # Final 変数
    MAX_SIZE: Final[int] = 100
    print(f"Final MAX_SIZE: {MAX_SIZE}")

    print()


# --- 型ガード ---
def type_guards() -> None:
    print("--- 型ガード ---")

    from typing import TypeGuard

    # isinstance による絞り込み
    def process(value: int | str) -> str:
        if isinstance(value, int):
            return f"int: {value * 2}"
        else:
            return f"str: {value.upper()}"

    print(f"isinstance guard: {process(42)}, {process('hello')}")

    # カスタム型ガード
    def is_string_list(val: List[Any]) -> TypeGuard[List[str]]:
        return all(isinstance(x, str) for x in val)

    items: List[Any] = ["a", "b", "c"]
    if is_string_list(items):
        # ここでは items は List[str]
        print(f"TypeGuard: {[s.upper() for s in items]}")

    print()


# --- データクラス ---
def dataclasses_demo() -> None:
    print("--- データクラス ---")

    from dataclasses import dataclass, field

    @dataclass
    class Point:
        x: float
        y: float

        def distance(self, other: "Point") -> float:
            return ((self.x - other.x)**2 + (self.y - other.y)**2) ** 0.5

    p1 = Point(0, 0)
    p2 = Point(3, 4)
    print(f"Point: {p1}")
    print(f"distance: {p1.distance(p2)}")

    # frozen (不変)
    @dataclass(frozen=True)
    class ImmutablePoint:
        x: float
        y: float

    ip = ImmutablePoint(10, 20)
    print(f"frozen: {ip}")

    # デフォルトファクトリ
    @dataclass
    class Container:
        name: str
        items: List[str] = field(default_factory=list)

    c = Container("box")
    c.items.append("item1")
    print(f"default_factory: {c}")

    print()


# --- 実行 ---
if __name__ == "__main__":
    basic_types()
    collection_types()
    union_and_optional()
    literal_types()
    generics()
    protocols()
    callable_types()
    typed_dict()
    final_and_classvar()
    type_guards()
    dataclasses_demo()
