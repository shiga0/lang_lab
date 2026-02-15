# オブジェクト指向比較

## 概要

| 言語 | クラス | 継承 | インターフェース | ミックスイン |
|------|--------|------|-----------------|-------------|
| Rust | なし (struct) | なし | Trait | Trait |
| Ruby | あり | 単一継承 | なし | Module (include) |
| Java | あり | 単一継承 | Interface | なし |
| Go | なし (struct) | なし | Interface (暗黙) | Embedding |

## Rust のアプローチ

Rust は「古典的なOOP」ではなく、**Trait** ベースの多態性を採用。

### 構造体 + メソッド
```rust
struct Rectangle {
    width: u32,
    height: u32,
}

impl Rectangle {
    // コンストラクタ (慣習的に new)
    fn new(width: u32, height: u32) -> Self {
        Self { width, height }
    }

    // メソッド
    fn area(&self) -> u32 {
        self.width * self.height
    }
}
```

### Trait (インターフェース)
```rust
trait Drawable {
    fn draw(&self);
}

impl Drawable for Rectangle {
    fn draw(&self) {
        println!("Drawing rectangle");
    }
}
```

### 継承の代わりに Composition
```rust
struct ColoredRectangle {
    rect: Rectangle,  // 継承ではなく包含
    color: String,
}
```

## Ruby のアプローチ

古典的なクラスベースOOP。

### クラス継承
```ruby
class Animal
  def speak
    raise NotImplementedError
  end
end

class Dog < Animal
  def speak
    "Woof!"
  end
end
```

### Module ミックスイン
```ruby
module Walkable
  def walk
    "Walking..."
  end
end

class Dog
  include Walkable
end
```

## 比較

| 観点 | Rust | Ruby |
|------|------|------|
| 多態性 | Trait オブジェクト / ジェネリクス | 継承 / Duck typing |
| コード再利用 | Trait デフォルト実装 / Composition | 継承 / Module |
| カプセル化 | pub/pub(crate)/private | public/protected/private |
| 動的ディスパッチ | dyn Trait | 常に動的 |
