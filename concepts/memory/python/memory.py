#!/usr/bin/env python3
"""メモリ管理 - Python 実装

Python は参照カウント + 循環参照GC でメモリ管理。
CPython の実装詳細を解説。
"""

import sys
import gc
import weakref
from typing import Any, Optional

print("=== Memory Management Demo ===\n")


# --- 参照とコピー ---
def references_and_copies() -> None:
    print("--- 参照とコピー ---")

    # イミュータブル (不変): コピーのように見える
    a = 42
    b = a
    b = 100
    print(f"イミュータブル: a = {a}, b = {b} (独立)")

    # ミュータブル (可変): 参照を共有
    list1 = [1, 2, 3]
    list2 = list1  # 参照をコピー
    list2.append(4)
    print(f"ミュータブル: list1 = {list1} (共有)")

    # シャローコピー
    import copy
    list3 = list1.copy()  # または copy.copy(list1)
    list3.append(5)
    print(f"シャローコピー: list1 = {list1}, list3 = {list3}")

    # ネストしたオブジェクトは共有
    nested = [[1, 2], [3, 4]]
    shallow = nested.copy()
    shallow[0].append(100)
    print(f"ネスト共有: nested = {nested}, shallow = {shallow}")

    # ディープコピー
    nested2 = [[1, 2], [3, 4]]
    deep = copy.deepcopy(nested2)
    deep[0].append(100)
    print(f"ディープコピー: nested2 = {nested2}, deep = {deep}")

    print()


# --- 参照カウント ---
def reference_counting() -> None:
    print("--- 参照カウント ---")

    class Tracked:
        def __init__(self, name: str) -> None:
            self.name = name
            print(f"  Created: {name}")

        def __del__(self) -> None:
            print(f"  Deleted: {self.name}")

    # 参照カウントの確認
    obj = Tracked("obj1")
    print(f"  refcount after creation: {sys.getrefcount(obj) - 1}")  # -1 for getrefcount's reference

    ref = obj
    print(f"  refcount after ref: {sys.getrefcount(obj) - 1}")

    del ref
    print(f"  refcount after del ref: {sys.getrefcount(obj) - 1}")

    # 参照カウントが0になると即座に解放
    print("  Deleting obj...")
    del obj

    print("  (obj deleted)")
    print()


# --- 循環参照とGC ---
def circular_references() -> None:
    print("--- 循環参照とGC ---")

    class Node:
        def __init__(self, name: str) -> None:
            self.name = name
            self.ref: Optional["Node"] = None

        def __del__(self) -> None:
            print(f"  Node deleted: {self.name}")

    # 循環参照を作成
    a = Node("A")
    b = Node("B")
    a.ref = b
    b.ref = a  # 循環参照

    # 参照を削除しても参照カウントは0にならない
    del a
    del b
    print("  After del: objects still exist (circular reference)")

    # GCで解放
    gc.collect()
    print("  After gc.collect(): objects deleted")

    print()


# --- weakref ---
def weak_references() -> None:
    print("--- weakref ---")

    class Data:
        def __init__(self, value: Any) -> None:
            self.value = value

        def __repr__(self) -> str:
            return f"Data({self.value})"

    # 弱参照
    obj = Data(42)
    weak = weakref.ref(obj)

    print(f"weak(): {weak()}")
    print(f"obj exists: {weak() is not None}")

    del obj
    print(f"After del obj: weak() = {weak()}")

    # WeakValueDictionary
    print("\nWeakValueDictionary:")
    cache: weakref.WeakValueDictionary[str, Data] = weakref.WeakValueDictionary()

    data1 = Data("value1")
    cache["key1"] = data1
    print(f"  cache['key1'] = {cache.get('key1')}")

    del data1
    print(f"  After del: cache.get('key1') = {cache.get('key1')}")

    # finalize (デストラクタの代替)
    print("\nfinalize:")
    obj2 = Data("cleanup")
    weakref.finalize(obj2, lambda: print("  finalize callback called"))
    del obj2

    print()


