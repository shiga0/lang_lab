#!/usr/bin/env python3
"""LinkedList - Python 実装"""

from typing import TypeVar, Generic, Optional, Iterator, List
from dataclasses import dataclass

print("=== LinkedList Demo ===\n")

T = TypeVar("T")


# --- Node ---
@dataclass
class Node(Generic[T]):
    value: T
    next: Optional["Node[T]"] = None


# --- LinkedList ---
class LinkedList(Generic[T]):
    def __init__(self) -> None:
        self._head: Optional[Node[T]] = None
        self._tail: Optional[Node[T]] = None
        self._size: int = 0

    @property
    def size(self) -> int:
        return self._size

    def is_empty(self) -> bool:
        return self._size == 0

    # 先頭に追加
    def push_front(self, value: T) -> "LinkedList[T]":
        node = Node(value)
        if self._head is None:
            self._head = node
            self._tail = node
        else:
            node.next = self._head
            self._head = node
        self._size += 1
        return self

    # 末尾に追加
    def push_back(self, value: T) -> "LinkedList[T]":
        node = Node(value)
        if self._tail is None:
            self._head = node
            self._tail = node
        else:
            self._tail.next = node
            self._tail = node
        self._size += 1
        return self

    # 先頭を削除
    def pop_front(self) -> Optional[T]:
        if self._head is None:
            return None
        value = self._head.value
        self._head = self._head.next
        if self._head is None:
            self._tail = None
        self._size -= 1
        return value

    # 先頭の値を参照
    @property
    def front(self) -> Optional[T]:
        return self._head.value if self._head else None

    # 末尾の値を参照
    @property
    def back(self) -> Optional[T]:
        return self._tail.value if self._tail else None

    # イテレーション
    def __iter__(self) -> Iterator[T]:
        current = self._head
        while current:
            yield current.value
            current = current.next

    def __len__(self) -> int:
        return self._size

    # 検索
    def find(self, value: T) -> Optional[Node[T]]:
        current = self._head
        while current:
            if current.value == value:
                return current
            current = current.next
        return None

    def __contains__(self, value: T) -> bool:
        return self.find(value) is not None

    # 削除
    def delete(self, value: T) -> Optional[T]:
        if self._head is None:
            return None

        # 先頭の場合
        if self._head.value == value:
            return self.pop_front()

        # それ以外
        prev = self._head
        current = self._head.next
        while current:
            if current.value == value:
                prev.next = current.next
                if current == self._tail:
                    self._tail = prev
                self._size -= 1
                return value
            prev = current
            current = current.next
        return None

    # 反転
    def reverse(self) -> "LinkedList[T]":
        if self._size <= 1:
            return self

        prev: Optional[Node[T]] = None
        current = self._head
        self._tail = self._head

        while current:
            next_node = current.next
            current.next = prev
            prev = current
            current = next_node

        self._head = prev
        return self

    # クリア
    def clear(self) -> None:
        self._head = None
        self._tail = None
        self._size = 0

    def to_list(self) -> List[T]:
        return list(self)

    def __repr__(self) -> str:
        return f"LinkedList([{' -> '.join(map(str, self))}])"


# --- デモ ---
print("--- Basic Operations ---")

lst: LinkedList[int] = LinkedList()
print(f"Empty list: {lst}")

lst.push_front(3)
lst.push_front(2)
lst.push_front(1)
print(f"After push_front 3, 2, 1: {lst}")

lst.push_back(4)
lst.push_back(5)
print(f"After push_back 4, 5: {lst}")

print(f"Size: {lst.size}")
print(f"Front: {lst.front}")
print(f"Back: {lst.back}")

print()

print("--- Pop and Delete ---")
print(f"pop_front: {lst.pop_front()}")
print(f"After pop_front: {lst}")

print(f"delete(4): {lst.delete(4)}")
print(f"After delete(4): {lst}")

print()

print("--- Iteration ---")
lst.clear()
for x in [1, 2, 3, 4, 5]:
    lst.push_back(x)
print(f"List: {lst}")
print(f"map (x * 2): {[x * 2 for x in lst]}")
print(f"filter (x > 2): {[x for x in lst if x > 2]}")
print(f"sum: {sum(lst)}")
print(f"3 in list: {3 in lst}")

print()

print("--- Reverse ---")
print(f"Before reverse: {lst}")
lst.reverse()
print(f"After reverse: {lst}")

print()

# --- テスト ---
print("--- Tests ---")


def run_tests() -> None:
    lst: LinkedList[int] = LinkedList()

    # 空リスト
    assert lst.is_empty(), "empty? failed"
    assert lst.size == 0, "size failed"

    # push_front
    lst.push_front(1)
    lst.push_front(2)
    lst.push_front(3)
    assert lst.to_list() == [3, 2, 1], "push_front failed"

    # push_back
    lst.push_back(10)
    assert lst.to_list() == [3, 2, 1, 10], "push_back failed"

    # front / back
    assert lst.front == 3, "front failed"
    assert lst.back == 10, "back failed"

    # pop_front
    assert lst.pop_front() == 3, "pop_front failed"
    assert lst.to_list() == [2, 1, 10], "after pop_front failed"

    # delete
    assert lst.delete(1) == 1, "delete failed"
    assert lst.to_list() == [2, 10], "after delete failed"

    # reverse
    lst.push_back(20)
    lst.reverse()
    assert lst.to_list() == [20, 10, 2], "reverse failed"

    # iteration
    assert len(list(lst)) == 3, "iteration failed"
    assert 10 in lst, "contains failed"

    print("All tests passed!")


run_tests()
