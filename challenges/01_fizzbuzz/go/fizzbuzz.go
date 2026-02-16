// FizzBuzz - Go 実装
package main

import (
	"fmt"
	"strings"
)

func main() {
	fmt.Println("=== FizzBuzz Demo ===\n")

	fmt.Println("--- Basic Implementation ---")
	fizzbuzzBasic(15)

	fmt.Println("\n--- Switch Implementation ---")
	for i := 1; i <= 15; i++ {
		fmt.Printf("%s ", fizzbuzzSwitch(i))
	}
	fmt.Println()

	fmt.Println("\n--- Functional Style ---")
	results := fizzbuzzFunctional(15)
	fmt.Println(strings.Join(results, ", "))

	fmt.Println("\n--- Generator ---")
	gen := fizzbuzzGenerator()
	fmt.Print("First 15: ")
	for i := 0; i < 15; i++ {
		fmt.Printf("%s ", gen())
	}
	fmt.Println()

	fmt.Println("\n--- Tests ---")
	runTests()
}

// 基本実装
func fizzbuzzBasic(n int) {
	for i := 1; i <= n; i++ {
		if i%15 == 0 {
			fmt.Println("FizzBuzz")
		} else if i%3 == 0 {
			fmt.Println("Fizz")
		} else if i%5 == 0 {
			fmt.Println("Buzz")
		} else {
			fmt.Println(i)
		}
	}
}

// switch 文
func fizzbuzzSwitch(n int) string {
	switch {
	case n%15 == 0:
		return "FizzBuzz"
	case n%3 == 0:
		return "Fizz"
	case n%5 == 0:
		return "Buzz"
	default:
		return fmt.Sprintf("%d", n)
	}
}

// 関数型スタイル
func fizzbuzzFunctional(n int) []string {
	result := make([]string, n)
	for i := 1; i <= n; i++ {
		var s strings.Builder
		if i%3 == 0 {
			s.WriteString("Fizz")
		}
		if i%5 == 0 {
			s.WriteString("Buzz")
		}
		if s.Len() == 0 {
			s.WriteString(fmt.Sprintf("%d", i))
		}
		result[i-1] = s.String()
	}
	return result
}

// ジェネレータ
func fizzbuzzGenerator() func() string {
	i := 0
	return func() string {
		i++
		return fizzbuzzSwitch(i)
	}
}

// チャネルを使ったジェネレータ
func fizzbuzzChannel(n int) <-chan string {
	ch := make(chan string)
	go func() {
		defer close(ch)
		for i := 1; i <= n; i++ {
			ch <- fizzbuzzSwitch(i)
		}
	}()
	return ch
}

// テスト
func runTests() {
	results := fizzbuzzFunctional(15)

	// 基本ケース
	if results[0] != "1" {
		panic("Failed: 1")
	}
	if results[1] != "2" {
		panic("Failed: 2")
	}

	// Fizz (3の倍数)
	if results[2] != "Fizz" {
		panic("Failed: 3")
	}
	if results[5] != "Fizz" {
		panic("Failed: 6")
	}

	// Buzz (5の倍数)
	if results[4] != "Buzz" {
		panic("Failed: 5")
	}
	if results[9] != "Buzz" {
		panic("Failed: 10")
	}

	// FizzBuzz (15の倍数)
	if results[14] != "FizzBuzz" {
		panic("Failed: 15")
	}

	fmt.Println("All tests passed!")
}
