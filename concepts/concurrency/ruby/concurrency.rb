#!/usr/bin/env ruby
# frozen_string_literal: true

# 並行処理 - Ruby 実装
#
# Ruby の並行処理:
# - Thread: GVL (Global VM Lock) の制限あり
# - Fiber: 協調的マルチタスク
# - Ractor: 真の並列処理 (Ruby 3.0+)

puts "=== Concurrency Demo ===\n\n"

# --- Thread ---
puts "--- Thread (GVL制限あり) ---"

# スレッドの作成と実行
threads = []
3.times do |i|
  threads << Thread.new(i) do |num|
    sleep(0.1)
    puts "  Thread #{num}: done"
  end
end

puts "Waiting for threads..."
threads.each(&:join)
puts "All threads completed"

puts

# スレッドでの共有状態
puts "--- Thread with Mutex ---"

counter = 0
mutex = Mutex.new

threads = 10.times.map do
  Thread.new do
    1000.times do
      mutex.synchronize do
        counter += 1
      end
    end
  end
end

threads.each(&:join)
puts "Counter (with Mutex): #{counter}"

puts

# --- Fiber (協調的マルチタスク) ---
puts "--- Fiber ---"

# Fiber は yield で明示的に制御を譲る
fiber = Fiber.new do
  puts "  Fiber: step 1"
  Fiber.yield "first"
  puts "  Fiber: step 2"
  Fiber.yield "second"
  puts "  Fiber: step 3"
  "third"
end

puts "Main: starting fiber"
puts "  Result: #{fiber.resume}"
puts "Main: after first resume"
puts "  Result: #{fiber.resume}"
puts "Main: after second resume"
puts "  Result: #{fiber.resume}"

puts

# Enumerator (内部で Fiber を使用)
puts "--- Enumerator (Fiber-based) ---"

fib = Enumerator.new do |y|
  a, b = 0, 1
  loop do
    y << a
    a, b = b, a + b
  end
end

puts "Fibonacci: #{fib.take(10).inspect}"

puts

# --- Queue (スレッド間通信) ---
puts "--- Queue ---"

queue = Queue.new

# プロデューサー
producer = Thread.new do
  5.times do |i|
    sleep(0.05)
    queue << "item-#{i}"
    puts "  Produced: item-#{i}"
  end
  queue << :done
end

# コンシューマー
consumer = Thread.new do
  loop do
    item = queue.pop
    break if item == :done
    puts "  Consumed: #{item}"
  end
end

producer.join
consumer.join
puts "Queue demo completed"

puts

# --- Ractor (Ruby 3.0+) ---
puts "--- Ractor (Ruby 3.0+) ---"

if RUBY_VERSION >= "3.0"
  # Ractor は真の並列処理
  # 各 Ractor は独立したメモリ空間を持つ

  ractor = Ractor.new do
    msg = Ractor.receive
    "Received: #{msg}"
  end

  ractor.send("hello from main")
  puts ractor.take

  # 複数の Ractor を並列実行
  puts "\nParallel Ractors:"
  ractors = 3.times.map do |i|
    Ractor.new(i) do |num|
      sleep(0.1)
      "Ractor #{num} done"
    end
  end

  ractors.each do |r|
    puts "  #{r.take}"
  end
else
  puts "(Ruby #{RUBY_VERSION} - Ractor は Ruby 3.0+ で利用可能)"
end

puts

# --- async gem の紹介 ---
puts "--- async (外部 gem) ---"
puts <<~ASYNC
  # async gem を使うと async/await 的な書き方ができる
  #
  # require 'async'
  #
  # Async do
  #   3.times do |i|
  #     Async do
  #       sleep(1)
  #       puts "Task \#{i} done"
  #     end
  #   end
  # end
  #
  # gem install async で利用可能
ASYNC

puts

# --- GVL の説明 ---
puts "--- GVL (Global VM Lock) ---"
puts <<~GVL
  Ruby の Thread は GVL (Global VM Lock) の制限を受ける:

  - CPU バウンドな処理は並列実行されない
  - I/O バウンドな処理では GVL が解放される

  解決策:
  1. I/O 処理では Thread が有効
  2. CPU 処理では Ractor を使う (Ruby 3.0+)
  3. プロセスを分ける (fork, parallel gem)
GVL
