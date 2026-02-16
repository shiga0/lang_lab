// LinkedList - Go 実装
package main

import (
	"fmt"
	"strings"
)

func main() {
	fmt.Println("=== LinkedList Demo ===\n")

	basicOperations()
	iteration()
	runTests()
}

// --- Node ---
type Node[T any] struct {
	Value T
	Next  *Node[T]
}

// --- LinkedList ---
type LinkedList[T any] struct {
	head *Node[T]
	tail *Node[T]
	size int
}

// 新しいリストを作成
func NewLinkedList[T any]() *LinkedList[T] {
	return &LinkedList[T]{}
}

// 長さを返す
func (l *LinkedList[T]) Len() int {
	return l.size
}

// 空かどうか
func (l *LinkedList[T]) IsEmpty() bool {
	return l.size == 0
}

// 先頭に追加
func (l *LinkedList[T]) PushFront(value T) {
	node := &Node[T]{Value: value, Next: l.head}
	if l.head == nil {
		l.tail = node
	}
	l.head = node
	l.size++
}

// 末尾に追加
func (l *LinkedList[T]) PushBack(value T) {
	node := &Node[T]{Value: value}
	if l.tail == nil {
		l.head = node
		l.tail = node
	} else {
		l.tail.Next = node
		l.tail = node
	}
	l.size++
}

// 先頭を削除して返す
func (l *LinkedList[T]) PopFront() (T, bool) {
	if l.head == nil {
		var zero T
		return zero, false
	}
	value := l.head.Value
	l.head = l.head.Next
	if l.head == nil {
		l.tail = nil
	}
	l.size--
	return value, true
}

// 先頭の値を参照
func (l *LinkedList[T]) Front() (T, bool) {
	if l.head == nil {
		var zero T
		return zero, false
	}
	return l.head.Value, true
}

// 末尾の値を参照
func (l *LinkedList[T]) Back() (T, bool) {
	if l.tail == nil {
		var zero T
		return zero, false
	}
	return l.tail.Value, true
}

// イテレーション
func (l *LinkedList[T]) ForEach(fn func(T)) {
	for node := l.head; node != nil; node = node.Next {
		fn(node.Value)
	}
}

// スライスに変換
func (l *LinkedList[T]) ToSlice() []T {
	result := make([]T, 0, l.size)
	l.ForEach(func(v T) {
		result = append(result, v)
	})
	return result
}

// 反転
func (l *LinkedList[T]) Reverse() {
	if l.size <= 1 {
		return
	}

	var prev *Node[T]
	current := l.head
	l.tail = l.head

	for current != nil {
		next := current.Next
		current.Next = prev
		prev = current
		current = next
	}

	l.head = prev
}

// 文字列表現
func (l *LinkedList[T]) String() string {
	var sb strings.Builder
	sb.WriteString("[")
	first := true
	l.ForEach(func(v T) {
		if !first {
			sb.WriteString(" -> ")
		}
		sb.WriteString(fmt.Sprintf("%v", v))
		first = false
	})
	sb.WriteString("]")
	return sb.String()
}

// --- デモ ---
func basicOperations() {
	fmt.Println("--- Basic Operations ---")

	list := NewLinkedList[int]()
	fmt.Printf("Empty list: %s\n", list)

	list.PushFront(3)
	list.PushFront(2)
	list.PushFront(1)
	fmt.Printf("After PushFront 3, 2, 1: %s\n", list)

	list.PushBack(4)
	list.PushBack(5)
	fmt.Printf("After PushBack 4, 5: %s\n", list)

	fmt.Printf("Len: %d\n", list.Len())

	if front, ok := list.Front(); ok {
		fmt.Printf("Front: %d\n", front)
	}
	if back, ok := list.Back(); ok {
		fmt.Printf("Back: %d\n", back)
	}

	fmt.Println()

	fmt.Println("--- Pop and Reverse ---")
	if val, ok := list.PopFront(); ok {
		fmt.Printf("PopFront: %d\n", val)
	}
	fmt.Printf("After PopFront: %s\n", list)

	list.Reverse()
	fmt.Printf("After Reverse: %s\n", list)

	fmt.Println()
}

func iteration() {
	fmt.Println("--- Iteration ---")

	list := NewLinkedList[string]()
	list.PushBack("apple")
	list.PushBack("banana")
	list.PushBack("cherry")

	fmt.Print("ForEach: ")
	list.ForEach(func(s string) {
		fmt.Printf("%s ", s)
	})
	fmt.Println()

	fmt.Printf("ToSlice: %v\n", list.ToSlice())

	fmt.Println()
}

func runTests() {
	fmt.Println("--- Tests ---")

	list := NewLinkedList[int]()

	// 空リスト
	if !list.IsEmpty() {
		panic("IsEmpty failed")
	}
	if list.Len() != 0 {
		panic("Len failed")
	}

	// PushFront
	list.PushFront(1)
	list.PushFront(2)
	list.PushFront(3)
	expected := []int{3, 2, 1}
	if !sliceEqual(list.ToSlice(), expected) {
		panic("PushFront failed")
	}

	// PushBack
	list.PushBack(10)
	expected = []int{3, 2, 1, 10}
	if !sliceEqual(list.ToSlice(), expected) {
		panic("PushBack failed")
	}

	// Front / Back
	if front, ok := list.Front(); !ok || front != 3 {
		panic("Front failed")
	}
	if back, ok := list.Back(); !ok || back != 10 {
		panic("Back failed")
	}

	// PopFront
	if val, ok := list.PopFront(); !ok || val != 3 {
		panic("PopFront failed")
	}
	expected = []int{2, 1, 10}
	if !sliceEqual(list.ToSlice(), expected) {
		panic("After PopFront failed")
	}

	// Reverse
	list.Reverse()
	expected = []int{10, 1, 2}
	if !sliceEqual(list.ToSlice(), expected) {
		panic("Reverse failed")
	}

	fmt.Println("All tests passed!")
}

func sliceEqual[T comparable](a, b []T) bool {
	if len(a) != len(b) {
		return false
	}
	for i := range a {
		if a[i] != b[i] {
			return false
		}
	}
	return true
}
