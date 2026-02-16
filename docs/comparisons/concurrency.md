# 並行処理比較

## 概要

| 言語 | モデル | 軽量スレッド | async/await | 共有状態 |
|------|--------|-------------|-------------|----------|
| Rust | スレッド + async | 要ランタイム | あり | Mutex/Arc |
| Ruby | Thread + Fiber + Ractor | Fiber | なし | GVL制限 |
| Go | goroutine + channel | あり | なし | channel推奨 |
| TypeScript | イベントループ | なし | あり | シングルスレッド |
| Python | Thread + asyncio | なし | あり | GIL制限 |
| C++ | thread + async | なし | あり (C++20) | mutex/atomic |

## Rust

### スレッド
```rust
use std::thread;

let handle = thread::spawn(|| {
    println!("Hello from thread");
});
handle.join().unwrap();
```

### async/await
```rust
async fn fetch_data() -> String {
    // 非同期処理
    "data".to_string()
}

// tokio などのランタイムが必要
```

### 共有状態
```rust
use std::sync::{Arc, Mutex};

let counter = Arc::new(Mutex::new(0));
let counter_clone = Arc::clone(&counter);

thread::spawn(move || {
    let mut num = counter_clone.lock().unwrap();
    *num += 1;
});
```

## Ruby

### Thread (GVL制限あり)
```ruby
threads = []
5.times do |i|
  threads << Thread.new { puts "Thread #{i}" }
end
threads.each(&:join)
```

### Fiber (協調的マルチタスク)
```ruby
fiber = Fiber.new do
  Fiber.yield 1
  Fiber.yield 2
  3
end

fiber.resume  # 1
fiber.resume  # 2
```

### Ractor (真の並列処理, Ruby 3.0+)
```ruby
ractor = Ractor.new do
  msg = Ractor.receive
  "Received: #{msg}"
end

ractor.send("hello")
puts ractor.take  # "Received: hello"
```

## 比較: パラダイム

### 共有状態 vs メッセージパッシング

| アプローチ | Rust | Go |
|-----------|------|-----|
| 共有状態 | Arc<Mutex<T>> | sync.Mutex |
| メッセージ | mpsc::channel | channel |
| 推奨 | 状況による | "Don't communicate by sharing memory; share memory by communicating" |

## 詳細

- [concepts/concurrency/](../../concepts/concurrency/)
- [languages/rust/](../../languages/rust/)
- [languages/ruby/](../../languages/ruby/)
- [languages/go/](../../languages/go/)
- [languages/typescript/](../../languages/typescript/)
- [languages/python/](../../languages/python/)
- [languages/cpp/](../../languages/cpp/)
