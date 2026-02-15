//! # Rust の並行処理
//!
//! Rust は「恐れなき並行性」を実現。
//! 型システムと所有権により、データ競合をコンパイル時に防ぐ。

use std::sync::{Arc, Mutex, mpsc};
use std::thread;
use std::time::Duration;

fn main() {
    println!("=== Rust 並行処理 ===\n");

    basic_threads();
    move_closure();
    shared_state();
    message_passing();

    // async は別途 tokio ランタイムが必要
    println!("--- async/await (tokio) ---");
    println!("  tokio::main で非同期処理を実行");
    println!("  (この例では同期版のみ)\n");
}

/// 基本的なスレッド
fn basic_threads() {
    println!("--- 基本的なスレッド ---");

    // スレッドを生成
    let handle = thread::spawn(|| {
        for i in 1..=3 {
            println!("  [子スレッド] カウント: {}", i);
            thread::sleep(Duration::from_millis(10));
        }
    });

    // メインスレッドでも作業
    for i in 1..=2 {
        println!("  [メインスレッド] カウント: {}", i);
        thread::sleep(Duration::from_millis(10));
    }

    // スレッドの終了を待つ
    handle.join().unwrap();
    println!("  両方のスレッドが完了\n");
}

/// move クロージャ
fn move_closure() {
    println!("--- move クロージャ ---");

    let v = vec![1, 2, 3];

    // move でベクタの所有権をスレッドに移動
    let handle = thread::spawn(move || {
        println!("  スレッド内でベクタを使用: {:?}", v);
    });

    // println!("{:?}", v);  // エラー: v は move された

    handle.join().unwrap();
    println!();
}

/// 共有状態 (Mutex + Arc)
fn shared_state() {
    println!("--- 共有状態 (Mutex + Arc) ---");

    // Arc: 複数スレッドで所有権を共有
    // Mutex: 排他的アクセスを保証
    let counter = Arc::new(Mutex::new(0));
    let mut handles = vec![];

    for i in 0..5 {
        let counter = Arc::clone(&counter);
        let handle = thread::spawn(move || {
            let mut num = counter.lock().unwrap();
            *num += 1;
            println!("  スレッド {} がカウンターをインクリメント", i);
        });
        handles.push(handle);
    }

    for handle in handles {
        handle.join().unwrap();
    }

    println!("  最終カウント: {}", *counter.lock().unwrap());
    println!();
}

/// メッセージパッシング (チャネル)
fn message_passing() {
    println!("--- メッセージパッシング (チャネル) ---");

    // mpsc: multiple producer, single consumer
    let (tx, rx) = mpsc::channel();

    // 複数の送信者
    let tx1 = tx.clone();
    thread::spawn(move || {
        let messages = vec!["hello", "from", "thread1"];
        for msg in messages {
            tx1.send(format!("[T1] {}", msg)).unwrap();
            thread::sleep(Duration::from_millis(10));
        }
    });

    thread::spawn(move || {
        let messages = vec!["more", "from", "thread2"];
        for msg in messages {
            tx.send(format!("[T2] {}", msg)).unwrap();
            thread::sleep(Duration::from_millis(10));
        }
    });

    // 受信側でイテレート
    for received in rx {
        println!("  受信: {}", received);
    }

    println!();
}

// ============================================================
// 以下は async/await の例 (tokio が必要)
// ============================================================

/*
#[tokio::main]
async fn async_example() {
    println!("--- async/await ---");

    // 非同期関数
    async fn fetch_data(id: u32) -> String {
        // 非同期的に待機
        tokio::time::sleep(Duration::from_millis(100)).await;
        format!("Data for {}", id)
    }

    // 並列実行
    let (result1, result2) = tokio::join!(
        fetch_data(1),
        fetch_data(2)
    );

    println!("  result1: {}", result1);
    println!("  result2: {}", result2);

    // select! で最初に完了したものを取得
    tokio::select! {
        val = fetch_data(3) => println!("  first: {}", val),
        val = fetch_data(4) => println!("  first: {}", val),
    }
}
*/
