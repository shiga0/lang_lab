//! # Rust のオブジェクト指向
//!
//! Rust は古典的なOOPではなく、Trait ベースの多態性を採用。
//! 「継承より合成」の原則を言語レベルでサポート。

fn main() {
    println!("=== Rust OOP ===\n");

    struct_and_impl();
    traits_example();
    trait_objects();
    composition_over_inheritance();
    associated_types();
}

/// 構造体と impl
fn struct_and_impl() {
    println!("--- 構造体と impl ---");

    #[derive(Debug)]
    struct Rectangle {
        width: u32,
        height: u32,
    }

    impl Rectangle {
        // 関連関数 (コンストラクタ)
        fn new(width: u32, height: u32) -> Self {
            Self { width, height }
        }

        fn square(size: u32) -> Self {
            Self { width: size, height: size }
        }

        // メソッド (&self を取る)
        fn area(&self) -> u32 {
            self.width * self.height
        }

        fn can_hold(&self, other: &Rectangle) -> bool {
            self.width > other.width && self.height > other.height
        }

        // 可変メソッド (&mut self)
        fn resize(&mut self, width: u32, height: u32) {
            self.width = width;
            self.height = height;
        }
    }

    let rect1 = Rectangle::new(30, 50);
    let rect2 = Rectangle::square(25);

    println!("  rect1: {:?}", rect1);
    println!("  rect1.area(): {}", rect1.area());
    println!("  rect1.can_hold(&rect2): {}", rect1.can_hold(&rect2));

    let mut rect3 = Rectangle::new(10, 20);
    rect3.resize(100, 200);
    println!("  rect3 after resize: {:?}", rect3);
    println!();
}

/// Trait (インターフェース)
fn traits_example() {
    println!("--- Trait ---");

    // Trait 定義
    trait Summary {
        fn summarize(&self) -> String;

        // デフォルト実装
        fn preview(&self) -> String {
            format!("({}...)", &self.summarize()[..10.min(self.summarize().len())])
        }
    }

    struct NewsArticle {
        headline: String,
        location: String,
        author: String,
    }

    impl Summary for NewsArticle {
        fn summarize(&self) -> String {
            format!("{}, by {} ({})", self.headline, self.author, self.location)
        }
    }

    struct Tweet {
        username: String,
        content: String,
    }

    impl Summary for Tweet {
        fn summarize(&self) -> String {
            format!("@{}: {}", self.username, self.content)
        }
    }

    let article = NewsArticle {
        headline: "Rust is awesome".to_string(),
        location: "Tokyo".to_string(),
        author: "John".to_string(),
    };

    let tweet = Tweet {
        username: "rustacean".to_string(),
        content: "Hello, world!".to_string(),
    };

    println!("  article: {}", article.summarize());
    println!("  tweet: {}", tweet.summarize());

    // ジェネリクスで Trait 境界
    fn notify<T: Summary>(item: &T) {
        println!("  [notify] {}", item.summarize());
    }

    notify(&article);
    notify(&tweet);
    println!();
}

/// Trait オブジェクト (動的ディスパッチ)
fn trait_objects() {
    println!("--- Trait オブジェクト (dyn) ---");

    trait Draw {
        fn draw(&self);
    }

    struct Circle {
        radius: u32,
    }

    impl Draw for Circle {
        fn draw(&self) {
            println!("    Drawing circle with radius {}", self.radius);
        }
    }

    struct Square {
        side: u32,
    }

    impl Draw for Square {
        fn draw(&self) {
            println!("    Drawing square with side {}", self.side);
        }
    }

    // Trait オブジェクトのベクタ (異なる型を格納)
    let shapes: Vec<Box<dyn Draw>> = vec![
        Box::new(Circle { radius: 10 }),
        Box::new(Square { side: 5 }),
        Box::new(Circle { radius: 20 }),
    ];

    println!("  Drawing all shapes:");
    for shape in shapes {
        shape.draw();  // 動的ディスパッチ
    }
    println!();
}

/// 継承より合成 (Composition over Inheritance)
fn composition_over_inheritance() {
    println!("--- 合成 (Composition) ---");

    // ベースとなる構造体
    struct Engine {
        horsepower: u32,
    }

    impl Engine {
        fn start(&self) {
            println!("    Engine started ({} HP)", self.horsepower);
        }
    }

    struct Wheels {
        count: u8,
    }

    impl Wheels {
        fn roll(&self) {
            println!("    {} wheels rolling", self.count);
        }
    }

    // 合成で車を作る (継承ではない)
    struct Car {
        engine: Engine,
        wheels: Wheels,
        brand: String,
    }

    impl Car {
        fn new(brand: &str, hp: u32) -> Self {
            Self {
                engine: Engine { horsepower: hp },
                wheels: Wheels { count: 4 },
                brand: brand.to_string(),
            }
        }

        fn drive(&self) {
            println!("  {} is driving:", self.brand);
            self.engine.start();
            self.wheels.roll();
        }
    }

    let car = Car::new("Toyota", 150);
    car.drive();
    println!();
}

/// 関連型 (Associated Types)
fn associated_types() {
    println!("--- 関連型 ---");

    trait Container {
        type Item;  // 関連型

        fn add(&mut self, item: Self::Item);
        fn get(&self) -> Option<&Self::Item>;
    }

    struct Stack<T> {
        items: Vec<T>,
    }

    impl<T> Container for Stack<T> {
        type Item = T;

        fn add(&mut self, item: Self::Item) {
            self.items.push(item);
        }

        fn get(&self) -> Option<&Self::Item> {
            self.items.last()
        }
    }

    let mut stack: Stack<i32> = Stack { items: vec![] };
    stack.add(1);
    stack.add(2);
    stack.add(3);
    println!("  stack top: {:?}", stack.get());
    println!();
}
