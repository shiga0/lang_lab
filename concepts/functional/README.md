# 関数型プログラミング比較

## 概要

| 言語 | FP サポート | イミュータブル | 高階関数 | パターンマッチ | 代数的データ型 |
|------|------------|---------------|----------|---------------|---------------|
| Rust | 強い | デフォルト | ○ | ○ | enum |
| Ruby | 中程度 | .freeze | ○ (ブロック) | case when | なし |
| Haskell | 純粋FP | デフォルト | ○ | ○ | data |
| JavaScript | 中程度 | const (浅い) | ○ | なし | なし |

## Rust の関数型機能

### 1. イミュータブルがデフォルト
```rust
let x = 5;       // 不変
let mut y = 5;   // 明示的に可変
```

### 2. クロージャ
```rust
let add = |a, b| a + b;
let double = |x| x * 2;
```

### 3. イテレータ + コンビネータ
```rust
vec![1, 2, 3]
    .iter()
    .map(|x| x * 2)
    .filter(|x| *x > 2)
    .collect::<Vec<_>>();
```

### 4. パターンマッチング
```rust
match value {
    Some(x) if x > 0 => println!("positive: {}", x),
    Some(x) => println!("non-positive: {}", x),
    None => println!("nothing"),
}
```

### 5. 代数的データ型 (enum)
```rust
enum List<T> {
    Nil,
    Cons(T, Box<List<T>>),
}
```

## Ruby の関数型機能

### ブロック / Proc / Lambda
```ruby
[1, 2, 3].map { |x| x * 2 }

add = ->(a, b) { a + b }
add.call(1, 2)
```

### Enumerable
```ruby
[1, 2, 3]
  .map { |x| x * 2 }
  .select { |x| x > 2 }
  .reduce(0) { |acc, x| acc + x }
```

## 比較: 遅延評価

```rust
// Rust: イテレータは遅延評価
(0..).take(5).collect::<Vec<_>>()  // [0, 1, 2, 3, 4]
```

```ruby
# Ruby: Enumerator::Lazy
(0..).lazy.take(5).to_a  # [0, 1, 2, 3, 4]
```
