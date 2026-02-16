// 並行処理 - Go 実装
//
// Go は goroutine と channel で並行処理をサポート
// "Don't communicate by sharing memory; share memory by communicating"

package main

import (
	"fmt"
	"sync"
	"time"
)

func main() {
	fmt.Println("=== Concurrency Demo ===\n")

	goroutines()
	channels()
	bufferedChannels()
	selectStatement()
	waitGroup()
	mutex()
	workerPool()
}

// --- goroutine ---
func goroutines() {
	fmt.Println("--- goroutine ---")

	// goroutine の起動
	go func() {
		fmt.Println("  Hello from goroutine!")
	}()

	// 複数の goroutine
	for i := 0; i < 3; i++ {
		go func(n int) {
			fmt.Printf("  goroutine %d\n", n)
		}(i)
	}

	time.Sleep(100 * time.Millisecond)
	fmt.Println()
}

// --- channel ---
func channels() {
	fmt.Println("--- channel ---")

	// チャネル作成
	ch := make(chan int)

	// 送信側 goroutine
	go func() {
		ch <- 42 // 送信
		fmt.Println("  Sent 42")
	}()

	// 受信
	value := <-ch
	fmt.Printf("  Received: %d\n", value)

	// チャネルを使った同期
	done := make(chan bool)
	go func() {
		fmt.Println("  Working...")
		time.Sleep(50 * time.Millisecond)
		done <- true
	}()
	<-done
	fmt.Println("  Done!")

	fmt.Println()
}

// --- バッファ付きチャネル ---
func bufferedChannels() {
	fmt.Println("--- バッファ付きチャネル ---")

	// バッファサイズ 3
	ch := make(chan int, 3)

	// ブロックせずに送信可能 (バッファに空きがある限り)
	ch <- 1
	ch <- 2
	ch <- 3
	fmt.Printf("  Sent 3 values, len=%d, cap=%d\n", len(ch), cap(ch))

	// 受信
	fmt.Printf("  Received: %d\n", <-ch)
	fmt.Printf("  Received: %d\n", <-ch)
	fmt.Printf("  len=%d after receiving 2\n", len(ch))

	// チャネルをクローズして range で受信
	ch2 := make(chan int, 5)
	for i := 0; i < 5; i++ {
		ch2 <- i
	}
	close(ch2)

	fmt.Print("  Range over closed channel: ")
	for v := range ch2 {
		fmt.Printf("%d ", v)
	}
	fmt.Println()

	fmt.Println()
}

// --- select ---
func selectStatement() {
	fmt.Println("--- select ---")

	ch1 := make(chan string)
	ch2 := make(chan string)

	go func() {
		time.Sleep(50 * time.Millisecond)
		ch1 <- "from ch1"
	}()

	go func() {
		time.Sleep(30 * time.Millisecond)
		ch2 <- "from ch2"
	}()

	// 最初に準備できたチャネルから受信
	for i := 0; i < 2; i++ {
		select {
		case msg := <-ch1:
			fmt.Printf("  Received: %s\n", msg)
		case msg := <-ch2:
			fmt.Printf("  Received: %s\n", msg)
		}
	}

	// タイムアウト
	ch3 := make(chan int)
	select {
	case <-ch3:
		fmt.Println("  Received from ch3")
	case <-time.After(50 * time.Millisecond):
		fmt.Println("  Timeout!")
	}

	// ノンブロッキング
	select {
	case msg := <-ch3:
		fmt.Printf("  Received: %d\n", msg)
	default:
		fmt.Println("  No message available (non-blocking)")
	}

	fmt.Println()
}

// --- WaitGroup ---
func waitGroup() {
	fmt.Println("--- WaitGroup ---")

	var wg sync.WaitGroup

	for i := 0; i < 5; i++ {
		wg.Add(1)
		go func(n int) {
			defer wg.Done()
			time.Sleep(time.Duration(n*10) * time.Millisecond)
			fmt.Printf("  Worker %d done\n", n)
		}(i)
	}

	wg.Wait()
	fmt.Println("  All workers completed")

	fmt.Println()
}

// --- Mutex ---
func mutex() {
	fmt.Println("--- Mutex ---")

	// Mutex なしでの競合 (危険)
	counterUnsafe := 0
	var wg1 sync.WaitGroup
	for i := 0; i < 100; i++ {
		wg1.Add(1)
		go func() {
			defer wg1.Done()
			counterUnsafe++ // データ競合!
		}()
	}
	wg1.Wait()
	fmt.Printf("  Counter (unsafe): %d (expected 100)\n", counterUnsafe)

	// Mutex で保護
	counterSafe := 0
	var mu sync.Mutex
	var wg2 sync.WaitGroup
	for i := 0; i < 100; i++ {
		wg2.Add(1)
		go func() {
			defer wg2.Done()
			mu.Lock()
			counterSafe++
			mu.Unlock()
		}()
	}
	wg2.Wait()
	fmt.Printf("  Counter (safe):   %d\n", counterSafe)

	// RWMutex (読み取りは並行可能)
	var rwmu sync.RWMutex
	data := make(map[string]int)

	// 書き込み
	rwmu.Lock()
	data["key"] = 42
	rwmu.Unlock()

	// 読み取り (複数 goroutine で並行可能)
	rwmu.RLock()
	_ = data["key"]
	rwmu.RUnlock()

	fmt.Println("  RWMutex allows concurrent reads")

	fmt.Println()
}

// --- Worker Pool ---
func workerPool() {
	fmt.Println("--- Worker Pool ---")

	jobs := make(chan int, 100)
	results := make(chan int, 100)

	// ワーカーを起動
	numWorkers := 3
	var wg sync.WaitGroup
	for w := 1; w <= numWorkers; w++ {
		wg.Add(1)
		go worker(w, jobs, results, &wg)
	}

	// ジョブを送信
	numJobs := 9
	for j := 1; j <= numJobs; j++ {
		jobs <- j
	}
	close(jobs)

	// ワーカーの完了を待つ
	go func() {
		wg.Wait()
		close(results)
	}()

	// 結果を収集
	fmt.Print("  Results: ")
	for r := range results {
		fmt.Printf("%d ", r)
	}
	fmt.Println()

	fmt.Println()
}

func worker(id int, jobs <-chan int, results chan<- int, wg *sync.WaitGroup) {
	defer wg.Done()
	for j := range jobs {
		fmt.Printf("  Worker %d processing job %d\n", id, j)
		time.Sleep(10 * time.Millisecond)
		results <- j * 2
	}
}
