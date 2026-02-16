#!/usr/bin/env ruby
# frozen_string_literal: true

# データ構造 - Ruby 実装
#
# Ruby の組み込みデータ構造と自作実装

puts "=== Data Structures Demo ===\n\n"

# --- Array ---
puts "--- Array ---"

arr = [1, 2, 3, 4, 5]
puts "Array: #{arr}"

# 基本操作
puts "push: #{arr.push(6)}"
puts "pop: #{arr.pop} => #{arr}"
puts "shift: #{arr.shift} => #{arr}"
puts "unshift: #{arr.unshift(0)}"

# スライス
puts "arr[1..3]: #{arr[1..3]}"
puts "arr[-2..]: #{arr[-2..]}"

# 検索
puts "include?(3): #{arr.include?(3)}"
puts "index(3): #{arr.index(3)}"

puts

# --- Hash ---
puts "--- Hash ---"

hash = { name: "Ruby", year: 1995, dynamic: true }
puts "Hash: #{hash}"

# 基本操作
puts "hash[:name]: #{hash[:name]}"
puts "hash[:missing]: #{hash[:missing].inspect}"
puts "hash.fetch(:missing, 'default'): #{hash.fetch(:missing, 'default')}"

# キーと値
puts "keys: #{hash.keys}"
puts "values: #{hash.values}"

# 変換
puts "transform_values: #{hash.transform_values(&:to_s)}"
puts "slice: #{hash.slice(:name, :year)}"

# デフォルト値付きハッシュ
counter = Hash.new(0)
%w[a b a c a b].each { |x| counter[x] += 1 }
puts "Counter: #{counter}"

puts

# --- Set ---
puts "--- Set ---"

require 'set'

set1 = Set.new([1, 2, 3])
set2 = Set.new([2, 3, 4])

puts "set1: #{set1.to_a}"
puts "set2: #{set2.to_a}"

puts "union: #{(set1 | set2).to_a}"
puts "intersection: #{(set1 & set2).to_a}"
puts "difference: #{(set1 - set2).to_a}"
puts "include?(2): #{set1.include?(2)}"

puts

# --- Queue (Thread-safe) ---
puts "--- Queue ---"

queue = Queue.new
queue << 1
queue << 2
queue << 3

puts "Queue size: #{queue.size}"
puts "pop: #{queue.pop}"
puts "pop: #{queue.pop}"
puts "Queue size: #{queue.size}"

puts

# --- Stack (Array で実装) ---
puts "--- Stack (using Array) ---"

class Stack
  def initialize
    @items = []
  end

  def push(item)
    @items.push(item)
  end

  def pop
    @items.pop
  end

  def peek
    @items.last
  end

  def empty?
    @items.empty?
  end

  def size
    @items.size
  end

  def to_s
    "Stack#{@items}"
  end
end

stack = Stack.new
stack.push(1)
stack.push(2)
stack.push(3)
puts "Stack: #{stack}"
puts "peek: #{stack.peek}"
puts "pop: #{stack.pop}"
puts "Stack: #{stack}"

puts

# --- LinkedList ---
puts "--- LinkedList ---"

class Node
  attr_accessor :value, :next_node

  def initialize(value)
    @value = value
    @next_node = nil
  end
end

class LinkedList
  include Enumerable

  def initialize
    @head = nil
    @size = 0
  end

  def push_front(value)
    node = Node.new(value)
    node.next_node = @head
    @head = node
    @size += 1
  end

  def push_back(value)
    node = Node.new(value)
    if @head.nil?
      @head = node
    else
      current = @head
      current = current.next_node while current.next_node
      current.next_node = node
    end
    @size += 1
  end

  def pop_front
    return nil if @head.nil?

    value = @head.value
    @head = @head.next_node
    @size -= 1
    value
  end

  def each
    current = @head
    while current
      yield current.value
      current = current.next_node
    end
  end

  def size
    @size
  end

  def to_s
    "LinkedList[#{to_a.join(', ')}]"
  end
end

list = LinkedList.new
list.push_front(1)
list.push_front(2)
list.push_front(3)
list.push_back(10)
puts "LinkedList: #{list}"
puts "pop_front: #{list.pop_front}"
puts "LinkedList: #{list}"
puts "map { * 2 }: #{list.map { |x| x * 2 }}"

puts

# --- Binary Tree ---
puts "--- Binary Tree ---"

class TreeNode
  attr_accessor :value, :left, :right

  def initialize(value)
    @value = value
    @left = nil
    @right = nil
  end
end

class BinarySearchTree
  def initialize
    @root = nil
  end

  def insert(value)
    @root = insert_recursive(@root, value)
  end

  def include?(value)
    search_recursive(@root, value)
  end

  def inorder
    result = []
    inorder_recursive(@root, result)
    result
  end

  private

  def insert_recursive(node, value)
    return TreeNode.new(value) if node.nil?

    if value < node.value
      node.left = insert_recursive(node.left, value)
    else
      node.right = insert_recursive(node.right, value)
    end
    node
  end

  def search_recursive(node, value)
    return false if node.nil?
    return true if node.value == value

    if value < node.value
      search_recursive(node.left, value)
    else
      search_recursive(node.right, value)
    end
  end

  def inorder_recursive(node, result)
    return if node.nil?

    inorder_recursive(node.left, result)
    result << node.value
    inorder_recursive(node.right, result)
  end
end

bst = BinarySearchTree.new
[5, 3, 7, 1, 4, 6, 8].each { |x| bst.insert(x) }
puts "Inorder: #{bst.inorder}"
puts "include?(4): #{bst.include?(4)}"
puts "include?(9): #{bst.include?(9)}"

puts

# --- LRU Cache ---
puts "--- LRU Cache ---"

class LRUCache
  def initialize(capacity)
    @capacity = capacity
    @cache = {}
  end

  def get(key)
    return nil unless @cache.key?(key)

    # 最近使用したのでリストの末尾へ移動
    value = @cache.delete(key)
    @cache[key] = value
    value
  end

  def put(key, value)
    @cache.delete(key) if @cache.key?(key)

    # 容量オーバーなら最古を削除
    @cache.shift if @cache.size >= @capacity

    @cache[key] = value
  end

  def to_s
    "LRUCache(#{@cache})"
  end
end

lru = LRUCache.new(3)
lru.put(:a, 1)
lru.put(:b, 2)
lru.put(:c, 3)
puts "After put a, b, c: #{lru}"

lru.get(:a)  # a を使用
lru.put(:d, 4)  # b が削除される
puts "After get(a), put(d): #{lru}"
