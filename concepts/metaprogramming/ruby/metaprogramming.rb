#!/usr/bin/env ruby
# frozen_string_literal: true

# メタプログラミング - Ruby 実装
#
# Ruby のメタプログラミングは非常に強力
# 実行時にクラスやメソッドを動的に生成・変更できる

puts "=== Metaprogramming Demo ===\n\n"

# --- 動的メソッド定義 ---
puts "--- define_method ---"

class DynamicClass
  # 動的にメソッドを定義
  %w[foo bar baz].each do |name|
    define_method(name) do
      "Called #{name}"
    end
  end

  # 引数付き
  define_method(:greet) do |name|
    "Hello, #{name}!"
  end
end

obj = DynamicClass.new
puts "obj.foo => #{obj.foo}"
puts "obj.bar => #{obj.bar}"
puts "obj.greet('Ruby') => #{obj.greet('Ruby')}"

puts

# --- method_missing ---
puts "--- method_missing ---"

class FlexibleHash
  def initialize(hash = {})
    @data = hash
  end

  def method_missing(name, *args)
    name_str = name.to_s

    if name_str.end_with?('=')
      # setter
      key = name_str.chomp('=').to_sym
      @data[key] = args.first
    elsif name_str.end_with?('?')
      # predicate
      key = name_str.chomp('?').to_sym
      !!@data[key]
    else
      # getter
      @data[name]
    end
  end

  def respond_to_missing?(name, include_private = false)
    true
  end
end

fh = FlexibleHash.new
fh.name = "Ruby"
fh.version = 3.2
puts "fh.name => #{fh.name}"
puts "fh.version => #{fh.version}"
puts "fh.name? => #{fh.name?}"
puts "fh.missing? => #{fh.missing?}"

puts

# --- class_eval / instance_eval ---
puts "--- class_eval / instance_eval ---"

class Person
  attr_accessor :name
end

# クラスにメソッドを追加
Person.class_eval do
  def greet
    "Hello, I'm #{@name}"
  end

  def self.species
    "Homo sapiens"
  end
end

person = Person.new
person.name = "Alice"
puts "person.greet => #{person.greet}"
puts "Person.species => #{Person.species}"

# 特定のインスタンスだけにメソッドを追加
person.instance_eval do
  def secret
    "This is #{@name}'s secret"
  end
end

puts "person.secret => #{person.secret}"

puts

# --- 特異クラス (Singleton Class) ---
puts "--- Singleton Class ---"

obj = Object.new

# 特異クラスを取得
singleton = obj.singleton_class
puts "Singleton class: #{singleton}"

# 特異メソッドを定義
def obj.hello
  "Hello from singleton method"
end

puts "obj.hello => #{obj.hello}"

# class << self 記法
class MyClass
  class << self
    def class_method
      "I'm a class method"
    end
  end
end

puts "MyClass.class_method => #{MyClass.class_method}"

puts

# --- フック ---
puts "--- Hooks ---"

class HookDemo
  # メソッドが追加されたとき
  def self.method_added(name)
    puts "  Method added: #{name}"
  end

  # サブクラスが作られたとき
  def self.inherited(subclass)
    puts "  Inherited by: #{subclass}"
  end

  # モジュールがインクルードされたとき
  def self.included(base)
    puts "  Included in: #{base}"
  end

  def instance_method
    "I'm an instance method"
  end
end

puts "Defining subclass:"
class SubDemo < HookDemo
end

puts

# --- モジュールの included と prepend ---
puts "--- Module included/prepended ---"

module Traceable
  def self.included(base)
    base.extend(ClassMethods)
    puts "  Traceable included in #{base}"
  end

  module ClassMethods
    def trace_methods(*names)
      names.each do |name|
        original = instance_method(name)
        define_method(name) do |*args, &block|
          puts "  [TRACE] Calling #{name}"
          original.bind(self).call(*args, &block)
        end
      end
    end
  end
end

class Calculator
  include Traceable

  def add(a, b)
    a + b
  end

  def multiply(a, b)
    a * b
  end

  trace_methods :add, :multiply
end

calc = Calculator.new
puts "calc.add(1, 2) => #{calc.add(1, 2)}"

puts

# --- リフレクション ---
puts "--- Reflection ---"

class Sample
  attr_accessor :value

  def initialize(value)
    @value = value
  end

  def public_method
    "public"
  end

  private

  def private_method
    "private"
  end
end

sample = Sample.new(42)

puts "Class: #{sample.class}"
puts "Superclass: #{sample.class.superclass}"
puts "Ancestors: #{sample.class.ancestors.take(3)}"
puts "Instance variables: #{sample.instance_variables}"
puts "Public methods (own): #{(sample.public_methods - Object.public_methods).sort}"
puts "respond_to?(:value): #{sample.respond_to?(:value)}"

# 動的にメソッドを呼び出す
puts "send(:public_method): #{sample.send(:public_method)}"
puts "send(:private_method): #{sample.send(:private_method)}"  # private も呼べる

puts

# --- eval ---
puts "--- eval ---"

# eval: 文字列を Ruby コードとして実行 (危険!)
code = "1 + 2 + 3"
puts "eval('#{code}') => #{eval(code)}"

# binding: 現在のスコープをキャプチャ
def get_binding(x)
  binding
end

b = get_binding(10)
puts "eval('x * 2', binding) => #{eval('x * 2', b)}"

# instance_variable_get/set
sample.instance_variable_set(:@secret, "hidden")
puts "instance_variable_get(:@secret) => #{sample.instance_variable_get(:@secret)}"

puts

# --- DSL 例 ---
puts "--- DSL Example ---"

class ConfigDSL
  def initialize(&block)
    @config = {}
    instance_eval(&block)
  end

  def set(key, value)
    @config[key] = value
  end

  def to_h
    @config
  end

  def method_missing(name, *args)
    if args.empty?
      @config[name]
    else
      @config[name] = args.first
    end
  end
end

config = ConfigDSL.new do
  set :database, "postgresql"
  host "localhost"
  port 5432
  debug true
end

puts "Config: #{config.to_h}"
