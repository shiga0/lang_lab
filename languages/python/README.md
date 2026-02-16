# Python

## 概要

Python は読みやすさを重視した汎用プログラミング言語。
シンプルな構文、豊富なライブラリ、幅広い用途で人気。

## 特徴

| 項目 | 内容 |
|------|------|
| 登場年 | 1991 |
| 開発者 | Guido van Rossum |
| パラダイム | マルチパラダイム (OOP/FP/手続き) |
| 型システム | 動的・強い (型ヒントあり) |
| 実行環境 | インタプリタ (CPython, PyPy等) |
| パッケージ管理 | pip / poetry / uv |

## 他言語との比較

| 観点 | Python | Rust | Ruby | TypeScript |
|------|--------|------|------|------------|
| 型システム | 動的 (型ヒント) | 静的・強い | 動的・強い | 静的・構造的 |
| 実行速度 | 遅め | 高速 | 遅め | 中程度 |
| 学習曲線 | 緩やか | 急 | 緩やか | 中程度 |
| 並行処理 | GIL制限 | 安全・高速 | GVL制限 | イベントループ |
| メタプログラミング | デコレータ, メタクラス | マクロ | 強力 | 型レベル |
| 主な用途 | データ分析, AI/ML, Web | システム | Web | Web, フルスタック |

## Python の特徴

### インデントベースの構文

```python
def greet(name: str) -> str:
    if name:
        return f"Hello, {name}!"
    else:
        return "Hello, World!"
```

### 動的型付け + 型ヒント

```python
# 動的型付け
x = 42
x = "hello"  # OK

# 型ヒント (実行時には無視される)
def add(a: int, b: int) -> int:
    return a + b

# mypy などで静的チェック可能
```

### リスト内包表記

```python
# リスト内包表記
squares = [x**2 for x in range(10)]

# ジェネレータ式
lazy_squares = (x**2 for x in range(10))

# 辞書内包表記
word_lengths = {word: len(word) for word in ["hello", "world"]}
```

### デコレータ

```python
def log(func):
    def wrapper(*args, **kwargs):
        print(f"Calling {func.__name__}")
        return func(*args, **kwargs)
    return wrapper

@log
def greet(name):
    return f"Hello, {name}"
```

## セットアップ

```bash
# Python バージョン確認
python3 --version

# 仮想環境の作成 (推奨)
python3 -m venv .venv
source .venv/bin/activate  # Linux/macOS
# .venv\Scripts\activate   # Windows

# パッケージインストール
pip install package_name

# requirements.txt から一括インストール
pip install -r requirements.txt
```

## 実行方法

```bash
# スクリプト実行
python3 script.py

# モジュールとして実行
python3 -m module_name

# インタラクティブシェル
python3
>>> print("Hello")

# ワンライナー
python3 -c "print('Hello')"
```

## ファイル構成

```
project/
├── src/
│   ├── __init__.py
│   └── main.py
├── tests/
│   └── test_main.py
├── requirements.txt
├── pyproject.toml    # モダンな設定ファイル
└── README.md
```

## リソース

- [Python 公式ドキュメント](https://docs.python.org/ja/)
- [Real Python](https://realpython.com/)
- [Python チュートリアル](https://docs.python.org/ja/3/tutorial/)
