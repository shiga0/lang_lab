// メタプログラミング - C++ 実装
//
// C++ はテンプレートメタプログラミング (TMP) が強力。
// コンパイル時計算、型操作、concepts など。

#include <iostream>
#include <string>
#include <type_traits>
#include <tuple>
#include <array>
#include <source_location>
#include <concepts>

using namespace std;

// === 型特性 (type_traits) ===
void type_traits_demo() {
    cout << "=== 型特性 (type_traits) ===" << endl;

    // 型の性質を調べる
    cout << "  is_integral<int>: " << boolalpha << is_integral_v<int> << endl;
    cout << "  is_integral<double>: " << is_integral_v<double> << endl;
    cout << "  is_floating_point<double>: " << is_floating_point_v<double> << endl;
    cout << "  is_pointer<int*>: " << is_pointer_v<int*> << endl;
    cout << "  is_reference<int&>: " << is_reference_v<int&> << endl;

    // 型の比較
    cout << "  is_same<int, int32_t>: " << is_same_v<int, int32_t> << endl;
    cout << "  is_same<int, long>: " << is_same_v<int, long> << endl;

    // 型変換
    cout << "  remove_const<const int> is int: "
         << is_same_v<remove_const_t<const int>, int> << endl;
    cout << "  remove_pointer<int*> is int: "
         << is_same_v<remove_pointer_t<int*>, int> << endl;
    cout << "  add_pointer<int> is int*: "
         << is_same_v<add_pointer_t<int>, int*> << endl;

    // decay
    cout << "  decay<const int&> is int: "
         << is_same_v<decay_t<const int&>, int> << endl;

    cout << endl;
}

// === コンパイル時計算 ===
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// テンプレートによる再帰計算（古典的TMP）
template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

void compile_time_computation() {
    cout << "=== コンパイル時計算 ===" << endl;

    // constexpr 関数
    constexpr int fact5 = factorial(5);
    constexpr int fib10 = fibonacci(10);

    cout << "  factorial(5) = " << fact5 << endl;
    cout << "  fibonacci(10) = " << fib10 << endl;

    // テンプレート計算
    cout << "  Factorial<6>::value = " << Factorial<6>::value << endl;

    // static_assert
    static_assert(factorial(5) == 120, "factorial(5) must be 120");
    static_assert(Factorial<6>::value == 720, "Factorial<6> must be 720");

    cout << endl;
}

// === SFINAE ===
// enable_if による条件付きオーバーロード

template<typename T>
enable_if_t<is_integral_v<T>, T>
double_value(T value) {
    cout << "  (integral version) ";
    return value * 2;
}

template<typename T>
enable_if_t<is_floating_point_v<T>, T>
double_value(T value) {
    cout << "  (floating point version) ";
    return value * 2.0;
}

void sfinae_demo() {
    cout << "=== SFINAE ===" << endl;

    cout << "double_value(5): " << double_value(5) << endl;
    cout << "double_value(3.14): " << double_value(3.14) << endl;

    cout << endl;
}

// === Concepts (C++20) ===
template<typename T>
concept Numeric = is_arithmetic_v<T>;

template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> same_as<T>;
};

template<typename T>
concept Printable = requires(T a, ostream& os) {
    { os << a } -> same_as<ostream&>;
};

template<typename T>
concept Container = requires(T c) {
    typename T::value_type;
    typename T::iterator;
    { c.begin() } -> same_as<typename T::iterator>;
    { c.end() } -> same_as<typename T::iterator>;
    { c.size() } -> convertible_to<size_t>;
};

template<Numeric T>
T add(T a, T b) {
    return a + b;
}

template<Printable T>
void print_value(const T& value) {
    cout << "  Value: " << value << endl;
}

void concepts_demo() {
    cout << "=== Concepts (C++20) ===" << endl;

    cout << "  add(10, 20) = " << add(10, 20) << endl;
    cout << "  add(1.5, 2.5) = " << add(1.5, 2.5) << endl;

    print_value(42);
    print_value("Hello");

    // コンセプトチェック
    static_assert(Numeric<int>);
    static_assert(Numeric<double>);
    static_assert(!Numeric<string>);
    static_assert(Addable<int>);
    static_assert(Addable<string>);
    static_assert(Container<vector<int>>);

    cout << endl;
}

