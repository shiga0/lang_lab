# C++ チートシート

## 基本構文

### Hello World
```cpp
#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
```

### 変数と型
```cpp
// 基本型
int i = 42;
double d = 3.14;
char c = 'A';
bool b = true;

// auto（型推論）
auto x = 42;        // int
auto y = 3.14;      // double
auto s = "hello";   // const char*

// const と constexpr
const int MAX = 100;           // 実行時定数
constexpr int SIZE = 10;       // コンパイル時定数

// 参照とポインタ
int value = 10;
int& ref = value;      // 参照
int* ptr = &value;     // ポインタ
```

### 文字列
```cpp
#include <string>
#include <string_view>

std::string s1 = "Hello";
std::string s2 = s1 + " World";

// 文字列操作
s1.length();          // 長さ
s1.substr(0, 3);      // 部分文字列
s1.find("llo");       // 検索
s1.empty();           // 空チェック

// string_view（C++17、コピーなし参照）
std::string_view sv = s1;
```

### コンテナ
```cpp
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <set>

// vector（動的配列）
std::vector<int> v = {1, 2, 3};
v.push_back(4);
v[0];                 // アクセス
v.at(0);              // 境界チェック付き

// array（固定長配列）
std::array<int, 3> arr = {1, 2, 3};

// map（順序付き連想配列）
std::map<std::string, int> m;
m["key"] = 42;

// unordered_map（ハッシュマップ）
std::unordered_map<std::string, int> um;

// set
std::set<int> s = {3, 1, 2};  // 自動ソート
```

## 制御構文

### if/else
```cpp
if (x > 0) {
    // ...
} else if (x < 0) {
    // ...
} else {
    // ...
}

// 初期化付き if（C++17）
if (auto it = m.find("key"); it != m.end()) {
    std::cout << it->second;
}
```

### switch
```cpp
switch (value) {
    case 1:
        // ...
        break;
    case 2:
    case 3:
        // ...
        break;
    default:
        // ...
}
```

### ループ
```cpp
// for
for (int i = 0; i < 10; ++i) {
    // ...
}

// 範囲 for（C++11）
for (const auto& item : container) {
    // ...
}

// while
while (condition) {
    // ...
}

// do-while
do {
    // ...
} while (condition);
```

## 関数

### 基本
```cpp
int add(int a, int b) {
    return a + b;
}

// デフォルト引数
void greet(const std::string& name = "World") {
    std::cout << "Hello, " << name << std::endl;
}

// オーバーロード
int max(int a, int b);
double max(double a, double b);
```

### ラムダ式
```cpp
// 基本
auto add = [](int a, int b) { return a + b; };

// キャプチャ
int x = 10;
auto f1 = [x]() { return x; };       // 値キャプチャ
auto f2 = [&x]() { x++; };           // 参照キャプチャ
auto f3 = [=]() { return x; };       // 全て値
auto f4 = [&]() { x++; };            // 全て参照

// ジェネリックラムダ（C++14）
auto print = [](const auto& x) { std::cout << x; };

// mutable
auto counter = [count = 0]() mutable { return ++count; };
```

### 関数テンプレート
```cpp
template<typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}

// concepts（C++20）
template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

template<Numeric T>
T add(T a, T b) {
    return a + b;
}
```

## クラス

### 基本
```cpp
class Point {
public:
    // コンストラクタ
    Point(double x, double y) : x_(x), y_(y) {}

    // デフォルトコンストラクタ
    Point() = default;

    // ゲッター
    double x() const { return x_; }
    double y() const { return y_; }

    // メソッド
    double distance(const Point& other) const {
        double dx = x_ - other.x_;
        double dy = y_ - other.y_;
        return std::sqrt(dx * dx + dy * dy);
    }

private:
    double x_ = 0;
    double y_ = 0;
};
```

### 継承
```cpp
class Animal {
public:
    virtual ~Animal() = default;
    virtual void speak() const = 0;  // 純粋仮想関数
};

class Dog : public Animal {
public:
    void speak() const override {
        std::cout << "Woof!" << std::endl;
    }
};
```

### 演算子オーバーロード
```cpp
class Vector2 {
public:
    double x, y;

    Vector2 operator+(const Vector2& other) const {
        return {x + other.x, y + other.y};
    }

    bool operator==(const Vector2& other) const {
        return x == other.x && y == other.y;
    }

    // C++20 の <=> でまとめて定義
    auto operator<=>(const Vector2&) const = default;
};
```

### struct
```cpp
// C++ の struct はデフォルト public のクラス
struct Point {
    double x;
    double y;

    double magnitude() const {
        return std::sqrt(x * x + y * y);
    }
};
```

## メモリ管理

### スマートポインタ
```cpp
#include <memory>

// unique_ptr（単独所有）
auto p1 = std::make_unique<int>(42);

// shared_ptr（共有所有）
auto p2 = std::make_shared<int>(42);
auto p3 = p2;  // 参照カウント増加

// weak_ptr（弱参照）
std::weak_ptr<int> wp = p2;
if (auto sp = wp.lock()) {
    // 有効な shared_ptr を取得
}
```

