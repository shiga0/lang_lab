# 型システム比較

## 概要

| 言語 | 静的/動的 | 強い/弱い | 型推論 | ジェネリクス | Null安全 |
|------|-----------|-----------|--------|-------------|----------|
| Rust | 静的 | 強い | あり | Trait bounds | Option<T> |
| Ruby | 動的 | 強い | - | Duck typing | nil (unsafe) |

## 静的型 vs 動的型

**静的型 (Rust)**
- コンパイル時に型チェック
- 実行前にエラー検出
- IDEの補完が強力

**動的型 (Ruby)**
- 実行時に型チェック
- 柔軟な記述が可能
- テストで補完

## ディレクトリ

```
type_system/
├── rust/
│   ├── src/main.rs       # 基本的な型の例
│   └── Cargo.toml
└── ruby/
    └── main.rb
```

## 比較ポイント

### 1. 変数宣言と型注釈

```rust
// Rust: 型推論あり、明示も可能
let x = 42;           // i32 と推論
let y: u64 = 100;     // 明示的に u64
```

```ruby
# Ruby: 型注釈なし（動的型）
x = 42          # Integer
y = "hello"     # String
x = "world"     # 同じ変数に別の型を代入可能
```

### 2. ジェネリクス

```rust
// Rust: trait bounds で制約
fn print_debug<T: std::fmt::Debug>(value: T) {
    println!("{:?}", value);
}
```

```ruby
# Ruby: Duck typing
def print_debug(value)
  puts value.inspect  # inspect メソッドがあれば OK
end
```

### 3. Null 安全

```rust
// Rust: Option<T> で明示的に扱う
fn find_user(id: u32) -> Option<User> {
    // Some(user) or None
}

// 使う側は必ずチェックが必要
if let Some(user) = find_user(1) {
    println!("{}", user.name);
}
```

```ruby
# Ruby: nil が返る可能性
def find_user(id)
  # user or nil
end

# nil チェックを忘れると NoMethodError
user = find_user(1)
puts user.name  # user が nil だとエラー
```
