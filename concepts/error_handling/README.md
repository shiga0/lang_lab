# エラー処理比較

## 概要

| 言語 | 方式 | 回復可能 | 回復不可能 | 特徴 |
|------|------|----------|-----------|------|
| Rust | Result/Option | Result<T, E> | panic! | 明示的、型安全 |
| Ruby | 例外 | rescue | raise | 暗黙的、柔軟 |
| Go | 多値返却 | error | panic/recover | 明示的、シンプル |
| Java | 例外 | try/catch | throw | 検査例外あり |

## Rust のエラー処理

### Result<T, E>
- 成功時: `Ok(T)`
- 失敗時: `Err(E)`
- `?` 演算子でエラー伝播
- パターンマッチングで処理

### Option<T>
- 値あり: `Some(T)`
- 値なし: `None`
- null/nil の代わり

### panic!
- 回復不可能なエラー
- スタックアンワインド（または即終了）
- 通常は使わない

## Ruby の例外処理

### 例外クラス階層
```
Exception
├── StandardError (rescue のデフォルト)
│   ├── RuntimeError
│   ├── ArgumentError
│   ├── TypeError
│   └── ...
└── SystemExit, SignalException, ...
```

### begin/rescue/ensure
```ruby
begin
  risky_operation
rescue SpecificError => e
  handle_error(e)
rescue => e  # StandardError
  generic_handler(e)
ensure
  cleanup  # 必ず実行
end
```

## 比較: 明示的 vs 暗黙的

```rust
// Rust: 呼び出し側が必ず処理
fn read_file(path: &str) -> Result<String, io::Error> {
    std::fs::read_to_string(path)
}

// 使う側
let content = read_file("test.txt")?;  // エラーなら早期リターン
```

```ruby
# Ruby: 処理しなくてもコンパイルは通る
def read_file(path)
  File.read(path)  # 例外が発生する可能性
end

# 使う側 (rescue なしでも動く)
content = read_file("test.txt")  # 失敗したら例外が上に伝播
```

Rust は「エラーを無視できない」設計。
Ruby は「必要な時だけ処理」の柔軟性。
