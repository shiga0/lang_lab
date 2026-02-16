// 関数型プログラミング - C++ 実装
//
// C++ は関数型パラダイムもサポート。
// ラムダ、高階関数、ranges (C++20)。

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <ranges>
#include <optional>

using namespace std;

// === ラムダ式 ===
void lambda_demo() {
    cout << "=== ラムダ式 ===" << endl;

    // 基本
    auto add = [](int a, int b) { return a + b; };
    cout << "  add(2, 3) = " << add(2, 3) << endl;

    // 型明示
    auto multiply = [](int a, int b) -> int { return a * b; };
    cout << "  multiply(4, 5) = " << multiply(4, 5) << endl;

    // キャプチャ
    int x = 10;
    auto by_value = [x]() { return x; };        // 値キャプチャ
    auto by_ref = [&x]() { x++; return x; };    // 参照キャプチャ
    auto all_value = [=]() { return x; };       // 全て値
    auto all_ref = [&]() { return x; };         // 全て参照

    cout << "  by_value(): " << by_value() << endl;
    cout << "  by_ref(): " << by_ref() << endl;
    cout << "  x after by_ref: " << x << endl;

    // mutable (値キャプチャを変更可能に)
    int count = 0;
    auto counter = [count]() mutable { return ++count; };
    cout << "  counter(): " << counter() << ", " << counter() << ", " << counter() << endl;

    // ジェネリックラムダ (C++14)
    auto print = [](const auto& x) { cout << "  " << x << endl; };
    print(42);
    print("Hello");
    print(3.14);

    // 初期化キャプチャ (C++14)
    auto capture_init = [value = 100]() { return value; };
    cout << "  init capture: " << capture_init() << endl;

    cout << endl;
}

// === 高階関数 ===
void higher_order_functions() {
    cout << "=== 高階関数 ===" << endl;

    vector<int> numbers = {1, 2, 3, 4, 5};

    // transform (map)
    vector<int> doubled;
    transform(numbers.begin(), numbers.end(),
              back_inserter(doubled),
              [](int x) { return x * 2; });
    cout << "  transform (*2): ";
    for (int x : doubled) cout << x << " ";
    cout << endl;

    // copy_if (filter)
    vector<int> evens;
    copy_if(numbers.begin(), numbers.end(),
            back_inserter(evens),
            [](int x) { return x % 2 == 0; });
    cout << "  copy_if (even): ";
    for (int x : evens) cout << x << " ";
    cout << endl;

    // accumulate (reduce/fold)
    int sum = accumulate(numbers.begin(), numbers.end(), 0);
    cout << "  accumulate (sum): " << sum << endl;

    int product = accumulate(numbers.begin(), numbers.end(), 1,
                             [](int a, int b) { return a * b; });
    cout << "  accumulate (product): " << product << endl;

    // for_each
    cout << "  for_each: ";
    for_each(numbers.begin(), numbers.end(),
             [](int x) { cout << x * x << " "; });
    cout << endl;

    // any_of, all_of, none_of
    bool has_even = any_of(numbers.begin(), numbers.end(),
                           [](int x) { return x % 2 == 0; });
    bool all_positive = all_of(numbers.begin(), numbers.end(),
                               [](int x) { return x > 0; });
    cout << "  any_of even: " << boolalpha << has_even << endl;
    cout << "  all_of positive: " << all_positive << endl;

    cout << endl;
}

// === std::function ===
void std_function_demo() {
    cout << "=== std::function ===" << endl;

    // 関数ポインタ、ラムダ、callable を統一的に扱う
    function<int(int, int)> operation;

    operation = [](int a, int b) { return a + b; };
    cout << "  add: " << operation(10, 5) << endl;

    operation = [](int a, int b) { return a * b; };
    cout << "  multiply: " << operation(10, 5) << endl;

    // コールバック
    auto execute = [](function<void()> callback) {
        cout << "  Before callback" << endl;
        callback();
        cout << "  After callback" << endl;
    };

    execute([]() { cout << "  Inside callback!" << endl; });

    // 関数を返す
    auto make_multiplier = [](int factor) -> function<int(int)> {
        return [factor](int x) { return x * factor; };
    };

    auto triple = make_multiplier(3);
    cout << "  triple(7): " << triple(7) << endl;

    cout << endl;
}

// === bind ===
void bind_demo() {
    cout << "=== bind ===" << endl;

    auto greet = [](const string& greeting, const string& name) {
        return greeting + ", " + name + "!";
    };

    // 部分適用
    auto hello = bind(greet, "Hello", placeholders::_1);
    auto goodbye = bind(greet, "Goodbye", placeholders::_1);

    cout << "  " << hello("Alice") << endl;
    cout << "  " << goodbye("Bob") << endl;

    // 引数の並び替え
    auto reversed = bind(greet, placeholders::_2, placeholders::_1);
    cout << "  " << reversed("World", "Hey") << endl;

    cout << endl;
}

