#!/usr/bin/env python3
"""メタプログラミング - Python 実装

Python はメタプログラミングが強力。
デコレータ、メタクラス、ディスクリプタ、動的属性アクセス。
"""

from typing import Any, Callable, TypeVar, Dict
from functools import wraps
import inspect

print("=== Metaprogramming Demo ===\n")

T = TypeVar("T")


# --- イントロスペクション ---
def introspection() -> None:
    print("--- イントロスペクション ---")

    class Person:
        species = "Human"

        def __init__(self, name: str, age: int) -> None:
            self.name = name
            self.age = age

        def greet(self) -> str:
            return f"Hello, I'm {self.name}"

    person = Person("Alice", 30)

    # type と isinstance
    print(f"type(person): {type(person)}")
    print(f"isinstance(person, Person): {isinstance(person, Person)}")

    # dir
    print(f"\ndir(person) (filtered):")
    for attr in dir(person):
        if not attr.startswith("_"):
            print(f"  {attr}")

    # getattr / setattr / hasattr
    print(f"\ngetattr(person, 'name'): {getattr(person, 'name')}")
    setattr(person, 'age', 31)
    print(f"setattr: age = {person.age}")
    print(f"hasattr(person, 'email'): {hasattr(person, 'email')}")

    # vars
    print(f"\nvars(person): {vars(person)}")

    # inspect
    print(f"\ninspect.getmembers (methods):")
    for name, method in inspect.getmembers(person, predicate=inspect.ismethod):
        print(f"  {name}")

    print()


# --- デコレータ ---
def decorators() -> None:
    print("--- デコレータ ---")

    # 関数デコレータ
    def log(func: Callable[..., T]) -> Callable[..., T]:
        @wraps(func)
        def wrapper(*args, **kwargs) -> T:
            print(f"  Calling {func.__name__} with {args}, {kwargs}")
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
                result = None
                for _ in range(times):
                    result = func(*args, **kwargs)
                return result  # type: ignore
            return wrapper
        return decorator

    @repeat(3)
    def say(msg: str) -> None:
        print(f"  {msg}")

    print("\n@repeat(3):")
    say("Hello")

    # クラスデコレータ
    def singleton(cls):
        instances: Dict[type, Any] = {}
        @wraps(cls)
        def get_instance(*args, **kwargs):
            if cls not in instances:
                instances[cls] = cls(*args, **kwargs)
            return instances[cls]
        return get_instance

    @singleton
    class Database:
        def __init__(self, host: str) -> None:
            self.host = host

    print("\n@singleton:")
    db1 = Database("localhost")
    db2 = Database("remote")
    print(f"db1 is db2: {db1 is db2}")
    print(f"db2.host: {db2.host}")

    print()


# --- ディスクリプタ ---
def descriptors() -> None:
    print("--- ディスクリプタ ---")

    class Validator:
        def __init__(self, min_value: float, max_value: float) -> None:
            self.min_value = min_value
            self.max_value = max_value

        def __set_name__(self, owner: type, name: str) -> None:
            self.name = name

        def __get__(self, obj: Any, objtype: type) -> Any:
            if obj is None:
                return self
            return obj.__dict__.get(self.name)

        def __set__(self, obj: Any, value: float) -> None:
            if not self.min_value <= value <= self.max_value:
                raise ValueError(
                    f"{self.name} must be between {self.min_value} and {self.max_value}"
                )
            obj.__dict__[self.name] = value

    class Product:
        price = Validator(0, 10000)
        quantity = Validator(0, 1000)

        def __init__(self, name: str, price: float, quantity: int) -> None:
            self.name = name
            self.price = price
            self.quantity = quantity

    product = Product("Widget", 99.99, 10)
    print(f"Product: {product.name}, ${product.price}, qty: {product.quantity}")

    try:
        product.price = -10
    except ValueError as e:
        print(f"ValueError: {e}")

    print()


# --- メタクラス ---
def metaclasses() -> None:
    print("--- メタクラス ---")

    # 基本的なメタクラス
    class LogMeta(type):
        def __new__(mcs, name, bases, namespace):
            print(f"  Creating class: {name}")
            cls = super().__new__(mcs, name, bases, namespace)
            return cls

        def __call__(cls, *args, **kwargs):
            print(f"  Instantiating: {cls.__name__}")
            return super().__call__(*args, **kwargs)

    class MyClass(metaclass=LogMeta):
        def __init__(self, value: int) -> None:
            self.value = value

    obj = MyClass(42)

    # 属性の自動登録
    print("\n自動登録メタクラス:")

    class RegisterMeta(type):
        registry: Dict[str, type] = {}

        def __new__(mcs, name, bases, namespace):
            cls = super().__new__(mcs, name, bases, namespace)
            mcs.registry[name] = cls
            return cls

    class Base(metaclass=RegisterMeta):
        pass

    class ChildA(Base):
        pass

    class ChildB(Base):
        pass

    print(f"Registry: {list(RegisterMeta.registry.keys())}")

    print()


