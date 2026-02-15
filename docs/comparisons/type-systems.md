# 型システム比較

## 概要

| 言語 | 静的/動的 | 強い/弱い | 型推論 | ジェネリクス | Null安全 |
|------|-----------|-----------|--------|-------------|----------|
| Rust | 静的 | 強い | あり | Trait bounds | Option<T> |
| Ruby | 動的 | 強い | - | Duck typing | nil (unsafe) |
| TypeScript | 静的 | 弱い | あり | あり | strict null checks |
| Go | 静的 | 強い | 限定的 | あり (1.18+) | なし |
| Python | 動的 | 強い | - | Type hints | None (unsafe) |

## 静的型 vs 動的型

### 静的型付け (Rust)

```rust
fn add(a: i32, b: i32) -> i32 {
    a + b
}

// コンパイル時にエラー
// add("hello", 1);
```

**メリット:**
- コンパイル時にバグ検出
- IDE補完が強力
- リファクタリングが安全

### 動的型付け (Ruby)

```ruby
def add(a, b)
  a + b
end

add(1, 2)       # 3
add("a", "b")   # "ab"
```

**メリット:**
- 柔軟な記述
- プロトタイピングが速い
- メタプログラミングが容易

## 強い型 vs 弱い型

### 強い型 (Rust, Ruby)

暗黙的な型変換をしない。

```rust
let x: i32 = 5;
// let y: i64 = x;  // エラー: 明示的な変換が必要
let y: i64 = x as i64;
```

```ruby
1 + "2"  # TypeError
```

### 弱い型 (JavaScript)

暗黙的な型変換を行う。

```javascript
1 + "2"  // "12"
1 == "1" // true
```

## ジェネリクス

### Rust: Trait Bounds

```rust
fn largest<T: PartialOrd>(list: &[T]) -> &T {
    // T は PartialOrd を実装している必要がある
}
```

### Ruby: Duck Typing

```ruby
def process(obj)
  obj.each { |x| puts x }
  # each メソッドがあれば何でもOK
end
```

## Null 安全

### Rust: Option<T>

```rust
fn find(id: u32) -> Option<User> {
    // Some(user) or None
}

// 使う側は必ず None を考慮する必要がある
match find(1) {
    Some(user) => println!("{}", user.name),
    None => println!("not found"),
}
```

### Ruby: nil

```ruby
def find(id)
  # user or nil
end

user = find(1)
user.name  # nil だと NoMethodError
```

## 詳細

- [concepts/type_system/](../../concepts/type_system/)
