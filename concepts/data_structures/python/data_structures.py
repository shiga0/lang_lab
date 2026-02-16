#!/usr/bin/env python3
"""データ構造 - Python 実装

Python 組み込みのデータ構造と collections モジュール。
"""

from typing import TypeVar, Generic, Optional, Iterator, List
from collections import deque, defaultdict, Counter, OrderedDict
from dataclasses import dataclass, field

print("=== Data Structures Demo ===\n")

T = TypeVar("T")


# --- リスト ---
def lists_demo() -> None:
    print("--- リスト ---")

    # 作成
    lst: List[int] = [1, 2, 3, 4, 5]
    print(f"List: {lst}")

    # 操作
    lst.append(6)
    lst.insert(0, 0)
    lst.extend([7, 8])
    print(f"After append/insert/extend: {lst}")

    # スライス
    print(f"lst[2:5]: {lst[2:5]}")
    print(f"lst[::2]: {lst[::2]}")
    print(f"lst[::-1]: {lst[::-1]}")

    # 内包表記
    squares = [x**2 for x in range(5)]
    print(f"Squares: {squares}")

    evens = [x for x in lst if x % 2 == 0]
    print(f"Evens: {evens}")

    # ソート
    unsorted = [3, 1, 4, 1, 5, 9]
    print(f"sorted(): {sorted(unsorted)}")
    print(f"sorted(reverse=True): {sorted(unsorted, reverse=True)}")

    print()


# --- タプル ---
def tuples_demo() -> None:
    print("--- タプル ---")

    # 不変
    point = (10, 20)
    person = ("Alice", 30, "alice@example.com")
    print(f"Point: {point}")
    print(f"Person: {person}")

    # アンパック
    x, y = point
    name, age, email = person
    print(f"Unpacked: x={x}, y={y}, name={name}")

    # 名前付きタプル
    from typing import NamedTuple

    class Point(NamedTuple):
        x: float
        y: float

    p = Point(3, 4)
    print(f"NamedTuple: {p}, x={p.x}")

    print()


# --- 辞書 ---
def dicts_demo() -> None:
    print("--- 辞書 ---")

    # 作成
    d = {"a": 1, "b": 2, "c": 3}
    print(f"Dict: {d}")

    # 操作
    d["d"] = 4
    del d["a"]
    print(f"After add/delete: {d}")

    # メソッド
    print(f"keys: {list(d.keys())}")
    print(f"values: {list(d.values())}")
    print(f"items: {list(d.items())}")
    print(f"get('x', 0): {d.get('x', 0)}")

    # setdefault
    d.setdefault("e", 5)
    print(f"setdefault: {d}")

    # 辞書内包表記
    squared = {k: v**2 for k, v in d.items()}
    print(f"Dict comprehension: {squared}")

    # マージ (Python 3.9+)
    d1 = {"a": 1, "b": 2}
    d2 = {"c": 3, "d": 4}
    merged = d1 | d2
    print(f"Merge |: {merged}")

    print()


# --- セット ---
def sets_demo() -> None:
    print("--- セット ---")

    s = {1, 2, 3, 4, 5}
    print(f"Set: {s}")

    # 操作
    s.add(6)
    s.discard(1)
    print(f"After add/discard: {s}")

    # 集合演算
    a = {1, 2, 3, 4}
    b = {3, 4, 5, 6}
    print(f"a | b (union): {a | b}")
    print(f"a & b (intersection): {a & b}")
    print(f"a - b (difference): {a - b}")
    print(f"a ^ b (symmetric diff): {a ^ b}")

    # frozenset (不変)
    fs = frozenset([1, 2, 3])
    print(f"frozenset: {fs}")

    print()


# --- deque ---
def deque_demo() -> None:
    print("--- deque ---")

    # 両端キュー
    d = deque([1, 2, 3])
    print(f"deque: {d}")

    d.append(4)
    d.appendleft(0)
    print(f"After append/appendleft: {d}")

    d.pop()
    d.popleft()
    print(f"After pop/popleft: {d}")

    # 回転
    d = deque([1, 2, 3, 4, 5])
    d.rotate(2)
    print(f"rotate(2): {d}")

    # maxlen
    d = deque(maxlen=3)
    d.extend([1, 2, 3, 4, 5])
    print(f"deque(maxlen=3): {d}")

    print()


# --- defaultdict ---
def defaultdict_demo() -> None:
    print("--- defaultdict ---")

    # リストをデフォルト値に
    dd: defaultdict[str, List[int]] = defaultdict(list)
    dd["a"].append(1)
    dd["a"].append(2)
    dd["b"].append(3)
    print(f"defaultdict(list): {dict(dd)}")

    # 整数をデフォルト値に
    counter: defaultdict[str, int] = defaultdict(int)
    for char in "abracadabra":
        counter[char] += 1
    print(f"defaultdict(int): {dict(counter)}")

    print()


# --- Counter ---
def counter_demo() -> None:
    print("--- Counter ---")

    c = Counter("abracadabra")
    print(f"Counter: {c}")
    print(f"most_common(3): {c.most_common(3)}")

    # 演算
    c1 = Counter(a=3, b=1)
    c2 = Counter(a=1, b=2)
    print(f"c1 + c2: {c1 + c2}")
    print(f"c1 - c2: {c1 - c2}")

    # elements
    c = Counter(a=2, b=3)
    print(f"elements: {list(c.elements())}")

    print()