// === ranges (C++20) ===
void ranges_demo() {
    cout << "=== ranges (C++20) ===" << endl;

    vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // パイプライン
    auto result = numbers
        | views::filter([](int x) { return x % 2 == 0; })
        | views::transform([](int x) { return x * x; });

    cout << "  filter even, square: ";
    for (int x : result) {
        cout << x << " ";
    }
    cout << endl;

    // take
    auto first_5 = numbers | views::take(5);
    cout << "  take(5): ";
    for (int x : first_5) cout << x << " ";
    cout << endl;

    // drop
    auto after_3 = numbers | views::drop(3);
    cout << "  drop(3): ";
    for (int x : after_3) cout << x << " ";
    cout << endl;

    // iota (無限シーケンス)
    auto naturals = views::iota(1) | views::take(10);
    cout << "  iota(1) | take(10): ";
    for (int x : naturals) cout << x << " ";
    cout << endl;

    // reverse
    auto reversed = numbers | views::reverse;
    cout << "  reverse: ";
    for (int x : reversed) cout << x << " ";
    cout << endl;

    // keys, values (map用)
    vector<pair<string, int>> data = {{"a", 1}, {"b", 2}, {"c", 3}};
    cout << "  keys: ";
    for (const auto& k : data | views::keys) cout << k << " ";
    cout << endl;

    cout << endl;
}

// === 関数合成 ===
template<typename F, typename G>
auto compose(F f, G g) {
    return [f, g](auto x) { return f(g(x)); };
}

template<typename... Fs>
auto pipeline(Fs... fs) {
    return [=](auto x) {
        return (... , (x = fs(x)));  // fold expression
    };
}

void composition_demo() {
    cout << "=== 関数合成 ===" << endl;

    auto add_one = [](int x) { return x + 1; };
    auto double_it = [](int x) { return x * 2; };
    auto square = [](int x) { return x * x; };

    // compose: f(g(x))
    auto composed = compose(double_it, add_one);
    cout << "  compose(double, add_one)(5) = " << composed(5) << endl;

    // pipeline
    auto pipe = pipeline(add_one, double_it, square);
    cout << "  pipeline(add_one, double, square)(5) = " << pipe(5) << endl;

    cout << endl;
}

// === カリー化 ===
auto curry_add = [](int a) {
    return [a](int b) {
        return [a, b](int c) {
            return a + b + c;
        };
    };
};

void curry_demo() {
    cout << "=== カリー化 ===" << endl;

    auto add_10 = curry_add(10);
    auto add_10_20 = add_10(20);
    cout << "  curry_add(10)(20)(30) = " << add_10_20(30) << endl;

    // 一度に呼び出し
    cout << "  curry_add(1)(2)(3) = " << curry_add(1)(2)(3) << endl;

    cout << endl;
}

// === optional でチェーン ===
optional<int> parse_int(const string& s) {
    try {
        return stoi(s);
    } catch (...) {
        return nullopt;
    }
}

optional<int> safe_divide(int a, int b) {
    if (b == 0) return nullopt;
    return a / b;
}

void optional_chain_demo() {
    cout << "=== optional チェーン ===" << endl;

    // and_then (flatMap)
    auto result = parse_int("10")
        .and_then([](int x) { return safe_divide(100, x); })
        .transform([](int x) { return x * 2; });

    if (result) {
        cout << "  Result: " << *result << endl;
    }

    // 失敗ケース
    auto failed = parse_int("abc")
        .and_then([](int x) { return safe_divide(100, x); });

    cout << "  Failed has value: " << boolalpha << failed.has_value() << endl;

    cout << endl;
}

// === 再帰ラムダ ===
void recursive_lambda_demo() {
    cout << "=== 再帰ラムダ ===" << endl;

    // function を使う方法
    function<int(int)> factorial = [&factorial](int n) -> int {
        return n <= 1 ? 1 : n * factorial(n - 1);
    };
    cout << "  factorial(5) = " << factorial(5) << endl;

    // Y コンビネータ風
    auto y_factorial = [](auto self, int n) -> int {
        return n <= 1 ? 1 : n * self(self, n - 1);
    };
    cout << "  Y-factorial(6) = " << y_factorial(y_factorial, 6) << endl;

    cout << endl;
}

// === constexpr ラムダ ===
void constexpr_lambda_demo() {
    cout << "=== constexpr ラムダ ===" << endl;

    constexpr auto square = [](int x) constexpr { return x * x; };
    constexpr int result = square(5);

    static_assert(square(5) == 25);

    cout << "  constexpr square(5) = " << result << endl;

    cout << endl;
}

int main() {
    cout << "=== Functional Programming Demo ===" << endl << endl;

    lambda_demo();
    higher_order_functions();
    std_function_demo();
    bind_demo();
    ranges_demo();
    composition_demo();
    curry_demo();
    optional_chain_demo();
    recursive_lambda_demo();
    constexpr_lambda_demo();

    return 0;
}
