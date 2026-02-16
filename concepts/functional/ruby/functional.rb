#!/usr/bin/env ruby
# frozen_string_literal: true

# 関数型プログラミング - Ruby 実装
#
# Ruby はマルチパラダイムで、関数型スタイルもサポート

puts "=== Functional Programming Demo ===\n\n"

# --- ブロック ---
puts "--- Blocks ---"

# do...end または {}
[1, 2, 3].each do |x|
  puts "  #{x}"
end

# 1行なら {}
result = [1, 2, 3].map { |x| x * 2 }
puts "map { |x| x * 2 } => #{result}"

# yield でブロックを呼び出す
def with_timing
  start = Time.now
  result = yield
  elapsed = Time.now - start
  puts "  Elapsed: #{elapsed.round(4)}s"
  result
end

with_timing { sleep(0.1) }

puts

# --- Proc と Lambda ---
puts "--- Proc and Lambda ---"

# Proc
double = Proc.new { |x| x * 2 }
puts "Proc: double.call(5) => #{double.call(5)}"

# Lambda (-> 記法)
triple = ->(x) { x * 3 }
puts "Lambda: triple.call(5) => #{triple.call(5)}"

# Proc と Lambda の違い
puts "\nProc vs Lambda:"
puts "  1. 引数チェック: Lambda は厳密、Proc は緩い"
puts "  2. return の挙動: Lambda は自身から戻る、Proc は外側から戻る"

# & でブロックと相互変換
puts "\n&:method syntax:"
puts [1, 2, 3].map(&:to_s).inspect

puts

# --- 高階関数 ---
puts "--- Higher-Order Functions ---"

# map / collect
puts "map: #{[1, 2, 3].map { |x| x * 2 }}"

# select / filter
puts "select: #{[1, 2, 3, 4, 5].select { |x| x.even? }}"

# reject
puts "reject: #{[1, 2, 3, 4, 5].reject { |x| x.even? }}"

# reduce / inject
puts "reduce: #{[1, 2, 3, 4, 5].reduce(0) { |sum, x| sum + x }}"

# find / detect
puts "find: #{[1, 2, 3, 4, 5].find { |x| x > 3 }}"

# all? / any? / none?
puts "all? even: #{[2, 4, 6].all?(&:even?)}"
puts "any? > 5: #{[1, 2, 3].any? { |x| x > 5 }}"

# flat_map
puts "flat_map: #{[[1, 2], [3, 4]].flat_map { |arr| arr.map { |x| x * 2 } }}"

# partition
evens, odds = [1, 2, 3, 4, 5].partition(&:even?)
puts "partition: evens=#{evens}, odds=#{odds}"

# group_by
grouped = [1, 2, 3, 4, 5].group_by { |x| x % 2 == 0 ? :even : :odd }
puts "group_by: #{grouped}"

puts

# --- メソッドチェーン ---
puts "--- Method Chaining ---"

result = (1..10)
  .select { |x| x > 3 }
  .map { |x| x * 2 }
  .take(3)
  .reduce(:+)

puts "(1..10).select { > 3 }.map { * 2 }.take(3).reduce(:+) => #{result}"

# then / yield_self (パイプライン的)
result = "hello"
  .then { |s| s.upcase }
  .then { |s| "#{s}!" }
  .then { |s| s * 3 }

puts "'hello'.then... => #{result}"

puts

# --- Enumerable::Lazy ---
puts "--- Lazy Evaluation ---"

# 無限シーケンスを扱える
infinite = (1..).lazy
  .map { |x| x * 2 }
  .select { |x| x % 3 == 0 }

puts "First 5 from infinite lazy sequence:"
puts "  #{infinite.take(5).force}"

# パフォーマンス: 中間配列を作らない
large_result = (1..1_000_000).lazy
  .map { |x| x * 2 }
  .select { |x| x % 100 == 0 }
  .take(3)
  .force

puts "Lazy on large range: #{large_result}"

puts

# --- カリー化と部分適用 ---
puts "--- Currying and Partial Application ---"

# カリー化
add = ->(a, b, c) { a + b + c }
curried = add.curry

puts "Curried: add.curry.(1).(2).(3) => #{curried.(1).(2).(3)}"

add_five = curried.(5)
puts "Partial: curried.(5).(10).(20) => #{add_five.(10).(20)}"

# 部分適用 (curry なし)
multiply = ->(x, y) { x * y }
double = multiply.curry.(2)
puts "Double: #{[1, 2, 3].map(&double)}"

puts

# --- クロージャ ---
puts "--- Closures ---"

def counter
  count = 0
  -> { count += 1 }
end

c = counter
puts "Counter: #{c.call}, #{c.call}, #{c.call}"

# 各呼び出しで独立したカウンター
c1 = counter
c2 = counter
puts "c1: #{c1.call}, #{c1.call}"
puts "c2: #{c2.call}"

puts

# --- パターンマッチング (Ruby 3.0+) ---
puts "--- Pattern Matching (Ruby 3.0+) ---"

def describe(obj)
  case obj
  in []
    "empty array"
  in [x]
    "single element: #{x}"
  in [x, *rest]
    "head: #{x}, tail: #{rest}"
  in { name:, age: }
    "person: #{name}, #{age} years old"
  in Integer => n if n > 100
    "large number: #{n}"
  in Integer => n
    "number: #{n}"
  else
    "unknown"
  end
end

puts "describe([]) => #{describe([])}"
puts "describe([1]) => #{describe([1])}"
puts "describe([1, 2, 3]) => #{describe([1, 2, 3])}"
puts "describe({name: 'Alice', age: 30}) => #{describe({ name: 'Alice', age: 30 })}"
puts "describe(42) => #{describe(42)}"
puts "describe(999) => #{describe(999)}"

puts

# --- イミュータブルなスタイル ---
puts "--- Immutable Style ---"

# frozen で変更を防ぐ
arr = [1, 2, 3].freeze
begin
  arr << 4
rescue FrozenError
  puts "Cannot modify frozen array"
end

# 変換して新しいオブジェクトを返す
original = [1, 2, 3]
modified = original + [4]  # 新しい配列
puts "Original: #{original}, Modified: #{modified}"

# ハッシュも同様
h1 = { a: 1, b: 2 }.freeze
h2 = h1.merge(c: 3)  # 新しいハッシュ
puts "h1: #{h1}, h2: #{h2}"