# --- スタック ---
def stack_demo() -> None:
    print("--- スタック ---")

    # リストで実装
    stack = []
    stack.append(1)
    stack.append(2)
    stack.append(3)
    print(f"Stack: {stack}")
    print(f"pop: {stack.pop()}")
    print(f"After pop: {stack}")

    # カスタムクラス
    class Stack(Generic[T]):
        def __init__(self) -> None:
            self._items: List[T] = []

        def push(self, item: T) -> None:
            self._items.append(item)

        def pop(self) -> T:
            return self._items.pop()

        def peek(self) -> T:
            return self._items[-1]

        def is_empty(self) -> bool:
            return len(self._items) == 0

        def __len__(self) -> int:
            return len(self._items)

        def __repr__(self) -> str:
            return f"Stack({self._items})"

    s: Stack[int] = Stack()
    s.push(1)
    s.push(2)
    s.push(3)
    print(f"\nCustom Stack: {s}")
    print(f"peek: {s.peek()}")
    print(f"pop: {s.pop()}")

    print()


# --- キュー ---
def queue_demo() -> None:
    print("--- キュー ---")

    # deque で実装
    from collections import deque

    queue = deque()
    queue.append(1)
    queue.append(2)
    queue.append(3)
    print(f"Queue: {list(queue)}")
    print(f"popleft: {queue.popleft()}")
    print(f"After popleft: {list(queue)}")

    # queue モジュール (スレッドセーフ)
    import queue

    q: queue.Queue[int] = queue.Queue()
    q.put(1)
    q.put(2)
    print(f"\nqueue.Queue: get = {q.get()}")

    # PriorityQueue
    pq: queue.PriorityQueue[tuple[int, str]] = queue.PriorityQueue()
    pq.put((3, "low"))
    pq.put((1, "high"))
    pq.put((2, "medium"))
    print(f"PriorityQueue: {pq.get()}, {pq.get()}")

    print()


# --- 連結リスト ---
def linked_list_demo() -> None:
    print("--- 連結リスト ---")

    @dataclass
    class Node(Generic[T]):
        value: T
        next: Optional["Node[T]"] = None

    class LinkedList(Generic[T]):
        def __init__(self) -> None:
            self.head: Optional[Node[T]] = None
            self.tail: Optional[Node[T]] = None
            self._size = 0

        def append(self, value: T) -> None:
            node = Node(value)
            if self.tail is None:
                self.head = self.tail = node
            else:
                self.tail.next = node
                self.tail = node
            self._size += 1

        def prepend(self, value: T) -> None:
            node = Node(value, self.head)
            if self.head is None:
                self.tail = node
            self.head = node
            self._size += 1

        def __iter__(self) -> Iterator[T]:
            current = self.head
            while current:
                yield current.value
                current = current.next

        def __len__(self) -> int:
            return self._size

        def __repr__(self) -> str:
            return f"LinkedList([{', '.join(map(str, self))}])"

    ll: LinkedList[int] = LinkedList()
    ll.append(1)
    ll.append(2)
    ll.append(3)
    ll.prepend(0)
    print(f"LinkedList: {ll}")
    print(f"len: {len(ll)}")

    print()


# --- 二分木 ---
def binary_tree_demo() -> None:
    print("--- 二分木 ---")

    @dataclass
    class TreeNode(Generic[T]):
        value: T
        left: Optional["TreeNode[T]"] = None
        right: Optional["TreeNode[T]"] = None

    class BinarySearchTree(Generic[T]):
        def __init__(self) -> None:
            self.root: Optional[TreeNode[T]] = None

        def insert(self, value: T) -> None:
            if self.root is None:
                self.root = TreeNode(value)
            else:
                self._insert(self.root, value)

        def _insert(self, node: TreeNode[T], value: T) -> None:
            if value < node.value:
                if node.left is None:
                    node.left = TreeNode(value)
                else:
                    self._insert(node.left, value)
            else:
                if node.right is None:
                    node.right = TreeNode(value)
                else:
                    self._insert(node.right, value)

        def search(self, value: T) -> bool:
            return self._search(self.root, value)

        def _search(self, node: Optional[TreeNode[T]], value: T) -> bool:
            if node is None:
                return False
            if value == node.value:
                return True
            if value < node.value:
                return self._search(node.left, value)
            return self._search(node.right, value)

        def inorder(self) -> List[T]:
            result: List[T] = []
            self._inorder(self.root, result)
            return result

        def _inorder(self, node: Optional[TreeNode[T]], result: List[T]) -> None:
            if node:
                self._inorder(node.left, result)
                result.append(node.value)
                self._inorder(node.right, result)

    bst: BinarySearchTree[int] = BinarySearchTree()
    for val in [5, 3, 7, 1, 4, 6, 8]:
        bst.insert(val)

    print(f"Inorder: {bst.inorder()}")
    print(f"search(4): {bst.search(4)}")
    print(f"search(9): {bst.search(9)}")

    print()


# --- heapq ---
def heapq_demo() -> None:
    print("--- heapq ---")

    import heapq

    # ヒープ化
    data = [3, 1, 4, 1, 5, 9, 2, 6]
    heapq.heapify(data)
    print(f"heapify: {data}")

    # 最小値を取得
    print(f"heappop: {heapq.heappop(data)}")
    print(f"After pop: {data}")

    # 追加
    heapq.heappush(data, 0)
    print(f"After push 0: {data}")

    # n 個の最小/最大
    data = [3, 1, 4, 1, 5, 9, 2, 6]
    print(f"nsmallest(3): {heapq.nsmallest(3, data)}")
    print(f"nlargest(3): {heapq.nlargest(3, data)}")

    print()


# --- 実行 ---
if __name__ == "__main__":
    lists_demo()
    tuples_demo()
    dicts_demo()
    sets_demo()
    deque_demo()
    defaultdict_demo()
    counter_demo()
    stack_demo()
    queue_demo()
    linked_list_demo()
    binary_tree_demo()
    heapq_demo()