### RAII
```cpp
class FileHandle {
public:
    FileHandle(const std::string& path) {
        file_ = std::fopen(path.c_str(), "r");
    }

    ~FileHandle() {
        if (file_) std::fclose(file_);
    }

    // コピー禁止
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    // ムーブ許可
    FileHandle(FileHandle&& other) noexcept : file_(other.file_) {
        other.file_ = nullptr;
    }

private:
    std::FILE* file_;
};
```

### move セマンティクス
```cpp
std::vector<int> v1 = {1, 2, 3};
std::vector<int> v2 = std::move(v1);  // v1 の中身を v2 に移動

// 完全転送
template<typename T>
void wrapper(T&& arg) {
    process(std::forward<T>(arg));
}
```

## エラー処理

### 例外
```cpp
#include <stdexcept>

void divide(int a, int b) {
    if (b == 0) {
        throw std::invalid_argument("Division by zero");
    }
    return a / b;
}

try {
    divide(10, 0);
} catch (const std::invalid_argument& e) {
    std::cerr << "Error: " << e.what() << std::endl;
} catch (...) {
    std::cerr << "Unknown error" << std::endl;
}
```

### optional（C++17）
```cpp
#include <optional>

std::optional<int> find(const std::vector<int>& v, int target) {
    for (int x : v) {
        if (x == target) return x;
    }
    return std::nullopt;
}

if (auto result = find(v, 42)) {
    std::cout << *result << std::endl;
}
```

### expected（C++23）
```cpp
#include <expected>

std::expected<int, std::string> divide(int a, int b) {
    if (b == 0) {
        return std::unexpected("Division by zero");
    }
    return a / b;
}

auto result = divide(10, 2);
if (result) {
    std::cout << *result << std::endl;
} else {
    std::cerr << result.error() << std::endl;
}
```

## 並行処理

### スレッド
```cpp
#include <thread>
#include <mutex>

std::mutex mtx;
int counter = 0;

void increment() {
    std::lock_guard<std::mutex> lock(mtx);
    ++counter;
}

std::thread t1(increment);
std::thread t2(increment);
t1.join();
t2.join();
```

### async/future
```cpp
#include <future>

auto future = std::async(std::launch::async, []() {
    return compute_something();
});

// 結果を待つ
int result = future.get();
```

### atomic
```cpp
#include <atomic>

std::atomic<int> counter{0};

// ロック不要
counter++;
counter.fetch_add(1);
```

## テンプレート

### クラステンプレート
```cpp
template<typename T>
class Stack {
public:
    void push(const T& value) {
        data_.push_back(value);
    }

    T pop() {
        T value = data_.back();
        data_.pop_back();
        return value;
    }

private:
    std::vector<T> data_;
};

Stack<int> s;
s.push(42);
```

### 可変引数テンプレート
```cpp
template<typename... Args>
void print(Args... args) {
    (std::cout << ... << args) << std::endl;
}

print(1, " + ", 2, " = ", 3);  // "1 + 2 = 3"
```

### SFINAE / concepts
```cpp
// C++17 以前: SFINAE
template<typename T>
std::enable_if_t<std::is_integral_v<T>, T>
square(T x) {
    return x * x;
}

// C++20: concepts
template<std::integral T>
T square(T x) {
    return x * x;
}
```

## モダン C++ 機能

### 構造化束縛（C++17）
```cpp
std::map<std::string, int> m = {{"a", 1}, {"b", 2}};

for (const auto& [key, value] : m) {
    std::cout << key << ": " << value << std::endl;
}

auto [x, y] = std::make_pair(1, 2);
```

### ranges（C++20）
```cpp
#include <ranges>

std::vector<int> v = {1, 2, 3, 4, 5};

auto result = v
    | std::views::filter([](int x) { return x % 2 == 0; })
    | std::views::transform([](int x) { return x * 2; });

for (int x : result) {
    std::cout << x << std::endl;  // 4, 8
}
```

### 三方比較演算子（C++20）
```cpp
struct Point {
    int x, y;
    auto operator<=>(const Point&) const = default;
};

Point p1{1, 2}, p2{1, 3};
if (p1 < p2) { /* ... */ }
```

### constexpr
```cpp
constexpr int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

// コンパイル時に計算
constexpr int result = factorial(5);  // 120
```

## イディオム

### PIMPL
```cpp
// header
class Widget {
public:
    Widget();
    ~Widget();
    void doSomething();
private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};

// source
struct Widget::Impl {
    // 実装詳細
};
```

### Curiously Recurring Template Pattern (CRTP)
```cpp
template<typename Derived>
class Base {
public:
    void interface() {
        static_cast<Derived*>(this)->implementation();
    }
};

class Derived : public Base<Derived> {
public:
    void implementation() {
        // ...
    }
};
```

## 実行例

```bash
# コンパイルと実行
clang++ -std=c++20 -Wall -O2 main.cpp -o main && ./main

# デバッグビルド
clang++ -std=c++20 -g -fsanitize=address main.cpp -o main

# アセンブリ出力
clang++ -std=c++20 -S -O2 main.cpp -o main.s
```
