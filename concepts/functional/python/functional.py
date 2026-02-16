#!/usr/bin/env python3
"""関数型プログラミング - Python 実装

Python は関数型パラダイムもサポート。
第一級関数、高階関数、イテレータ、ジェネレータ。
"""

from functools import reduce, partial, lru_cache, wraps
from itertools import chain, groupby, combinations, permutations, count, takewhile
from typing import Callable, TypeVar, Iterable, Iterator, List, Optional

print("=== Functional Programming Demo ===\n")

T = TypeVar("T")
U = TypeVar("U")


# --- 第一級関数 ---
def first_class_functions() -> None:
    print("--- 第一級関数 ---")

    # 関数を変数に代入
    def add(a: int, b: int) -> int:
        return a + b

    operation = add
    print(f"operation(2, 3) = {operation(2, 3)}")

    # 関数を引数として渡す
    def apply(x: int, f: Callable[[int], int]) -> int:
        return f(x)

    double = lambda x: x * 2
    print(f"apply(5, double) = {apply(5, double)}")

    # 関数を返す
    def make_adder(n: int) -> Callable[[int], int]:
        return lambda x: x + n

    add_five = make_adder(5)
    print(f"make_adder(5)(10) = {add_five(10)}")

    print()


# --- クロージャ ---
def closures() -> None:
    print("--- クロージャ ---")

    # カウンター
    def make_counter() -> Callable[[], int]:
        count = 0

        def counter() -> int:
            nonlocal count
            count += 1
            return count

        return counter

    counter = make_counter()
    print(f"counter() = {counter()}")
    print(f"counter() = {counter()}")
    print(f"counter() = {counter()}")

    # 独立したカウンター
    counter2 = make_counter()
    print(f"counter2() = {counter2()}")

    # フィボナッチジェネレータ
    def make_fibonacci() -> Callable[[], int]:
        a, b = 0, 1

        def fib() -> int:
            nonlocal a, b
            result = a
            a, b = b, a + b
            return result

        return fib

    fib = make_fibonacci()
    fibs = [fib() for _ in range(10)]
    print(f"Fibonacci: {fibs}")

    print()


# --- 高階関数 ---
def higher_order_functions() -> None:
    print("--- 高階関数 ---")

    numbers = [1, 2, 3, 4, 5]

    # map
    doubled = list(map(lambda x: x * 2, numbers))
    print(f"map (*2): {doubled}")

    # filter
    evens = list(filter(lambda x: x % 2 == 0, numbers))
    print(f"filter (even): {evens}")

    # reduce
    total = reduce(lambda a, b: a + b, numbers, 0)
    print(f"reduce (sum): {total}")

    # 内包表記 (推奨)
    print("\n内包表記:")
    doubled2 = [x * 2 for x in numbers]
    print(f"  [x * 2 for x in numbers]: {doubled2}")

    evens2 = [x for x in numbers if x % 2 == 0]
    print(f"  [x for x if even]: {evens2}")

    # zip
    names = ["Alice", "Bob", "Charlie"]
    ages = [30, 25, 35]
    pairs = list(zip(names, ages))
    print(f"\nzip: {pairs}")

    # enumerate
    for i, name in enumerate(names):
        print(f"  {i}: {name}")

    print()


# --- ラムダ式 ---
def lambda_expressions() -> None:
    print("--- ラムダ式 ---")

    # 基本
    add = lambda a, b: a + b
    print(f"lambda add: {add(2, 3)}")

    # ソートのキー
    data = [("Alice", 30), ("Bob", 25), ("Charlie", 35)]
    sorted_by_age = sorted(data, key=lambda x: x[1])
    print(f"sorted by age: {sorted_by_age}")

    # 複数の条件
    sorted_complex = sorted(data, key=lambda x: (-x[1], x[0]))
    print(f"sorted by -age, name: {sorted_complex}")

    print()


# --- partial (部分適用) ---
def partial_application() -> None:
    print("--- partial (部分適用) ---")

    def power(base: int, exp: int) -> int:
        return base ** exp

    square = partial(power, exp=2)
    cube = partial(power, exp=3)

    print(f"square(5) = {square(5)}")
    print(f"cube(3) = {cube(3)}")

    # 関数合成
    def compose(*functions):
        def inner(arg):
            result = arg
            for f in reversed(functions):
                result = f(result)
            return result
        return inner

    add_one = lambda x: x + 1
    double = lambda x: x * 2
    composed = compose(double, add_one)
    print(f"\ncompose(double, add_one)(5) = {composed(5)}")

    print()


