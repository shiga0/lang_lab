# Ruby

## 概要

| 項目 | 値 |
|------|-----|
| 登場年 | 1995 |
| 設計者 | まつもとゆきひろ (Matz) |
| パラダイム | オブジェクト指向, 関数型, 手続き型 |
| 型付け | 動的・強い |
| 実行形式 | インタプリタ |
| 主な用途 | Web (Rails), スクリプト, DevOps |

## 特徴

### 全てがオブジェクト
```ruby
1.class          # => Integer
"hello".class    # => String
nil.class        # => NilClass
true.class       # => TrueClass
```

### ブロックとイテレータ
```ruby
[1, 2, 3].each { |x| puts x }
[1, 2, 3].map { |x| x * 2 }     # => [2, 4, 6]
[1, 2, 3].select { |x| x > 1 }  # => [2, 3]
```

### メタプログラミング
```ruby
class Foo
  define_method(:hello) { "Hello!" }
end

Foo.new.hello  # => "Hello!"
```

### Duck Typing
```ruby
def process(obj)
  obj.each { |x| puts x }
  # each があれば何でも OK
end

process([1, 2, 3])
process({ a: 1, b: 2 })
```

## Rust との比較

| 観点 | Ruby | Rust |
|------|------|------|
| 型チェック | 実行時 | コンパイル時 |
| メモリ管理 | GC | 所有権 |
| Null | nil (unsafe) | Option<T> |
| エラー | 例外 | Result<T, E> |
| 速度 | 遅い | 速い |
| 記述量 | 少ない | 多い |
| 学習曲線 | 緩やか | 急 |

## 実装例

- [concepts/](../../concepts/) - 概念別の実装
- [challenges/](../../challenges/) - チャレンジ

## リンク

- [公式サイト](https://www.ruby-lang.org/ja/)
- [Ruby リファレンスマニュアル](https://docs.ruby-lang.org/ja/)
- [RubyGems](https://rubygems.org/)
