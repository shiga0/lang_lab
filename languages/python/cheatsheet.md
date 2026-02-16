# Python チートシート

## 基本型

```python
# 数値
x: int = 42
y: float = 3.14
z: complex = 1 + 2j

# 文字列
s: str = "hello"
s2: str = 'hello'
s3: str = """複数行
文字列"""
f_string: str = f"x = {x}"

# ブール
b: bool = True
b2: bool = False

# None
n: None = None

# 型チェック
type(42)        # <class 'int'>
isinstance(42, int)  # True
```

## コレクション

```python
# リスト (可変)
lst: list[int] = [1, 2, 3]
lst.append(4)
lst.extend([5, 6])
lst.pop()
lst[0]          # 1
lst[-1]         # 最後の要素
lst[1:3]        # [2, 3] (スライス)
lst[::2]        # [1, 3, 5] (ステップ)

# タプル (不変)
tup: tuple[int, str, bool] = (1, "hello", True)
x, y, z = tup   # アンパック

# セット
s: set[int] = {1, 2, 3}
s.add(4)
s.remove(1)
s | {4, 5}      # 和集合
s & {2, 3}      # 積集合
s - {2}         # 差集合

# 辞書
d: dict[str, int] = {"a": 1, "b": 2}
d["c"] = 3
d.get("x", 0)   # キーがなければデフォルト値
d.keys()
d.values()
d.items()
```

## 内包表記

```python
# リスト内包表記
squares = [x**2 for x in range(10)]
evens = [x for x in range(10) if x % 2 == 0]
matrix = [[i*j for j in range(3)] for i in range(3)]

# 辞書内包表記
word_lens = {w: len(w) for w in ["hello", "world"]}

# セット内包表記
unique_lens = {len(w) for w in ["hello", "world", "hi"]}

# ジェネレータ式 (遅延評価)
lazy = (x**2 for x in range(1000000))
```

## 関数

```python
# 基本
def add(a: int, b: int) -> int:
    return a + b

# デフォルト引数
def greet(name: str = "World") -> str:
    return f"Hello, {name}!"

# 可変長引数
def sum_all(*args: int) -> int:
    return sum(args)

# キーワード引数
def config(**kwargs: str) -> dict:
    return kwargs

# アンパック
def foo(a, b, c):
    return a + b + c

foo(*[1, 2, 3])         # リストをアンパック
foo(**{"a": 1, "b": 2, "c": 3})  # 辞書をアンパック

# ラムダ
double = lambda x: x * 2
add = lambda a, b: a + b

# 高階関数
list(map(lambda x: x * 2, [1, 2, 3]))
list(filter(lambda x: x > 0, [-1, 0, 1, 2]))
from functools import reduce
reduce(lambda a, b: a + b, [1, 2, 3, 4])
```

## 制御構文

```python
# if
if x > 0:
    print("positive")
elif x < 0:
    print("negative")
else:
    print("zero")

# 三項演算子
result = "yes" if condition else "no"

# match (Python 3.10+)
match command:
    case "quit":
        exit()
    case "help":
        show_help()
    case ["go", direction]:
        move(direction)
    case _:
        print("Unknown")

# for
for i in range(5):
    print(i)

for i, value in enumerate(["a", "b", "c"]):
    print(i, value)

for key, value in d.items():
    print(key, value)

# while
while condition:
    do_something()

# break / continue
for i in range(10):
    if i == 5:
        break
    if i % 2 == 0:
        continue
    print(i)

# else (ループが正常終了した場合)
for i in range(5):
    if i == 10:
        break
else:
    print("ループ完了")
```

## クラス

