// CLI Tool - TypeScript 実装 (TODO管理ツール)
//
// Node.js で動作する CLI アプリケーション

import * as fs from "fs";
import * as path from "path";
import * as readline from "readline";

// --- 型定義 ---
interface Task {
  id: number;
  description: string;
  done: boolean;
}

interface Options {
  filePath: string;
  verbose: boolean;
}

// --- TaskStore ---
class TaskStore {
  constructor(private filePath: string) {}

  load(): Task[] {
    if (!fs.existsSync(this.filePath)) {
      return [];
    }

    const content = fs.readFileSync(this.filePath, "utf-8");
    const tasks: Task[] = [];
    let id = 1;

    for (const line of content.split("\n")) {
      const trimmed = line.trim();
      if (!trimmed) continue;

      const done = trimmed.startsWith("[x]");
      let description = trimmed;
      if (trimmed.startsWith("[x] ") || trimmed.startsWith("[ ] ")) {
        description = trimmed.slice(4);
      }

      tasks.push({ id: id++, description, done });
    }

    return tasks;
  }

  save(tasks: Task[]): void {
    const lines = tasks.map((task) => {
      const prefix = task.done ? "[x]" : "[ ]";
      return `${prefix} ${task.description}`;
    });
    fs.writeFileSync(this.filePath, lines.join("\n") + "\n");
  }
}

// --- コマンド ---
function runAdd(store: TaskStore, description: string, options: Options): void {
  const tasks = store.load();
  const task: Task = {
    id: tasks.length + 1,
    description,
    done: false,
  };
  tasks.push(task);
  store.save(tasks);

  console.log(`Added: ${description}`);
  if (options.verbose) {
    console.log(`  Total tasks: ${tasks.length}`);
  }
}

function runList(store: TaskStore, options: Options): void {
  const tasks = store.load();

  if (tasks.length === 0) {
    console.log("No tasks found.");
    return;
  }

  console.log("Tasks:");
  for (const task of tasks) {
    const status = task.done ? "✓" : " ";
    console.log(`  ${task.id} [${status}] ${task.description}`);
  }

  if (options.verbose) {
    const doneCount = tasks.filter((t) => t.done).length;
    console.log();
    console.log(`  Total: ${tasks.length}, Done: ${doneCount}, Pending: ${tasks.length - doneCount}`);
  }
}

function runDone(store: TaskStore, id: number, options: Options): void {
  const tasks = store.load();

  if (id < 1 || id > tasks.length) {
    console.error(`Error: task ${id} not found`);
    return;
  }

  const task = tasks[id - 1];
  if (task.done) {
    console.log(`Task ${id} is already done`);
    return;
  }

  task.done = true;
  store.save(tasks);
  console.log(`Done: ${task.description}`);
}

function runUndo(store: TaskStore, id: number, options: Options): void {
  const tasks = store.load();

  if (id < 1 || id > tasks.length) {
    console.error(`Error: task ${id} not found`);
    return;
  }

  const task = tasks[id - 1];
  if (!task.done) {
    console.log(`Task ${id} is not completed`);
    return;
  }

  task.done = false;
  store.save(tasks);
  console.log(`Undone: ${task.description}`);
}

function runDelete(store: TaskStore, id: number, options: Options): void {
  const tasks = store.load();

  if (id < 1 || id > tasks.length) {
    console.error(`Error: task ${id} not found`);
    return;
  }

  const task = tasks[id - 1];
  tasks.splice(id - 1, 1);

  // ID を振り直す
  tasks.forEach((t, i) => (t.id = i + 1));

  store.save(tasks);
  console.log(`Deleted: ${task.description}`);
}

function runClear(store: TaskStore, options: Options): void {
  const tasks = store.load();
  const pending = tasks.filter((t) => !t.done);
  const done = tasks.filter((t) => t.done);

  if (done.length === 0) {
    console.log("No completed tasks to clear.");
    return;
  }

  // ID を振り直す
  pending.forEach((t, i) => (t.id = i + 1));

  store.save(pending);
  console.log(`Cleared ${done.length} completed task(s).`);

  if (options.verbose) {
    for (const task of done) {
      console.log(`  - ${task.description}`);
    }
  }
}

function printHelp(): void {
  console.log(`
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
`);
}

// --- 引数パーサー ---
function parseArgs(args: string[]): { command: string; cmdArgs: string[]; options: Options } {
  const options: Options = {
    filePath: "todo.txt",
    verbose: false,
  };

  const remaining: string[] = [];
  let i = 0;

  while (i < args.length) {
    const arg = args[i];

    if (arg === "-f" || arg === "--file") {
      options.filePath = args[++i] ?? "todo.txt";
    } else if (arg === "-v" || arg === "--verbose") {
      options.verbose = true;
    } else if (arg === "-h" || arg === "--help") {
      printHelp();
      process.exit(0);
    } else if (!arg.startsWith("-")) {
      remaining.push(arg);
    }
    i++;
  }

  const command = remaining[0] ?? "help";
  const cmdArgs = remaining.slice(1);

  return { command, cmdArgs, options };
}

// --- メイン ---
function main(): void {
  const args = process.argv.slice(2);

  if (args.length === 0) {
    printHelp();
    return;
  }

  const { command, cmdArgs, options } = parseArgs(args);
  const store = new TaskStore(options.filePath);

  switch (command) {
    case "add":
      if (cmdArgs.length === 0) {
        console.error("Error: add requires a task description");
        return;
      }
      runAdd(store, cmdArgs.join(" "), options);
      break;

    case "list":
    case "ls":
      runList(store, options);
      break;

    case "done": {
      if (cmdArgs.length === 0) {
        console.error("Error: done requires a task ID");
        return;
      }
      const id = parseInt(cmdArgs[0]);
      if (isNaN(id) || id <= 0) {
        console.error("Error: invalid task ID");
        return;
      }
      runDone(store, id, options);
      break;
    }

    case "undo": {
      if (cmdArgs.length === 0) {
        console.error("Error: undo requires a task ID");
        return;
      }
      const id = parseInt(cmdArgs[0]);
      if (isNaN(id) || id <= 0) {
        console.error("Error: invalid task ID");
        return;
      }
      runUndo(store, id, options);
      break;
    }

    case "delete":
    case "rm": {
      if (cmdArgs.length === 0) {
        console.error("Error: delete requires a task ID");
        return;
      }
      const id = parseInt(cmdArgs[0]);
      if (isNaN(id) || id <= 0) {
        console.error("Error: invalid task ID");
        return;
      }
      runDelete(store, id, options);
      break;
    }

    case "clear":
      runClear(store, options);
      break;

    case "help":
      printHelp();
      break;

    default:
      console.error(`Unknown command: ${command}`);
      printHelp();
  }
}

main();
