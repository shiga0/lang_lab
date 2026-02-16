# C++

## 概要

C++ は1983年に Bjarne Stroustrup によって開発された、C言語を拡張したマルチパラダイム言語。
システムプログラミング、ゲーム開発、組み込みシステム、高性能アプリケーションで広く使用される。

## 特徴

| 項目 | 内容 |
|------|------|
| 登場年 | 1983 (標準化 1998) |
| 設計者 | Bjarne Stroustrup |
| パラダイム | マルチ（手続き型、OOP、ジェネリック、関数型） |
| 型システム | 静的・強い型付け |
| メモリ管理 | 手動 + RAII + スマートポインタ |
| 実行形式 | コンパイル（ネイティブ） |
| 主な用途 | システム、ゲーム、組み込み、HPC |

## 他言語との比較

### vs Rust
- **共通点**: ゼロコスト抽象化、ネイティブコンパイル、メモリ制御
- **C++ の特徴**: 歴史的資産、柔軟性（危険も許容）、成熟したエコシステム
- **Rust の特徴**: メモリ安全性保証、所有権システム、モダンなツールチェーン

### vs Go
- **共通点**: ネイティブコンパイル、高性能
- **C++ の特徴**: より低レベルな制御、テンプレートメタプログラミング
- **Go の特徴**: シンプルさ、GC、goroutine

### vs Python
- **共通点**: マルチパラダイム
- **C++ の特徴**: 高速実行、メモリ制御、コンパイル時チェック
- **Python の特徴**: 動的型付け、短い開発サイクル、豊富なライブラリ

## バージョン履歴

| バージョン | 年 | 主な機能 |
|-----------|-----|---------|
| C++98 | 1998 | 最初の標準化、STL |
| C++03 | 2003 | バグ修正 |
| C++11 | 2011 | auto、ラムダ、スマートポインタ、move |
| C++14 | 2014 | ジェネリックラムダ、constexpr拡張 |
| C++17 | 2017 | optional、variant、filesystem |
| C++20 | 2020 | concepts、ranges、coroutines、modules |
| C++23 | 2023 | std::expected、ranges拡張 |

## セットアップ

### macOS
```bash
# Xcode Command Line Tools（通常プリインストール）
xcode-select --install

# または Homebrew で最新版
brew install llvm
brew install gcc
```

### コンパイルと実行
```bash
# 基本
clang++ -std=c++20 main.cpp -o main && ./main

# 最適化あり
clang++ -std=c++20 -O2 main.cpp -o main

# 警告を厳格に
clang++ -std=c++20 -Wall -Wextra -Werror main.cpp -o main
```

## ツールチェーン

| ツール | 用途 |
|-------|------|
| clang++ / g++ | コンパイラ |
| CMake | ビルドシステム |
| clang-format | フォーマッタ |
| clang-tidy | リンター |
| Valgrind / ASan | メモリデバッグ |
| gdb / lldb | デバッガ |

## リソース

- [cppreference.com](https://en.cppreference.com/) - 最も信頼できるリファレンス
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/) - ベストプラクティス
- [Compiler Explorer (Godbolt)](https://godbolt.org/) - オンラインコンパイル・アセンブリ確認
