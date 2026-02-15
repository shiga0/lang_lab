//! # Rust の関数型プログラミング
//!
//! Rust は命令型と関数型のハイブリッド。
//! イミュータブル・イテレータ・パターンマッチングなど FP の概念を多く採用。

fn main() {
    println!("=== Rust 関数型プログラミング ===\n");

    closures();
    iterators();
    combinators();
    pattern_matching();
    algebraic_data_types();
    higher_order_functions();
}

/// クロージャ
fn closures() {
    println!("--- クロージャ ---");

    // 基本的なクロージャ
    let add = |a, b| a + b;
    let double = |x| x * 2;
    println!("  add(1, 2) = {}", add(1, 2));
    println!("  double(5) = {}", double(5));

    // 環境をキャプチャ
    let factor = 10;
    let multiply = |x| x * factor;  // factor をキャプチャ
    println!("  multiply(5) = {}", multiply(5));

    // move クロージャ (所有権を移動)
    let s = String::from("hello");
    let print_s = move || println!("  captured: {}", s);
    print_s();
    // println!("{}", s);  // エラー: s は move された

    // 型注釈付きクロージャ
    let typed_add: fn(i32, i32) -> i32 = |a, b| a + b;
    println!("  typed_add(10, 20) = {}", typed_add(10, 20));

    println!();
}

/// イテレータ
fn iterators() {
    println!("--- イテレータ ---");

    let numbers = vec![1, 2, 3, 4, 5];

    // iter() は参照のイテレータ
    print!("  iter: ");
    for n in numbers.iter() {
        print!("{} ", n);
    }
    println!();

    // into_iter() は所有権を取る
    let numbers2 = vec![1, 2, 3];
    print!("  into_iter: ");
    for n in numbers2.into_iter() {
        print!("{} ", n);
    }
    println!();
    // numbers2 はもう使えない

    // iter_mut() は可変参照
    let mut numbers3 = vec![1, 2, 3];
    for n in numbers3.iter_mut() {
        *n *= 2;
    }
    println!("  iter_mut result: {:?}", numbers3);

    // 無限イテレータ
    let first_five: Vec<u32> = (1..).take(5).collect();
    println!("  infinite (1..): {:?}", first_five);

    // 遅延評価
    let sum: i32 = (1..=100)
        .filter(|x| x % 2 == 0)  // まだ計算されない
        .take(5)                  // まだ計算されない
        .sum();                   // ここで初めて計算
    println!("  lazy sum: {}", sum);

    println!();
}

/// コンビネータ
fn combinators() {
    println!("--- コンビネータ ---");

    let numbers = vec![1, 2, 3, 4, 5, 6, 7, 8, 9, 10];

    // map: 各要素を変換
    let doubled: Vec<_> = numbers.iter().map(|x| x * 2).collect();
    println!("  map (doubled): {:?}", doubled);

    // filter: 条件に合う要素のみ
    let evens: Vec<_> = numbers.iter().filter(|x| *x % 2 == 0).collect();
    println!("  filter (evens): {:?}", evens);

    // fold (reduce): 畳み込み
    let sum = numbers.iter().fold(0, |acc, x| acc + x);
    println!("  fold (sum): {}", sum);

    // find: 最初に条件を満たす要素
    let first_even = numbers.iter().find(|x| *x % 2 == 0);
    println!("  find (first even): {:?}", first_even);

    // any / all
    let has_five = numbers.iter().any(|x| *x == 5);
    let all_positive = numbers.iter().all(|x| *x > 0);
    println!("  any(5): {}, all(>0): {}", has_five, all_positive);

    // チェーン
    let result: Vec<_> = numbers
        .iter()
        .filter(|x| *x % 2 == 0)
        .map(|x| x * x)
        .take(3)
        .collect();
    println!("  chain: {:?}", result);

    // flat_map
    let nested = vec![vec![1, 2], vec![3, 4], vec![5, 6]];
    let flattened: Vec<_> = nested.iter().flat_map(|v| v.iter()).collect();
    println!("  flat_map: {:?}", flattened);

    // zip
    let a = vec![1, 2, 3];
    let b = vec!["one", "two", "three"];
    let zipped: Vec<_> = a.iter().zip(b.iter()).collect();
    println!("  zip: {:?}", zipped);

    // enumerate
    let indexed: Vec<_> = numbers.iter().enumerate().take(3).collect();
    println!("  enumerate: {:?}", indexed);

    println!();
}

