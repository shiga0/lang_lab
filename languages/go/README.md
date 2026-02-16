# Go

## 概要

| 項目 | 値 |
|------|-----|
| 登場年 | 2009 |
| 設計者 | Robert Griesemer, Rob Pike, Ken Thompson (Google) |
| パラダイム | 命令型, 並行プログラミング |
| 型付け | 静的・強い |
| 実行形式 | コンパイル (ネイティブ) |
| 主な用途 | サーバー, CLI, インフラ, クラウド |

## 特徴

### シンプルな構文
```go
package main

import "fmt"

func main() {
    fmt.Println("Hello, Go!")
}
```

### goroutine と channel
```go
ch := make(chan int)

go func() {
    ch <- 42  // 送信
}()

value := <-ch  // 受信
```

### インターフェース (暗黙的実装)
```go
type Reader interface {
    Read(p []byte) (n int, err error)
}

// 明示的な implements 宣言は不要
// メソッドがあれば自動的に満たす
```

### エラー処理
```go
file, err := os.Open("file.txt")
if err != nil {
    return err
}
defer file.Close()
```

## Rust / Ruby との比較

| 観点 | Go | Rust | Ruby |
|------|-----|------|------|
| 型チェック | コンパイル時 | コンパイル時 | 実行時 |
| メモリ管理 | GC | 所有権 | GC |
| Null | nil (unsafe) | Option<T> | nil |
| エラー | 多値返却 | Result<T, E> | 例外 |
| 並行処理 | goroutine | async/await | Thread/Ractor |
| ジェネリクス | あり (1.18+) | あり | Duck typing |
| 速度 | 速い | 最速 | 遅い |
| 学習曲線 | 緩やか | 急 | 緩やか |

## 実装例

- [concepts/](../../concepts/) - 概念別の実装
- [challenges/](../../challenges/) - チャレンジ

## リンク

- [公式サイト](https://go.dev/)
- [A Tour of Go](https://go.dev/tour/)
- [Go Packages](https://pkg.go.dev/)
