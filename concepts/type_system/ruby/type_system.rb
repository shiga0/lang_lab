#!/usr/bin/env ruby
# frozen_string_literal: true

# 型システム - Ruby 実装
#
# Ruby は動的型付け言語
# 型チェックは実行時に行われる

puts "=== Type System Demo ===\n\n"

# --- 基本型 ---
puts "--- 基本型 ---"

# 数値
integer = 42
float = 3.14
puts "Integer: #{integer} (#{integer.class})"
puts "Float: #{float} (#{float.class})"

# 文字列
string = "Hello, Ruby!"
puts "String: #{string} (#{string.class})"

# シンボル (イミュータブルな識別子)
symbol = :hello
puts "Symbol: #{symbol} (#{symbol.class})"

# 真偽値
bool_true = true
bool_false = false
puts "Boolean: #{bool_true} (#{bool_true.class}), #{bool_false} (#{bool_false.class})"

# nil (Rust の None に相当)
nothing = nil
puts "Nil: #{nothing.inspect} (#{nothing.class})"

puts

# --- コレクション ---
puts "--- コレクション ---"

# 配列 (任意の型を混在可能)
array = [1, "two", :three, 4.0]
puts "Array: #{array} (#{array.class})"

# ハッシュ
hash = { name: "Ruby", year: 1995, dynamic: true }
puts "Hash: #{hash} (#{hash.class})"

# 範囲
range = 1..5
puts "Range: #{range} (#{range.class})"
puts "  to_a: #{range.to_a}"

puts

# --- Duck Typing ---
puts "--- Duck Typing ---"

# each メソッドがあれば何でも処理できる
def process_each(obj)
  result = []
  obj.each { |x| result << x }
  result
end

puts "Array: #{process_each([1, 2, 3])}"
puts "Range: #{process_each(1..3)}"
puts "Hash:  #{process_each({ a: 1, b: 2 })}"

puts

# --- 動的型付けの例 ---
puts "--- 動的型付け ---"

# 同じ変数に異なる型を代入可能
x = 42
puts "x = #{x} (#{x.class})"

x = "now a string"
puts "x = #{x} (#{x.class})"

x = [1, 2, 3]
puts "x = #{x} (#{x.class})"

puts

# --- 型の強さ ---
puts "--- 強い型付け ---"

# Ruby は強い型付け (暗黙の型変換をしない)
begin
  result = 1 + "2"  # TypeError
rescue TypeError => e
  puts "1 + \"2\" => TypeError: #{e.message}"
end

# 明示的な変換が必要
puts "1 + \"2\".to_i => #{1 + "2".to_i}"
puts "1.to_s + \"2\" => #{1.to_s + "2"}"

puts

# --- 型チェックメソッド ---
puts "--- 型チェック ---"

value = "hello"

# クラスチェック
puts "value.class => #{value.class}"
puts "value.is_a?(String) => #{value.is_a?(String)}"
puts "value.is_a?(Object) => #{value.is_a?(Object)}"
puts "value.kind_of?(String) => #{value.kind_of?(String)}"
puts "value.instance_of?(String) => #{value.instance_of?(String)}"

# respond_to? (Duck Typing の基本)
puts "value.respond_to?(:upcase) => #{value.respond_to?(:upcase)}"
puts "value.respond_to?(:nonexistent) => #{value.respond_to?(:nonexistent)}"

puts

# --- Struct (軽量なデータ構造) ---
puts "--- Struct ---"

# Rust の struct に近い
Point = Struct.new(:x, :y) do
  def distance_from_origin
    Math.sqrt(x**2 + y**2)
  end
end

point = Point.new(3, 4)
puts "Point: #{point}"
puts "Distance: #{point.distance_from_origin}"

puts

# --- Data クラス (Ruby 3.2+, イミュータブル) ---
puts "--- Data (Ruby 3.2+) ---"

if RUBY_VERSION >= "3.2"
  # イミュータブルな値オブジェクト
  Person = Data.define(:name, :age)
  person = Person.new(name: "Alice", age: 30)
  puts "Person: #{person}"
else
  puts "(Ruby #{RUBY_VERSION} - Data クラスは Ruby 3.2+ で利用可能)"
end

puts

# --- RBS による型注釈 (外部ファイル) ---
puts "--- RBS (型注釈) ---"
puts <<~RBS
  # RBS ファイルでは型注釈を書ける (type_system.rbs)
  #
  # class User
  #   attr_reader name: String
  #   attr_reader age: Integer
  #
  #   def initialize: (name: String, age: Integer) -> void
  #   def greet: () -> String
  # end
  #
  # steep や sorbet で型チェック可能
RBS