/// パターンマッチング
fn pattern_matching() {
    println!("--- パターンマッチング ---");

    // 基本的な match
    let number = 13;
    match number {
        1 => println!("  one"),
        2 | 3 | 5 | 7 | 11 | 13 => println!("  prime: {}", number),
        13..=19 => println!("  teen"),
        _ => println!("  other"),
    }

    // 分解
    let point = (3, 4);
    match point {
        (0, 0) => println!("  origin"),
        (x, 0) => println!("  on x-axis at {}", x),
        (0, y) => println!("  on y-axis at {}", y),
        (x, y) => println!("  at ({}, {})", x, y),
    }

    // ガード
    let pair = (2, -2);
    match pair {
        (x, y) if x == y => println!("  equal"),
        (x, y) if x + y == 0 => println!("  sum is zero"),
        (x, _) if x % 2 == 1 => println!("  x is odd"),
        _ => println!("  no match"),
    }

    // @ バインディング
    let message = "hello";
    match message.len() {
        n @ 1..=5 => println!("  short (len={})", n),
        n @ 6..=10 => println!("  medium (len={})", n),
        n => println!("  long (len={})", n),
    }

    // if let
    let config_max = Some(3u8);
    if let Some(max) = config_max {
        println!("  max is {}", max);
    }

    // while let
    let mut stack = vec![1, 2, 3];
    print!("  while let: ");
    while let Some(top) = stack.pop() {
        print!("{} ", top);
    }
    println!();

    println!();
}

/// 代数的データ型 (Enum)
fn algebraic_data_types() {
    println!("--- 代数的データ型 ---");

    // Sum type (直和型)
    #[derive(Debug)]
    enum Shape {
        Circle { radius: f64 },
        Rectangle { width: f64, height: f64 },
        Triangle { base: f64, height: f64 },
    }

    impl Shape {
        fn area(&self) -> f64 {
            match self {
                Shape::Circle { radius } => std::f64::consts::PI * radius * radius,
                Shape::Rectangle { width, height } => width * height,
                Shape::Triangle { base, height } => 0.5 * base * height,
            }
        }
    }

    let shapes = vec![
        Shape::Circle { radius: 2.0 },
        Shape::Rectangle { width: 3.0, height: 4.0 },
        Shape::Triangle { base: 5.0, height: 6.0 },
    ];

    for shape in &shapes {
        println!("  {:?} area: {:.2}", shape, shape.area());
    }

    // 再帰的なデータ型
    #[derive(Debug)]
    enum List<T> {
        Nil,
        Cons(T, Box<List<T>>),
    }

    let list = List::Cons(1, Box::new(List::Cons(2, Box::new(List::Cons(3, Box::new(List::Nil))))));
    println!("  recursive list: {:?}", list);

    println!();
}

/// 高階関数
fn higher_order_functions() {
    println!("--- 高階関数 ---");

    // 関数を引数に取る
    fn apply<F>(f: F, x: i32) -> i32
    where
        F: Fn(i32) -> i32,
    {
        f(x)
    }

    let double = |x| x * 2;
    let result = apply(double, 5);
    println!("  apply(double, 5) = {}", result);

    // 関数を返す
    fn make_adder(n: i32) -> impl Fn(i32) -> i32 {
        move |x| x + n
    }

    let add_ten = make_adder(10);
    println!("  add_ten(5) = {}", add_ten(5));

    // カリー化的な
    fn curry_add(a: i32) -> impl Fn(i32) -> i32 {
        move |b| a + b
    }

    let add_five = curry_add(5);
    println!("  curry_add(5)(3) = {}", add_five(3));

    // 関数合成
    fn compose<F, G, A, B, C>(f: F, g: G) -> impl Fn(A) -> C
    where
        F: Fn(A) -> B,
        G: Fn(B) -> C,
    {
        move |x| g(f(x))
    }

    let double = |x: i32| x * 2;
    let increment = |x: i32| x + 1;
    let double_then_increment = compose(double, increment);
    println!("  compose(double, increment)(5) = {}", double_then_increment(5));

    println!();
}
