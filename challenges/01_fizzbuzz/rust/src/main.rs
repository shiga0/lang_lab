//! FizzBuzz - Rust 実装
//!
//! 複数のアプローチを示す

fn main() {
    println!("=== 基本版 ===");
    fizzbuzz_basic(15);

    println!("\n=== match 版 ===");
    fizzbuzz_match(15);

    println!("\n=== イテレータ版 ===");
    fizzbuzz_iterator(15);

    println!("\n=== 拡張版 (カスタムルール) ===");
    fizzbuzz_extended(20);
}

/// 基本的な実装
fn fizzbuzz_basic(n: u32) {
    for i in 1..=n {
        if i % 15 == 0 {
            println!("FizzBuzz");
        } else if i % 3 == 0 {
            println!("Fizz");
        } else if i % 5 == 0 {
            println!("Buzz");
        } else {
            println!("{}", i);
        }
    }
}

/// match を使った実装
fn fizzbuzz_match(n: u32) {
    for i in 1..=n {
        match (i % 3, i % 5) {
            (0, 0) => println!("FizzBuzz"),
            (0, _) => println!("Fizz"),
            (_, 0) => println!("Buzz"),
            _ => println!("{}", i),
        }
    }
}

/// イテレータを使った実装
fn fizzbuzz_iterator(n: u32) {
    (1..=n)
        .map(|i| match (i % 3, i % 5) {
            (0, 0) => "FizzBuzz".to_string(),
            (0, _) => "Fizz".to_string(),
            (_, 0) => "Buzz".to_string(),
            _ => i.to_string(),
        })
        .for_each(|s| println!("{}", s));
}

/// 拡張版: カスタムルールに対応
fn fizzbuzz_extended(n: u32) {
    let rules: Vec<(u32, &str)> = vec![
        (3, "Fizz"),
        (5, "Buzz"),
        (7, "Bazz"),
    ];

    for i in 1..=n {
        let result: String = rules
            .iter()
            .filter(|(divisor, _)| i % divisor == 0)
            .map(|(_, word)| *word)
            .collect();

        if result.is_empty() {
            println!("{}", i);
        } else {
            println!("{}", result);
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_fizzbuzz_values() {
        let result: Vec<String> = (1..=15)
            .map(|i| match (i % 3, i % 5) {
                (0, 0) => "FizzBuzz".to_string(),
                (0, _) => "Fizz".to_string(),
                (_, 0) => "Buzz".to_string(),
                _ => i.to_string(),
            })
            .collect();

        assert_eq!(result[0], "1");
        assert_eq!(result[2], "Fizz");
        assert_eq!(result[4], "Buzz");
        assert_eq!(result[14], "FizzBuzz");
    }
}
