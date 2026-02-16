#!/usr/bin/env ruby
# frozen_string_literal: true

# エラー処理 - Ruby 実装
#
# Ruby は例外ベースのエラー処理
# begin-rescue-ensure パターン

puts "=== Error Handling Demo ===\n\n"

# --- 基本的な例外処理 ---
puts "--- begin-rescue-ensure ---"

def divide(a, b)
  a / b
rescue ZeroDivisionError => e
  puts "Error: #{e.message}"
  nil
end

puts "10 / 2 = #{divide(10, 2)}"
puts "10 / 0 = #{divide(10, 0).inspect}"

puts

# 完全な構文
puts "--- Full syntax ---"

begin
  puts "  Trying risky operation..."
  result = 10 / 0
  puts "  Result: #{result}"
rescue ZeroDivisionError => e
  puts "  Caught ZeroDivisionError: #{e.message}"
rescue StandardError => e
  puts "  Caught StandardError: #{e.message}"
else
  puts "  No error occurred"
ensure
  puts "  Cleanup (always runs)"
end

puts

# --- 例外クラス階層 ---
puts "--- Exception Hierarchy ---"
puts <<~HIERARCHY
  Exception
  ├── NoMemoryError
  ├── ScriptError
  │   ├── LoadError
  │   ├── NotImplementedError
  │   └── SyntaxError
  ├── SignalException
  │   └── Interrupt
  └── StandardError (通常はこれを rescue)
      ├── ArgumentError
      ├── IOError
      ├── NameError
      │   └── NoMethodError
      ├── RangeError
      ├── RuntimeError (raise のデフォルト)
      ├── TypeError
      └── ZeroDivisionError
HIERARCHY

puts

# --- raise ---
puts "--- raise ---"

def validate_age(age)
  raise ArgumentError, "Age must be positive" if age < 0
  raise ArgumentError, "Age must be reasonable" if age > 150
  age
end

begin
  validate_age(-5)
rescue ArgumentError => e
  puts "Validation error: #{e.message}"
end

puts

# --- カスタム例外 ---
puts "--- Custom Exception ---"

class ValidationError < StandardError
  attr_reader :field

  def initialize(message, field: nil)
    super(message)
    @field = field
  end
end

class User
  def initialize(name, age)
    raise ValidationError.new("Name is required", field: :name) if name.nil? || name.empty?
    raise ValidationError.new("Age must be positive", field: :age) if age < 0

    @name = name
    @age = age
  end
end

begin
  User.new("", 25)
rescue ValidationError => e
  puts "Validation failed on #{e.field}: #{e.message}"
end

puts

# --- retry ---
puts "--- retry ---"

attempts = 0

begin
  attempts += 1
  puts "  Attempt #{attempts}"
  raise "Temporary failure" if attempts < 3
  puts "  Success!"
rescue RuntimeError => e
  puts "  Error: #{e.message}"
  retry if attempts < 3
  puts "  Giving up after #{attempts} attempts"
end

puts

# --- 後置 rescue ---
puts "--- Inline rescue ---"

# 簡潔だがデバッグしにくい
result = (10 / 0 rescue "division error")
puts "10 / 0 rescue '...' = #{result}"

# デフォルト値として便利
value = JSON.parse("invalid") rescue nil
puts "JSON.parse('invalid') rescue nil = #{value.inspect}"

puts

# --- Result パターン (Rust 風) ---
puts "--- Result Pattern (Rust-like) ---"

# Rust の Result<T, E> を模倣
class Result
  attr_reader :value, :error

  def initialize(value: nil, error: nil)
    @value = value
    @error = error
  end

  def ok?
    @error.nil?
  end

  def err?
    !ok?
  end

  def unwrap
    raise @error if err?
    @value
  end

  def unwrap_or(default)
    ok? ? @value : default
  end

  def map
    return self if err?
    Result.ok(yield(@value))
  rescue StandardError => e
    Result.err(e)
  end

  def self.ok(value)
    new(value: value)
  end

  def self.err(error)
    new(error: error)
  end
end

def safe_divide(a, b)
  return Result.err("Division by zero") if b == 0
  Result.ok(a / b)
end

result1 = safe_divide(10, 2)
result2 = safe_divide(10, 0)

puts "safe_divide(10, 2):"
puts "  ok? = #{result1.ok?}, value = #{result1.value}"

puts "safe_divide(10, 0):"
puts "  ok? = #{result2.ok?}, error = #{result2.error}"

# チェーン
puts "\nChained operations:"
result = safe_divide(100, 5)
  .map { |x| x * 2 }
  .map { |x| x + 10 }
puts "  100 / 5 * 2 + 10 = #{result.value}"

puts

# --- throw / catch (非例外的な制御フロー) ---
puts "--- throw / catch ---"

# 例外ではなく、深いネストからの脱出
def find_in_matrix(matrix, target)
  catch(:found) do
    matrix.each_with_index do |row, i|
      row.each_with_index do |val, j|
        throw(:found, [i, j]) if val == target
      end
    end
    nil
  end
end

matrix = [
  [1, 2, 3],
  [4, 5, 6],
  [7, 8, 9]
]

pos = find_in_matrix(matrix, 5)
puts "Found 5 at position: #{pos.inspect}"

pos = find_in_matrix(matrix, 99)
puts "Found 99 at position: #{pos.inspect}"
