#!/usr/bin/env ruby
# frozen_string_literal: true

# JSON Parser - Ruby 実装 (再帰下降パーサー)

puts "=== JSON Parser Demo ===\n\n"

# --- ParseError ---
class ParseError < StandardError
  attr_reader :position

  def initialize(message, position)
    super("Parse error at position #{position}: #{message}")
    @position = position
  end
end

# --- Tokenizer ---
class Tokenizer
  attr_reader :position

  def initialize(input)
    @input = input
    @position = 0
  end

  def current_char
    @input[@position]
  end

  def peek(offset = 0)
    @input[@position + offset]
  end

  def advance
    char = current_char
    @position += 1
    char
  end

  def skip_whitespace
    advance while current_char && current_char.match?(/\s/)
  end

  def eof?
    @position >= @input.length
  end

  def expect(char)
    if current_char != char
      raise ParseError.new("Expected '#{char}' but got '#{current_char}'", @position)
    end
    advance
  end
end

# --- Parser ---
class JSONParser
  def initialize(input)
    @tokenizer = Tokenizer.new(input)
  end

  def parse
    @tokenizer.skip_whitespace
    value = parse_value
    @tokenizer.skip_whitespace

    unless @tokenizer.eof?
      raise ParseError.new("Unexpected characters after JSON value", @tokenizer.position)
    end

    value
  end

  private

  def parse_value
    @tokenizer.skip_whitespace

    case @tokenizer.current_char
    when 'n'
      parse_null
    when 't', 'f'
      parse_bool
    when '"'
      parse_string
    when '['
      parse_array
    when '{'
      parse_object
    when '-', '0'..'9'
      parse_number
    else
      raise ParseError.new("Unexpected character: '#{@tokenizer.current_char}'", @tokenizer.position)
    end
  end

  def parse_null
    expect_keyword('null')
    nil
  end

  def parse_bool
    if @tokenizer.current_char == 't'
      expect_keyword('true')
      true
    else
      expect_keyword('false')
      false
    end
  end

  def expect_keyword(keyword)
    keyword.each_char do |expected|
      actual = @tokenizer.advance
      if actual != expected
        raise ParseError.new("Expected '#{expected}' but got '#{actual}'", @tokenizer.position)
      end
    end
  end

  def parse_string
    @tokenizer.advance  # consume opening "
    result = +''

    loop do
      char = @tokenizer.advance

      case char
      when nil
        raise ParseError.new("Unterminated string", @tokenizer.position)
      when '"'
        break
      when '\\'
        result << parse_escape_sequence
      else
        result << char
      end
    end

    result
  end

  def parse_escape_sequence
    char = @tokenizer.advance

    case char
    when 'n' then "\n"
    when 't' then "\t"
    when 'r' then "\r"
    when '"' then '"'
    when '\\' then '\\'
    when '/' then '/'
    when 'u'
      # Unicode escape
      hex = 4.times.map { @tokenizer.advance }.join
      [hex.to_i(16)].pack('U')
    else
      raise ParseError.new("Invalid escape sequence: \\#{char}", @tokenizer.position)
    end
  end

  def parse_number
    start_pos = @tokenizer.position
    num_str = +''

    # 負号
    if @tokenizer.current_char == '-'
      num_str << @tokenizer.advance
    end

    # 整数部
    if @tokenizer.current_char == '0'
      num_str << @tokenizer.advance
    elsif @tokenizer.current_char&.match?(/[1-9]/)
      num_str << @tokenizer.advance while @tokenizer.current_char&.match?(/\d/)
    else
      raise ParseError.new("Expected digit", @tokenizer.position)
    end

    # 小数部
    if @tokenizer.current_char == '.'
      num_str << @tokenizer.advance
      unless @tokenizer.current_char&.match?(/\d/)
        raise ParseError.new("Expected digit after decimal point", @tokenizer.position)
      end
      num_str << @tokenizer.advance while @tokenizer.current_char&.match?(/\d/)
    end

    # 指数部
    if @tokenizer.current_char&.match?(/[eE]/)
      num_str << @tokenizer.advance
      num_str << @tokenizer.advance if @tokenizer.current_char&.match?(/[+-]/)
      unless @tokenizer.current_char&.match?(/\d/)
        raise ParseError.new("Expected digit in exponent", @tokenizer.position)
      end
      num_str << @tokenizer.advance while @tokenizer.current_char&.match?(/\d/)
    end

    num_str.include?('.') || num_str.include?('e') || num_str.include?('E') ?
      num_str.to_f : num_str.to_i
  end

  def parse_array
    @tokenizer.advance  # consume [
    @tokenizer.skip_whitespace

    arr = []

    # 空配列
    if @tokenizer.current_char == ']'
      @tokenizer.advance
      return arr
    end

    loop do
      arr << parse_value
      @tokenizer.skip_whitespace

      case @tokenizer.current_char
      when ','
        @tokenizer.advance
        @tokenizer.skip_whitespace
      when ']'
        @tokenizer.advance
        break
      else
        raise ParseError.new("Expected ',' or ']'", @tokenizer.position)
      end
    end

    arr
  end

  def parse_object
    @tokenizer.advance  # consume {
    @tokenizer.skip_whitespace

    obj = {}

    # 空オブジェクト
    if @tokenizer.current_char == '}'
      @tokenizer.advance
      return obj
    end

    loop do
      @tokenizer.skip_whitespace

      # キー
      unless @tokenizer.current_char == '"'
        raise ParseError.new("Expected string key", @tokenizer.position)
      end
      key = parse_string

      @tokenizer.skip_whitespace
      @tokenizer.expect(':')

      # 値
      value = parse_value
      obj[key] = value

      @tokenizer.skip_whitespace

      case @tokenizer.current_char
      when ','
        @tokenizer.advance
      when '}'
        @tokenizer.advance
        break
      else
        raise ParseError.new("Expected ',' or '}'", @tokenizer.position)
      end
    end

    obj
  end
