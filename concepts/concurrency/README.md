# 並行処理比較

## 概要

| 言語 | モデル | 軽量スレッド | async/await | 共有状態 |
|------|--------|-------------|-------------|----------|
| Rust | スレッド + async | なし (tokio等で) | あり | Mutex/Arc |
| Ruby | Thread + Fiber + Ractor | Fiber | なし | GVL制限 |
| Go | goroutine + channel | あり | なし | channel推奨 |
| JavaScript | イベントループ | なし | あり | シングルスレッド |

## Rust の並行処理

### 1. スレッド (std::thread)
- OSスレッドを直接使用
- `move` クロージャで所有権を移動
- `Arc<Mutex<T>>` で共有状態

### 2. async/await
- 非同期ランタイム (tokio, async-std) が必要
- `Future` トレイト
- ゼロコスト抽象化

### 3. メッセージパッシング
- `std::sync::mpsc` (multiple producer, single consumer)
- チャネルでデータを送受信

## Ruby の並行処理

### 1. Thread
- GVL (Global VM Lock) により同時実行は1スレッド
- I/O待ち時は他スレッドが実行可能
- CPUバウンドでは効果薄い

### 2. Fiber
- 協調的マルチタスク
- 明示的に `yield` で制御を渡す
- Enumerator の内部で使用

### 3. Ractor (Ruby 3.0+)
- GVLを持たない並列実行ユニット
- メッセージパッシングで通信
- 真の並列処理が可能

## 比較: 共有状態 vs メッセージパッシング

```
共有状態 (Rust):                  メッセージパッシング (Go風):
┌─────────┐                       ┌─────────┐
│ Thread1 │──┐                    │ Thread1 │────→ channel ────→ Thread2
└─────────┘  │                    └─────────┘
             │ Arc<Mutex<T>>
┌─────────┐  │
│ Thread2 │──┤
└─────────┘  │
             ↓
        ┌─────────┐
        │  Data   │
        └─────────┘
```

Rustは両方サポート。「共有状態より通信で共有」が推奨。
