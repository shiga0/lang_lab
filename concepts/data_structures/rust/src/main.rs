//! データ構造 - Rust 実装
//!
//! Rust の標準ライブラリのデータ構造

use std::collections::{BinaryHeap, HashMap, HashSet, VecDeque};

fn main() {
    println!("=== Data Structures Demo ===\n");

    demo_vec();
    demo_vecdeque();
    demo_hashmap();
    demo_hashset();
    demo_binary_heap();
    demo_custom_struct();
}

/// Vec - 動的配列
fn demo_vec() {
    println!("--- Vec (動的配列) ---");

    let mut vec = Vec::new();
    vec.push(1);
    vec.push(2);
    vec.push(3);
    println!("push: {:?}", vec);

    // マクロで初期化
    let vec2 = vec![4, 5, 6];
    println!("vec!: {:?}", vec2);

    // インデックスアクセス
    println!("vec[0]: {}", vec[0]);

    // 安全なアクセス
    println!("vec.get(10): {:?}", vec.get(10));

    // イテレーション
    for x in &vec {
        print!("{} ", x);
    }
    println!("\n");
}

/// VecDeque - 両端キュー
fn demo_vecdeque() {
    println!("--- VecDeque (両端キュー) ---");

    let mut deque: VecDeque<i32> = VecDeque::new();

    // 両端から追加
    deque.push_back(2);
    deque.push_front(1);
    deque.push_back(3);
    println!("deque: {:?}", deque);

    // 両端から削除
    println!("pop_front: {:?}", deque.pop_front());
    println!("pop_back: {:?}", deque.pop_back());
    println!("after pop: {:?}\n", deque);
}

/// HashMap - ハッシュマップ
fn demo_hashmap() {
    println!("--- HashMap ---");

    let mut map: HashMap<String, i32> = HashMap::new();

    map.insert("apple".to_string(), 100);
    map.insert("banana".to_string(), 50);
    map.insert("cherry".to_string(), 75);
    println!("map: {:?}", map);

    // 取得
    println!("apple: {:?}", map.get("apple"));

    // 存在しないキー
    println!("grape: {:?}", map.get("grape"));

    // entry API
    map.entry("date".to_string()).or_insert(200);
    println!("after entry: {:?}", map);

    // イテレーション
    for (key, value) in &map {
        println!("  {}: {}", key, value);
    }
    println!();
}

/// HashSet - 集合
fn demo_hashset() {
    println!("--- HashSet ---");

    let mut set1: HashSet<i32> = HashSet::new();
    set1.insert(1);
    set1.insert(2);
    set1.insert(3);

    let set2: HashSet<i32> = [2, 3, 4].into_iter().collect();

    println!("set1: {:?}", set1);
    println!("set2: {:?}", set2);

    // 和集合
    let union: HashSet<_> = set1.union(&set2).collect();
    println!("union: {:?}", union);

    // 積集合
    let intersection: HashSet<_> = set1.intersection(&set2).collect();
    println!("intersection: {:?}", intersection);

    // 差集合
    let difference: HashSet<_> = set1.difference(&set2).collect();
    println!("difference: {:?}\n", difference);
}

/// BinaryHeap - 優先度キュー (最大ヒープ)
fn demo_binary_heap() {
    println!("--- BinaryHeap (優先度キュー) ---");

    let mut heap = BinaryHeap::new();
    heap.push(3);
    heap.push(1);
    heap.push(4);
    heap.push(1);
    heap.push(5);

    println!("heap: {:?}", heap);
    println!("peek (max): {:?}", heap.peek());

    // 降順で取り出し
    print!("pop order: ");
    while let Some(val) = heap.pop() {
        print!("{} ", val);
    }
    println!("\n");
}

/// カスタム構造体
fn demo_custom_struct() {
    println!("--- Custom Struct ---");

    // スタック
    let mut stack: Stack<i32> = Stack::new();
    stack.push(1);
    stack.push(2);
    stack.push(3);
    println!("stack: {:?}", stack);
    println!("pop: {:?}", stack.pop());
    println!("peek: {:?}", stack.peek());

    // キュー
    let mut queue: Queue<i32> = Queue::new();
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);
    println!("\nqueue: {:?}", queue);
    println!("dequeue: {:?}", queue.dequeue());
    println!("front: {:?}", queue.front());
}

/// スタック (LIFO)
#[derive(Debug)]
struct Stack<T> {
    items: Vec<T>,
}

impl<T> Stack<T> {
    fn new() -> Self {
        Stack { items: Vec::new() }
    }

    fn push(&mut self, item: T) {
        self.items.push(item);
    }

    fn pop(&mut self) -> Option<T> {
        self.items.pop()
    }

    fn peek(&self) -> Option<&T> {
        self.items.last()
    }

    fn is_empty(&self) -> bool {
        self.items.is_empty()
    }

    fn len(&self) -> usize {
        self.items.len()
    }
}

/// キュー (FIFO)
#[derive(Debug)]
struct Queue<T> {
    items: VecDeque<T>,
}

impl<T> Queue<T> {
    fn new() -> Self {
        Queue {
            items: VecDeque::new(),
        }
    }

    fn enqueue(&mut self, item: T) {
        self.items.push_back(item);
    }

    fn dequeue(&mut self) -> Option<T> {
        self.items.pop_front()
    }

    fn front(&self) -> Option<&T> {
        self.items.front()
    }

    fn is_empty(&self) -> bool {
        self.items.is_empty()
    }

    fn len(&self) -> usize {
        self.items.len()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_stack() {
        let mut stack = Stack::new();
        assert!(stack.is_empty());

        stack.push(1);
        stack.push(2);
        stack.push(3);

        assert_eq!(stack.len(), 3);
        assert_eq!(stack.peek(), Some(&3));
        assert_eq!(stack.pop(), Some(3));
        assert_eq!(stack.pop(), Some(2));
        assert_eq!(stack.len(), 1);
    }

    #[test]
    fn test_queue() {
        let mut queue = Queue::new();
        assert!(queue.is_empty());

        queue.enqueue(1);
        queue.enqueue(2);
        queue.enqueue(3);

        assert_eq!(queue.len(), 3);
        assert_eq!(queue.front(), Some(&1));
        assert_eq!(queue.dequeue(), Some(1));
        assert_eq!(queue.dequeue(), Some(2));
        assert_eq!(queue.len(), 1);
    }
}
