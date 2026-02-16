#!/usr/bin/env ruby
# frozen_string_literal: true

# メモリ管理 - Ruby 実装
#
# Ruby は GC (Garbage Collection) でメモリ管理
# Mark & Sweep アルゴリズム

puts "=== Memory Management Demo ===\n\n"

# --- 参照とオブジェクト ---
puts "--- 参照とオブジェクト ---"

# Ruby の変数はオブジェクトへの参照
a = "hello"
b = a  # 同じオブジェクトを参照

puts "a = #{a.inspect}, object_id: #{a.object_id}"
puts "b = #{b.inspect}, object_id: #{b.object_id}"
puts "a.object_id == b.object_id: #{a.object_id == b.object_id}"

# b を変更すると a も変わる (同じオブジェクト)
b.upcase!
puts "After b.upcase!:"
puts "  a = #{a.inspect}"
puts "  b = #{b.inspect}"

puts

# --- イミュータブル vs ミュータブル ---
puts "--- イミュータブル vs ミュータブル ---"

# イミュータブル: Integer, Symbol, true, false, nil
num1 = 42
num2 = 42
puts "Integer: #{num1.object_id} == #{num2.object_id}: #{num1.object_id == num2.object_id}"

sym1 = :hello
sym2 = :hello
puts "Symbol:  #{sym1.object_id} == #{sym2.object_id}: #{sym1.object_id == sym2.object_id}"

# ミュータブル: String, Array, Hash
str1 = "hello"
str2 = "hello"
puts "String:  #{str1.object_id} == #{str2.object_id}: #{str1.object_id == str2.object_id}"

puts

# --- コピー ---
puts "--- コピー ---"

original = [1, [2, 3], { a: 4 }]
puts "Original: #{original.inspect}"

# シャローコピー (dup / clone)
shallow = original.dup
puts "Shallow copy (dup): #{shallow.inspect}"

shallow[0] = 100
puts "After shallow[0] = 100:"
puts "  Original: #{original.inspect}"  # 変わらない
puts "  Shallow:  #{shallow.inspect}"

shallow[1][0] = 200  # ネストしたオブジェクトは共有
puts "After shallow[1][0] = 200:"
puts "  Original: #{original.inspect}"  # 変わる!
puts "  Shallow:  #{shallow.inspect}"

# ディープコピー
require 'json'
original2 = [1, [2, 3], { "a" => 4 }]
deep = JSON.parse(JSON.generate(original2))
puts "\nDeep copy (JSON): #{deep.inspect}"

# または Marshal
original3 = [1, [2, 3], { a: 4 }]
deep2 = Marshal.load(Marshal.dump(original3))
puts "Deep copy (Marshal): #{deep2.inspect}"

puts

# --- freeze (凍結) ---
puts "--- freeze ---"

frozen_str = "immutable".freeze
puts "frozen_str.frozen?: #{frozen_str.frozen?}"

begin
  frozen_str.upcase!
rescue FrozenError => e
  puts "FrozenError: #{e.message}"
end

# frozen_string_literal: true マジックコメントで
# 全ての文字列リテラルを自動的に freeze

puts

# --- GC (Garbage Collection) ---
puts "--- GC ---"

# GC の統計
puts "GC.stat:"
stat = GC.stat
puts "  count: #{stat[:count]}"
puts "  heap_allocated_pages: #{stat[:heap_allocated_pages]}"
puts "  total_allocated_objects: #{stat[:total_allocated_objects]}"
puts "  total_freed_objects: #{stat[:total_freed_objects]}"

# 手動 GC
puts "\nManual GC.start:"
before = GC.stat[:count]
GC.start
after = GC.stat[:count]
puts "  GC count: #{before} -> #{after}"

puts

# --- WeakRef ---
puts "--- WeakRef ---"

require 'weakref'

# 強参照: GC されない
strong = "I won't be collected"

# 弱参照: GC される可能性がある
obj = Object.new
weak = WeakRef.new(obj)
puts "Before GC: weak.weakref_alive? = #{weak.weakref_alive?}"

# オブジェクトへの参照を削除して GC
obj = nil
GC.start

begin
  puts "After GC: weak.weakref_alive? = #{weak.weakref_alive?}"
rescue RefError => e
  puts "After GC: RefError - #{e.message}"
end

puts

# --- ObjectSpace ---
puts "--- ObjectSpace ---"

# 特定クラスのオブジェクト数をカウント
string_count = ObjectSpace.each_object(String).count
array_count = ObjectSpace.each_object(Array).count
puts "String objects: #{string_count}"
puts "Array objects: #{array_count}"

# メモリ使用量の概算
puts "\nMemory usage (approximate):"
puts "  #{`ps -o rss= -p #{Process.pid}`.strip.to_i / 1024} MB"

puts

# --- メモリリーク例 ---
puts "--- メモリリークの原因 ---"
puts <<~LEAK
  # 1. 循環参照
  a = []
  b = []
  a << b
  b << a
  # => Ruby の GC は循環参照を検出できる (Mark & Sweep)

  # 2. グローバル変数やクラス変数への蓄積
  $cache = {}
  def fetch(key)
    $cache[key] ||= expensive_operation(key)
  end
  # => キャッシュが無限に成長する

  # 3. クロージャによる参照保持
  def create_leak
    large_data = "x" * 1_000_000
    -> { large_data.length }  # large_data が保持される
  end
LEAK