# --- イテレータとジェネレータ ---
def iterators_and_generators() -> None:
    print("--- イテレータとジェネレータ ---")

    # ジェネレータ関数
    def countdown(n: int) -> Iterator[int]:
        while n > 0:
            yield n
            n -= 1

    print(f"countdown: {list(countdown(5))}")

    # ジェネレータ式
    squares = (x**2 for x in range(10))
    print(f"generator expression: {list(squares)}")

    # 無限ジェネレータ
    def naturals() -> Iterator[int]:
        n = 1
        while True:
            yield n
            n += 1

    from itertools import islice
    first_10 = list(islice(naturals(), 10))
    print(f"first 10 naturals: {first_10}")

    # send と yield
    def accumulator() -> Iterator[int]:
        total = 0
        while True:
            value = yield total
            if value is not None:
                total += value

    acc = accumulator()
    next(acc)  # 初期化
    print(f"\naccumulator:")
    print(f"  send(10): {acc.send(10)}")
    print(f"  send(20): {acc.send(20)}")
    print(f"  send(5): {acc.send(5)}")

    print()


# --- itertools ---
def itertools_demo() -> None:
    print("--- itertools ---")

    # chain
    chained = list(chain([1, 2], [3, 4], [5]))
    print(f"chain: {chained}")

    # groupby
    data = [("A", 1), ("A", 2), ("B", 3), ("B", 4)]
    grouped = {k: list(v) for k, v in groupby(data, key=lambda x: x[0])}
    print(f"groupby: {grouped}")

    # combinations
    combs = list(combinations([1, 2, 3], 2))
    print(f"combinations: {combs}")

    # permutations
    perms = list(permutations([1, 2, 3], 2))
    print(f"permutations: {perms}")

    # takewhile
    taken = list(takewhile(lambda x: x < 5, [1, 2, 3, 4, 5, 6, 7]))
    print(f"takewhile (<5): {taken}")

    # count
    from itertools import islice
    counted = list(islice(count(10, 2), 5))  # 10 から 2 ずつ 5 個
    print(f"count(10, 2): {counted}")

    print()


# --- メモ化 ---
def memoization() -> None:
    print("--- メモ化 ---")

    call_count = 0

    @lru_cache(maxsize=128)
    def fib(n: int) -> int:
        nonlocal call_count
        call_count += 1
        if n < 2:
            return n
        return fib(n - 1) + fib(n - 2)

    result = fib(30)
    print(f"fib(30) = {result}")
    print(f"call count: {call_count}")
    print(f"cache info: {fib.cache_info()}")

    # キャッシュクリア
    fib.cache_clear()

    print()


# --- デコレータ ---
def decorator_patterns() -> None:
    print("--- デコレータ ---")

    # 基本的なデコレータ
    def log(func: Callable[..., T]) -> Callable[..., T]:
        @wraps(func)
        def wrapper(*args, **kwargs) -> T:
            print(f"  Calling {func.__name__}")
            result = func(*args, **kwargs)
            print(f"  Returned {result}")
            return result
        return wrapper

    @log
    def add(a: int, b: int) -> int:
        return a + b

    add(2, 3)

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
    def say_hello() -> str:
        print("  Hello!")
        return "done"

    print("\n@repeat(3):")
    say_hello()

    print()


# --- 不変性 ---
def immutability() -> None:
    print("--- 不変性 ---")

    # タプル (不変)
    t = (1, 2, 3)
    # t[0] = 10  # TypeError

    # frozenset
    fs = frozenset([1, 2, 3])
    # fs.add(4)  # AttributeError

    # 不変的な更新
    original = [1, 2, 3]
    updated = [*original, 4]
    print(f"original: {original}")
    print(f"updated: {updated}")

    # 辞書の不変的な更新
    d1 = {"a": 1, "b": 2}
    d2 = {**d1, "c": 3}
    print(f"d1: {d1}")
    print(f"d2: {d2}")

    # NamedTuple (不変)
    from typing import NamedTuple

    class Point(NamedTuple):
        x: float
        y: float

    p = Point(10, 20)
    # p.x = 100  # AttributeError
    print(f"\nNamedTuple: {p}")

    print()


# --- パイプライン ---
def pipeline_demo() -> None:
    print("--- パイプライン ---")

    def pipe(value: T, *functions: Callable) -> T:
        result = value
        for f in functions:
            result = f(result)
        return result

    result = pipe(
        [1, 2, 3, 4, 5],
        lambda x: [n * 2 for n in x],
        lambda x: [n for n in x if n > 4],
        sum
    )
    print(f"pipe result: {result}")

    # メソッドチェーン風
    class Pipeline:
        def __init__(self, data):
            self.data = data

        def map(self, f):
            return Pipeline([f(x) for x in self.data])

        def filter(self, f):
            return Pipeline([x for x in self.data if f(x)])

        def reduce(self, f, initial):
            return reduce(f, self.data, initial)

        def collect(self):
            return self.data

    result2 = (
        Pipeline([1, 2, 3, 4, 5])
        .map(lambda x: x * 2)
        .filter(lambda x: x > 4)
        .reduce(lambda a, b: a + b, 0)
    )
    print(f"Pipeline result: {result2}")

    print()


# --- 実行 ---
if __name__ == "__main__":
    first_class_functions()
    closures()
    higher_order_functions()
    lambda_expressions()
    partial_application()
    iterators_and_generators()
    itertools_demo()
    memoization()
    decorator_patterns()
    immutability()
    pipeline_demo()