```python
from dataclasses import dataclass
from typing import ClassVar

class Person:
    # クラス変数
    species: ClassVar[str] = "Human"

    def __init__(self, name: str, age: int) -> None:
        self.name = name
        self.age = age

    def greet(self) -> str:
        return f"Hello, I'm {self.name}"

    @property
    def info(self) -> str:
        return f"{self.name} ({self.age})"

    @classmethod
    def from_dict(cls, data: dict) -> "Person":
        return cls(data["name"], data["age"])

    @staticmethod
    def is_adult(age: int) -> bool:
        return age >= 18

    def __str__(self) -> str:
        return f"Person({self.name})"

    def __repr__(self) -> str:
        return f"Person(name={self.name!r}, age={self.age})"

# 継承
class Employee(Person):
    def __init__(self, name: str, age: int, company: str) -> None:
        super().__init__(name, age)
        self.company = company

# データクラス (Python 3.7+)
@dataclass
class Point:
    x: float
    y: float

    def distance(self, other: "Point") -> float:
        return ((self.x - other.x)**2 + (self.y - other.y)**2) ** 0.5

# frozen (不変)
@dataclass(frozen=True)
class ImmutablePoint:
    x: float
    y: float
```

## 例外処理

```python
# try-except
try:
    result = 10 / 0
except ZeroDivisionError as e:
    print(f"Error: {e}")
except (TypeError, ValueError):
    print("Type or Value error")
except Exception as e:
    print(f"Unexpected: {e}")
else:
    print("No exception")
finally:
    print("Always runs")

# 例外を発生
def divide(a: int, b: int) -> float:
    if b == 0:
        raise ValueError("Division by zero")
    return a / b

# カスタム例外
class ValidationError(Exception):
    def __init__(self, message: str, field: str):
        super().__init__(message)
        self.field = field

# コンテキストマネージャ
with open("file.txt") as f:
    content = f.read()

# 複数のコンテキスト
with open("a.txt") as a, open("b.txt") as b:
    pass
```

## デコレータ

```python
from functools import wraps
from typing import Callable, TypeVar

T = TypeVar("T")

# 基本的なデコレータ
def log(func: Callable[..., T]) -> Callable[..., T]:
    @wraps(func)
    def wrapper(*args, **kwargs) -> T:
        print(f"Calling {func.__name__}")
        result = func(*args, **kwargs)
        print(f"Returned {result}")
        return result
    return wrapper

@log
def add(a: int, b: int) -> int:
    return a + b

# 引数付きデコレータ
def repeat(times: int):
    def decorator(func: Callable[..., T]) -> Callable[..., T]:
        @wraps(func)
        def wrapper(*args, **kwargs) -> T:
            for _ in range(times):
                result = func(*args, **kwargs)
            return result
        return wrapper
    return decorator

@repeat(3)
def say_hello():
    print("Hello")

# クラスデコレータ
def singleton(cls):
    instances = {}
    def get_instance(*args, **kwargs):
        if cls not in instances:
            instances[cls] = cls(*args, **kwargs)
        return instances[cls]
    return get_instance
```

## ジェネレータとイテレータ

```python
# ジェネレータ関数
def countdown(n: int):
    while n > 0:
        yield n
        n -= 1

for x in countdown(5):
    print(x)

# ジェネレータ式
squares = (x**2 for x in range(10))

# send と throw
def accumulator():
    total = 0
    while True:
        value = yield total
        if value is not None:
            total += value

acc = accumulator()
next(acc)       # 0
acc.send(10)    # 10
acc.send(20)    # 30

# カスタムイテレータ
class Counter:
    def __init__(self, limit: int):
        self.limit = limit
        self.count = 0

    def __iter__(self):
        return self

    def __next__(self):
        if self.count >= self.limit:
            raise StopIteration
        self.count += 1
        return self.count
```

## 非同期処理

