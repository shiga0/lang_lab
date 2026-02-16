# ツールチェイン比較

## 概要

| 言語 | パッケージマネージャ | ビルドツール | フォーマッタ | リンター |
|------|---------------------|-------------|-------------|---------|
| Rust | Cargo | Cargo | rustfmt | Clippy |
| Ruby | RubyGems/Bundler | Rake | RuboCop | RuboCop |
| Go | go mod | go build | gofmt | golint/staticcheck |
| Node.js | npm/yarn/pnpm | webpack/vite | Prettier | ESLint |
| Python | pip/poetry | setuptools | Black | Ruff/Flake8 |
| C++ | vcpkg/Conan | CMake/Make | clang-format | clang-tidy |

## Rust: Cargo

### 基本コマンド
```bash
cargo new project_name    # 新規プロジェクト
cargo build              # ビルド
cargo build --release    # リリースビルド
cargo run                # 実行
cargo test               # テスト
cargo check              # 高速な型チェック
cargo doc --open         # ドキュメント生成
```

### Cargo.toml
```toml
[package]
name = "my_project"
version = "0.1.0"
edition = "2021"

[dependencies]
serde = { version = "1.0", features = ["derive"] }
tokio = { version = "1", features = ["full"] }

[dev-dependencies]
criterion = "0.5"
```

### ワークスペース
```toml
[workspace]
members = ["crate1", "crate2"]
```

## Ruby: Bundler

### 基本コマンド
```bash
bundle init           # Gemfile 作成
bundle install        # 依存関係インストール
bundle exec rake      # Bundler 環境で実行
bundle update         # 依存関係更新
```

### Gemfile
```ruby
source 'https://rubygems.org'

gem 'rails', '~> 7.0'
gem 'pg'

group :development, :test do
  gem 'rspec-rails'
  gem 'rubocop'
end
```

## Go: go mod

### 基本コマンド
```bash
go mod init module_name  # モジュール初期化
go build                 # ビルド
go run .                 # 実行
go test ./...            # テスト
go get package@version   # 依存追加
go mod tidy              # 依存整理
```

### go.mod
```
module example.com/my_project

go 1.21

require (
    github.com/gin-gonic/gin v1.9.1
)
```

## C++: CMake

### 基本コマンド
```bash
cmake -B build           # ビルドディレクトリ生成
cmake --build build      # ビルド
cmake --build build --config Release  # リリースビルド
ctest --test-dir build   # テスト実行
```

### CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.20)
project(my_project CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main src/main.cpp)

# テスト
enable_testing()
add_executable(tests tests/test_main.cpp)
add_test(NAME tests COMMAND tests)
```

## テスト

### Rust
```rust
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_add() {
        assert_eq!(add(2, 3), 5);
    }

    #[test]
    #[should_panic]
    fn test_panic() {
        panic!("expected");
    }
}
```

### Ruby (RSpec)
```ruby
RSpec.describe Calculator do
  describe '#add' do
    it 'adds two numbers' do
      expect(Calculator.add(2, 3)).to eq(5)
    end
  end
end
```

### Go
```go
func TestAdd(t *testing.T) {
    result := Add(2, 3)
    if result != 5 {
        t.Errorf("Add(2, 3) = %d; want 5", result)
    }
}
```

### C++ (Google Test)
```cpp
#include <gtest/gtest.h>

TEST(CalculatorTest, Add) {
    EXPECT_EQ(add(2, 3), 5);
}

TEST(CalculatorTest, AddNegative) {
    EXPECT_EQ(add(-1, 1), 0);
}
```

## フォーマッタ

### Rust: rustfmt
```bash
cargo fmt              # フォーマット
cargo fmt --check      # チェックのみ
```

### Ruby: RuboCop
```bash
rubocop                # チェック
rubocop -a             # 自動修正
```

### Go: gofmt
```bash
gofmt -w .             # フォーマット
```

### C++: clang-format
```bash
clang-format -i src/*.cpp    # フォーマット
clang-format --dry-run src/*.cpp  # チェックのみ
```

## リンター

### Rust: Clippy
```bash
cargo clippy                    # リント
cargo clippy -- -D warnings     # 警告をエラーに
```

### Ruby: RuboCop
```bash
rubocop                # リント
rubocop --auto-gen-config  # 設定生成
```

### Go: staticcheck
```bash
staticcheck ./...
```

### C++: clang-tidy
```bash
clang-tidy src/*.cpp -- -std=c++20    # リント
clang-tidy src/*.cpp --fix -- -std=c++20  # 自動修正
```

## ベンチマーク

### Rust (Criterion)
```rust
use criterion::{black_box, criterion_group, criterion_main, Criterion};

fn fibonacci_benchmark(c: &mut Criterion) {
    c.bench_function("fib 20", |b| b.iter(|| fibonacci(black_box(20))));
}

criterion_group!(benches, fibonacci_benchmark);
criterion_main!(benches);
```

### Ruby (Benchmark)
```ruby
require 'benchmark'

Benchmark.bm do |x|
  x.report("fib:") { fibonacci(20) }
end
```

### Go
```go
func BenchmarkFibonacci(b *testing.B) {
    for i := 0; i < b.N; i++ {
        Fibonacci(20)
    }
}
```

### C++ (Google Benchmark)
```cpp
#include <benchmark/benchmark.h>

static void BM_Fibonacci(benchmark::State& state) {
    for (auto _ : state) {
        fibonacci(20);
    }
}
BENCHMARK(BM_Fibonacci);

BENCHMARK_MAIN();
```

## 詳細

- [languages/rust/](../../languages/rust/)
- [languages/ruby/](../../languages/ruby/)
- [languages/go/](../../languages/go/)
- [languages/typescript/](../../languages/typescript/)
- [languages/python/](../../languages/python/)
- [languages/cpp/](../../languages/cpp/)
