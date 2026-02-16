// 型システム - C++ 実装
//
// C++ は静的型付け言語。テンプレート、concepts、auto による型推論をサポート。

#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <any>
#include <type_traits>
#include <concepts>

using namespace std;

// === 基本型 ===
void basic_types() {
    cout << "=== 基本型 ===" << endl;

    // 整数型
    int i = 42;
    long l = 100L;
    unsigned int ui = 100U;
    int64_t i64 = 100;

    // 浮動小数点
    float f = 3.14f;
    double d = 3.14159265359;

    // 文字・文字列
    char c = 'A';
    string s = "Hello";

    // ブーリアン
    bool b = true;

    cout << "int: " << i << ", double: " << d << endl;
    cout << "string: " << s << ", bool: " << boolalpha << b << endl;

    // サイズ
    cout << "sizeof(int): " << sizeof(int) << endl;
    cout << "sizeof(double): " << sizeof(double) << endl;
    cout << "sizeof(string): " << sizeof(string) << endl;

    cout << endl;
}

// === auto と型推論 ===
void type_inference() {
    cout << "=== auto と型推論 ===" << endl;

    // auto
    auto x = 42;          // int
    auto y = 3.14;        // double
    auto z = "hello";     // const char*
    auto s = string("hello"); // std::string

    cout << "auto x = 42 -> " << typeid(x).name() << endl;
    cout << "auto y = 3.14 -> " << typeid(y).name() << endl;

    // decltype
    int a = 10;
    decltype(a) b = 20;   // int
    decltype(a + 0.0) c = 3.14;  // double

    cout << "decltype(a) b -> " << typeid(b).name() << endl;
    cout << "decltype(a + 0.0) c -> " << typeid(c).name() << endl;

    // auto with references
    auto& ref = a;
    const auto& cref = a;

    // Structured bindings (C++17)
    pair<int, string> p = {1, "one"};
    auto [num, str] = p;
    cout << "Structured binding: " << num << ", " << str << endl;

    cout << endl;
}

// === テンプレート ===
template<typename T>
T add(T a, T b) {
    return a + b;
}

template<typename T, typename U>
auto add_mixed(T a, U b) -> decltype(a + b) {
    return a + b;
}

// 可変引数テンプレート
template<typename... Args>
void print_all(Args... args) {
    (cout << ... << args) << endl;
}

void templates_demo() {
    cout << "=== テンプレート ===" << endl;

    cout << "add(1, 2) = " << add(1, 2) << endl;
    cout << "add(1.5, 2.5) = " << add(1.5, 2.5) << endl;
    cout << "add_mixed(1, 2.5) = " << add_mixed(1, 2.5) << endl;

    print_all("Values: ", 1, ", ", 2.5, ", ", "hello");

    cout << endl;
}

// === Concepts (C++20) ===
template<typename T>
concept Numeric = is_arithmetic_v<T>;

template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> same_as<T>;
};

template<Numeric T>
T multiply(T a, T b) {
    return a * b;
}

template<Addable T>
T sum(T a, T b) {
    return a + b;
}

void concepts_demo() {
    cout << "=== Concepts (C++20) ===" << endl;

    cout << "multiply(3, 4) = " << multiply(3, 4) << endl;
    cout << "multiply(2.5, 3.0) = " << multiply(2.5, 3.0) << endl;

    cout << "sum(10, 20) = " << sum(10, 20) << endl;
    cout << "sum(string(\"Hello\"), string(\" World\")) = "
         << sum(string("Hello"), string(" World")) << endl;

    // コンパイル時チェック
    static_assert(Numeric<int>);
    static_assert(Numeric<double>);
    static_assert(!Numeric<string>);

    cout << endl;
}

