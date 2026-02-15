//! Linked List - Rust 実装
//!
//! Rust での連結リストは所有権の良い練習になる

use std::fmt::Debug;

fn main() {
    println!("=== Linked List Demo ===\n");

    let mut list: LinkedList<i32> = LinkedList::new();

    println!("push_front(1), push_front(2), push_front(3)");
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    println!("list: {:?}", list);
    println!("len: {}", list.len());

    println!("\npush_back(10)");
    list.push_back(10);
    println!("list: {:?}", list);

    println!("\npop_front(): {:?}", list.pop_front());
    println!("list: {:?}", list);

    println!("\n--- Iteration ---");
    for item in list.iter() {
        println!("  {}", item);
    }
}

/// 連結リストのノード
struct Node<T> {
    value: T,
    next: Option<Box<Node<T>>>,
}

/// 単方向連結リスト
pub struct LinkedList<T> {
    head: Option<Box<Node<T>>>,
    len: usize,
}

impl<T> LinkedList<T> {
    /// 新しい空のリストを作成
    pub fn new() -> Self {
        LinkedList { head: None, len: 0 }
    }

    /// リストの長さを返す
    pub fn len(&self) -> usize {
        self.len
    }

    /// リストが空かどうか
    pub fn is_empty(&self) -> bool {
        self.len == 0
    }

    /// 先頭に要素を追加
    pub fn push_front(&mut self, value: T) {
        let new_node = Box::new(Node {
            value,
            next: self.head.take(),  // 現在の head を新ノードの next に
        });
        self.head = Some(new_node);
        self.len += 1;
    }

    /// 末尾に要素を追加
    pub fn push_back(&mut self, value: T) {
        let new_node = Box::new(Node { value, next: None });

        // 末尾を探す
        let mut current = &mut self.head;
        while let Some(ref mut node) = current {
            current = &mut node.next;
        }

        *current = Some(new_node);
        self.len += 1;
    }

    /// 先頭の要素を削除して返す
    pub fn pop_front(&mut self) -> Option<T> {
        self.head.take().map(|node| {
            self.head = node.next;
            self.len -= 1;
            node.value
        })
    }

    /// イテレータを返す
    pub fn iter(&self) -> Iter<T> {
        Iter {
            current: self.head.as_deref(),
        }
    }
}

impl<T> Default for LinkedList<T> {
    fn default() -> Self {
        Self::new()
    }
}

/// イテレータ
pub struct Iter<'a, T> {
    current: Option<&'a Node<T>>,
}

impl<'a, T> Iterator for Iter<'a, T> {
    type Item = &'a T;

    fn next(&mut self) -> Option<Self::Item> {
        self.current.map(|node| {
            self.current = node.next.as_deref();
            &node.value
        })
    }
}

impl<T: Debug> Debug for LinkedList<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "[")?;
        let mut first = true;
        for item in self.iter() {
            if !first {
                write!(f, ", ")?;
            }
            write!(f, "{:?}", item)?;
            first = false;
        }
        write!(f, "]")
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_push_front() {
        let mut list = LinkedList::new();
        list.push_front(1);
        list.push_front(2);
        list.push_front(3);

        let items: Vec<_> = list.iter().collect();
        assert_eq!(items, vec![&3, &2, &1]);
    }

    #[test]
    fn test_push_back() {
        let mut list = LinkedList::new();
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);

        let items: Vec<_> = list.iter().collect();
        assert_eq!(items, vec![&1, &2, &3]);
    }

    #[test]
    fn test_pop_front() {
        let mut list = LinkedList::new();
        list.push_front(1);
        list.push_front(2);

        assert_eq!(list.pop_front(), Some(2));
        assert_eq!(list.pop_front(), Some(1));
        assert_eq!(list.pop_front(), None);
    }

    #[test]
    fn test_len() {
        let mut list = LinkedList::new();
        assert_eq!(list.len(), 0);
        assert!(list.is_empty());

        list.push_front(1);
        list.push_front(2);
        assert_eq!(list.len(), 2);
        assert!(!list.is_empty());
    }
}
