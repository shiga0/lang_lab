// エラー処理 - C++ 実装
//
// C++ は例外ベースのエラー処理。
// C++17 以降は optional、variant、C++23 では expected も使える。

#include <iostream>
#include <stdexcept>
#include <string>
#include <optional>
#include <variant>
#include <cassert>
#include <source_location>
#include <system_error>

using namespace std;

// === 例外の基本 ===
void exception_basics() {
    cout << "=== 例外の基本 ===" << endl;

    // throw と catch
    auto divide = [](int a, int b) -> int {
        if (b == 0) {
            throw invalid_argument("Division by zero");
        }
        return a / b;
    };

    try {
        cout << "  10 / 2 = " << divide(10, 2) << endl;
        cout << "  10 / 0 = " << divide(10, 0) << endl;
    } catch (const invalid_argument& e) {
        cout << "  Caught invalid_argument: " << e.what() << endl;
    }

    // 複数の例外をキャッチ
    try {
        throw runtime_error("Something went wrong");
    } catch (const invalid_argument& e) {
        cout << "  invalid_argument: " << e.what() << endl;
    } catch (const runtime_error& e) {
        cout << "  runtime_error: " << e.what() << endl;
    } catch (const exception& e) {
        cout << "  exception: " << e.what() << endl;
    } catch (...) {
        cout << "  Unknown exception" << endl;
    }

    cout << endl;
}

// === 標準例外階層 ===
void standard_exceptions() {
    cout << "=== 標準例外階層 ===" << endl;

    // exception
    //   ├── logic_error
    //   │     ├── invalid_argument
    //   │     ├── domain_error
    //   │     ├── length_error
    //   │     └── out_of_range
    //   └── runtime_error
    //         ├── range_error
    //         ├── overflow_error
    //         └── underflow_error

    try {
        throw out_of_range("Index out of bounds");
    } catch (const logic_error& e) {
        cout << "  logic_error: " << e.what() << endl;
    }

    try {
        throw overflow_error("Integer overflow");
    } catch (const runtime_error& e) {
        cout << "  runtime_error: " << e.what() << endl;
    }

    cout << endl;
}

// === カスタム例外 ===
class ValidationError : public runtime_error {
public:
    ValidationError(const string& message, const string& field)
        : runtime_error(message), field_(field) {}

    const string& field() const { return field_; }

private:
    string field_;
};

class NetworkError : public runtime_error {
public:
    NetworkError(const string& message, int status_code)
        : runtime_error(message), status_code_(status_code) {}

    int status_code() const { return status_code_; }

private:
    int status_code_;
};

void custom_exceptions() {
    cout << "=== カスタム例外 ===" << endl;

    auto validate_user = [](const string& name, const string& email) {
        if (name.empty()) {
            throw ValidationError("Name is required", "name");
        }
        if (email.find('@') == string::npos) {
            throw ValidationError("Invalid email format", "email");
        }
    };

    try {
        validate_user("", "alice@example.com");
    } catch (const ValidationError& e) {
        cout << "  ValidationError: " << e.what()
             << " (field: " << e.field() << ")" << endl;
    }

    try {
        throw NetworkError("Connection timeout", 504);
    } catch (const NetworkError& e) {
        cout << "  NetworkError: " << e.what()
             << " (status: " << e.status_code() << ")" << endl;
    }

    cout << endl;
}

// === noexcept ===
void noexcept_demo() {
    cout << "=== noexcept ===" << endl;

    // noexcept: 例外を投げないことを宣言
    auto safe_add = [](int a, int b) noexcept -> int {
        return a + b;
    };

    // noexcept(expr): 条件付き noexcept
    auto maybe_throw = [](bool do_throw) noexcept(false) {
        if (do_throw) throw runtime_error("Thrown");
    };

    cout << "  safe_add is noexcept: " << boolalpha << noexcept(safe_add(1, 2)) << endl;
    cout << "  maybe_throw is noexcept: " << noexcept(maybe_throw(false)) << endl;

    // noexcept 関数で例外を投げると terminate()
    // terminate() は回復不能 → プログラム終了

    cout << endl;
}

// === RAII とリソース管理 ===
class FileRAII {
public:
    FileRAII(const string& filename) : filename_(filename) {
        cout << "  Opening: " << filename_ << endl;
        // 実際は fopen()
    }

    ~FileRAII() {
        cout << "  Closing: " << filename_ << endl;
        // 実際は fclose()
    }

    void write(const string& data) {
        if (data.empty()) {
            throw invalid_argument("Empty data");
        }
        cout << "  Writing: " << data << endl;
    }

    // コピー禁止
    FileRAII(const FileRAII&) = delete;
    FileRAII& operator=(const FileRAII&) = delete;

private:
    string filename_;
};

void raii_exception_safety() {
    cout << "=== RAII と例外安全性 ===" << endl;

    try {
        FileRAII file("test.txt");
        file.write("Hello");
        file.write("");  // 例外発生
    } catch (const exception& e) {
        cout << "  Exception: " << e.what() << endl;
    }
    cout << "  File automatically closed (exception safety)" << endl;

    cout << endl;
}

// === optional (C++17) ===
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

