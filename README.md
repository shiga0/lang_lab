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
│   ├── ruby/                    # Ruby
│   ├── go/                      # Go
│   ├── typescript/              # TypeScript
│   └── python/                  # Python
│
├── docs/comparisons/            # 比較ドキュメント
│
└── challenges/                  # 同じ課題を各言語で解く
```

## 比較表

| 観点 | Rust | Ruby | Go | TypeScript | Python |
|------|------|------|-----|------------|--------|
| 登場年 | 2015 | 1995 | 2009 | 2012 | 1991 |
| パラダイム | マルチ (手続き/OOP/FP) | マルチ (OOP/FP) | マルチ (手続き/並行) | マルチ (OOP/FP) | マルチ (OOP/FP/手続き) |
| 型システム | 静的・強い | 動的・強い | 静的・強い | 静的・構造的 | 動的・強い (型ヒント) |
| 型推論 | あり | - (動的) | あり (:=) | あり | - (動的) |
| ジェネリクス | あり (trait bounds) | - (Duck typing) | あり (Go 1.18+) | あり | TypeVar, Generic |
| メモリ管理 | 所有権システム | GC | 並行GC | GC (V8) | 参照カウント + GC |
| Null安全 | Option<T> | nil (安全ではない) | nil (ゼロ値) | strictNullChecks | None (Optional) |
| エラー処理 | Result<T, E> | 例外 | 多値戻り (error) | 例外 / Result | 例外 |
| 並行処理 | async/await, スレッド | Thread, Fiber, Ractor | goroutine, channel | async/await, Promise | asyncio, threading, GIL |
| メタプログラミング | マクロ (衛生的) | 強力 (eval, define_method等) | reflect, go generate | 型レベル, デコレータ | 強力 (metaclass, decorator) |
| 実行形式 | コンパイル (ネイティブ) | インタプリタ | コンパイル (ネイティブ) | トランスパイル (JS) | インタプリタ (バイトコード) |
| パッケージ管理 | Cargo (crates.io) | Bundler (RubyGems) | go mod (proxy.golang.org) | npm (npmjs.com) | pip (PyPI) |
| 主な用途 | システム, CLI, WebAssembly | Web, スクリプト, DevOps | クラウド, CLI, インフラ | Web, Node.js, フルスタック | データ科学, Web, 自動化 |

## 学び方

### ステップ 1: 概念を理解する（concepts/）

新しい言語を学ぶとき、まず概念ごとに動作を確認する：

```bash
# 例: Rust のメモリ管理を学ぶ
cd concepts/memory/rust
cargo run        # 実行して動作を見る
cat src/main.rs  # コードを読んで理解する
```

各概念ディレクトリには、その言語での実装例とコメントによる解説がある。

### ステップ 2: 比較ドキュメントで俯瞰する（docs/comparisons/）

複数言語の違いを比較表で確認：

```bash
cat docs/comparisons/memory-models.md
```

- Rust → 所有権システム（コンパイル時チェック）
- Ruby → GC（Mark & Sweep）
- Go → 並行GC（低レイテンシ）

...という違いが一目でわかる。

### ステップ 3: チャレンジで手を動かす（challenges/）

同じ課題を違う言語で実装して、違いを体感する：

```bash
# Rust 版の連結リストを読む
cat challenges/02_linked_list/rust/src/main.rs

# 自分で Ruby 版を実装してみる
mkdir -p challenges/02_linked_list/ruby
vim challenges/02_linked_list/ruby/linked_list.rb
```

### ステップ 4: 新しい言語を追加する

新しい言語を学ぶ最良の方法は、このリポジトリに追加すること：

1. **チートシート作成**: `languages/<lang>/cheatsheet.md`
2. **概念を実装**: `concepts/*/<lang>/` を埋めていく
3. **チャレンジ実装**: `challenges/*/<lang>/`
4. **比較ドキュメント更新**: `docs/comparisons/*.md` に列を追加

### 推奨する学習フロー

```
新言語を学ぶとき:

1. チートシートを作る
   └─ 基礎文法を整理（変数、関数、制御構文）

2. 概念を順番に実装
   └─ type_system → memory → error_handling → concurrency → ...

3. チャレンジで実践
   └─ fizzbuzz → linked_list → http_server → ...

4. 比較表を更新
   └─ 他言語との違いを言語化して記録
```

### 学習のコツ

- **実行してから読む**: まず `cargo run` で動作を見て、その後コードを読む
- **コメントを活用**: 各実装には日本語コメントで解説がある
- **比較で覚える**: 「Ruby ではこう書くけど、Rust ではこう」という形で記憶
- **書いて覚える**: 読むだけでなく、自分で実装を追加する

## ドキュメント

- [型システム比較](docs/comparisons/type-systems.md)
- [メモリモデル比較](docs/comparisons/memory-models.md)
- [並行処理比較](docs/comparisons/concurrency.md)
- [構文比較](docs/comparisons/syntax.md)
- [ツールチェーン比較](docs/comparisons/tooling.md)

## 対応言語

- [x] Rust
- [x] Ruby
- [x] Go
- [x] TypeScript
- [x] Python
- [ ] Zig
- [ ] ...

## クイックスタート

```bash
# Rust: 概念を確認
cd concepts/type_system/rust
cargo run

# Ruby: 概念を確認
cd concepts/type_system/ruby
ruby type_system.rb

# Rust: チャレンジを実行
cd challenges/01_fizzbuzz/rust
cargo run

# Ruby: チャレンジを実行
cd challenges/01_fizzbuzz/ruby
ruby fizzbuzz.rb

# Go: 概念を確認
cd concepts/type_system/go
go run .

# Go: チャレンジを実行
cd challenges/01_fizzbuzz/go
go run .

# TypeScript: 概念を確認 (tsx が必要)
cd concepts/type_system/typescript
npx tsx type_system.ts

# TypeScript: チャレンジを実行
cd challenges/01_fizzbuzz/typescript
npx tsx fizzbuzz.ts

# Python: 概念を確認
cd concepts/type_system/python
python type_system.py

# Python: チャレンジを実行
cd challenges/01_fizzbuzz/python
python fizzbuzz.py
```
