# Ruby チートシート

## 基本

### 変数
```ruby
# ローカル変数
name = "Ruby"
age = 29

# インスタンス変数
@name = "instance"

# クラス変数
@@count = 0

# グローバル変数 (非推奨)
$global = "avoid"

# 定数
MAX_SIZE = 100
```

### 型
```ruby
# 数値
42          # Integer
3.14        # Float
1_000_000   # 区切り文字

# 文字列
"hello"           # String
'literal'         # 式展開なし
"Hello, #{name}"  # 式展開

# シンボル
:symbol
:hello

# 配列
[1, 2, 3]
%w[apple banana cherry]  # => ["apple", "banana", "cherry"]

# ハッシュ
{ name: "Ruby", year: 1995 }
{ "key" => "value" }

# 範囲
1..5    # 1, 2, 3, 4, 5 (含む)
1...5   # 1, 2, 3, 4 (含まない)

# nil と真偽値
nil
true
false
```

## 制御構文

### 条件分岐
```ruby
# if
if x > 0
  puts "positive"
elsif x < 0
  puts "negative"
else
  puts "zero"
end

# 後置 if
puts "positive" if x > 0

# unless
puts "not zero" unless x == 0

# 三項演算子
result = x > 0 ? "positive" : "non-positive"

# case
case value
when 1
  "one"
when 2, 3
  "two or three"
when 4..10
  "four to ten"
else
  "other"
end

# パターンマッチング (Ruby 3.0+)
case point
in { x: 0, y: }
  puts "on y-axis at #{y}"
in { x:, y: 0 }
  puts "on x-axis at #{x}"
in { x:, y: }
  puts "at (#{x}, #{y})"
end
```

### ループ
```ruby
# each (推奨)
[1, 2, 3].each { |x| puts x }

# times
5.times { |i| puts i }

# upto / downto
1.upto(5) { |i| puts i }
5.downto(1) { |i| puts i }

# while
while condition
  # ...
end

# until
until done
  # ...
end

# loop
loop do
  break if done
end
```

## メソッド

### 定義
```ruby
def greet(name)
  "Hello, #{name}!"
end

# デフォルト引数
def greet(name = "World")
  "Hello, #{name}!"
end

# キーワード引数
def create_user(name:, age:, admin: false)
  { name: name, age: age, admin: admin }
end

create_user(name: "Alice", age: 30)

# 可変長引数
def sum(*numbers)
  numbers.sum
end

# ブロック引数
def with_logging(&block)
  puts "Start"
  result = block.call
  puts "End"
  result
end
```

### ラムダとProc
```ruby
# ラムダ
add = ->(a, b) { a + b }
add.call(1, 2)  # => 3

# Proc
double = Proc.new { |x| x * 2 }
double.call(5)  # => 10

# &: 記法
[1, 2, 3].map(&:to_s)  # => ["1", "2", "3"]
```

## クラスとモジュール

### クラス
```ruby
class User
  # クラス変数
  @@count = 0

  # アクセサ
  attr_reader :name       # getter のみ
  attr_writer :email      # setter のみ
  attr_accessor :age      # 両方

  # コンストラクタ
  def initialize(name, age)
    @name = name
    @age = age
    @@count += 1
  end

  # インスタンスメソッド
  def greet
    "Hello, I'm #{@name}"
  end

  # クラスメソッド
  def self.count
    @@count
  end

  # private メソッド
  private

  def secret
    "private method"
  end
end

user = User.new("Alice", 30)
user.greet           # => "Hello, I'm Alice"
User.count           # => 1
```

### 継承
```ruby
class Admin < User
  def initialize(name, age, level)
    super(name, age)
    @level = level
  end

  def greet
    "#{super}, I'm an admin"
  end
end
```

### モジュール
```ruby
# 名前空間
module MyApp
  class User
  end
end

user = MyApp::User.new

# ミックスイン
module Loggable
  def log(message)
    puts "[LOG] #{message}"
  end
end

class Server
  include Loggable  # インスタンスメソッドとして追加
end

Server.new.log("Started")

# extend
class Client
  extend Loggable   # クラスメソッドとして追加
end

Client.log("Connected")
```

## イテレータとEnumerable

```ruby
arr = [1, 2, 3, 4, 5]

# 変換
arr.map { |x| x * 2 }       # => [2, 4, 6, 8, 10]
arr.flat_map { |x| [x, x] } # => [1, 1, 2, 2, ...]

# フィルタ
arr.select { |x| x > 2 }    # => [3, 4, 5]
arr.reject { |x| x > 2 }    # => [1, 2]
arr.find { |x| x > 2 }      # => 3

# 畳み込み
arr.reduce(0) { |sum, x| sum + x }  # => 15
arr.sum                              # => 15

# 判定
arr.any? { |x| x > 3 }      # => true
arr.all? { |x| x > 0 }      # => true
arr.none? { |x| x < 0 }     # => true

# ソート
arr.sort                    # => [1, 2, 3, 4, 5]
arr.sort_by { |x| -x }      # => [5, 4, 3, 2, 1]

# グループ化
arr.group_by { |x| x % 2 }  # => {1=>[1, 3, 5], 0=>[2, 4]}

# チェーン
arr.select { |x| x > 2 }
   .map { |x| x * 2 }
   .sum                     # => 24
```

## エラー処理

```ruby
# begin-rescue-ensure
begin
  risky_operation
rescue ArgumentError => e
  puts "Argument error: #{e.message}"
rescue StandardError => e
  puts "Error: #{e.message}"
ensure
  cleanup
end

# raise
raise "Something went wrong"
raise ArgumentError, "Invalid value"

# カスタム例外
class MyError < StandardError; end

raise MyError, "Custom error"
```

## ファイル操作

```ruby
# 読み込み
content = File.read("file.txt")
lines = File.readlines("file.txt")

# ブロック形式 (自動クローズ)
File.open("file.txt", "r") do |f|
  f.each_line { |line| puts line }
end

# 書き込み
File.write("file.txt", "content")

File.open("file.txt", "w") do |f|
  f.puts "Hello"
end
```

## 便利メソッド

```ruby
# 文字列
"hello".upcase          # => "HELLO"
"  hello  ".strip       # => "hello"
"hello".chars           # => ["h", "e", "l", "l", "o"]
"a,b,c".split(",")      # => ["a", "b", "c"]
["a", "b"].join("-")    # => "a-b"

# 配列
[1, 2, 3].first         # => 1
[1, 2, 3].last          # => 3
[1, 2, 3].take(2)       # => [1, 2]
[1, 2, 3].drop(1)       # => [2, 3]
[1, 2] + [3, 4]         # => [1, 2, 3, 4]
[1, 2, 2, 3].uniq       # => [1, 2, 3]
[1, 2, 3].include?(2)   # => true

# ハッシュ
h = { a: 1, b: 2 }
h.keys                  # => [:a, :b]
h.values                # => [1, 2]
h.merge(c: 3)           # => {a: 1, b: 2, c: 3}
h.slice(:a)             # => {a: 1}
h.transform_values { |v| v * 2 }  # => {a: 2, b: 4}
```

## Ruby 3.x 新機能

```ruby
# Ractor (並列処理)
ractor = Ractor.new do
  Ractor.receive * 2
end
ractor.send(21)
ractor.take  # => 42

# パターンマッチング
case [1, 2, 3]
in [first, *rest]
  puts "first: #{first}, rest: #{rest}"
end

# 1行メソッド定義
def double(x) = x * 2

# ハッシュの省略記法
x = 1
y = 2
{ x:, y: }  # => { x: 1, y: 2 }
```