// === variant (C++17) ===
void variant_demo() {
    cout << "=== variant (代数的データ型) ===" << endl;

    variant<int, double, string> v;

    v = 42;
    cout << "variant<int, double, string> = 42 -> index: " << v.index() << endl;
    cout << "  get<int>: " << get<int>(v) << endl;

    v = 3.14;
    cout << "variant = 3.14 -> index: " << v.index() << endl;

    v = "hello";
    cout << "variant = \"hello\" -> index: " << v.index() << endl;

    // visit
    v = 100;
    visit([](auto&& arg) {
        cout << "visit: " << arg << " (type: " << typeid(arg).name() << ")" << endl;
    }, v);

    // holds_alternative
    cout << "holds_alternative<int>: " << boolalpha << holds_alternative<int>(v) << endl;

    cout << endl;
}

// === optional (C++17) ===
optional<int> safe_divide(int a, int b) {
    if (b == 0) return nullopt;
    return a / b;
}

void optional_demo() {
    cout << "=== optional ===" << endl;

    auto result1 = safe_divide(10, 2);
    if (result1) {
        cout << "10 / 2 = " << *result1 << endl;
    }

    auto result2 = safe_divide(10, 0);
    cout << "10 / 0 has_value: " << boolalpha << result2.has_value() << endl;
    cout << "value_or(0): " << result2.value_or(0) << endl;

    // optional with reference
    optional<string> name = "Alice";
    cout << "name.value_or(\"Unknown\"): " << name.value_or("Unknown") << endl;

    cout << endl;
}

// === any (C++17) ===
void any_demo() {
    cout << "=== any ===" << endl;

    any a = 42;
    cout << "any = 42, type: " << a.type().name() << endl;
    cout << "any_cast<int>: " << any_cast<int>(a) << endl;

    a = string("hello");
    cout << "any = \"hello\", type: " << a.type().name() << endl;

    a = 3.14;
    cout << "any = 3.14, type: " << a.type().name() << endl;

    // 型チェック
    if (a.type() == typeid(double)) {
        cout << "It's a double: " << any_cast<double>(a) << endl;
    }

    cout << endl;
}

// === 型特性 (type_traits) ===
void type_traits_demo() {
    cout << "=== 型特性 (type_traits) ===" << endl;

    cout << "is_integral<int>: " << boolalpha << is_integral_v<int> << endl;
    cout << "is_integral<double>: " << is_integral_v<double> << endl;
    cout << "is_floating_point<double>: " << is_floating_point_v<double> << endl;
    cout << "is_same<int, int32_t>: " << is_same_v<int, int32_t> << endl;

    // 型変換
    cout << "remove_const<const int>: same as int? "
         << is_same_v<remove_const_t<const int>, int> << endl;

    // 条件付き型
    using type = conditional_t<sizeof(int) == 4, int, long>;
    cout << "conditional_t<sizeof(int)==4, int, long>: " << typeid(type).name() << endl;

    cout << endl;
}

// === constexpr ===
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr int fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

void constexpr_demo() {
    cout << "=== constexpr ===" << endl;

    // コンパイル時計算
    constexpr int fact5 = factorial(5);
    constexpr int fib10 = fib(10);

    cout << "factorial(5) = " << fact5 << " (computed at compile time)" << endl;
    cout << "fib(10) = " << fib10 << " (computed at compile time)" << endl;

    // static_assert でコンパイル時チェック
    static_assert(factorial(5) == 120);
    static_assert(fib(10) == 55);

    cout << endl;
}

// === ユーザー定義リテラル ===
constexpr long double operator"" _km(long double km) {
    return km * 1000.0;
}

constexpr long double operator"" _m(long double m) {
    return m;
}

constexpr unsigned long long operator"" _KB(unsigned long long kb) {
    return kb * 1024;
}

void user_defined_literals_demo() {
    cout << "=== ユーザー定義リテラル ===" << endl;

    auto distance_m = 5.0_km;
    cout << "5.0_km = " << distance_m << " meters" << endl;

    auto size = 64_KB;
    cout << "64_KB = " << size << " bytes" << endl;

    cout << endl;
}

int main() {
    cout << "=== Type System Demo ===" << endl << endl;

    basic_types();
    type_inference();
    templates_demo();
    concepts_demo();
    variant_demo();
    optional_demo();
    any_demo();
    type_traits_demo();
    constexpr_demo();
    user_defined_literals_demo();

    return 0;
}
