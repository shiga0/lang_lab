# Rust チートシート

## 変数と型

```rust
// 変数 (イミュータブル)
let x = 5;
let x: i32 = 5;

// 可変変数
let mut y = 10;
y = 20;

// 定数
const MAX_POINTS: u32 = 100_000;

// シャドーイング
let x = 5;
let x = x + 1;  // 新しい x
```

## 基本型

```rust
// 整数
let a: i8 = -128;        // i8, i16, i32, i64, i128, isize
let b: u8 = 255;         // u8, u16, u32, u64, u128, usize

// 浮動小数点
let c: f64 = 3.14;       // f32, f64

// 真偽値
let d: bool = true;

// 文字 (4バイト Unicode)
let e: char = '🦀';

// 文字列
let s1: &str = "hello";           // 文字列スライス
let s2: String = String::from("hello");  // 所有する文字列

// タプル
let tup: (i32, f64, char) = (500, 6.4, 'y');
let (x, y, z) = tup;     // 分解
let first = tup.0;       // インデックスアクセス

// 配列 (固定長)
let arr: [i32; 5] = [1, 2, 3, 4, 5];
let arr2 = [3; 5];       // [3, 3, 3, 3, 3]

// ベクタ (可変長)
let vec: Vec<i32> = vec![1, 2, 3];
```

## 関数

```rust
fn add(a: i32, b: i32) -> i32 {
    a + b  // return 省略可 (最後の式)
}

fn no_return() {
    println!("no return");
}

fn early_return(n: i32) -> i32 {
    if n < 0 {
        return 0;  // 早期リターン
    }
    n * 2
}
```

## 制御フロー

```rust
// if
if x > 5 {
    println!("big");
} else if x > 0 {
    println!("small");
} else {
    println!("zero or negative");
}

// if 式
let y = if x > 0 { 1 } else { -1 };

// loop
loop {
    if condition { break; }
}

// loop with value
let result = loop {
    if done { break value; }
};

// while
while x > 0 {
    x -= 1;
}

// for
for i in 0..5 { }        // 0, 1, 2, 3, 4
for i in 0..=5 { }       // 0, 1, 2, 3, 4, 5
for item in vec.iter() { }
for (i, item) in vec.iter().enumerate() { }

// match
match x {
    1 => println!("one"),
    2 | 3 => println!("two or three"),
    4..=10 => println!("four to ten"),
    n if n < 0 => println!("negative"),
    _ => println!("other"),
}
```

## 所有権と借用

```rust
// 所有権の移動
let s1 = String::from("hello");
let s2 = s1;         // s1 は無効に

// Clone
let s3 = s2.clone();  // 深いコピー

// 参照 (借用)
let len = calculate_length(&s3);  // 不変参照
fn calculate_length(s: &String) -> usize { s.len() }

// 可変参照
fn change(s: &mut String) { s.push_str("!"); }
let mut s = String::from("hello");
change(&mut s);
```

## 構造体

```rust
struct User {
    name: String,
    age: u32,
}

impl User {
    // 関連関数 (コンストラクタ)
    fn new(name: &str, age: u32) -> Self {
        Self {
            name: name.to_string(),
            age,
        }
    }

    // メソッド
    fn greet(&self) {
        println!("Hello, {}", self.name);
    }
}

let user = User::new("Alice", 30);
user.greet();
```

## 列挙型 (Enum)

```rust
enum Message {
    Quit,
    Move { x: i32, y: i32 },
    Write(String),
    ChangeColor(i32, i32, i32),
}

let msg = Message::Move { x: 10, y: 20 };

match msg {
    Message::Quit => println!("quit"),
    Message::Move { x, y } => println!("move to {}, {}", x, y),
    Message::Write(text) => println!("write: {}", text),
    Message::ChangeColor(r, g, b) => println!("color: {}, {}, {}", r, g, b),
}
```

## Option と Result

```rust
// Option<T>
let some: Option<i32> = Some(5);
let none: Option<i32> = None;

match some {
    Some(x) => println!("{}", x),
    None => println!("nothing"),
}

// よく使うメソッド
some.unwrap();           // panic if None
some.unwrap_or(0);       // default
some.map(|x| x * 2);     // transform
some.and_then(|x| Some(x * 2));

// Result<T, E>
fn divide(a: f64, b: f64) -> Result<f64, String> {
    if b == 0.0 {
        Err("division by zero".to_string())
    } else {
        Ok(a / b)
    }
}

// ? 演算子
fn calc() -> Result<f64, String> {
    let x = divide(10.0, 2.0)?;  // Err なら早期リターン
    Ok(x * 2.0)
}
```

## Trait

```rust
trait Summary {
    fn summarize(&self) -> String;

    // デフォルト実装
    fn preview(&self) -> String {
        format!("Preview: {}", self.summarize())
    }
}

struct Article { title: String }

impl Summary for Article {
    fn summarize(&self) -> String {
        self.title.clone()
    }
}

// ジェネリクス + Trait 境界
fn notify<T: Summary>(item: &T) {
    println!("{}", item.summarize());
}

// where 構文
fn notify2<T>(item: &T)
where
    T: Summary + Clone,
{
    println!("{}", item.summarize());
}
```

## イテレータ

```rust
let v = vec![1, 2, 3];

// 基本
v.iter();           // &T
v.iter_mut();       // &mut T
v.into_iter();      // T (所有権を取る)

// コンビネータ
v.iter()
    .map(|x| x * 2)
    .filter(|x| *x > 2)
    .take(5)
    .collect::<Vec<_>>();

// その他
v.iter().fold(0, |acc, x| acc + x);  // 畳み込み
v.iter().find(|x| **x > 2);          // 検索
v.iter().any(|x| *x > 2);            // 存在チェック
v.iter().all(|x| *x > 0);            // 全チェック
```

## クロージャ

```rust
// 基本
let add = |a, b| a + b;
let add_typed: fn(i32, i32) -> i32 = |a, b| a + b;

// 環境キャプチャ
let factor = 2;
let multiply = |x| x * factor;

// move (所有権を移動)
let s = String::from("hello");
let print_s = move || println!("{}", s);
```

## モジュール

```rust
// lib.rs or main.rs
mod my_module;           // my_module.rs or my_module/mod.rs を読み込み

// 定義
mod my_module {
    pub fn public_fn() {}
    fn private_fn() {}
}

// 使用
use my_module::public_fn;
use std::collections::HashMap;
use std::io::{self, Read, Write};  // 複数
```

## マクロ

```rust
// 組み込みマクロ
println!("Hello, {}!", name);
format!("value: {}", x);
vec![1, 2, 3];
panic!("error!");
assert_eq!(a, b);
dbg!(expression);  // デバッグ出力

// カスタムマクロ
macro_rules! say_hello {
    () => {
        println!("Hello!");
    };
    ($name:expr) => {
        println!("Hello, {}!", $name);
    };
}
```

## 属性

```rust
#[derive(Debug, Clone, PartialEq)]
struct MyStruct {}

#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
        assert_eq!(2 + 2, 4);
    }
}

#[allow(dead_code)]
fn unused() {}

#[must_use]
fn important() -> Result<(), ()> { Ok(()) }
```

## Cargo コマンド

```bash
cargo new project_name    # 新規プロジェクト
cargo build              # ビルド
cargo build --release    # リリースビルド
cargo run                # 実行
cargo test               # テスト
cargo check              # 高速チェック
cargo clippy             # リンター
cargo fmt                # フォーマット
cargo doc --open         # ドキュメント生成
cargo add package_name   # 依存追加
```
