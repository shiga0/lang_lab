#!/usr/bin/env ruby
# frozen_string_literal: true

# CLI Tool - Ruby 実装 (TODO管理ツール)

require 'optparse'
require 'fileutils'

# --- Task ---
Task = Struct.new(:id, :description, :done, keyword_init: true) do
  def to_line
    prefix = done ? '[x]' : '[ ]'
    "#{prefix} #{description}"
  end

  def self.from_line(id, line)
    done = line.start_with?('[x]')
    description = if done || line.start_with?('[ ]')
                    line[4..].strip
                  else
                    line.strip
                  end
    new(id: id, description: description, done: done)
  end
end

# --- TaskStore ---
class TaskStore
  def initialize(file_path)
    @file_path = file_path
  end

  def load
    return [] unless File.exist?(@file_path)

    File.readlines(@file_path).each_with_index.map do |line, i|
      next if line.strip.empty?
      Task.from_line(i + 1, line)
    end.compact
  end

  def save(tasks)
    content = tasks.map(&:to_line).join("\n")
    content += "\n" unless content.empty?
    File.write(@file_path, content)
  end
end

# --- Commands ---
module Commands
  class Base
    def initialize(store:, verbose: false)
      @store = store
      @verbose = verbose
    end

    def verbose_log(message)
      puts "  [verbose] #{message}" if @verbose
    end
  end

  class Add < Base
    def run(description)
      tasks = @store.load
      tasks << Task.new(
        id: tasks.size + 1,
        description: description,
        done: false
      )
      @store.save(tasks)
      puts "Added: #{description}"
      verbose_log("Total tasks: #{tasks.size}")
    end
  end

  class List < Base
    def run
      tasks = @store.load

      if tasks.empty?
        puts "No tasks found."
        return
      end

      puts "Tasks:"
      tasks.each do |task|
        status = task.done ? '✓' : ' '
        puts "  #{task.id} [#{status}] #{task.description}"
      end

      if @verbose
        done_count = tasks.count(&:done)
        puts "\n  Total: #{tasks.size}, Done: #{done_count}, Pending: #{tasks.size - done_count}"
      end
    end
  end

  class Done < Base
    def run(id)
      tasks = @store.load
      task = tasks.find { |t| t.id == id }

      if task.nil?
        puts "Task #{id} not found"
        return
      end

      if task.done
        puts "Task #{id} is already done"
        return
      end

      task.done = true
      @store.save(tasks)
      puts "Done: #{task.description}"
    end
  end

  class Clear < Base
    def run
      tasks = @store.load
      done_tasks, pending_tasks = tasks.partition(&:done)

      if done_tasks.empty?
        puts "No completed tasks to clear."
        return
      end

      # IDを振り直して保存
      pending_tasks.each_with_index { |t, i| t.id = i + 1 }
      @store.save(pending_tasks)

      puts "Cleared #{done_tasks.size} completed task(s)."

      if @verbose
        done_tasks.each { |t| puts "  - #{t.description}" }
      end
    end
  end

  class Undo < Base
    def run(id)
      tasks = @store.load
      task = tasks.find { |t| t.id == id }

      if task.nil?
        puts "Task #{id} not found"
        return
      end

      unless task.done
        puts "Task #{id} is not completed"
        return
      end

      task.done = false
      @store.save(tasks)
      puts "Undone: #{task.description}"
    end
  end

  class Delete < Base
    def run(id)
      tasks = @store.load
      task = tasks.find { |t| t.id == id }

      if task.nil?
        puts "Task #{id} not found"
        return
      end

      tasks.delete(task)
      tasks.each_with_index { |t, i| t.id = i + 1 }
      @store.save(tasks)
      puts "Deleted: #{task.description}"
    end
  end
end

