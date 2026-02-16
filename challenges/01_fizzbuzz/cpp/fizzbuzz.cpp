// FizzBuzz - C++ 実装

#include <iostream>
#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <functional>
#include <cassert>

using namespace std;

// === 基本実装 ===
void fizzbuzz_basic(int n) {
    cout << "--- Basic Implementation ---" << endl;

    for (int i = 1; i <= n; ++i) {
        if (i % 15 == 0) {
            cout << "FizzBuzz" << endl;
        } else if (i % 3 == 0) {
            cout << "Fizz" << endl;
        } else if (i % 5 == 0) {
            cout << "Buzz" << endl;
        } else {
            cout << i << endl;
        }
    }
    cout << endl;
}

// === 関数型スタイル ===
string fizzbuzz_transform(int i) {
    if (i % 15 == 0) return "FizzBuzz";
    if (i % 3 == 0) return "Fizz";
    if (i % 5 == 0) return "Buzz";
    return to_string(i);
}

vector<string> fizzbuzz_functional(int n) {
    vector<string> result;
    result.reserve(n);

    for (int i = 1; i <= n; ++i) {
        result.push_back(fizzbuzz_transform(i));
    }

    return result;
}

void functional_demo() {
    cout << "--- Functional Style ---" << endl;

    auto result = fizzbuzz_functional(15);

    for (const auto& s : result) {
        cout << s << " ";
    }
    cout << endl << endl;
}

// === ranges (C++20) ===
void fizzbuzz_ranges() {
    cout << "--- Ranges (C++20) ---" << endl;

    auto result = views::iota(1, 16)
        | views::transform([](int i) -> string {
            if (i % 15 == 0) return "FizzBuzz";
            if (i % 3 == 0) return "Fizz";
            if (i % 5 == 0) return "Buzz";
            return to_string(i);
        });

    for (const auto& s : result) {
        cout << s << " ";
    }
    cout << endl << endl;
}

// === 文字列連結スタイル ===
void fizzbuzz_concat() {
    cout << "--- String Concatenation Style ---" << endl;

    for (int i = 1; i <= 15; ++i) {
        string s;
        if (i % 3 == 0) s += "Fizz";
        if (i % 5 == 0) s += "Buzz";
        if (s.empty()) s = to_string(i);
        cout << s << " ";
    }
    cout << endl << endl;
}

// === テンプレートメタプログラミング ===
template<int N>
struct FizzBuzz {
    static string get() {
        if constexpr (N % 15 == 0) return "FizzBuzz";
        else if constexpr (N % 3 == 0) return "Fizz";
        else if constexpr (N % 5 == 0) return "Buzz";
        else return to_string(N);
    }
};

void fizzbuzz_template() {
    cout << "--- Template Metaprogramming ---" << endl;

    cout << "1: " << FizzBuzz<1>::get() << endl;
    cout << "3: " << FizzBuzz<3>::get() << endl;
    cout << "5: " << FizzBuzz<5>::get() << endl;
    cout << "15: " << FizzBuzz<15>::get() << endl;

    cout << endl;
}

// === constexpr ===
constexpr auto fizzbuzz_constexpr(int i) {
    struct Result {
        bool is_fizzbuzz;
        bool is_fizz;
        bool is_buzz;
        int number;
    };

    return Result{
        i % 15 == 0,
        i % 3 == 0 && i % 5 != 0,
        i % 5 == 0 && i % 3 != 0,
        i
    };
}

void constexpr_demo() {
    cout << "--- constexpr ---" << endl;

    constexpr auto r3 = fizzbuzz_constexpr(3);
    constexpr auto r5 = fizzbuzz_constexpr(5);
    constexpr auto r15 = fizzbuzz_constexpr(15);

    static_assert(r3.is_fizz);
    static_assert(r5.is_buzz);
    static_assert(r15.is_fizzbuzz);

    cout << "Compile-time checks passed!" << endl;
    cout << endl;
}

// === ジェネレータ風 ===
class FizzBuzzGenerator {
public:
    class Iterator {
    public:
        using iterator_category = input_iterator_tag;
        using value_type = string;
        using difference_type = ptrdiff_t;
        using pointer = const string*;
        using reference = const string&;

        Iterator(int current, int end)
            : current_(current), end_(end) {
            if (current_ <= end_) update_value();
        }

        const string& operator*() const { return value_; }

        Iterator& operator++() {
            ++current_;
            if (current_ <= end_) update_value();
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return current_ != other.current_;
        }

    private:
        void update_value() {
            value_ = fizzbuzz_transform(current_);
        }

        int current_;
        int end_;
        string value_;
    };

    FizzBuzzGenerator(int start, int end) : start_(start), end_(end) {}

    Iterator begin() const { return Iterator(start_, end_); }
    Iterator end() const { return Iterator(end_ + 1, end_); }

private:
    int start_;
    int end_;
};

void generator_demo() {
    cout << "--- Generator Style ---" << endl;

    FizzBuzzGenerator gen(1, 15);

    for (const auto& s : gen) {
        cout << s << " ";
    }
    cout << endl << endl;
}

// === ルールテーブル ===
void fizzbuzz_rules() {
    cout << "--- Rules Table ---" << endl;

    vector<pair<int, string>> rules = {
        {15, "FizzBuzz"},
        {3, "Fizz"},
        {5, "Buzz"}
    };

    for (int i = 1; i <= 15; ++i) {
        string result;
        for (const auto& [divisor, word] : rules) {
            if (i % divisor == 0) {
                result = word;
                break;
            }
        }
        if (result.empty()) result = to_string(i);
        cout << result << " ";
    }
    cout << endl << endl;
}

// === テスト ===
void run_tests() {
    cout << "--- Tests ---" << endl;

    auto result = fizzbuzz_functional(15);

    // 基本ケース
    assert(result[0] == "1");
    assert(result[1] == "2");

    // Fizz (3の倍数)
    assert(result[2] == "Fizz");
    assert(result[5] == "Fizz");

    // Buzz (5の倍数)
    assert(result[4] == "Buzz");
    assert(result[9] == "Buzz");

    // FizzBuzz (15の倍数)
    assert(result[14] == "FizzBuzz");

    cout << "All tests passed!" << endl;
}

int main() {
    cout << "=== FizzBuzz Demo ===" << endl << endl;

    fizzbuzz_basic(15);
    functional_demo();
    fizzbuzz_ranges();
    fizzbuzz_concat();
    fizzbuzz_template();
    constexpr_demo();
    generator_demo();
    fizzbuzz_rules();
    run_tests();

    return 0;
}
