// 型システム - Go 実装
//
// Go は静的型付け言語
// 型推論、インターフェース、ジェネリクス (1.18+) をサポート

package main

import (
	"fmt"
)

func main() {
	fmt.Println("=== Type System Demo ===\n")

	basicTypes()
	compositeTypes()
	typeInference()
	interfaces()
	generics()
	typeAssertions()
}

// --- 基本型 ---
func basicTypes() {
	fmt.Println("--- 基本型 ---")

	// 整数型
	var i int = 42
	var i8 int8 = 127
	var i64 int64 = 9223372036854775807
	fmt.Printf("int: %d, int8: %d, int64: %d\n", i, i8, i64)

	// 符号なし整数
	var u uint = 42
	var u8 uint8 = 255 // byte
	fmt.Printf("uint: %d, uint8: %d\n", u, u8)

	// 浮動小数点
	var f32 float32 = 3.14
	var f64 float64 = 3.141592653589793
	fmt.Printf("float32: %f, float64: %f\n", f32, f64)

	// 真偽値
	var b bool = true
	fmt.Printf("bool: %t\n", b)

	// 文字列
	var s string = "Hello, Go!"
	fmt.Printf("string: %s\n", s)

	// rune (Unicode コードポイント)
	var r rune = '日'
	fmt.Printf("rune: %c (U+%04X)\n", r, r)

	fmt.Println()
}

// --- 複合型 ---
func compositeTypes() {
	fmt.Println("--- 複合型 ---")

	// 配列 (固定長)
	var arr [3]int = [3]int{1, 2, 3}
	fmt.Printf("Array: %v (len=%d)\n", arr, len(arr))

	// スライス (可変長)
	slice := []int{1, 2, 3, 4, 5}
	slice = append(slice, 6)
	fmt.Printf("Slice: %v (len=%d, cap=%d)\n", slice, len(slice), cap(slice))

	// マップ
	m := map[string]int{
		"one":   1,
		"two":   2,
		"three": 3,
	}
	fmt.Printf("Map: %v\n", m)

	// 構造体
	type Point struct {
		X, Y int
	}
	p := Point{X: 10, Y: 20}
	fmt.Printf("Struct: %+v\n", p)

	// ポインタ
	ptr := &p
	fmt.Printf("Pointer: %p, *ptr = %+v\n", ptr, *ptr)

	fmt.Println()
}

// --- 型推論 ---
func typeInference() {
	fmt.Println("--- 型推論 ---")

	// := で型推論
	x := 42         // int
	y := 3.14       // float64
	z := "hello"    // string
	b := true       // bool

	fmt.Printf("x: %T = %v\n", x, x)
	fmt.Printf("y: %T = %v\n", y, y)
	fmt.Printf("z: %T = %v\n", z, z)
	fmt.Printf("b: %T = %v\n", b, b)

	// 複合リテラルの型推論
	slice := []int{1, 2, 3}
	m := map[string]int{"a": 1}
	fmt.Printf("slice: %T\n", slice)
	fmt.Printf("map: %T\n", m)

	fmt.Println()
}

// --- インターフェース ---
func interfaces() {
	fmt.Println("--- インターフェース ---")

	// インターフェース定義
	type Shape interface {
		Area() float64
	}

	// 構造体
	type Rectangle struct {
		Width, Height float64
	}

	type Circle struct {
		Radius float64
	}

	// メソッド実装 (暗黙的にインターフェースを満たす)
	// Rectangle に Area メソッドを追加
	// (この例ではローカル関数内なので実際には追加できないが、概念を示す)

	// 代わりに既存の型でデモ
	fmt.Println("インターフェースは暗黙的に実装される")
	fmt.Println("implements キーワードは不要")

	// Stringer インターフェース
	type Person struct {
		Name string
		Age  int
	}
	p := Person{Name: "Alice", Age: 30}
	fmt.Printf("Person: %+v\n", p)

	// 空インターフェース (any)
	var anything interface{} = 42
	fmt.Printf("any: %v (type: %T)\n", anything, anything)
	anything = "hello"
	fmt.Printf("any: %v (type: %T)\n", anything, anything)

	fmt.Println()
}

// --- ジェネリクス (Go 1.18+) ---
func generics() {
	fmt.Println("--- ジェネリクス (Go 1.18+) ---")

	// ジェネリック関数
	result := Min(3, 5)
	fmt.Printf("Min(3, 5) = %d\n", result)

	resultF := Min(3.14, 2.71)
	fmt.Printf("Min(3.14, 2.71) = %f\n", resultF)

	// ジェネリックスライス操作
	ints := []int{1, 2, 3}
	strings := []string{"a", "b", "c"}
	fmt.Printf("Contains(ints, 2) = %t\n", Contains(ints, 2))
	fmt.Printf("Contains(strings, \"x\") = %t\n", Contains(strings, "x"))

	// ジェネリックスタック
	stack := &Stack[int]{}
	stack.Push(1)
	stack.Push(2)
	stack.Push(3)
	fmt.Printf("Stack: %v\n", stack.items)

	val, ok := stack.Pop()
	fmt.Printf("Pop: %d (ok=%t)\n", val, ok)

	fmt.Println()
}

// ジェネリック Min 関数
type Ordered interface {
	~int | ~int8 | ~int16 | ~int32 | ~int64 |
		~uint | ~uint8 | ~uint16 | ~uint32 | ~uint64 |
		~float32 | ~float64 | ~string
}

func Min[T Ordered](a, b T) T {
	if a < b {
		return a
	}
	return b
}

// ジェネリック Contains 関数
func Contains[T comparable](slice []T, value T) bool {
	for _, v := range slice {
		if v == value {
			return true
		}
	}
	return false
}

// ジェネリック Stack
type Stack[T any] struct {
	items []T
}

func (s *Stack[T]) Push(item T) {
	s.items = append(s.items, item)
}

func (s *Stack[T]) Pop() (T, bool) {
	if len(s.items) == 0 {
		var zero T
		return zero, false
	}
	item := s.items[len(s.items)-1]
	s.items = s.items[:len(s.items)-1]
	return item, true
}

// --- 型アサーション ---
func typeAssertions() {
	fmt.Println("--- 型アサーション ---")

	var i interface{} = "hello"

	// 型アサーション
	s, ok := i.(string)
	fmt.Printf("i.(string) = %q, ok = %t\n", s, ok)

	n, ok := i.(int)
	fmt.Printf("i.(int) = %d, ok = %t\n", n, ok)

	// 型スイッチ
	checkType := func(v interface{}) string {
		switch v := v.(type) {
		case int:
			return fmt.Sprintf("int: %d", v)
		case string:
			return fmt.Sprintf("string: %s", v)
		case bool:
			return fmt.Sprintf("bool: %t", v)
		default:
			return fmt.Sprintf("unknown: %T", v)
		}
	}

	fmt.Println(checkType(42))
	fmt.Println(checkType("hello"))
	fmt.Println(checkType(true))
	fmt.Println(checkType(3.14))
}