# --- CLI ---
class CLI
  def initialize
    @options = {
      file: 'todo.txt',
      verbose: false
    }
    @parser = build_parser
  end

  def run(args)
    remaining = @parser.parse(args)

    if remaining.empty?
      print_help
      return
    end

    command = remaining.shift
    store = TaskStore.new(@options[:file])

    case command
    when 'add'
      description = remaining.join(' ')
      if description.empty?
        puts "Error: add requires a task description"
        return
      end
      Commands::Add.new(store: store, verbose: @options[:verbose]).run(description)

    when 'list', 'ls'
      Commands::List.new(store: store, verbose: @options[:verbose]).run

    when 'done'
      id = remaining.first&.to_i
      if id.nil? || id <= 0
        puts "Error: done requires a valid task ID"
        return
      end
      Commands::Done.new(store: store, verbose: @options[:verbose]).run(id)

    when 'undo'
      id = remaining.first&.to_i
      if id.nil? || id <= 0
        puts "Error: undo requires a valid task ID"
        return
      end
      Commands::Undo.new(store: store, verbose: @options[:verbose]).run(id)

    when 'delete', 'rm'
      id = remaining.first&.to_i
      if id.nil? || id <= 0
        puts "Error: delete requires a valid task ID"
        return
      end
      Commands::Delete.new(store: store, verbose: @options[:verbose]).run(id)

    when 'clear'
      Commands::Clear.new(store: store, verbose: @options[:verbose]).run

    when 'help'
      print_help

    else
      puts "Unknown command: #{command}"
      print_help
    end
  end

  private

  def build_parser
    OptionParser.new do |opts|
      opts.banner = "Usage: todo <command> [options]"

      opts.on('-f', '--file PATH', 'Use a custom file (default: todo.txt)') do |path|
        @options[:file] = path
      end

      opts.on('-v', '--verbose', 'Show verbose output') do
        @options[:verbose] = true
      end

      opts.on('-h', '--help', 'Show this help message') do
        print_help
        exit
      end
    end
  end

  def print_help
    puts <<~HELP

      todo - A simple TODO CLI tool

      USAGE:
          todo <COMMAND> [OPTIONS]

      COMMANDS:
          add <task>    Add a new task
          list, ls      List all tasks
          done <id>     Mark a task as done
          undo <id>     Mark a task as not done
          delete <id>   Delete a task
          clear         Clear all completed tasks
          help          Show this help message

      OPTIONS:
          -f, --file <path>    Use a custom file (default: todo.txt)
          -v, --verbose        Show verbose output

      EXAMPLES:
          todo add "Buy milk"
          todo list
          todo done 1
          todo list --verbose

    HELP
  end
end

# --- テスト ---
def run_tests
  require 'tempfile'

  puts "--- Tests ---"

  Tempfile.create('todo_test') do |file|
    store = TaskStore.new(file.path)

    # 空のタスクリスト
    tasks = store.load
    raise "empty load failed" unless tasks.empty?

    # タスク追加
    store.save([
      Task.new(id: 1, description: "Task 1", done: false),
      Task.new(id: 2, description: "Task 2", done: true)
    ])

    tasks = store.load
    raise "load count failed" unless tasks.size == 2
    raise "task 1 failed" unless tasks[0].description == "Task 1"
    raise "task 1 done failed" unless tasks[0].done == false
    raise "task 2 done failed" unless tasks[1].done == true

    # Task.from_line
    task = Task.from_line(1, "[ ] Test task")
    raise "from_line undone failed" unless task.done == false
    raise "from_line description failed" unless task.description == "Test task"

    task = Task.from_line(1, "[x] Done task")
    raise "from_line done failed" unless task.done == true

    # Task.to_line
    task = Task.new(id: 1, description: "Test", done: false)
    raise "to_line undone failed" unless task.to_line == "[ ] Test"

    task.done = true
    raise "to_line done failed" unless task.to_line == "[x] Test"

    puts "All tests passed!"
  end
end

# --- メイン ---
if __FILE__ == $PROGRAM_NAME
  if ARGV.first == '--test'
    run_tests
  else
    CLI.new.run(ARGV)
  end
end
