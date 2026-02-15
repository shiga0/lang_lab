//! メタプログラミング - Rust 実装
//!
//! Rust のマクロと derive

// === マクロ定義 (使用前に定義が必要) ===

/// 単純な加算マクロ
macro_rules! add {
    ($a:expr, $b:expr) => {
        $a + $b
    };
}

/// 可変引数の合計マクロ
macro_rules! sum {
    ($($x:expr),*) => {
        {
            let mut total = 0;
            $(
                total += $x;
            )*
            total
        }
    };
}

/// 関数を作成するマクロ
macro_rules! create_function {
    ($name:ident) => {
        fn $name() {
            println!("Function {} called", stringify!($name));
        }
    };
}

/// 複数パターンのマクロ
macro_rules! say {
    ($msg:expr) => {
        println!("say: {}", $msg);
    };
    ($prefix:expr, $msg:expr) => {
        println!("{}: {}", $prefix, $msg);
    };
}

/// HashMap を作成するマクロ
macro_rules! hashmap {
    ($($key:expr => $value:expr),* $(,)?) => {
        {
            let mut map = std::collections::HashMap::new();
            $(
                map.insert($key, $value);
            )*
            map
        }
    };
}

/// 構造体を定義するマクロ
macro_rules! new_struct {
    ($name:ident, $($field:ident: $type:ty),* $(,)?) => {
        struct $name {
            $($field: $type),*
        }
    };
}

// === メイン関数 ===

fn main() {
    println!("=== Metaprogramming Demo ===\n");

    demo_declarative_macros();
    demo_derive_macros();
    demo_custom_derive();
    demo_macro_rules();
}

/// 宣言的マクロ (macro_rules!)
fn demo_declarative_macros() {
    println!("--- Declarative Macros ---");

    // vec! マクロの使用
    let v = vec![1, 2, 3];
    println!("vec!: {:?}", v);

    // println! マクロ
    println!("println! with args: {} + {} = {}", 1, 2, 3);

    // カスタムマクロ
    let result = add!(1, 2);
    println!("add!(1, 2): {}", result);

    // 可変引数マクロ
    let total = sum!(1, 2, 3, 4, 5);
    println!("sum!(1, 2, 3, 4, 5): {}", total);

    println!();
}

/// derive マクロ
fn demo_derive_macros() {
    println!("--- Derive Macros ---");

    // Debug derive
    let point = Point { x: 10, y: 20 };
    println!("Debug: {:?}", point);

    // Clone derive
    let point2 = point.clone();
    println!("Cloned: {:?}", point2);

    // PartialEq derive
    println!("Equal: {}", point == point2);

    // Default derive
    let default_point: Point = Default::default();
    println!("Default: {:?}", default_point);

    println!();
}

#[derive(Debug, Clone, PartialEq, Default)]
struct Point {
    x: i32,
    y: i32,
}

/// カスタム derive 的なパターン (トレイト実装)
fn demo_custom_derive() {
    println!("--- Custom Trait Implementation ---");

    let user = User {
        name: "Alice".to_string(),
        age: 30,
    };

    // Display トレイト
    println!("Display: {}", user);

    // Describe トレイト
    println!("Describe: {}", user.describe());

    let product = Product {
        name: "Laptop".to_string(),
        price: 999.99,
    };
    println!("Describe: {}", product.describe());

    println!();
}

struct User {
    name: String,
    age: u32,
}

impl std::fmt::Display for User {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{} (age: {})", self.name, self.age)
    }
}

/// カスタムトレイト
trait Describe {
    fn describe(&self) -> String;
}

impl Describe for User {
    fn describe(&self) -> String {
        format!("User: {} is {} years old", self.name, self.age)
    }
}

struct Product {
    name: String,
    price: f64,
}

impl Describe for Product {
    fn describe(&self) -> String {
        format!("Product: {} costs ${:.2}", self.name, self.price)
    }
}

/// macro_rules! パターン
fn demo_macro_rules() {
    println!("--- macro_rules! Patterns ---");

    // リテラルパターン
    create_function!(foo);
    foo();

    // 複数パターン
    say!("hello");
    say!("greeting", "world");

    // 繰り返しパターン
    let map = hashmap! {
        "one" => 1,
        "two" => 2,
        "three" => 3
    };
    println!("hashmap!: {:?}", map);

    // 識別子パターン
    new_struct!(Person, name: String, age: u32);
    let person = Person {
        name: "Bob".to_string(),
        age: 25,
    };
    println!("new_struct! Person: {} is {} years old", person.name, person.age);

    println!();
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_add_macro() {
        assert_eq!(add!(2, 3), 5);
        assert_eq!(add!(10, 20), 30);
    }

    #[test]
    fn test_sum_macro() {
        assert_eq!(sum!(1, 2, 3), 6);
        assert_eq!(sum!(10, 20, 30, 40), 100);
    }

    #[test]
    fn test_hashmap_macro() {
        let map = hashmap! {
            "a" => 1,
            "b" => 2,
        };
        assert_eq!(map.get("a"), Some(&1));
        assert_eq!(map.get("b"), Some(&2));
    }

    #[test]
    fn test_derive_debug() {
        let point = Point { x: 1, y: 2 };
        let debug_str = format!("{:?}", point);
        assert!(debug_str.contains("Point"));
    }

    #[test]
    fn test_derive_clone() {
        let point = Point { x: 1, y: 2 };
        let cloned = point.clone();
        assert_eq!(point, cloned);
    }

    #[test]
    fn test_describe_trait() {
        let user = User {
            name: "Test".to_string(),
            age: 25,
        };
        assert!(user.describe().contains("Test"));
        assert!(user.describe().contains("25"));
    }
}