// === 可変引数テンプレート ===
void print() {
    cout << endl;
}

template<typename T, typename... Args>
void print(const T& first, const Args&... rest) {
    cout << first;
    if constexpr (sizeof...(rest) > 0) {
        cout << ", ";
        print(rest...);
    } else {
        cout << endl;
    }
}

// Fold expressions (C++17)
template<typename... Args>
auto sum(Args... args) {
    return (... + args);  // 左畳み込み
}

template<typename... Args>
void print_all(Args&&... args) {
    (cout << ... << args) << endl;
}

void variadic_templates_demo() {
    cout << "=== 可変引数テンプレート ===" << endl;

    cout << "  print: ";
    print(1, 2.5, "hello", 'c');

    cout << "  sum(1, 2, 3, 4, 5) = " << sum(1, 2, 3, 4, 5) << endl;

    cout << "  print_all: ";
    print_all("a", "b", "c", 123);

    cout << endl;
}

// === タプル操作 ===
template<typename Tuple, size_t... Is>
void print_tuple_impl(const Tuple& t, index_sequence<Is...>) {
    ((cout << (Is == 0 ? "" : ", ") << get<Is>(t)), ...);
}

template<typename... Args>
void print_tuple(const tuple<Args...>& t) {
    cout << "(";
    print_tuple_impl(t, index_sequence_for<Args...>{});
    cout << ")" << endl;
}

void tuple_metaprogramming() {
    cout << "=== タプル操作 ===" << endl;

    auto t = make_tuple(1, "hello", 3.14);

    cout << "  tuple: ";
    print_tuple(t);

    cout << "  tuple_size: " << tuple_size_v<decltype(t)> << endl;
    cout << "  element 1: " << get<1>(t) << endl;

    // 型を取得
    using second_type = tuple_element_t<1, decltype(t)>;
    cout << "  type of element 1: " << typeid(second_type).name() << endl;

    cout << endl;
}

// === 型リスト ===
template<typename... Types>
struct TypeList {};

template<typename List>
struct TypeListSize;

template<typename... Types>
struct TypeListSize<TypeList<Types...>> {
    static constexpr size_t value = sizeof...(Types);
};

template<typename List>
constexpr size_t TypeListSize_v = TypeListSize<List>::value;

template<size_t I, typename List>
struct TypeAt;

template<typename Head, typename... Tail>
struct TypeAt<0, TypeList<Head, Tail...>> {
    using type = Head;
};

template<size_t I, typename Head, typename... Tail>
struct TypeAt<I, TypeList<Head, Tail...>> {
    using type = typename TypeAt<I - 1, TypeList<Tail...>>::type;
};

template<size_t I, typename List>
using TypeAt_t = typename TypeAt<I, List>::type;

void type_list_demo() {
    cout << "=== 型リスト ===" << endl;

    using MyTypes = TypeList<int, double, string>;

    cout << "  TypeList size: " << TypeListSize_v<MyTypes> << endl;

    using FirstType = TypeAt_t<0, MyTypes>;
    using SecondType = TypeAt_t<1, MyTypes>;

    cout << "  First type is int: " << is_same_v<FirstType, int> << endl;
    cout << "  Second type is double: " << is_same_v<SecondType, double> << endl;

    cout << endl;
}

// === if constexpr ===
template<typename T>
string type_name() {
    if constexpr (is_integral_v<T>) {
        return "integral";
    } else if constexpr (is_floating_point_v<T>) {
        return "floating point";
    } else if constexpr (is_same_v<T, string>) {
        return "string";
    } else {
        return "unknown";
    }
}

void if_constexpr_demo() {
    cout << "=== if constexpr ===" << endl;

    cout << "  type_name<int>(): " << type_name<int>() << endl;
    cout << "  type_name<double>(): " << type_name<double>() << endl;
    cout << "  type_name<string>(): " << type_name<string>() << endl;

    cout << endl;
}

