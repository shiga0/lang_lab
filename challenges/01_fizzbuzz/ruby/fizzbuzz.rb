#!/usr/bin/env ruby
# frozen_string_literal: true

# FizzBuzz - Ruby 実装

puts "=== FizzBuzz Demo ===\n\n"

# --- 基本実装 ---
puts "--- Basic Implementation ---"

def fizzbuzz_basic(n)
  (1..n).each do |i|
    if (i % 15).zero?
      puts "FizzBuzz"
    elsif (i % 3).zero?
      puts "Fizz"
    elsif (i % 5).zero?
      puts "Buzz"
    else
      puts i
    end
  end
end

fizzbuzz_basic(15)

puts

# --- case 文 ---
puts "--- Case Expression ---"

def fizzbuzz_case(n)
  (1..n).map do |i|
    case [i % 3, i % 5]
    when [0, 0] then "FizzBuzz"
    when [0, _] then "Fizz"
    when [_, 0] then "Buzz"
    else i.to_s
    end
  end
end

puts fizzbuzz_case(15).join(", ")

puts

# --- 関数型スタイル ---
puts "--- Functional Style ---"

def fizzbuzz_functional(n)
  (1..n).map do |i|
    fizz = (i % 3).zero? ? "Fizz" : ""
    buzz = (i % 5).zero? ? "Buzz" : ""
    result = fizz + buzz
    result.empty? ? i.to_s : result
  end
end

puts fizzbuzz_functional(15).join(", ")

puts

# --- パターンマッチング (Ruby 3.0+) ---
puts "--- Pattern Matching (Ruby 3.0+) ---"

def fizzbuzz_pattern(n)
  (1..n).map do |i|
    case [(i % 3).zero?, (i % 5).zero?]
    in [true, true]
      "FizzBuzz"
    in [true, false]
      "Fizz"
    in [false, true]
      "Buzz"
    else
      i.to_s
    end
  end
end

puts fizzbuzz_pattern(15).join(", ")

puts

# --- ジェネレータ (Enumerator) ---
puts "--- Enumerator (Infinite) ---"

fizzbuzz_enum = Enumerator.new do |y|
  i = 1
  loop do
    result = ""
    result += "Fizz" if (i % 3).zero?
    result += "Buzz" if (i % 5).zero?
    y << (result.empty? ? i.to_s : result)
    i += 1
  end
end

puts "First 15: #{fizzbuzz_enum.take(15).join(', ')}"
puts "Next 5: #{fizzbuzz_enum.take(20).drop(15).join(', ')}"

puts

# --- メソッドチェーン ---
puts "--- Method Chain ---"

result = (1..15)
  .lazy
  .map { |i| [(i % 3).zero?, (i % 5).zero?, i] }
  .map do |div3, div5, i|
    case [div3, div5]
    when [true, true] then "FizzBuzz"
    when [true, false] then "Fizz"
    when [false, true] then "Buzz"
    else i.to_s
    end
  end
  .force

puts result.join(", ")

puts

# --- ワンライナー ---
puts "--- One-liner ---"

puts (1..15).map { |i| (s = "#{'Fizz' if i % 3 == 0}#{'Buzz' if i % 5 == 0}").empty? ? i : s }.join(", ")

puts

# --- テスト ---
puts "--- Tests ---"

def test_fizzbuzz
  result = fizzbuzz_functional(15)

  # 基本ケース
  raise "Failed: 1" unless result[0] == "1"
  raise "Failed: 2" unless result[1] == "2"

  # Fizz (3の倍数)
  raise "Failed: 3" unless result[2] == "Fizz"
  raise "Failed: 6" unless result[5] == "Fizz"

  # Buzz (5の倍数)
  raise "Failed: 5" unless result[4] == "Buzz"
  raise "Failed: 10" unless result[9] == "Buzz"

  # FizzBuzz (15の倍数)
  raise "Failed: 15" unless result[14] == "FizzBuzz"

  puts "All tests passed!"
end

test_fizzbuzz