end

# --- 便利メソッド ---
def parse_json(input)
  JSONParser.new(input).parse
end

# --- デモ ---
examples = [
  'null',
  'true',
  '42',
  '3.14',
  '"hello"',
  '[1, 2, 3]',
  '{"name": "Ruby", "version": 3.2}',
  '{"nested": {"array": [1, true, null]}}'
]

examples.each do |json|
  puts "Input:  #{json}"
  begin
    result = parse_json(json)
    puts "Parsed: #{result.inspect}"
  rescue ParseError => e
    puts "Error:  #{e.message}"
  end
  puts
end

# --- テスト ---
puts "--- Tests ---"

def run_tests
  # null
  raise "null failed" unless parse_json('null').nil?

  # bool
  raise "true failed" unless parse_json('true') == true
  raise "false failed" unless parse_json('false') == false

  # number
  raise "integer failed" unless parse_json('42') == 42
  raise "negative failed" unless parse_json('-17') == -17
  raise "float failed" unless parse_json('3.14') == 3.14
  raise "exponent failed" unless parse_json('1e10') == 1e10

  # string
  raise "string failed" unless parse_json('"hello"') == "hello"
  raise "escape failed" unless parse_json('"hello\\nworld"') == "hello\nworld"

  # array
  raise "empty array failed" unless parse_json('[]') == []
  raise "array failed" unless parse_json('[1, 2, 3]') == [1, 2, 3]
  raise "mixed array failed" unless parse_json('[true, null]') == [true, nil]

  # object
  raise "empty object failed" unless parse_json('{}') == {}
  raise "object failed" unless parse_json('{"name": "Ruby"}') == { "name" => "Ruby" }

  # nested
  nested = parse_json('{"arr": [1, {"nested": true}]}')
  raise "nested failed" unless nested["arr"][1]["nested"] == true

  # whitespace
  raise "whitespace failed" unless parse_json("  { \"key\" : \"value\" }  ") == { "key" => "value" }

  # errors
  begin
    parse_json('')
    raise "empty input should fail"
  rescue ParseError
    # expected
  end

  begin
    parse_json('{')
    raise "unclosed object should fail"
  rescue ParseError
    # expected
  end

  puts "All tests passed!"
end

run_tests
