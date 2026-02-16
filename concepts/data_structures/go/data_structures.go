// データ構造 - Go 実装
//
// Go の組み込みデータ構造と自作実装

package main

import (
	"container/heap"
	"container/list"
	"fmt"
	"sync"
)

func main() {
	fmt.Println("=== Data Structures Demo ===\n")

	slices()
	maps()
	linkedList()
	stackAndQueue()
	priorityQueue()
	set()
	syncMap()
}

// --- スライス ---
func slices() {
	fmt.Println("--- スライス ---")

	// 作成
	s1 := []int{1, 2, 3, 4, 5}
	s2 := make([]int, 5)    // len=5, cap=5
	s3 := make([]int, 0, 5) // len=0, cap=5

	fmt.Printf("s1: %v, len=%d, cap=%d\n", s1, len(s1), cap(s1))
	fmt.Printf("s2: %v, len=%d, cap=%d\n", s2, len(s2), cap(s2))
	fmt.Printf("s3: %v, len=%d, cap=%d\n", s3, len(s3), cap(s3))

	// 追加
	s3 = append(s3, 1, 2, 3)
	fmt.Printf("After append: %v\n", s3)

	// スライシング
	fmt.Printf("s1[1:3]: %v\n", s1[1:3])
	fmt.Printf("s1[:3]:  %v\n", s1[:3])
	fmt.Printf("s1[2:]:  %v\n", s1[2:])

	// コピー
	dst := make([]int, len(s1))
	copy(dst, s1)
	dst[0] = 100
	fmt.Printf("s1: %v, dst: %v\n", s1, dst)

	// 削除 (index 2)
	idx := 2
	s1 = append(s1[:idx], s1[idx+1:]...)
	fmt.Printf("After delete index 2: %v\n", s1)

	fmt.Println()
}

// --- マップ ---
func maps() {
	fmt.Println("--- マップ ---")

	// 作成
	m := map[string]int{
		"one":   1,
		"two":   2,
		"three": 3,
	}
	fmt.Printf("map: %v\n", m)

	// アクセス
	fmt.Printf("m[\"one\"]: %d\n", m["one"])

	// 存在確認
	val, ok := m["four"]
	fmt.Printf("m[\"four\"]: %d, exists: %t\n", val, ok)

	// 追加/更新
	m["four"] = 4
	fmt.Printf("After add: %v\n", m)

	// 削除
	delete(m, "one")
	fmt.Printf("After delete: %v\n", m)

	// イテレーション
	fmt.Print("Keys: ")
	for k := range m {
		fmt.Printf("%s ", k)
	}
	fmt.Println()

	fmt.Println()
}

// --- 連結リスト (container/list) ---
func linkedList() {
	fmt.Println("--- 連結リスト ---")

	// 標準ライブラリの双方向リスト
	l := list.New()

	// 追加
	l.PushBack(1)
	l.PushBack(2)
	l.PushBack(3)
	l.PushFront(0)

	// イテレーション
	fmt.Print("List: ")
	for e := l.Front(); e != nil; e = e.Next() {
		fmt.Printf("%v ", e.Value)
	}
	fmt.Println()

	// 削除
	front := l.Front()
	l.Remove(front)
	fmt.Print("After remove front: ")
	for e := l.Front(); e != nil; e = e.Next() {
		fmt.Printf("%v ", e.Value)
	}
	fmt.Println()

	// 自作の単方向リスト
	fmt.Println("\nCustom singly linked list:")
	myList := &SinglyLinkedList[int]{}
	myList.PushFront(3)
	myList.PushFront(2)
	myList.PushFront(1)
	myList.PushBack(4)
	fmt.Printf("List: %v\n", myList.ToSlice())
	fmt.Printf("PopFront: %v\n", myList.PopFront())
	fmt.Printf("After pop: %v\n", myList.ToSlice())

	fmt.Println()
}

// 自作単方向リスト
type Node[T any] struct {
	Value T
	Next  *Node[T]
}

type SinglyLinkedList[T any] struct {
	Head *Node[T]
	size int
}

func (l *SinglyLinkedList[T]) PushFront(value T) {
	l.Head = &Node[T]{Value: value, Next: l.Head}
	l.size++
}

func (l *SinglyLinkedList[T]) PushBack(value T) {
	node := &Node[T]{Value: value}
	if l.Head == nil {
		l.Head = node
	} else {
		current := l.Head
		for current.Next != nil {
			current = current.Next
		}
		current.Next = node
	}
	l.size++
}

func (l *SinglyLinkedList[T]) PopFront() T {
	if l.Head == nil {
		var zero T
		return zero
	}
	value := l.Head.Value
	l.Head = l.Head.Next
	l.size--
	return value
}

func (l *SinglyLinkedList[T]) ToSlice() []T {
	result := make([]T, 0, l.size)
	for current := l.Head; current != nil; current = current.Next {
		result = append(result, current.Value)
	}
	return result
}

