#!/usr/bin/env ruby
# frozen_string_literal: true

# LinkedList - Ruby 実装

puts "=== LinkedList Demo ===\n\n"

# --- Node ---
class Node
  attr_accessor :value, :next_node

  def initialize(value)
    @value = value
    @next_node = nil
  end
end

# --- LinkedList ---
class LinkedList
  include Enumerable

  def initialize
    @head = nil
    @tail = nil
    @size = 0
  end

  attr_reader :size

  def empty?
    @size.zero?
  end

  # 先頭に追加
  def push_front(value)
    node = Node.new(value)
    if @head.nil?
      @head = node
      @tail = node
    else
      node.next_node = @head
      @head = node
    end
    @size += 1
    self
  end

  # 末尾に追加
  def push_back(value)
    node = Node.new(value)
    if @tail.nil?
      @head = node
      @tail = node
    else
      @tail.next_node = node
      @tail = node
    end
    @size += 1
    self
  end

  # 先頭を削除
  def pop_front
    return nil if @head.nil?

    value = @head.value
    @head = @head.next_node
    @tail = nil if @head.nil?
    @size -= 1
    value
  end

  # 先頭の値を参照
  def front
    @head&.value
  end

  # 末尾の値を参照
  def back
    @tail&.value
  end

  # イテレーション
  def each
    return enum_for(:each) unless block_given?

    current = @head
    while current
      yield current.value
      current = current.next_node
    end
  end

  # 検索
  def find_node(value)
    current = @head
    while current
      return current if current.value == value
      current = current.next_node
    end
    nil
  end

  # 削除
  def delete(value)
    return nil if @head.nil?

    # 先頭の場合
    if @head.value == value
      return pop_front
    end

    # それ以外
    prev = @head
    current = @head.next_node
    while current
      if current.value == value
        prev.next_node = current.next_node
        @tail = prev if current == @tail
        @size -= 1
        return value
      end
      prev = current
      current = current.next_node
    end
    nil
  end

  # 反転
  def reverse!
    return self if @size <= 1

    prev = nil
    current = @head
    @tail = @head

    while current
      next_node = current.next_node
      current.next_node = prev
      prev = current
      current = next_node
    end

    @head = prev
    self
  end

  # クリア
  def clear
    @head = nil
    @tail = nil
    @size = 0
  end

  def to_s
    "[#{to_a.join(' -> ')}]"
  end

  def inspect
    "#<LinkedList: #{self}>"
  end
end

# --- デモ ---
puts "--- Basic Operations ---"

list = LinkedList.new
puts "Empty list: #{list}"

list.push_front(3)
list.push_front(2)
list.push_front(1)
puts "After push_front 3, 2, 1: #{list}"

list.push_back(4)
list.push_back(5)
puts "After push_back 4, 5: #{list}"

puts "Size: #{list.size}"
puts "Front: #{list.front}"
puts "Back: #{list.back}"

puts

puts "--- Pop and Delete ---"
puts "pop_front: #{list.pop_front}"
puts "After pop_front: #{list}"

puts "delete(4): #{list.delete(4)}"
puts "After delete(4): #{list}"

puts

puts "--- Enumerable ---"
list.clear
[1, 2, 3, 4, 5].each { |x| list.push_back(x) }
puts "List: #{list}"
puts "map { x * 2 }: #{list.map { |x| x * 2 }}"
puts "select { x > 2 }: #{list.select { |x| x > 2 }}"
puts "reduce(:+): #{list.reduce(:+)}"

puts

puts "--- Reverse ---"
puts "Before reverse: #{list}"
list.reverse!
puts "After reverse: #{list}"

puts

# --- テスト ---
puts "--- Tests ---"

def run_tests
  list = LinkedList.new

  # 空リスト
  raise "empty? failed" unless list.empty?
  raise "size failed" unless list.size == 0

  # push_front
  list.push_front(1)
  list.push_front(2)
  list.push_front(3)
  raise "push_front failed" unless list.to_a == [3, 2, 1]

  # push_back
  list.push_back(10)
  raise "push_back failed" unless list.to_a == [3, 2, 1, 10]

  # front / back
  raise "front failed" unless list.front == 3
  raise "back failed" unless list.back == 10

  # pop_front
  raise "pop_front failed" unless list.pop_front == 3
  raise "after pop_front failed" unless list.to_a == [2, 1, 10]

  # delete
  raise "delete failed" unless list.delete(1) == 1
  raise "after delete failed" unless list.to_a == [2, 10]

  # reverse
  list.push_back(20)
  list.reverse!
  raise "reverse failed" unless list.to_a == [20, 10, 2]

  # Enumerable
  raise "each failed" unless list.map { |x| x }.size == 3
  raise "include? failed" unless list.include?(10)

  puts "All tests passed!"
end

run_tests