void optional_demo() {
    cout << "=== optional (C++17) ===" << endl;

    auto result1 = parse_int("42");
    if (result1) {
        cout << "  Parsed: " << *result1 << endl;
    }

    auto result2 = parse_int("not a number");
    cout << "  Parse failed: has_value = " << boolalpha << result2.has_value() << endl;
    cout << "  value_or(-1): " << result2.value_or(-1) << endl;

    // チェーン
    auto result = safe_divide(10, 2)
        .and_then([](int x) { return safe_divide(x, 2); })
        .transform([](int x) { return x * 10; });

    if (result) {
        cout << "  Chained result: " << *result << endl;
    }

    cout << endl;
}

// === variant (C++17) ===
using Result = variant<int, string>;

Result compute(bool success) {
    if (success) {
        return 42;
    }
    return string("Error: computation failed");
}

void variant_as_result() {
    cout << "=== variant as Result ===" << endl;

    auto handle_result = [](const Result& r) {
        if (holds_alternative<int>(r)) {
            cout << "  Success: " << get<int>(r) << endl;
        } else {
            cout << "  Error: " << get<string>(r) << endl;
        }
    };

    handle_result(compute(true));
    handle_result(compute(false));

    // visit
    auto result = compute(true);
    visit([](auto&& arg) {
        using T = decay_t<decltype(arg)>;
        if constexpr (is_same_v<T, int>) {
            cout << "  Visited int: " << arg << endl;
        } else {
            cout << "  Visited string: " << arg << endl;
        }
    }, result);

    cout << endl;
}

// === expected-like pattern (pre-C++23) ===
template<typename T, typename E>
class Expected {
public:
    Expected(const T& value) : value_(value), has_value_(true) {}
    Expected(const E& error) : error_(error), has_value_(false) {}

    bool has_value() const { return has_value_; }
    const T& value() const { return value_; }
    const E& error() const { return error_; }

    operator bool() const { return has_value_; }
    const T& operator*() const { return value_; }

private:
    T value_{};
    E error_{};
    bool has_value_;
};

Expected<int, string> safe_divide_expected(int a, int b) {
    if (b == 0) {
        return string("Division by zero");
    }
    return a / b;
}

void expected_demo() {
    cout << "=== Expected パターン ===" << endl;

    auto result1 = safe_divide_expected(10, 2);
    if (result1) {
        cout << "  10 / 2 = " << *result1 << endl;
    }

    auto result2 = safe_divide_expected(10, 0);
    if (!result2) {
        cout << "  Error: " << result2.error() << endl;
    }

    cout << endl;
}

// === error_code (システムエラー) ===
void error_code_demo() {
    cout << "=== error_code (システムエラー) ===" << endl;

    error_code ec = make_error_code(errc::no_such_file_or_directory);

    cout << "  error_code: " << ec << endl;
    cout << "  message: " << ec.message() << endl;
    cout << "  category: " << ec.category().name() << endl;

    // エラーコードでの関数
    auto read_file = [](const string& path) -> error_code {
        // シミュレーション
        if (path == "missing.txt") {
            return make_error_code(errc::no_such_file_or_directory);
        }
        return {};  // success
    };

    auto ec2 = read_file("missing.txt");
    if (ec2) {
        cout << "  Read error: " << ec2.message() << endl;
    }

    cout << endl;
}

// === assert ===
void assert_demo() {
    cout << "=== assert ===" << endl;

    auto process = [](int value) {
        assert(value >= 0 && "value must be non-negative");
        // NDEBUG が定義されていると assert は無効
        cout << "  Processing: " << value << endl;
    };

    process(10);
    // process(-1);  // アサーション失敗で abort()

    // static_assert (コンパイル時)
    static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");

    cout << "  static_assert passed" << endl;

    cout << endl;
}

// === source_location (C++20) ===
void log_error(const string& message,
               const source_location& loc = source_location::current()) {
    cout << "  [ERROR] " << loc.file_name() << ":"
         << loc.line() << " (" << loc.function_name() << "): "
         << message << endl;
}

void source_location_demo() {
    cout << "=== source_location (C++20) ===" << endl;

    log_error("Something went wrong");

    auto nested = []() {
        log_error("Nested error");
    };
    nested();

    cout << endl;
}

// === 例外安全性レベル ===
void exception_safety_levels() {
    cout << "=== 例外安全性レベル ===" << endl;

    // 1. No-throw guarantee: 例外を投げない (noexcept)
    // 2. Strong guarantee: 例外時は操作前の状態に戻る
    // 3. Basic guarantee: 例外時もリソースリークなし、不変条件を維持
    // 4. No guarantee: 何も保証しない

    cout << R"(
  1. No-throw guarantee
     - 例外を投げない (noexcept)
     - 例: デストラクタ、ムーブ操作

  2. Strong guarantee
     - 例外時は操作前の状態に戻る
     - 例: copy-and-swap idiom

  3. Basic guarantee
     - 例外時もリソースリークなし
     - 不変条件を維持
     - 例: 多くの STL 操作

  4. No guarantee
     - 何も保証しない
     - 避けるべき
)" << endl;
}

int main() {
    cout << "=== Error Handling Demo ===" << endl << endl;

    exception_basics();
    standard_exceptions();
    custom_exceptions();
    noexcept_demo();
    raii_exception_safety();
    optional_demo();
    variant_as_result();
    expected_demo();
    error_code_demo();
    assert_demo();
    source_location_demo();
    exception_safety_levels();

    return 0;
}