// --- スタックとキュー ---
func stackAndQueue() {
	fmt.Println("--- スタックとキュー ---")

	// スタック (スライスで実装)
	fmt.Println("Stack:")
	stack := &Stack[int]{}
	stack.Push(1)
	stack.Push(2)
	stack.Push(3)
	fmt.Printf("  Stack: %v\n", stack.items)
	fmt.Printf("  Pop: %v\n", stack.Pop())
	fmt.Printf("  Peek: %v\n", stack.Peek())

	// キュー (スライスで実装)
	fmt.Println("\nQueue:")
	queue := &Queue[string]{}
	queue.Enqueue("first")
	queue.Enqueue("second")
	queue.Enqueue("third")
	fmt.Printf("  Queue: %v\n", queue.items)
	fmt.Printf("  Dequeue: %v\n", queue.Dequeue())
	fmt.Printf("  Front: %v\n", queue.Front())

	fmt.Println()
}

// スタック
type Stack[T any] struct {
	items []T
}

func (s *Stack[T]) Push(item T) {
	s.items = append(s.items, item)
}

func (s *Stack[T]) Pop() T {
	if len(s.items) == 0 {
		var zero T
		return zero
	}
	item := s.items[len(s.items)-1]
	s.items = s.items[:len(s.items)-1]
	return item
}

func (s *Stack[T]) Peek() T {
	if len(s.items) == 0 {
		var zero T
		return zero
	}
	return s.items[len(s.items)-1]
}

func (s *Stack[T]) IsEmpty() bool {
	return len(s.items) == 0
}

// キュー
type Queue[T any] struct {
	items []T
}

func (q *Queue[T]) Enqueue(item T) {
	q.items = append(q.items, item)
}

func (q *Queue[T]) Dequeue() T {
	if len(q.items) == 0 {
		var zero T
		return zero
	}
	item := q.items[0]
	q.items = q.items[1:]
	return item
}

func (q *Queue[T]) Front() T {
	if len(q.items) == 0 {
		var zero T
		return zero
	}
	return q.items[0]
}

func (q *Queue[T]) IsEmpty() bool {
	return len(q.items) == 0
}

// --- 優先度キュー (container/heap) ---
func priorityQueue() {
	fmt.Println("--- 優先度キュー ---")

	// container/heap を使った優先度キュー
	pq := &IntHeap{3, 1, 4, 1, 5, 9, 2, 6}
	heap.Init(pq)

	heap.Push(pq, 0)

	fmt.Print("Pop order (min heap): ")
	for pq.Len() > 0 {
		fmt.Printf("%d ", heap.Pop(pq))
	}
	fmt.Println()

	fmt.Println()
}

// heap.Interface を実装
type IntHeap []int

func (h IntHeap) Len() int           { return len(h) }
func (h IntHeap) Less(i, j int) bool { return h[i] < h[j] }
func (h IntHeap) Swap(i, j int)      { h[i], h[j] = h[j], h[i] }

func (h *IntHeap) Push(x any) {
	*h = append(*h, x.(int))
}

func (h *IntHeap) Pop() any {
	old := *h
	n := len(old)
	x := old[n-1]
	*h = old[0 : n-1]
	return x
}

// --- Set ---
func set() {
	fmt.Println("--- Set (map[T]struct{}) ---")

	// Go には組み込みの Set がないので、map で代用
	set1 := make(Set[int])
	set1.Add(1)
	set1.Add(2)
	set1.Add(3)

	set2 := make(Set[int])
	set2.Add(2)
	set2.Add(3)
	set2.Add(4)

	fmt.Printf("set1: %v\n", set1.ToSlice())
	fmt.Printf("set2: %v\n", set2.ToSlice())
	fmt.Printf("set1.Contains(2): %t\n", set1.Contains(2))
	fmt.Printf("Union: %v\n", set1.Union(set2).ToSlice())
	fmt.Printf("Intersection: %v\n", set1.Intersection(set2).ToSlice())

	fmt.Println()
}

type Set[T comparable] map[T]struct{}

func (s Set[T]) Add(item T) {
	s[item] = struct{}{}
}

func (s Set[T]) Remove(item T) {
	delete(s, item)
}

func (s Set[T]) Contains(item T) bool {
	_, ok := s[item]
	return ok
}

func (s Set[T]) ToSlice() []T {
	result := make([]T, 0, len(s))
	for k := range s {
		result = append(result, k)
	}
	return result
}

func (s Set[T]) Union(other Set[T]) Set[T] {
	result := make(Set[T])
	for k := range s {
		result.Add(k)
	}
	for k := range other {
		result.Add(k)
	}
	return result
}

func (s Set[T]) Intersection(other Set[T]) Set[T] {
	result := make(Set[T])
	for k := range s {
		if other.Contains(k) {
			result.Add(k)
		}
	}
	return result
}

// --- sync.Map ---
func syncMap() {
	fmt.Println("--- sync.Map (並行安全) ---")

	var m sync.Map

	// 書き込み
	m.Store("key1", "value1")
	m.Store("key2", "value2")

	// 読み取り
	if val, ok := m.Load("key1"); ok {
		fmt.Printf("key1: %v\n", val)
	}

	// 存在しなければ保存
	actual, loaded := m.LoadOrStore("key3", "value3")
	fmt.Printf("key3: %v, loaded: %t\n", actual, loaded)

	// イテレーション
	fmt.Print("All keys: ")
	m.Range(func(key, value any) bool {
		fmt.Printf("%v ", key)
		return true
	})
	fmt.Println()
}
