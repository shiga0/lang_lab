// メモリ管理 - Go 実装
//
// Go は GC (Garbage Collection) でメモリ管理
// 並行 GC で低レイテンシを実現

package main

import (
	"fmt"
	"runtime"
	"unsafe"
)

// Point for pointer demo
type Point struct {
	X, Y int
}

func main() {
	fmt.Println("=== Memory Management Demo ===\n")

	valuesAndPointers()
	heapVsStack()
	slicesAndMemory()
	garbageCollection()
	memoryStats()
}

// --- 値とポインタ ---
func valuesAndPointers() {
	fmt.Println("--- 値とポインタ ---")

	// 値型: コピーが渡される
	x := 42
	y := x // コピー
	y = 100
	fmt.Printf("x = %d, y = %d (値はコピー)\n", x, y)

	// ポインタ: 参照を渡す
	a := 42
	b := &a // ポインタ
	*b = 100
	fmt.Printf("a = %d, *b = %d (ポインタは参照)\n", a, *b)

	// 構造体
	p1 := Point{X: 10, Y: 20}
	p2 := p1 // コピー
	p2.X = 100
	fmt.Printf("p1 = %+v, p2 = %+v (構造体はコピー)\n", p1, p2)

	// ポインタで渡す
	p3 := &Point{X: 10, Y: 20}
	modifyPoint(p3)
	fmt.Printf("p3 after modify = %+v\n", *p3)

	fmt.Println()
}

func modifyPoint(p *Point) {
	p.X = 999
}

// --- スタック vs ヒープ ---
func heapVsStack() {
	fmt.Println("--- スタック vs ヒープ ---")

	// スタック: 関数のローカル変数
	// コンパイラがエスケープ解析で判断
	stackVar := 42
	fmt.Printf("スタック変数: %d (アドレス: %p)\n", stackVar, &stackVar)

	// ヒープ: エスケープする変数
	heapVar := createOnHeap()
	fmt.Printf("ヒープ変数: %d (アドレス: %p)\n", *heapVar, heapVar)

	// 大きな構造体はヒープに
	largeStruct := make([]byte, 1024*1024) // 1MB
	fmt.Printf("大きなスライス: len=%d\n", len(largeStruct))

	fmt.Println()
}

// 関数を抜けても参照が残る -> ヒープにエスケープ
func createOnHeap() *int {
	x := 42
	return &x
}

// --- スライスとメモリ ---
func slicesAndMemory() {
	fmt.Println("--- スライスとメモリ ---")

	// スライスの内部構造
	// type slice struct {
	//     array unsafe.Pointer  // 配列へのポインタ
	//     len   int             // 長さ
	//     cap   int             // 容量
	// }

	s := make([]int, 3, 5)
	fmt.Printf("slice: len=%d, cap=%d\n", len(s), cap(s))
	fmt.Printf("slice header size: %d bytes\n", unsafe.Sizeof(s))

	// append での再割り当て
	s1 := []int{1, 2, 3}
	fmt.Printf("s1 before append: cap=%d, addr=%p\n", cap(s1), s1)

	s1 = append(s1, 4, 5, 6, 7, 8) // 容量超過で再割り当て
	fmt.Printf("s1 after append:  cap=%d, addr=%p\n", cap(s1), s1)

	// スライスは配列を共有
	original := []int{1, 2, 3, 4, 5}
	sub := original[1:4]
	fmt.Printf("original: %v\n", original)
	fmt.Printf("sub:      %v\n", sub)

	sub[0] = 100 // original にも影響
	fmt.Printf("After sub[0]=100:\n")
	fmt.Printf("original: %v\n", original)
	fmt.Printf("sub:      %v\n", sub)

	// コピーして独立させる
	independent := make([]int, len(sub))
	copy(independent, sub)
	independent[0] = 999
	fmt.Printf("independent: %v (original unchanged: %v)\n", independent, original)

	fmt.Println()
}

// --- GC (Garbage Collection) ---
func garbageCollection() {
	fmt.Println("--- GC (Garbage Collection) ---")

	// GC の特徴
	fmt.Println("Go の GC:")
	fmt.Println("  - 並行 GC (Concurrent Mark & Sweep)")
	fmt.Println("  - 低レイテンシ (STW 時間を最小化)")
	fmt.Println("  - 世代なし (Non-generational)")

	// GC 前のメモリ
	var m runtime.MemStats
	runtime.ReadMemStats(&m)
	fmt.Printf("\nBefore allocation: HeapAlloc = %d KB\n", m.HeapAlloc/1024)

	// メモリを割り当て
	allocations := make([]*[1024]byte, 1000)
	for i := range allocations {
		allocations[i] = new([1024]byte)
	}

	runtime.ReadMemStats(&m)
	fmt.Printf("After allocation:  HeapAlloc = %d KB\n", m.HeapAlloc/1024)

	// 参照を消してGC
	allocations = nil
	runtime.GC() // 手動 GC

	runtime.ReadMemStats(&m)
	fmt.Printf("After GC:          HeapAlloc = %d KB\n", m.HeapAlloc/1024)

	fmt.Println()
}

// --- メモリ統計 ---
func memoryStats() {
	fmt.Println("--- メモリ統計 ---")

	var m runtime.MemStats
	runtime.ReadMemStats(&m)

	fmt.Printf("HeapAlloc:    %d KB (使用中のヒープ)\n", m.HeapAlloc/1024)
	fmt.Printf("HeapSys:      %d KB (OS から確保したヒープ)\n", m.HeapSys/1024)
	fmt.Printf("HeapIdle:     %d KB (未使用のヒープ)\n", m.HeapIdle/1024)
	fmt.Printf("HeapReleased: %d KB (OS に返却済み)\n", m.HeapReleased/1024)
	fmt.Printf("StackSys:     %d KB (スタック)\n", m.StackSys/1024)
	fmt.Printf("NumGC:        %d (GC 実行回数)\n", m.NumGC)
	fmt.Printf("NumGoroutine: %d\n", runtime.NumGoroutine())

	// GOGC 環境変数
	fmt.Println("\nGC 設定:")
	fmt.Println("  GOGC=100 (デフォルト): ヒープが 100% 増加したら GC")
	fmt.Println("  GOGC=off: GC 無効化")
	fmt.Println("  GOMEMLIMIT: メモリ上限を設定 (Go 1.19+)")
}