```python
import asyncio

# async 関数
async def fetch_data(url: str) -> str:
    await asyncio.sleep(1)  # 非同期待機
    return f"Data from {url}"

# 実行
async def main():
    result = await fetch_data("https://example.com")
    print(result)

asyncio.run(main())

# 並行実行
async def main():
    tasks = [
        fetch_data("url1"),
        fetch_data("url2"),
        fetch_data("url3"),
    ]
    results = await asyncio.gather(*tasks)
    return results

# async for
async def async_range(n: int):
    for i in range(n):
        await asyncio.sleep(0.1)
        yield i

async def main():
    async for i in async_range(5):
        print(i)

# async with
async with aiohttp.ClientSession() as session:
    async with session.get(url) as response:
        data = await response.text()
```

## モジュールとパッケージ

```python
# インポート
import os
from pathlib import Path
from typing import List, Dict, Optional
from collections import defaultdict, Counter

# 相対インポート
from . import sibling_module
from .. import parent_module
from .subpackage import something

# __all__ でエクスポート制御
__all__ = ["public_func", "PublicClass"]

# if __name__ == "__main__"
if __name__ == "__main__":
    main()
```

## 型ヒント

```python
from typing import (
    List, Dict, Set, Tuple, Optional, Union,
    Callable, TypeVar, Generic, Any,
    Literal, TypedDict, Protocol
)

# 基本
def greet(name: str) -> str:
    return f"Hello, {name}"

# Optional (None 許容)
def find(id: int) -> Optional[str]:
    return None

# Union
def process(value: Union[int, str]) -> str:
    return str(value)

# Python 3.10+ の新構文
def process(value: int | str) -> str:
    return str(value)

# ジェネリクス
T = TypeVar("T")

def first(items: List[T]) -> Optional[T]:
    return items[0] if items else None

# Generic クラス
class Stack(Generic[T]):
    def __init__(self) -> None:
        self._items: List[T] = []

    def push(self, item: T) -> None:
        self._items.append(item)

    def pop(self) -> T:
        return self._items.pop()

# Protocol (構造的部分型)
class Drawable(Protocol):
    def draw(self) -> None: ...

# TypedDict
class UserDict(TypedDict):
    name: str
    age: int
    email: str

# Literal
Mode = Literal["r", "w", "a"]

def open_file(path: str, mode: Mode) -> None:
    pass
```

## よく使う標準ライブラリ

```python
# pathlib (パス操作)
from pathlib import Path
p = Path("dir/file.txt")
p.exists()
p.read_text()
p.write_text("content")
p.parent
p.stem
p.suffix
list(p.glob("*.py"))

# collections
from collections import defaultdict, Counter, deque
dd = defaultdict(list)
dd["key"].append(1)
c = Counter(["a", "b", "a"])
c.most_common(2)
d = deque([1, 2, 3])
d.appendleft(0)

# itertools
from itertools import chain, groupby, combinations, permutations
list(chain([1, 2], [3, 4]))
list(combinations([1, 2, 3], 2))

# functools
from functools import partial, lru_cache, reduce
add5 = partial(add, 5)
@lru_cache(maxsize=128)
def fib(n):
    if n < 2: return n
    return fib(n-1) + fib(n-2)

# contextlib
from contextlib import contextmanager
@contextmanager
def timer():
    start = time.time()
    yield
    print(f"Elapsed: {time.time() - start}")

# dataclasses
from dataclasses import dataclass, field
@dataclass
class Config:
    name: str
    values: list = field(default_factory=list)

# json
import json
json.dumps({"key": "value"})
json.loads('{"key": "value"}')

# re (正規表現)
import re
re.match(r"\d+", "123abc")
re.findall(r"\d+", "a1b2c3")
re.sub(r"\d+", "X", "a1b2")
```

## コマンドライン引数

```python
# sys.argv
import sys
print(sys.argv)  # ['script.py', 'arg1', 'arg2']

# argparse
import argparse
parser = argparse.ArgumentParser(description="My CLI")
parser.add_argument("name", help="Your name")
parser.add_argument("-v", "--verbose", action="store_true")
parser.add_argument("-n", "--number", type=int, default=1)
args = parser.parse_args()
print(args.name, args.verbose, args.number)
```