# --- メモリ使用量 ---
def memory_usage() -> None:
    print("--- メモリ使用量 ---")

    # オブジェクトのサイズ
    print(f"sys.getsizeof(0): {sys.getsizeof(0)} bytes")
    print(f"sys.getsizeof(42): {sys.getsizeof(42)} bytes")
    print(f"sys.getsizeof(''): {sys.getsizeof('')} bytes")
    print(f"sys.getsizeof('hello'): {sys.getsizeof('hello')} bytes")
    print(f"sys.getsizeof([]): {sys.getsizeof([])} bytes")
    print(f"sys.getsizeof([1,2,3]): {sys.getsizeof([1, 2, 3])} bytes")
    print(f"sys.getsizeof({{}}): {sys.getsizeof({})} bytes")

    # 注意: getsizeof は浅いサイズのみ
    nested = [[1, 2, 3], [4, 5, 6]]
    print(f"\nnested list shallow size: {sys.getsizeof(nested)} bytes")
    print("  (does not include nested objects)")

    print()


# --- GC の操作 ---
def gc_operations() -> None:
    print("--- GC の操作 ---")

    # GC 統計
    print(f"gc.isenabled(): {gc.isenabled()}")
    print(f"gc.get_count(): {gc.get_count()}")
    print(f"gc.get_threshold(): {gc.get_threshold()}")

    # 世代別 GC
    print("\n世代別 GC:")
    print("  Generation 0: 新しいオブジェクト")
    print("  Generation 1: Generation 0 を生き残ったオブジェクト")
    print("  Generation 2: 長期間生存するオブジェクト")

    # 手動 GC
    collected = gc.collect()
    print(f"\ngc.collect(): {collected} objects collected")

    # GC を無効化 (パフォーマンス重視の場合)
    # gc.disable()
    # gc.enable()

    print()


# --- __slots__ ---
def slots_demo() -> None:
    print("--- __slots__ ---")

    # 通常のクラス
    class NormalClass:
        def __init__(self, x: int, y: int) -> None:
            self.x = x
            self.y = y

    # __slots__ を使用
    class SlottedClass:
        __slots__ = ("x", "y")

        def __init__(self, x: int, y: int) -> None:
            self.x = x
            self.y = y

    normal = NormalClass(1, 2)
    slotted = SlottedClass(1, 2)

    print(f"NormalClass size: {sys.getsizeof(normal)} bytes")
    print(f"SlottedClass size: {sys.getsizeof(slotted)} bytes")

    # __dict__ の有無
    print(f"\nNormalClass has __dict__: {hasattr(normal, '__dict__')}")
    print(f"SlottedClass has __dict__: {hasattr(slotted, '__dict__')}")

    # slotted.z = 3  # AttributeError

    print()


# --- intern (文字列の重複排除) ---
def string_interning() -> None:
    print("--- String Interning ---")

    # 短い文字列は自動的に intern される
    a = "hello"
    b = "hello"
    print(f"'hello' is 'hello': {a is b}")

    # 長い文字列や動的な文字列
    c = "hello world " * 100
    d = "hello world " * 100
    print(f"long string is: {c is d}")

    # 手動で intern
    e = sys.intern("hello world " * 100)
    f = sys.intern("hello world " * 100)
    print(f"interned string is: {e is f}")

    print()


# --- memoryview ---
def memoryview_demo() -> None:
    print("--- memoryview ---")

    # バイト配列のメモリビュー
    data = bytearray(b"Hello, World!")
    view = memoryview(data)

    print(f"Original: {data}")
    print(f"memoryview: {view.tolist()[:5]}...")

    # コピーなしでスライス
    sub = view[0:5]
    sub[0] = ord("h")  # 小文字に変更
    print(f"After modify view: {data}")

    # 形状の変更 (numpy 的な操作)
    arr = bytearray(range(12))
    view2 = memoryview(arr).cast("B", (3, 4))
    print(f"\nReshaped (3x4):")
    for row in view2.tolist():
        print(f"  {row}")

    print()


# --- 実行 ---
if __name__ == "__main__":
    references_and_copies()
    reference_counting()
    circular_references()
    weak_references()
    memory_usage()
    gc_operations()
    slots_demo()
    string_interning()
    memoryview_demo()
