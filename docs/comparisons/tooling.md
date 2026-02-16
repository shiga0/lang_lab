# ツールチェイン比較

## 概要

| 言語 | パッケージマネージャ | ビルドツール | フォーマッタ | リンター |
|------|---------------------|-------------|-------------|---------|
| Rust | Cargo | Cargo | rustfmt | Clippy |
| Ruby | RubyGems/Bundler | Rake | RuboCop | RuboCop |
| Go | go mod | go build | gofmt | golint/staticcheck |
| Node.js | npm/yarn/pnpm | webpack/vite | Prettier | ESLint |
| Python | pip/poetry | setuptools | Black | Ruff/Flake8 |

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

## 詳細

- [languages/rust/](../../languages/rust/)
- [languages/ruby/](../../languages/ruby/)
- [languages/go/](../../languages/go/)
