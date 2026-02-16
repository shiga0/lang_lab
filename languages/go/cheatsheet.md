# Go チートシート

## 基本

### 変数
```go
// var で宣言
var name string = "Go"
var age int = 14

// 型推論
var x = 42

// 短縮形 (関数内のみ)
y := 3.14

// 複数宣言
var (
    a = 1
    b = 2
)

// 定数
const Pi = 3.14159
const (
    StatusOK = 200
    StatusNotFound = 404
)
```

### 基本型
```go
// 数値
int, int8, int16, int32, int64
uint, uint8, uint16, uint32, uint64
float32, float64
complex64, complex128

// 文字列
string

// 真偽値
bool

// バイト
byte  // uint8 の別名
rune  // int32 の別名 (Unicode コードポイント)
```

### ゼロ値
```go
var i int       // 0
var f float64   // 0.0
var s string    // ""
var b bool      // false
var p *int      // nil
```

## 複合型

### 配列とスライス
```go
// 配列 (固定長)
var arr [3]int = [3]int{1, 2, 3}
arr := [...]int{1, 2, 3}  // 長さを推論

// スライス (可変長)
slice := []int{1, 2, 3}
slice = append(slice, 4, 5)

// make でスライス作成
s := make([]int, 5)      // len=5, cap=5
s := make([]int, 0, 10)  // len=0, cap=10

// スライシング
s[1:3]   // index 1 to 2
s[:3]    // index 0 to 2
s[1:]    // index 1 to end
```

### マップ
```go
// リテラル
m := map[string]int{
    "one": 1,
    "two": 2,
}

// make
m := make(map[string]int)

// 操作
m["three"] = 3           // 追加/更新
value := m["one"]        // 取得
value, ok := m["one"]    // 存在確認
delete(m, "one")         // 削除
```

### 構造体
```go
type Person struct {
    Name string
    Age  int
}

// 初期化
p := Person{Name: "Alice", Age: 30}
p := Person{"Alice", 30}

// ポインタ
pp := &Person{Name: "Bob", Age: 25}
pp.Name  // 自動デリファレンス
```

## 制御構文

### if
```go
if x > 0 {
    fmt.Println("positive")
} else if x < 0 {
    fmt.Println("negative")
} else {
    fmt.Println("zero")
}

// 初期化文付き
if err := doSomething(); err != nil {
    return err
}
```

### switch
```go
switch x {
case 1:
    fmt.Println("one")
case 2, 3:
    fmt.Println("two or three")
default:
    fmt.Println("other")
}

// 条件なし switch
switch {
case x > 0:
    fmt.Println("positive")
case x < 0:
    fmt.Println("negative")
}

// 型 switch
switch v := i.(type) {
case int:
    fmt.Println("int:", v)
case string:
    fmt.Println("string:", v)
}
```

### for
```go
// 基本
for i := 0; i < 10; i++ {
    fmt.Println(i)
}

// while 相当
for x > 0 {
    x--
}

// 無限ループ
for {
    break
}

// range
for i, v := range slice {
    fmt.Println(i, v)
}

for key, value := range m {
    fmt.Println(key, value)
}
```

## 関数

### 定義
```go
func add(a, b int) int {
    return a + b
}

// 複数返り値
func divide(a, b int) (int, error) {
    if b == 0 {
        return 0, errors.New("division by zero")
    }
    return a / b, nil
}

// 名前付き返り値
func split(sum int) (x, y int) {
    x = sum * 4 / 9
    y = sum - x
    return  // naked return
}

// 可変長引数
func sum(nums ...int) int {
    total := 0
    for _, n := range nums {
        total += n
    }
    return total
}
```

### 無名関数とクロージャ
```go
// 無名関数
add := func(a, b int) int {
    return a + b
}

// クロージャ
func counter() func() int {
    count := 0
    return func() int {
        count++
        return count
    }
}
```

## メソッドとインターフェース

### メソッド
```go
type Rectangle struct {
    Width, Height float64
}

// 値レシーバ
func (r Rectangle) Area() float64 {
    return r.Width * r.Height
}

// ポインタレシーバ
func (r *Rectangle) Scale(factor float64) {
    r.Width *= factor
    r.Height *= factor
}
```

### インターフェース
```go
type Shape interface {
    Area() float64
}

// 暗黙的実装 (implements 不要)
type Circle struct {
    Radius float64
}

func (c Circle) Area() float64 {
    return math.Pi * c.Radius * c.Radius
}

// Circle は自動的に Shape を満たす
var s Shape = Circle{Radius: 5}
```

### 空インターフェース
```go
// 任意の型を受け取る
func printAny(v interface{}) {
    fmt.Println(v)
}

// Go 1.18+ では any が使える
func printAny(v any) {
    fmt.Println(v)
}
```

## エラー処理

### 基本パターン
```go
result, err := someFunction()
if err != nil {
    return err  // または return fmt.Errorf("context: %w", err)
}
```

### カスタムエラー
```go
type MyError struct {
    Code    int
    Message string
}

func (e *MyError) Error() string {
    return fmt.Sprintf("error %d: %s", e.Code, e.Message)
}
```

### errors パッケージ
```go
import "errors"

var ErrNotFound = errors.New("not found")

// ラップ
err := fmt.Errorf("failed to load: %w", ErrNotFound)

// アンラップ
if errors.Is(err, ErrNotFound) {
    // ...
}
```

## 並行処理

### goroutine
```go
go func() {
    fmt.Println("in goroutine")
}()
```

### channel
```go
// 作成
ch := make(chan int)        // unbuffered
ch := make(chan int, 10)    // buffered

// 送受信
ch <- 42      // 送信
value := <-ch // 受信

// クローズ
close(ch)

// range で受信
for v := range ch {
    fmt.Println(v)
}

// select
select {
case v := <-ch1:
    fmt.Println(v)
case ch2 <- x:
    fmt.Println("sent")
default:
    fmt.Println("no communication")
}
```

### sync パッケージ
```go
import "sync"

// Mutex
var mu sync.Mutex
mu.Lock()
// critical section
mu.Unlock()

// WaitGroup
var wg sync.WaitGroup
wg.Add(1)
go func() {
    defer wg.Done()
    // work
}()
wg.Wait()

// Once
var once sync.Once
once.Do(func() {
    // 一度だけ実行
})
```

## ジェネリクス (Go 1.18+)

```go
// ジェネリック関数
func Min[T constraints.Ordered](a, b T) T {
    if a < b {
        return a
    }
    return b
}

// ジェネリック型
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
```

## defer, panic, recover

```go
// defer: 関数終了時に実行
func example() {
    defer fmt.Println("cleanup")
    // ...
}

// panic と recover
func safeDivide(a, b int) (result int, err error) {
    defer func() {
        if r := recover(); r != nil {
            err = fmt.Errorf("panic: %v", r)
        }
    }()
    if b == 0 {
        panic("division by zero")
    }
    return a / b, nil
}
```

## よく使うパッケージ

```go
import (
    "fmt"       // フォーマット出力
    "strings"   // 文字列操作
    "strconv"   // 型変換
    "os"        // OS 操作
    "io"        // I/O
    "net/http"  // HTTP
    "encoding/json"  // JSON
    "time"      // 時間
    "context"   // コンテキスト
    "sync"      // 同期
)
```
