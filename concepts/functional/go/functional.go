// 関数型プログラミング - Go 実装
//
// Go は関数型言語ではないが、関数型のパターンを使える
// 第一級関数、クロージャ、高階関数をサポート

package main

import (
	"fmt"
	"sort"
	"strings"
)

func main() {
	fmt.Println("=== Functional Programming Demo ===\n")

	firstClassFunctions()
	closures()
	higherOrderFunctions()
	functionTypes()
	methodValues()
	deferredExecution()
}

// --- 第一級関数 ---
func firstClassFunctions() {
	fmt.Println("--- 第一級関数 ---")

	// 関数を変数に代入
	add := func(a, b int) int {
		return a + b
	}
	fmt.Printf("add(2, 3) = %d\n", add(2, 3))

	// 関数を引数として渡す
	result := apply(5, double)
	fmt.Printf("apply(5, double) = %d\n", result)

	// 関数を返す
	addFive := makeAdder(5)
	fmt.Printf("makeAdder(5)(10) = %d\n", addFive(10))

	fmt.Println()
}

func double(x int) int {
	return x * 2
}

func apply(x int, f func(int) int) int {
	return f(x)
}

func makeAdder(n int) func(int) int {
	return func(x int) int {
		return x + n
	}
}

// --- クロージャ ---
func closures() {
	fmt.Println("--- クロージャ ---")

	// カウンター
	counter := makeCounter()
	fmt.Printf("counter() = %d\n", counter())
	fmt.Printf("counter() = %d\n", counter())
	fmt.Printf("counter() = %d\n", counter())

	// 独立したカウンター
	counter2 := makeCounter()
	fmt.Printf("counter2() = %d\n", counter2())

	// フィボナッチジェネレータ
	fib := fibonacci()
	fmt.Print("Fibonacci: ")
	for i := 0; i < 10; i++ {
		fmt.Printf("%d ", fib())
	}
	fmt.Println()

	fmt.Println()
}

func makeCounter() func() int {
	count := 0
	return func() int {
		count++
		return count
	}
}

func fibonacci() func() int {
	a, b := 0, 1
	return func() int {
		result := a
		a, b = b, a+b
		return result
	}
}

// --- 高階関数 ---
func higherOrderFunctions() {
	fmt.Println("--- 高階関数 ---")

	numbers := []int{1, 2, 3, 4, 5}

	// Map
	doubled := Map(numbers, func(x int) int { return x * 2 })
	fmt.Printf("Map (*2): %v\n", doubled)

	// Filter
	evens := Filter(numbers, func(x int) bool { return x%2 == 0 })
	fmt.Printf("Filter (even): %v\n", evens)

	// Reduce
	sum := Reduce(numbers, 0, func(acc, x int) int { return acc + x })
	fmt.Printf("Reduce (sum): %d\n", sum)

	// チェーン
	result := Reduce(
		Filter(
			Map(numbers, func(x int) int { return x * 2 }),
			func(x int) bool { return x > 4 },
		),
		0,
		func(acc, x int) int { return acc + x },
	)
	fmt.Printf("Chain (map->filter->reduce): %d\n", result)

	// 標準ライブラリの sort
	strs := []string{"banana", "apple", "cherry"}
	sort.Slice(strs, func(i, j int) bool {
		return strs[i] < strs[j]
	})
	fmt.Printf("Sorted: %v\n", strs)

	fmt.Println()
}

// ジェネリック Map
func Map[T, U any](slice []T, f func(T) U) []U {
	result := make([]U, len(slice))
	for i, v := range slice {
		result[i] = f(v)
	}
	return result
}

// ジェネリック Filter
func Filter[T any](slice []T, f func(T) bool) []T {
	var result []T
	for _, v := range slice {
		if f(v) {
			result = append(result, v)
		}
	}
	return result
}

// ジェネリック Reduce
func Reduce[T, U any](slice []T, initial U, f func(U, T) U) U {
	result := initial
	for _, v := range slice {
		result = f(result, v)
	}
	return result
}

// --- 関数型 ---
func functionTypes() {
	fmt.Println("--- 関数型 ---")

	// 型エイリアス
	type IntFunc func(int) int
	type Predicate func(int) bool
	type BinaryOp func(int, int) int

	var f IntFunc = func(x int) int { return x * x }
	fmt.Printf("IntFunc: f(5) = %d\n", f(5))

	// 関数のスライス
	operations := []BinaryOp{
		func(a, b int) int { return a + b },
		func(a, b int) int { return a - b },
		func(a, b int) int { return a * b },
	}

	names := []string{"add", "sub", "mul"}
	for i, op := range operations {
		fmt.Printf("%s(10, 3) = %d\n", names[i], op(10, 3))
	}

	fmt.Println()
}

// Calculator for method values demo
type FuncCalculator struct {
	value int
}

func (c *FuncCalculator) Add(x int) int {
	c.value += x
	return c.value
}

// --- メソッド値 ---
func methodValues() {
	fmt.Println("--- メソッド値 ---")

	calc := &FuncCalculator{value: 10}

	// メソッド値: レシーバをバインド
	addFunc := calc.Add
	fmt.Printf("addFunc(5) = %d\n", addFunc(5))
	fmt.Printf("addFunc(3) = %d\n", addFunc(3))

	// メソッド式: レシーバを引数として渡す
	addExpr := (*FuncCalculator).Add
	calc2 := &FuncCalculator{value: 100}
	fmt.Printf("addExpr(calc2, 50) = %d\n", addExpr(calc2, 50))

	fmt.Println()
}

// --- 遅延評価パターン ---
func deferredExecution() {
	fmt.Println("--- 遅延評価パターン ---")

	// サンク (Thunk): 遅延評価する関数
	type Thunk[T any] func() T

	// 遅延初期化
	expensive := func() Thunk[string] {
		var result string
		computed := false
		return func() string {
			if !computed {
				fmt.Println("  Computing expensive value...")
				result = strings.Repeat("*", 10)
				computed = true
			}
			return result
		}
	}()

	fmt.Println("Before first call:")
	fmt.Printf("Result: %s\n", expensive())
	fmt.Println("After first call (cached):")
	fmt.Printf("Result: %s\n", expensive())

	// ジェネレータパターン
	fmt.Println("\nGenerator pattern:")
	gen := rangeGenerator(1, 5)
	for {
		v, ok := gen()
		if !ok {
			break
		}
		fmt.Printf("%d ", v)
	}
	fmt.Println()
}

func rangeGenerator(start, end int) func() (int, bool) {
	current := start
	return func() (int, bool) {
		if current > end {
			return 0, false
		}
		result := current
		current++
		return result, true
	}
}
