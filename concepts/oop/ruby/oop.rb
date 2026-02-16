#!/usr/bin/env ruby
# frozen_string_literal: true

# オブジェクト指向 - Ruby 実装
#
# Ruby は純粋なオブジェクト指向言語
# 全てがオブジェクト

puts "=== OOP Demo ===\n\n"

# --- 全てがオブジェクト ---
puts "--- Everything is an Object ---"

puts "1.class => #{1.class}"
puts "3.14.class => #{3.14.class}"
puts "'hello'.class => #{'hello'.class}"
puts "true.class => #{true.class}"
puts "nil.class => #{nil.class}"
puts "[1,2,3].class => #{[1, 2, 3].class}"
puts "{ a: 1 }.class => #{{ a: 1 }.class}"

# メソッドもオブジェクト
puts "1.method(:+).class => #{1.method(:+).class}"

puts

# --- クラス定義 ---
puts "--- Class Definition ---"

class User
  # クラス変数
  @@count = 0

  # アクセサ
  attr_reader :name       # getter のみ
  attr_accessor :email    # getter + setter

  # コンストラクタ
  def initialize(name, email = nil)
    @name = name
    @email = email
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

  # to_s のオーバーライド
  def to_s
    "User(name: #{@name}, email: #{@email})"
  end
end

user1 = User.new("Alice", "alice@example.com")
user2 = User.new("Bob")

puts user1.greet
puts "User.count = #{User.count}"
puts user1.to_s

puts

# --- 継承 ---
puts "--- Inheritance ---"

class Admin < User
  attr_reader :level

  def initialize(name, email, level)
    super(name, email)
    @level = level
  end

  # オーバーライド
  def greet
    "#{super}, I'm an admin (level #{@level})"
  end
end

admin = Admin.new("Charlie", "charlie@example.com", 5)
puts admin.greet

puts

# --- モジュールとミックスイン ---
puts "--- Modules and Mixins ---"

# モジュール: 共通機能を定義
module Loggable
  def log(message)
    puts "[#{self.class}] #{message}"
  end
end

module Timestampable
  def created_at
    @created_at ||= Time.now
  end
end

class Server
  include Loggable       # インスタンスメソッドとして追加
  include Timestampable

  def start
    log("Starting server...")
  end
end

server = Server.new
server.start
puts "Created at: #{server.created_at}"

puts

# extend: クラスメソッドとして追加
class Client
  extend Loggable
end

Client.log("Client connected")

puts

# --- Duck Typing ---
puts "--- Duck Typing ---"

def make_noise(animal)
  animal.speak
end

class Dog
  def speak
    "Woof!"
  end
end

class Cat
  def speak
    "Meow!"
  end
end

class Robot
  def speak
    "Beep boop!"
  end
end

[Dog.new, Cat.new, Robot.new].each do |obj|
  puts "#{obj.class}: #{make_noise(obj)}"
end

puts

# --- アクセス制御 ---
puts "--- Access Control ---"

class BankAccount
  def initialize(balance)
    @balance = balance
  end

  # public (デフォルト)
  def deposit(amount)
    validate_amount(amount)
    @balance += amount
    log_transaction("Deposit", amount)
  end

  def balance
    @balance
  end

  # protected: 同じクラスのインスタンス間でアクセス可能
  protected

  def balance_raw
    @balance
  end

  # private: 自分自身からのみアクセス可能
  private

  def validate_amount(amount)
    raise ArgumentError, "Amount must be positive" if amount <= 0
  end

  def log_transaction(type, amount)
    puts "  [LOG] #{type}: #{amount}, Balance: #{@balance}"
  end
end

account = BankAccount.new(100)
account.deposit(50)
puts "Balance: #{account.balance}"

puts

# --- Comparable と Enumerable ---
puts "--- Comparable ---"

class Version
  include Comparable

  attr_reader :major, :minor, :patch

  def initialize(version_string)
    @major, @minor, @patch = version_string.split('.').map(&:to_i)
  end

  def <=>(other)
    [major, minor, patch] <=> [other.major, other.minor, other.patch]
  end

  def to_s
    "#{major}.#{minor}.#{patch}"
  end
end

v1 = Version.new("1.0.0")
v2 = Version.new("1.2.0")
v3 = Version.new("2.0.0")

puts "#{v1} < #{v2}: #{v1 < v2}"
puts "#{v2} < #{v3}: #{v2 < v3}"
puts "Sorted: #{[v3, v1, v2].sort.map(&:to_s)}"

puts

# --- メタプログラミング ---
puts "--- Metaprogramming ---"

class DynamicClass
  # 動的にメソッドを定義
  %w[foo bar baz].each do |name|
    define_method(name) do
      "Called #{name}"
    end
  end

  # method_missing: 存在しないメソッドの呼び出しをキャッチ
  def method_missing(name, *args)
    if name.to_s.start_with?("get_")
      attr_name = name.to_s.sub("get_", "")
      instance_variable_get("@#{attr_name}")
    else
      super
    end
  end

  def respond_to_missing?(name, include_private = false)
    name.to_s.start_with?("get_") || super
  end

  def initialize
    @secret = "hidden value"
  end
end

obj = DynamicClass.new
puts "obj.foo => #{obj.foo}"
puts "obj.bar => #{obj.bar}"
puts "obj.get_secret => #{obj.get_secret}"
puts "obj.respond_to?(:get_secret) => #{obj.respond_to?(:get_secret)}"

puts

# --- Struct と Data ---
puts "--- Struct (Lightweight Class) ---"

Point = Struct.new(:x, :y) do
  def distance_from_origin
    Math.sqrt(x**2 + y**2)
  end
end

point = Point.new(3, 4)
puts "Point: #{point}"
puts "Distance: #{point.distance_from_origin}"
