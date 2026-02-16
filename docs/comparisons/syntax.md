# 構文比較

## 概要

| 言語 | パラダイム | ブロック | セミコロン | インデント |
|------|-----------|---------|-----------|-----------|
| Rust | マルチ | `{}` | 必須 | 慣習的 |
| Ruby | マルチ | `do/end`, `{}` | 不要 | 慣習的 |
| Python | マルチ | インデント | 不要 | 必須 |
| Go | 命令型 | `{}` | 自動挿入 | 慣習的 |
| TypeScript | マルチ | `{}` | 自動挿入 | 慣習的 |
| C++ | マルチ | `{}` | 必須 | 慣習的 |

## 変数宣言

### Rust
```rust
let x = 5;              // 不変
let mut y = 10;         // 可変
const MAX: i32 = 100;   // 定数
static NAME: &str = ""; // 静的変数
```

### Ruby
```ruby
x = 5           # ローカル変数
@x = 5          # インスタンス変数
@@x = 5         # クラス変数
$x = 5          # グローバル変数
X = 5           # 定数
```

### Go
```go
var x int = 5    // 明示的
x := 5           // 短縮形
const MAX = 100  // 定数
```

### C++
```cpp
int x = 5;                // 明示的
auto y = 10;              // 型推論
const int MAX = 100;      // 定数
constexpr int SIZE = 50;  // コンパイル時定数
static int count = 0;     // 静的変数
```

## 関数定義

### Rust
```rust
fn add(a: i32, b: i32) -> i32 {
    a + b  // 最後の式が戻り値
}

// クロージャ
let add = |a, b| a + b;
```

### Ruby
```ruby
def add(a, b)
  a + b  # 最後の式が戻り値
end

# ラムダ
add = ->(a, b) { a + b }
```

### Go
```go
func add(a, b int) int {
    return a + b
}

// 無名関数
add := func(a, b int) int { return a + b }
```

### C++
```cpp
int add(int a, int b) {
    return a + b;
}

// ラムダ
auto add = [](int a, int b) { return a + b; };

// テンプレート関数
template<typename T>
T add(T a, T b) { return a + b; }
```

## 制御構文

### if/else

**Rust:**
```rust
if x > 0 {
    println!("positive");
} else if x < 0 {
    println!("negative");
} else {
    println!("zero");
}

// 式として使用
let sign = if x > 0 { "+" } else { "-" };
```

**Ruby:**
```ruby
if x > 0
  puts "positive"
elsif x < 0
  puts "negative"
else
  puts "zero"
end

# 後置 if
puts "positive" if x > 0

# 三項演算子
sign = x > 0 ? "+" : "-"
```

**C++:**
```cpp
if (x > 0) {
    cout << "positive" << endl;
} else if (x < 0) {
    cout << "negative" << endl;
} else {
    cout << "zero" << endl;
}

// 三項演算子
string sign = x > 0 ? "+" : "-";
```

### ループ

**Rust:**
```rust
// for
for i in 0..5 {
    println!("{}", i);
}

// while
while x > 0 {
    x -= 1;
}

// loop (無限)
loop {
    if done { break; }
}
```

**Ruby:**
```ruby
# for (あまり使わない)
for i in 0..4
  puts i
end

# each (推奨)
(0..4).each { |i| puts i }

# while
while x > 0
  x -= 1
end

# loop
loop do
  break if done
end
```

**C++:**
```cpp
// for
for (int i = 0; i < 5; ++i) {
    cout << i << endl;
}

// range-based for (C++11)
for (auto& item : items) {
    cout << item << endl;
}

// while
while (x > 0) {
    x--;
}
```

## パターンマッチング

### Rust
```rust
match value {
    1 => println!("one"),
    2 | 3 => println!("two or three"),
    4..=10 => println!("four to ten"),
    _ => println!("other"),
}

// 構造体の分解
match point {
    Point { x: 0, y } => println!("on y-axis at {}", y),
    Point { x, y: 0 } => println!("on x-axis at {}", x),
    Point { x, y } => println!("at ({}, {})", x, y),
}
```

### Ruby
```ruby
case value
when 1
  puts "one"
when 2, 3
  puts "two or three"
when 4..10
  puts "four to ten"
else
  puts "other"
end

# パターンマッチング (Ruby 3.0+)
case point
in { x: 0, y: }
  puts "on y-axis at #{y}"
in { x:, y: 0 }
  puts "on x-axis at #{x}"
in { x:, y: }
  puts "at (#{x}, #{y})"
end
```

### C++
```cpp
switch (value) {
    case 1:
        cout << "one" << endl;
        break;
    case 2:
    case 3:
        cout << "two or three" << endl;
        break;
    default:
        cout << "other" << endl;
}

// variant + visit (C++17)
std::visit([](auto&& arg) {
    cout << arg << endl;
}, variant_value);
```

## 構造体/クラス

### Rust
```rust
struct Point {
    x: i32,
    y: i32,
}

impl Point {
    fn new(x: i32, y: i32) -> Self {
        Point { x, y }
    }

    fn distance(&self, other: &Point) -> f64 {
        let dx = (self.x - other.x) as f64;
        let dy = (self.y - other.y) as f64;
        (dx * dx + dy * dy).sqrt()
    }
}
```

### Ruby
```ruby
class Point
  attr_accessor :x, :y

  def initialize(x, y)
    @x = x
    @y = y
  end

  def distance(other)
    dx = @x - other.x
    dy = @y - other.y
    Math.sqrt(dx * dx + dy * dy)
  end
end
```

### C++
```cpp
struct Point {
    int x;
    int y;

    Point(int x, int y) : x(x), y(y) {}

    double distance(const Point& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return sqrt(dx * dx + dy * dy);
    }
};
```

## モジュール/名前空間

### Rust
```rust
mod math {
    pub fn add(a: i32, b: i32) -> i32 {
        a + b
    }
}

use math::add;
```

### Ruby
```ruby
module Math
  def self.add(a, b)
    a + b
  end
end

include Math
```

### C++
```cpp
namespace math {
    int add(int a, int b) {
        return a + b;
    }
}

using namespace math;
// または
using math::add;
```

## 詳細

- [languages/rust/cheatsheet.md](../../languages/rust/cheatsheet.md)
- [languages/ruby/cheatsheet.md](../../languages/ruby/cheatsheet.md)
- [languages/go/cheatsheet.md](../../languages/go/cheatsheet.md)
- [languages/typescript/cheatsheet.md](../../languages/typescript/cheatsheet.md)
- [languages/python/cheatsheet.md](../../languages/python/cheatsheet.md)
- [languages/cpp/cheatsheet.md](../../languages/cpp/cheatsheet.md)
