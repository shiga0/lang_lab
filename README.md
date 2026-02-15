# lang_lab

プログラミング言語の仕組みを比較・学習するためのリファレンス集。

## 概要

新しい言語を学ぶ時、既知の言語と比較することで理解が深まる。
このリポジトリは、異なる言語の概念・構文・パラダイムを横断的に比較できる環境を提供する。

## 構成

```
lang_lab/
├── concepts/                    # 概念別の実装例
│   ├── type_system/             # 型システム
│   ├── memory/                  # メモリ管理
│   ├── concurrency/             # 並行処理
│   ├── error_handling/          # エラー処理
│   ├── metaprogramming/         # メタプログラミング
│   ├── oop/                     # オブジェクト指向
│   ├── functional/              # 関数型パラダイム
│   └── data_structures/         # データ構造
│
├── languages/                   # 言語別リファレンス
│   ├── rust/                    # Rust
│   └── ruby/                    # Ruby
│
├── docs/comparisons/            # 比較ドキュメント
│
└── challenges/                  # 同じ課題を各言語で解く
```

## 比較表

| 観点 | Rust | Ruby |
|------|------|------|
| 登場年 | 2015 | 1995 |
| パラダイム | マルチ (手続き/OOP/FP) | マルチ (OOP/FP) |
| 型システム | 静的・強い | 動的・強い |
| 型推論 | あり | - (動的) |
| ジェネリクス | あり (trait bounds) | - (Duck typing) |
| メモリ管理 | 所有権システム | GC |
| Null安全 | Option<T> | nil (安全ではない) |
| エラー処理 | Result<T, E> | 例外 |
| 並行処理 | async/await, スレッド | Thread, Fiber, Ractor |
| メタプログラミング | マクロ (衛生的) | 強力 (eval, define_method等) |
| 実行形式 | コンパイル (ネイティブ) | インタプリタ |
| パッケージ管理 | Cargo (crates.io) | Bundler (RubyGems) |
| 主な用途 | システム, CLI, WebAssembly | Web, スクリプト, DevOps |

## 学習の進め方

### 1. 概念から入る
「並行処理ってどう違う？」→ `concepts/concurrency/` を見る

### 2. 言語リファレンス
「Rustの構文を思い出したい」→ `languages/rust/cheatsheet.md`

### 3. 同じ問題を解く
「実際に書くと違いが分かる」→ `challenges/` で同じ課題を各言語で実装

## ドキュメント

- [型システム比較](docs/comparisons/type-systems.md)
- [メモリモデル比較](docs/comparisons/memory-models.md)
- [並行処理比較](docs/comparisons/concurrency.md)
- [構文比較](docs/comparisons/syntax.md)
- [ツールチェーン比較](docs/comparisons/tooling.md)

## 対応言語

- [x] Rust
- [ ] Ruby
- [ ] Go
- [ ] TypeScript
- [ ] Python
- [ ] Zig
- [ ] ...

## クイックスタート

```bash
# 特定の概念を確認
cd concepts/type_system/rust
cargo run

# チャレンジを実行
cd challenges/01_fizzbuzz/rust
cargo run
```
