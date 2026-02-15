//! # Rust のエラー処理
//!
//! Rust は Result/Option で明示的にエラーを扱う。
//! 「エラーを無視できない」設計により安全性を高める。

use std::fs::File;
use std::io::{self, Read};

fn main() {
    println!("=== Rust エラー処理 ===\n");

    result_basics();
    option_basics();
    question_mark_operator();
    custom_errors();
    combinators();
}

/// Result の基本
fn result_basics() {
    println!("--- Result<T, E> の基本 ---");

    // Result を返す関数
    fn divide(a: f64, b: f64) -> Result<f64, String> {
        if b == 0.0 {
            Err("Division by zero".to_string())
        } else {
            Ok(a / b)
        }
    }

    // パターンマッチング
    match divide(10.0, 2.0) {
        Ok(result) => println!("  10 / 2 = {}", result),
        Err(e) => println!("  Error: {}", e),
    }

    match divide(10.0, 0.0) {
        Ok(result) => println!("  10 / 0 = {}", result),
        Err(e) => println!("  Error: {}", e),
    }

    // unwrap (失敗時は panic)
    let result = divide(20.0, 4.0).unwrap();
    println!("  unwrap: {}", result);

    // expect (カスタムメッセージ付き unwrap)
    let result = divide(30.0, 5.0).expect("除算に失敗");
    println!("  expect: {}", result);

    // unwrap_or (デフォルト値)
    let result = divide(10.0, 0.0).unwrap_or(0.0);
    println!("  unwrap_or: {}", result);

    // unwrap_or_else (遅延評価)
    let result = divide(10.0, 0.0).unwrap_or_else(|e| {
        println!("  (エラー処理: {})", e);
        -1.0
    });
    println!("  unwrap_or_else: {}", result);

    println!();
}

/// Option の基本
fn option_basics() {
    println!("--- Option<T> の基本 ---");

    fn find_element(arr: &[i32], target: i32) -> Option<usize> {
        arr.iter().position(|&x| x == target)
    }

    let numbers = [10, 20, 30, 40, 50];

    // パターンマッチング
    match find_element(&numbers, 30) {
        Some(index) => println!("  30 は index {} にある", index),
        None => println!("  見つからない"),
    }

    // if let
    if let Some(index) = find_element(&numbers, 100) {
        println!("  100 は index {} にある", index);
    } else {
        println!("  100 は見つからない");
    }

    // unwrap_or
    let index = find_element(&numbers, 999).unwrap_or(usize::MAX);
    println!("  unwrap_or: {}", index);

    // is_some / is_none
    println!("  30 exists: {}", find_element(&numbers, 30).is_some());
    println!("  99 exists: {}", find_element(&numbers, 99).is_some());

    println!();
}

/// ? 演算子
fn question_mark_operator() {
    println!("--- ? 演算子 ---");

    // ? 演算子でエラーを伝播
    fn read_username_from_file() -> Result<String, io::Error> {
        let mut file = File::open("/tmp/username.txt")?;  // エラーなら即リターン
        let mut username = String::new();
        file.read_to_string(&mut username)?;
        Ok(username)
    }

    // チェーンも可能
    fn read_username_short() -> Result<String, io::Error> {
        let mut username = String::new();
        File::open("/tmp/username.txt")?.read_to_string(&mut username)?;
        Ok(username)
    }

    // もっと短く
    fn read_username_shortest() -> Result<String, io::Error> {
        std::fs::read_to_string("/tmp/username.txt")
    }

    match read_username_from_file() {
        Ok(name) => println!("  Username: {}", name),
        Err(e) => println!("  ファイル読み込みエラー: {} (これは想定内)", e.kind()),
    }

    // Option でも ? が使える
    fn last_char_of_first_line(text: &str) -> Option<char> {
        text.lines().next()?.chars().last()
    }

    let text = "Hello\nWorld";
    println!("  最初の行の最後の文字: {:?}", last_char_of_first_line(text));
    println!("  空文字の場合: {:?}", last_char_of_first_line(""));

    println!();
}

/// カスタムエラー型
fn custom_errors() {
    println!("--- カスタムエラー型 ---");

    // 手動で定義
    #[derive(Debug)]
    enum AppError {
        NotFound(String),
        InvalidInput(String),
        IoError(io::Error),
    }

    impl std::fmt::Display for AppError {
        fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            match self {
                AppError::NotFound(msg) => write!(f, "Not found: {}", msg),
                AppError::InvalidInput(msg) => write!(f, "Invalid input: {}", msg),
                AppError::IoError(e) => write!(f, "IO error: {}", e),
            }
        }
    }

    impl std::error::Error for AppError {}

    // From トレイトで自動変換
    impl From<io::Error> for AppError {
        fn from(error: io::Error) -> Self {
            AppError::IoError(error)
        }
    }

    fn process_file(path: &str) -> Result<String, AppError> {
        if path.is_empty() {
            return Err(AppError::InvalidInput("path is empty".to_string()));
        }
        let content = std::fs::read_to_string(path)?;  // io::Error → AppError
        Ok(content)
    }

    match process_file("") {
        Ok(content) => println!("  Content: {}", content),
        Err(e) => println!("  Error: {}", e),
    }

    // thiserror クレートで簡潔に書ける (Cargo.toml に追加済み)
    // anyhow クレートでアプリケーションエラーを簡単に扱える

    println!();
}

/// コンビネータ
fn combinators() {
    println!("--- コンビネータ ---");

    // map: 成功時に値を変換
    let maybe_num: Option<i32> = Some(5);
    let doubled = maybe_num.map(|x| x * 2);
    println!("  map: {:?}", doubled);

    // and_then (flatMap): 成功時に別の Option/Result を返す
    fn square(x: i32) -> Option<i32> {
        Some(x * x)
    }
    let result = Some(3).and_then(square);
    println!("  and_then: {:?}", result);

    // map_err: エラーを変換
    let result: Result<i32, &str> = Err("error");
    let mapped = result.map_err(|e| format!("wrapped: {}", e));
    println!("  map_err: {:?}", mapped);

    // ok_or: Option を Result に変換
    let opt: Option<i32> = Some(42);
    let res: Result<i32, &str> = opt.ok_or("no value");
    println!("  ok_or: {:?}", res);

    // transpose: Option<Result> ↔ Result<Option>
    let opt_res: Option<Result<i32, &str>> = Some(Ok(42));
    let res_opt: Result<Option<i32>, &str> = opt_res.transpose();
    println!("  transpose: {:?}", res_opt);

    // collect: Iterator<Item=Result<T, E>> → Result<Vec<T>, E>
    let results: Vec<Result<i32, &str>> = vec![Ok(1), Ok(2), Ok(3)];
    let collected: Result<Vec<i32>, _> = results.into_iter().collect();
    println!("  collect (all ok): {:?}", collected);

    let results: Vec<Result<i32, &str>> = vec![Ok(1), Err("fail"), Ok(3)];
    let collected: Result<Vec<i32>, _> = results.into_iter().collect();
    println!("  collect (with err): {:?}", collected);

    println!();
}