// === consteval (C++20) ===
consteval int compile_time_only(int n) {
    return n * n;
}

void consteval_demo() {
    cout << "=== consteval (C++20) ===" << endl;

    // consteval は必ずコンパイル時に評価される
    constexpr int result = compile_time_only(5);
    cout << "  compile_time_only(5) = " << result << endl;

    // 以下はコンパイルエラー（実行時の値を渡せない）
    // int x = 5;
    // int y = compile_time_only(x);  // エラー

    cout << endl;
}

// === ユーザー定義リテラル ===
constexpr long double operator"" _deg(long double deg) {
    return deg * 3.14159265358979323846L / 180.0L;
}

constexpr unsigned long long operator"" _KB(unsigned long long kb) {
    return kb * 1024;
}

constexpr unsigned long long operator"" _MB(unsigned long long mb) {
    return mb * 1024 * 1024;
}

void user_defined_literals_demo() {
    cout << "=== ユーザー定義リテラル ===" << endl;

    constexpr auto radians = 90.0_deg;
    cout << "  90.0_deg = " << radians << " radians" << endl;

    constexpr auto bytes = 64_KB;
    cout << "  64_KB = " << bytes << " bytes" << endl;

    constexpr auto megabytes = 2_MB;
    cout << "  2_MB = " << megabytes << " bytes" << endl;

    cout << endl;
}

// === CRTP (Curiously Recurring Template Pattern) ===
template<typename Derived>
class Comparable {
public:
    bool operator!=(const Derived& other) const {
        return !(*static_cast<const Derived*>(this) == other);
    }

    bool operator>(const Derived& other) const {
        return other < *static_cast<const Derived*>(this);
    }

    bool operator<=(const Derived& other) const {
        return !(other < *static_cast<const Derived*>(this));
    }

    bool operator>=(const Derived& other) const {
        return !(*static_cast<const Derived*>(this) < other);
    }
};

class Number : public Comparable<Number> {
public:
    int value;

    Number(int v) : value(v) {}

    bool operator==(const Number& other) const {
        return value == other.value;
    }

    bool operator<(const Number& other) const {
        return value < other.value;
    }
};

void crtp_demo() {
    cout << "=== CRTP ===" << endl;

    Number a(5), b(10);

    cout << "  a == b: " << boolalpha << (a == b) << endl;
    cout << "  a != b: " << (a != b) << endl;
    cout << "  a < b: " << (a < b) << endl;
    cout << "  a > b: " << (a > b) << endl;
    cout << "  a <= b: " << (a <= b) << endl;
    cout << "  a >= b: " << (a >= b) << endl;

    cout << endl;
}

// === コンパイル時文字列 ===
template<size_t N>
struct FixedString {
    char data[N];

    constexpr FixedString(const char (&str)[N]) {
        for (size_t i = 0; i < N; ++i) {
            data[i] = str[i];
        }
    }

    constexpr size_t size() const { return N - 1; }
    constexpr char operator[](size_t i) const { return data[i]; }
};

template<FixedString S>
constexpr auto get_string() {
    return S;
}

void compile_time_string_demo() {
    cout << "=== コンパイル時文字列 ===" << endl;

    constexpr auto str = get_string<"Hello">();
    cout << "  FixedString: ";
    for (size_t i = 0; i < str.size(); ++i) {
        cout << str[i];
    }
    cout << endl;

    cout << "  size: " << str.size() << endl;

    cout << endl;
}

int main() {
    cout << "=== Metaprogramming Demo ===" << endl << endl;

    type_traits_demo();
    compile_time_computation();
    sfinae_demo();
    concepts_demo();
    variadic_templates_demo();
    tuple_metaprogramming();
    type_list_demo();
    if_constexpr_demo();
    consteval_demo();
    user_defined_literals_demo();
    crtp_demo();
    compile_time_string_demo();

    return 0;
}
