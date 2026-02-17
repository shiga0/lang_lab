# C

## 概要

C は1972年に Dennis Ritchie によって開発された、手続き型プログラミング言語。
UNIX オペレーティングシステムの開発のために設計され、現在もシステムプログラミング、組み込みシステム、OS開発の基盤として広く使用される。

## 特徴

| 項目 | 内容 |
|------|------|
| 登場年 | 1972 (標準化 1989 - ANSI C) |
| 設計者 | Dennis Ritchie |
| パラダイム | 手続き型、構造化 |
| 型システム | 静的・弱い型付け |
| メモリ管理 | 完全手動（malloc/free） |
| 実行形式 | コンパイル（ネイティブ） |
| 主な用途 | OS、組み込み、ドライバ、言語処理系 |

## 他言語との比較

### vs C++
- **共通点**: 構文の類似性、ネイティブコンパイル、低レベル制御
- **C の特徴**: シンプル、小さな言語仕様、予測可能なコンパイル結果
- **C++ の特徴**: OOP、テンプレート、RAII、より強い型システム

### vs Rust
- **共通点**: システムプログラミング、ゼロコスト抽象化志向
- **C の特徴**: シンプルさ、広範な移植性、膨大な既存コード資産
- **Rust の特徴**: メモリ安全性保証、所有権システム、モダンな機能

### vs Go
- **共通点**: シンプルさの追求、ネイティブコンパイル
- **C の特徴**: より低レベルな制御、ランタイムなし
- **Go の特徴**: GC、goroutine、モダンなツールチェーン

## バージョン履歴

| バージョン | 年 | 主な機能 |
|-----------|-----|---------|
| K&R C | 1978 | オリジナル（The C Programming Language 初版） |
| C89/C90 | 1989/1990 | ANSI/ISO 標準化、プロトタイプ宣言 |
| C99 | 1999 | inline、可変長配列、// コメント、stdint.h |
| C11 | 2011 | _Generic、_Static_assert、マルチスレッド |
| C17 | 2017 | バグ修正（C11 の改訂） |
| C23 | 2023 | typeof、nullptr、改良された定数式 |

## セットアップ

### macOS
```bash
# Xcode Command Line Tools（通常プリインストール）
xcode-select --install

# または Homebrew で GCC
brew install gcc
```

### コンパイルと実行
```bash
# 基本
clang -std=c11 main.c -o main && ./main

# 最適化あり
clang -std=c11 -O2 main.c -o main

# 警告を厳格に
clang -std=c11 -Wall -Wextra -Werror main.c -o main

# GCC の場合
gcc -std=c11 -Wall -Wextra main.c -o main && ./main
```

## ツールチェーン

| ツール | 用途 |
|-------|------|
| clang / gcc | コンパイラ |
| make / CMake | ビルドシステム |
| clang-format | フォーマッタ |
| clang-tidy | リンター |
| Valgrind / ASan | メモリデバッグ |
| gdb / lldb | デバッガ |

## リソース

- [cppreference.com (C)](https://en.cppreference.com/w/c) - C言語リファレンス
- [The C Programming Language](https://en.wikipedia.org/wiki/The_C_Programming_Language) - K&R本
- [Compiler Explorer (Godbolt)](https://godbolt.org/) - オンラインコンパイル・アセンブリ確認
