# Challenge 02: Linked List

## 問題

単方向連結リストを実装する。

### 必須機能
- `push_front`: 先頭に追加
- `push_back`: 末尾に追加
- `pop_front`: 先頭を削除して返す
- `len`: 長さを返す
- `iter`: イテレータを返す

## 学べること

| 言語 | 学べる概念 |
|------|-----------|
| Rust | Box, Option, 所有権, イテレータ実装 |
| Ruby | クラス, 参照, each の実装 |

## Rust 特有の課題

- `Box<T>` でヒープ割り当て
- 再帰的なデータ構造
- `Option<T>` での null 安全
- 所有権と借用の管理

## 実装

- [Rust](./rust/)
- [Ruby](./ruby/)

## 発展課題

1. 双方向連結リスト
2. `insert_at(index, value)`
3. `reverse()`
