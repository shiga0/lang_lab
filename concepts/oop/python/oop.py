#!/usr/bin/env python3
"""オブジェクト指向 - Python 実装

Python は全てがオブジェクト。
クラス、継承、多重継承、メタクラスをサポート。
"""

from abc import ABC, abstractmethod
from dataclasses import dataclass, field
from typing import List, Optional, ClassVar, Protocol

print("=== OOP Demo ===\n")


# --- クラスの基本 ---
def class_basics() -> None:
    print("--- クラスの基本 ---")

    class Person:
        # クラス変数
        species: ClassVar[str] = "Human"

        def __init__(self, name: str, age: int) -> None:
            # インスタンス変数
            self.name = name
            self.age = age

        def greet(self) -> str:
            return f"Hello, I'm {self.name}"

        def __str__(self) -> str:
            return f"Person({self.name}, {self.age})"

        def __repr__(self) -> str:
            return f"Person(name={self.name!r}, age={self.age})"

    person = Person("Alice", 30)
    print(f"person: {person}")
    print(f"greet: {person.greet()}")
    print(f"species: {Person.species}")

    print()


# --- プロパティ ---
def properties() -> None:
    print("--- プロパティ ---")

    class Circle:
        def __init__(self, radius: float) -> None:
            self._radius = radius

        @property
        def radius(self) -> float:
            return self._radius

        @radius.setter
        def radius(self, value: float) -> None:
            if value < 0:
                raise ValueError("Radius must be non-negative")
            self._radius = value

        @property
        def area(self) -> float:
            import math
            return math.pi * self._radius ** 2

    circle = Circle(5)
    print(f"radius: {circle.radius}")
    print(f"area: {circle.area:.2f}")

    circle.radius = 10
    print(f"new area: {circle.area:.2f}")

    try:
        circle.radius = -1
    except ValueError as e:
        print(f"Error: {e}")

    print()


# --- クラスメソッドとスタティックメソッド ---
def class_and_static_methods() -> None:
    print("--- クラスメソッドとスタティックメソッド ---")

    class Date:
        def __init__(self, year: int, month: int, day: int) -> None:
            self.year = year
            self.month = month
            self.day = day

        @classmethod
        def from_string(cls, date_str: str) -> "Date":
            year, month, day = map(int, date_str.split("-"))
            return cls(year, month, day)

        @staticmethod
        def is_valid_date(year: int, month: int, day: int) -> bool:
            return 1 <= month <= 12 and 1 <= day <= 31

        def __str__(self) -> str:
            return f"{self.year}-{self.month:02d}-{self.day:02d}"

    date1 = Date(2024, 1, 15)
    date2 = Date.from_string("2024-02-20")

    print(f"date1: {date1}")
    print(f"date2: {date2}")
    print(f"is_valid_date(2024, 2, 30): {Date.is_valid_date(2024, 2, 30)}")

    print()


# --- 継承 ---
def inheritance() -> None:
    print("--- 継承 ---")

    class Animal:
        def __init__(self, name: str) -> None:
            self.name = name

        def speak(self) -> str:
            return f"{self.name} makes a sound"

        def move(self) -> str:
            return f"{self.name} moves"

    class Dog(Animal):
        def __init__(self, name: str, breed: str) -> None:
            super().__init__(name)
            self.breed = breed

        def speak(self) -> str:
            return f"{self.name} says: Woof!"

        def fetch(self) -> str:
            return f"{self.name} fetches the ball"

    dog = Dog("Buddy", "Golden Retriever")
    print(f"speak: {dog.speak()}")
    print(f"move: {dog.move()}")
    print(f"fetch: {dog.fetch()}")

    print()


# --- 多重継承と MRO ---
def multiple_inheritance() -> None:
    print("--- 多重継承と MRO ---")

    class A:
        def method(self) -> str:
            return "A"

    class B(A):
        def method(self) -> str:
            return f"B -> {super().method()}"

    class C(A):
        def method(self) -> str:
            return f"C -> {super().method()}"

    class D(B, C):
        def method(self) -> str:
            return f"D -> {super().method()}"

    d = D()
    print(f"D().method(): {d.method()}")
    print(f"MRO: {[c.__name__ for c in D.__mro__]}")

    # Mixin パターン
    class LogMixin:
        def log(self, message: str) -> None:
            print(f"  [LOG] {message}")

    class SerializeMixin:
        def to_dict(self) -> dict:
            return self.__dict__

    class User(LogMixin, SerializeMixin):
        def __init__(self, name: str) -> None:
            self.name = name

    user = User("Alice")
    user.log("User created")
    print(f"to_dict: {user.to_dict()}")

    print()


# --- 抽象基底クラス ---
def abstract_classes() -> None:
    print("--- 抽象基底クラス ---")

    class Shape(ABC):
        @abstractmethod
        def area(self) -> float:
            pass

        @abstractmethod
        def perimeter(self) -> float:
            pass

        def describe(self) -> str:
            return f"Area: {self.area():.2f}, Perimeter: {self.perimeter():.2f}"

    class Rectangle(Shape):
        def __init__(self, width: float, height: float) -> None:
            self.width = width
            self.height = height

        def area(self) -> float:
            return self.width * self.height

        def perimeter(self) -> float:
            return 2 * (self.width + self.height)

    class Circle(Shape):
        def __init__(self, radius: float) -> None:
            self.radius = radius

        def area(self) -> float:
            import math
            return math.pi * self.radius ** 2

        def perimeter(self) -> float:
            import math
            return 2 * math.pi * self.radius

    shapes: List[Shape] = [Rectangle(10, 5), Circle(7)]
    for shape in shapes:
        print(f"{shape.__class__.__name__}: {shape.describe()}")

    print()