# --- __getattr__ / __setattr__ ---
def dynamic_attributes() -> None:
    print("--- 動的属性アクセス ---")

    class DynamicObject:
        def __init__(self) -> None:
            self._data: Dict[str, Any] = {}

        def __getattr__(self, name: str) -> Any:
            if name.startswith("_"):
                raise AttributeError(name)
            return self._data.get(name, f"<undefined: {name}>")

        def __setattr__(self, name: str, value: Any) -> None:
            if name.startswith("_"):
                super().__setattr__(name, value)
            else:
                self._data[name] = value

        def __delattr__(self, name: str) -> None:
            if name in self._data:
                del self._data[name]

    obj = DynamicObject()
    obj.foo = "bar"
    obj.count = 42

    print(f"obj.foo: {obj.foo}")
    print(f"obj.count: {obj.count}")
    print(f"obj.undefined: {obj.undefined}")

    print()


# --- __call__ ---
def callable_objects() -> None:
    print("--- callable オブジェクト ---")

    class Counter:
        def __init__(self) -> None:
            self.count = 0

        def __call__(self) -> int:
            self.count += 1
            return self.count

    counter = Counter()
    print(f"counter(): {counter()}")
    print(f"counter(): {counter()}")
    print(f"counter(): {counter()}")

    # デコレータとして
    class Retry:
        def __init__(self, times: int) -> None:
            self.times = times

        def __call__(self, func: Callable[..., T]) -> Callable[..., T]:
            @wraps(func)
            def wrapper(*args, **kwargs) -> T:
                for i in range(self.times):
                    try:
                        return func(*args, **kwargs)
                    except Exception as e:
                        print(f"  Attempt {i+1} failed: {e}")
                        if i == self.times - 1:
                            raise
                raise RuntimeError("Should not reach here")
            return wrapper

    @Retry(3)
    def flaky_function() -> str:
        import random
        if random.random() < 0.7:
            raise ValueError("Random failure")
        return "Success!"

    print("\n@Retry(3):")
    try:
        result = flaky_function()
        print(f"  Result: {result}")
    except ValueError:
        print("  All attempts failed")

    print()


# --- exec と eval ---
def exec_and_eval() -> None:
    print("--- exec と eval ---")

    # eval: 式を評価
    result = eval("2 + 3 * 4")
    print(f"eval('2 + 3 * 4'): {result}")

    # 変数を含む式
    x = 10
    result = eval("x * 2", {"x": x})
    print(f"eval('x * 2'): {result}")

    # exec: 文を実行
    code = """
def greet(name):
    return f"Hello, {name}!"
"""
    namespace: Dict[str, Any] = {}
    exec(code, namespace)
    print(f"exec defined greet: {namespace['greet']('World')}")

    # 注意: eval/exec はセキュリティリスク
    print("\n注意: eval/exec はユーザー入力に使用しないこと")

    print()


# --- __new__ ---
def new_method() -> None:
    print("--- __new__ ---")

    # 不変オブジェクトの作成
    class ImmutablePoint:
        def __new__(cls, x: float, y: float):
            instance = super().__new__(cls)
            object.__setattr__(instance, "x", x)
            object.__setattr__(instance, "y", y)
            return instance

        def __setattr__(self, name: str, value: Any) -> None:
            raise AttributeError("ImmutablePoint is immutable")

        def __repr__(self) -> str:
            return f"ImmutablePoint({self.x}, {self.y})"

    p = ImmutablePoint(10, 20)
    print(f"ImmutablePoint: {p}")
    try:
        p.x = 100
    except AttributeError as e:
        print(f"AttributeError: {e}")

    # インスタンスの再利用 (Flyweight)
    class CachedInstance:
        _cache: Dict[tuple, "CachedInstance"] = {}

        def __new__(cls, *args):
            key = (cls, args)
            if key not in cls._cache:
                cls._cache[key] = super().__new__(cls)
            return cls._cache[key]

        def __init__(self, value: int) -> None:
            self.value = value

    print("\nCachedInstance:")
    a = CachedInstance(1)
    b = CachedInstance(1)
    c = CachedInstance(2)
    print(f"a is b: {a is b}")
    print(f"a is c: {a is c}")

    print()


# --- inspect モジュール ---
def inspect_module() -> None:
    print("--- inspect モジュール ---")

    def example_function(a: int, b: str = "default") -> str:
        """Example function docstring."""
        return f"{a}: {b}"

    # シグネチャ
    sig = inspect.signature(example_function)
    print(f"Signature: {sig}")
    for name, param in sig.parameters.items():
        print(f"  {name}: {param.annotation}, default={param.default}")

    # ソースコード
    source = inspect.getsource(example_function)
    print(f"\nSource:\n{source}")

    # ドキュメント
    print(f"Docstring: {inspect.getdoc(example_function)}")

    print()


# --- 実行 ---
if __name__ == "__main__":
    introspection()
    decorators()
    descriptors()
    metaclasses()
    dynamic_attributes()
    callable_objects()
    exec_and_eval()
    new_method()
    inspect_module()
