#!/usr/bin/env python3
"""FizzBuzz - Python 実装"""

from typing import Iterator, List

print("=== FizzBuzz Demo ===\n")

# --- 基本実装 ---
print("--- Basic Implementation ---")


def fizzbuzz_basic(n: int) -> None:
    for i in range(1, n + 1):
        if i % 15 == 0:
            print("FizzBuzz")
        elif i % 3 == 0:
            print("Fizz")
        elif i % 5 == 0:
            print("Buzz")
        else:
            print(i)


fizzbuzz_basic(15)

print()

# --- match 文 (Python 3.10+) ---
print("--- Match Expression (Python 3.10+) ---")


def fizzbuzz_match(n: int) -> List[str]:
    result = []
    for i in range(1, n + 1):
        match (i % 3, i % 5):
            case (0, 0):
                result.append("FizzBuzz")
            case (0, _):
                result.append("Fizz")
            case (_, 0):
                result.append("Buzz")
            case _:
                result.append(str(i))
    return result


print(", ".join(fizzbuzz_match(15)))

print()

# --- 関数型スタイル ---
print("--- Functional Style ---")


def fizzbuzz_functional(n: int) -> List[str]:
    def fizz(i: int) -> str:
        return "Fizz" if i % 3 == 0 else ""

    def buzz(i: int) -> str:
        return "Buzz" if i % 5 == 0 else ""

    return [fizz(i) + buzz(i) or str(i) for i in range(1, n + 1)]


print(", ".join(fizzbuzz_functional(15)))

print()

# --- ジェネレータ ---
print("--- Generator (Infinite) ---")


def fizzbuzz_generator() -> Iterator[str]:
    i = 1
    while True:
        result = ""
        if i % 3 == 0:
            result += "Fizz"
        if i % 5 == 0:
            result += "Buzz"
        yield result or str(i)
        i += 1


from itertools import islice

gen = fizzbuzz_generator()
first_15 = list(islice(gen, 15))
print(f"First 15: {', '.join(first_15)}")

# 同じジェネレータから次の5つ
next_5 = list(islice(gen, 5))
print(f"Next 5: {', '.join(next_5)}")

print()

# --- 辞書ベース ---
print("--- Dictionary-based ---")


def fizzbuzz_dict(n: int) -> List[str]:
    rules = [(15, "FizzBuzz"), (3, "Fizz"), (5, "Buzz")]
    return [
        next((word for divisor, word in rules if i % divisor == 0), str(i))
        for i in range(1, n + 1)
    ]


print(", ".join(fizzbuzz_dict(15)))

print()

# --- ワンライナー ---
print("--- One-liner ---")

result = [("Fizz" * (i % 3 == 0) + "Buzz" * (i % 5 == 0)) or str(i) for i in range(1, 16)]
print(", ".join(result))

print()

# --- map/filter ---
print("--- Map/Filter ---")


def transform(i: int) -> str:
    if i % 15 == 0:
        return "FizzBuzz"
    if i % 3 == 0:
        return "Fizz"
    if i % 5 == 0:
        return "Buzz"
    return str(i)


result = list(map(transform, range(1, 16)))
print(", ".join(result))

print()

# --- テスト ---
print("--- Tests ---")


def test_fizzbuzz() -> None:
    result = fizzbuzz_functional(15)

    # 基本ケース
    assert result[0] == "1", "Failed: 1"
    assert result[1] == "2", "Failed: 2"

    # Fizz (3の倍数)
    assert result[2] == "Fizz", "Failed: 3"
    assert result[5] == "Fizz", "Failed: 6"

    # Buzz (5の倍数)
    assert result[4] == "Buzz", "Failed: 5"
    assert result[9] == "Buzz", "Failed: 10"

    # FizzBuzz (15の倍数)
    assert result[14] == "FizzBuzz", "Failed: 15"

    print("All tests passed!")


test_fizzbuzz()
