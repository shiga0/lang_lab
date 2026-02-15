//! # Rust の型システム
//!
//! Rust は静的型付け・強い型付けの言語。
//! コンパイル時に型チェックが行われ、実行時エラーを防ぐ。

fn main() {
    println!("=== Rust 型システム ===\n");

    basic_types();
    type_inference();
    generics_example();
    option_and_result();
    newtype_pattern();
    type_aliases();
}

/// 基本的な型
fn basic_types() {
    println!("--- 基本型 ---");

    // 整数型: i8, i16, i32, i64, i128, isize
    //        u8, u16, u32, u64, u128, usize
    let signed: i32 = -42;
    let unsigned: u32 = 42;
    let arch_size: usize = 100;  // アーキテクチャ依存

    // 浮動小数点: f32, f64
    let float: f64 = 3.14;

    // 真偽値
    let boolean: bool = true;

    // 文字 (4バイト Unicode)
    let character: char = '🦀';

    // 文字列
    let string_slice: &str = "hello";        // 文字列スライス (借用)
    let string_owned: String = String::from("world");  // 所有する文字列

    // タプル
    let tuple: (i32, f64, char) = (42, 3.14, 'x');
    let (a, b, c) = tuple;  // 分解

    // 配列 (固定長)
    let array: [i32; 5] = [1, 2, 3, 4, 5];

    // ベクタ (可変長)
    let vector: Vec<i32> = vec![1, 2, 3];

    println!("  signed: {}, unsigned: {}", signed, unsigned);
    println!("  float: {}, bool: {}, char: {}", float, boolean, character);
    println!("  tuple.0: {}, array[0]: {}", tuple.0, array[0]);
    println!();
}

/// 型推論
fn type_inference() {
    println!("--- 型推論 ---");

    // 型は推論される
    let x = 42;              // i32 と推論
    let y = 3.14;            // f64 と推論
    let z = "hello";         // &str と推論

    // 使われ方から推論
    let mut vec = Vec::new();
    vec.push(1);             // Vec<i32> と推論

    // 推論できない場合は注釈が必要
    let parsed: i32 = "42".parse().unwrap();
    // または turbofish 構文
    let parsed2 = "42".parse::<i32>().unwrap();

    println!("  x: {}, y: {}, z: {}", x, y, z);
    println!("  parsed: {}, parsed2: {}", parsed, parsed2);
    println!();
}

/// ジェネリクス
fn generics_example() {
    println!("--- ジェネリクス ---");

    // ジェネリック関数
    fn largest<T: PartialOrd>(list: &[T]) -> &T {
        let mut largest = &list[0];
        for item in list {
            if item > largest {
                largest = item;
            }
        }
        largest
    }

    let numbers = vec![34, 50, 25, 100, 65];
    let result = largest(&numbers);
    println!("  largest number: {}", result);

    let chars = vec!['y', 'm', 'a', 'q'];
    let result = largest(&chars);
    println!("  largest char: {}", result);

    // ジェネリック構造体
    #[derive(Debug)]
    struct Point<T> {
        x: T,
        y: T,
    }

    let integer_point = Point { x: 5, y: 10 };
    let float_point = Point { x: 1.0, y: 4.0 };
    println!("  integer point: {:?}", integer_point);
    println!("  float point: {:?}", float_point);

    // 複数の型パラメータ
    #[derive(Debug)]
    struct MixedPoint<T, U> {
        x: T,
        y: U,
    }

    let mixed = MixedPoint { x: 5, y: 4.0 };
    println!("  mixed point: {:?}", mixed);
    println!();
}

/// Option と Result
fn option_and_result() {
    println!("--- Option と Result ---");

    // Option<T>: 値があるかもしれない
    fn find_element(arr: &[i32], target: i32) -> Option<usize> {
        arr.iter().position(|&x| x == target)
    }

    let numbers = [1, 2, 3, 4, 5];

    // パターンマッチング
    match find_element(&numbers, 3) {
        Some(index) => println!("  Found at index: {}", index),
        None => println!("  Not found"),
    }

    // if let
    if let Some(index) = find_element(&numbers, 10) {
        println!("  Found at: {}", index);
    } else {
        println!("  10 is not found");
    }

    // メソッドチェーン
    let doubled = find_element(&numbers, 2)
        .map(|i| i * 2)
        .unwrap_or(0);
    println!("  doubled index: {}", doubled);

    // Result<T, E>: 成功か失敗
    fn divide(a: f64, b: f64) -> Result<f64, String> {
        if b == 0.0 {
            Err("Division by zero".to_string())
        } else {
            Ok(a / b)
        }
    }

    match divide(10.0, 2.0) {
        Ok(result) => println!("  10 / 2 = {}", result),
        Err(e) => println!("  Error: {}", e),
    }

    // ? 演算子でエラー伝播
    fn calc() -> Result<f64, String> {
        let a = divide(10.0, 2.0)?;
        let b = divide(a, 2.5)?;
        Ok(b)
    }

    println!("  calc result: {:?}", calc());
    println!();
}

/// Newtype パターン
fn newtype_pattern() {
    println!("--- Newtype パターン ---");

    // 型エイリアスとは異なり、別の型として扱われる
    #[derive(Debug, Clone, Copy)]
    struct UserId(u64);

    #[derive(Debug, Clone, Copy)]
    struct ProductId(u64);

    fn get_user(id: UserId) {
        println!("  Getting user: {:?}", id);
    }

    fn get_product(id: ProductId) {
        println!("  Getting product: {:?}", id);
    }

    let user_id = UserId(1);
    let product_id = ProductId(2);

    get_user(user_id);
    get_product(product_id);

    // コンパイルエラー: 型が異なる
    // get_user(product_id);

    println!("  -> UserId と ProductId は別の型として区別される");
    println!();
}

/// 型エイリアス
fn type_aliases() {
    println!("--- 型エイリアス ---");

    // 長い型に別名をつける
    type Kilometers = i32;
    type Thunk = Box<dyn Fn() + Send + 'static>;

    let distance: Kilometers = 100;
    println!("  distance: {} km", distance);

    // Result のエイリアス (std::io でよく使われる)
    type IoResult<T> = Result<T, std::io::Error>;

    fn read_something() -> IoResult<String> {
        Ok("data".to_string())
    }

    println!("  read: {:?}", read_something());
    println!();
}
