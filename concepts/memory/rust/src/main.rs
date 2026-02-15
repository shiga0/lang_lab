//! # Rust のメモリ管理: 所有権システム
//!
//! Rust はGCなしでメモリ安全性を保証する。
//! 所有権・借用・ライフタイムの3つの概念がその基盤。

fn main() {
    println!("=== Rust メモリ管理 ===\n");

    ownership_basics();
    move_semantics();
    borrowing();
    mutable_borrowing();
    lifetimes();
    smart_pointers();
}

/// 所有権の基本
fn ownership_basics() {
    println!("--- 所有権の基本 ---");

    // スタック上のデータ (Copy トレイト実装型)
    let x = 5;
    let y = x;  // コピー
    println!("  x = {}, y = {} (整数はコピーされる)", x, y);

    // ヒープ上のデータ
    let s1 = String::from("hello");
    let s2 = s1;  // 所有権の移動 (move)
    // println!("{}", s1);  // エラー: s1 はもう無効
    println!("  s2 = {} (String は move される)", s2);

    // Clone で明示的にコピー
    let s3 = String::from("world");
    let s4 = s3.clone();
    println!("  s3 = {}, s4 = {} (clone で両方有効)", s3, s4);

    println!();
}

/// Move セマンティクス
fn move_semantics() {
    println!("--- Move セマンティクス ---");

    fn take_ownership(s: String) {
        println!("  関数内で受け取った: {}", s);
        // 関数終了時に s はドロップ
    }

    fn give_ownership() -> String {
        let s = String::from("yours");
        s  // 所有権を返す
    }

    fn take_and_give_back(s: String) -> String {
        s  // 所有権を受け取って返す
    }

    let s1 = String::from("hello");
    take_ownership(s1);
    // println!("{}", s1);  // エラー: s1 は move された

    let s2 = give_ownership();
    println!("  受け取った所有権: {}", s2);

    let s3 = String::from("temp");
    let s4 = take_and_give_back(s3);
    println!("  返された所有権: {}", s4);

    println!();
}

/// 借用 (不変参照)
fn borrowing() {
    println!("--- 借用 (不変参照) ---");

    fn calculate_length(s: &String) -> usize {
        s.len()
        // s は借用しているだけなのでドロップされない
    }

    let s = String::from("hello");
    let len = calculate_length(&s);  // 参照を渡す
    println!("  '{}' の長さ: {}", s, len);  // s はまだ使える

    // 複数の不変参照は OK
    let r1 = &s;
    let r2 = &s;
    println!("  r1 = {}, r2 = {}", r1, r2);

    println!();
}

/// 可変借用
fn mutable_borrowing() {
    println!("--- 可変借用 ---");

    fn append_world(s: &mut String) {
        s.push_str(" world");
    }

    let mut s = String::from("hello");
    append_world(&mut s);
    println!("  変更後: {}", s);

    // 可変参照は1つだけ
    let r1 = &mut s;
    // let r2 = &mut s;  // エラー: 同時に2つの可変参照は不可
    r1.push_str("!");
    println!("  さらに変更: {}", r1);

    // 不変と可変の同時参照は不可
    let mut s2 = String::from("test");
    let r3 = &s2;      // 不変参照
    // let r4 = &mut s2;  // エラー: 不変参照がある間は可変参照不可
    println!("  r3 = {}", r3);
    // r3 のスコープが終わったので可変参照可能
    let r5 = &mut s2;
    r5.push_str("!");
    println!("  r5 = {}", r5);

    println!();
}

/// ライフタイム
fn lifetimes() {
    println!("--- ライフタイム ---");

    // 明示的なライフタイム注釈
    fn longest<'a>(x: &'a str, y: &'a str) -> &'a str {
        if x.len() > y.len() { x } else { y }
    }

    let s1 = String::from("long string");
    let s2 = String::from("short");
    let result = longest(&s1, &s2);
    println!("  longest: {}", result);

    // 構造体とライフタイム
    #[derive(Debug)]
    struct ImportantExcerpt<'a> {
        part: &'a str,
    }

    let novel = String::from("Call me Ishmael. Some years ago...");
    let first_sentence = novel.split('.').next().unwrap();
    let excerpt = ImportantExcerpt { part: first_sentence };
    println!("  excerpt: {:?}", excerpt);

    // 'static ライフタイム
    let s: &'static str = "I have a static lifetime.";
    println!("  static: {}", s);

    println!();
}

/// スマートポインタ
fn smart_pointers() {
    println!("--- スマートポインタ ---");

    // Box<T>: ヒープ上にデータを置く
    let b = Box::new(5);
    println!("  Box: {}", b);

    // 再帰的な型に必須
    #[derive(Debug)]
    enum List {
        Cons(i32, Box<List>),
        Nil,
    }
    use List::{Cons, Nil};

    let list = Cons(1, Box::new(Cons(2, Box::new(Cons(3, Box::new(Nil))))));
    println!("  List: {:?}", list);

    // Rc<T>: 参照カウント（単一スレッド）
    use std::rc::Rc;
    let a = Rc::new(String::from("shared"));
    let b = Rc::clone(&a);
    let c = Rc::clone(&a);
    println!("  Rc count: {} (a, b, c が共有)", Rc::strong_count(&a));

    // RefCell<T>: 実行時借用チェック
    use std::cell::RefCell;
    let data = RefCell::new(5);
    *data.borrow_mut() += 1;  // 実行時に借用ルールをチェック
    println!("  RefCell: {}", data.borrow());

    // Arc<T>: スレッド安全な参照カウント
    use std::sync::Arc;
    let arc = Arc::new(vec![1, 2, 3]);
    let arc_clone = Arc::clone(&arc);
    println!("  Arc: {:?}", arc_clone);

    println!();
}