# --- Protocol (構造的部分型) ---
def protocols() -> None:
    print("--- Protocol (構造的部分型) ---")

    class Drawable(Protocol):
        def draw(self) -> str: ...

    class Square:
        def __init__(self, size: float) -> None:
            self.size = size

        def draw(self) -> str:
            return f"Square({self.size})"

    class Text:
        def __init__(self, content: str) -> None:
            self.content = content

        def draw(self) -> str:
            return f"Text: {self.content}"

    def render(item: Drawable) -> None:
        print(f"  Rendering: {item.draw()}")

    render(Square(10))
    render(Text("Hello"))

    print()


# --- データクラス ---
def dataclasses_demo() -> None:
    print("--- データクラス ---")

    @dataclass
    class Point:
        x: float
        y: float

        def distance(self, other: "Point") -> float:
            return ((self.x - other.x)**2 + (self.y - other.y)**2) ** 0.5

    p1 = Point(0, 0)
    p2 = Point(3, 4)
    print(f"p1: {p1}")
    print(f"p2: {p2}")
    print(f"distance: {p1.distance(p2)}")
    print(f"p1 == Point(0, 0): {p1 == Point(0, 0)}")

    # 不変データクラス
    @dataclass(frozen=True)
    class ImmutablePoint:
        x: float
        y: float

    ip = ImmutablePoint(10, 20)
    print(f"\nfrozen: {ip}")
    # ip.x = 100  # FrozenInstanceError

    # デフォルトファクトリ
    @dataclass
    class Container:
        name: str
        items: List[str] = field(default_factory=list)

    c = Container("box")
    c.items.append("item1")
    print(f"default_factory: {c}")

    print()


# --- 特殊メソッド ---
def special_methods() -> None:
    print("--- 特殊メソッド ---")

    class Vector:
        def __init__(self, x: float, y: float) -> None:
            self.x = x
            self.y = y

        def __add__(self, other: "Vector") -> "Vector":
            return Vector(self.x + other.x, self.y + other.y)

        def __sub__(self, other: "Vector") -> "Vector":
            return Vector(self.x - other.x, self.y - other.y)

        def __mul__(self, scalar: float) -> "Vector":
            return Vector(self.x * scalar, self.y * scalar)

        def __eq__(self, other: object) -> bool:
            if not isinstance(other, Vector):
                return NotImplemented
            return self.x == other.x and self.y == other.y

        def __len__(self) -> int:
            return 2

        def __getitem__(self, index: int) -> float:
            if index == 0:
                return self.x
            elif index == 1:
                return self.y
            raise IndexError

        def __iter__(self):
            yield self.x
            yield self.y

        def __repr__(self) -> str:
            return f"Vector({self.x}, {self.y})"

    v1 = Vector(1, 2)
    v2 = Vector(3, 4)

    print(f"v1 + v2: {v1 + v2}")
    print(f"v1 - v2: {v1 - v2}")
    print(f"v1 * 3: {v1 * 3}")
    print(f"v1 == Vector(1, 2): {v1 == Vector(1, 2)}")
    print(f"len(v1): {len(v1)}")
    print(f"v1[0]: {v1[0]}")
    print(f"list(v1): {list(v1)}")

    print()


# --- __slots__ ---
def slots_demo() -> None:
    print("--- __slots__ ---")

    class WithoutSlots:
        def __init__(self, x: int, y: int) -> None:
            self.x = x
            self.y = y

    class WithSlots:
        __slots__ = ("x", "y")

        def __init__(self, x: int, y: int) -> None:
            self.x = x
            self.y = y

    import sys

    obj1 = WithoutSlots(1, 2)
    obj2 = WithSlots(1, 2)

    print(f"Without __slots__: {sys.getsizeof(obj1)} bytes, has __dict__: {hasattr(obj1, '__dict__')}")
    print(f"With __slots__: {sys.getsizeof(obj2)} bytes, has __dict__: {hasattr(obj2, '__dict__')}")

    # __slots__ があると動的に属性を追加できない
    obj1.z = 3  # OK
    # obj2.z = 3  # AttributeError

    print()


# --- メタクラス ---
def metaclass_demo() -> None:
    print("--- メタクラス ---")

    # メタクラスはクラスを生成するクラス
    class SingletonMeta(type):
        _instances: dict = {}

        def __call__(cls, *args, **kwargs):
            if cls not in cls._instances:
                cls._instances[cls] = super().__call__(*args, **kwargs)
            return cls._instances[cls]

    class Singleton(metaclass=SingletonMeta):
        def __init__(self, value: int) -> None:
            self.value = value

    s1 = Singleton(1)
    s2 = Singleton(2)  # 同じインスタンスを返す

    print(f"s1.value: {s1.value}")
    print(f"s2.value: {s2.value}")
    print(f"s1 is s2: {s1 is s2}")

    print()


# --- 実行 ---
if __name__ == "__main__":
    class_basics()
    properties()
    class_and_static_methods()
    inheritance()
    multiple_inheritance()
    abstract_classes()
    protocols()
    dataclasses_demo()
    special_methods()
    slots_demo()
    metaclass_demo()
