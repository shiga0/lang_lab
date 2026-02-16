#!/usr/bin/env python3
"""CLI Tool - Python 実装 (TODO管理ツール)"""

import argparse
import sys
from pathlib import Path
from dataclasses import dataclass
from typing import List, Optional

# --- Task ---
@dataclass
class Task:
    id: int
    description: str
    done: bool = False

    def to_line(self) -> str:
        prefix = "[x]" if self.done else "[ ]"
        return f"{prefix} {self.description}"

    @classmethod
    def from_line(cls, id: int, line: str) -> "Task":
        line = line.strip()
        if line.startswith("[x]"):
            return cls(id=id, description=line[4:].strip(), done=True)
        elif line.startswith("[ ]"):
            return cls(id=id, description=line[4:].strip(), done=False)
        else:
            return cls(id=id, description=line, done=False)


# --- TaskStore ---
class TaskStore:
    def __init__(self, file_path: str):
        self._path = Path(file_path)

    def load(self) -> List[Task]:
        if not self._path.exists():
            return []

        tasks = []
        for i, line in enumerate(self._path.read_text().splitlines(), start=1):
            if line.strip():
                tasks.append(Task.from_line(i, line))
        return tasks

    def save(self, tasks: List[Task]) -> None:
        content = "\n".join(task.to_line() for task in tasks)
        if content:
            content += "\n"
        self._path.write_text(content)


# --- Commands ---
class Commands:
    def __init__(self, store: TaskStore, verbose: bool = False):
        self._store = store
        self._verbose = verbose

    def _verbose_log(self, message: str) -> None:
        if self._verbose:
            print(f"  [verbose] {message}")

    def add(self, description: str) -> None:
        tasks = self._store.load()
        tasks.append(Task(id=len(tasks) + 1, description=description, done=False))
        self._store.save(tasks)
        print(f"Added: {description}")
        self._verbose_log(f"Total tasks: {len(tasks)}")

    def list(self) -> None:
        tasks = self._store.load()

        if not tasks:
            print("No tasks found.")
            return

        print("Tasks:")
        for task in tasks:
            status = "\u2713" if task.done else " "
            print(f"  {task.id} [{status}] {task.description}")

        if self._verbose:
            done_count = sum(1 for t in tasks if t.done)
            print(f"\n  Total: {len(tasks)}, Done: {done_count}, Pending: {len(tasks) - done_count}")

    def done(self, id: int) -> None:
        tasks = self._store.load()
        task = next((t for t in tasks if t.id == id), None)

        if task is None:
            print(f"Task {id} not found")
            return

        if task.done:
            print(f"Task {id} is already done")
            return

        task.done = True
        self._store.save(tasks)
        print(f"Done: {task.description}")

    def undo(self, id: int) -> None:
        tasks = self._store.load()
        task = next((t for t in tasks if t.id == id), None)

        if task is None:
            print(f"Task {id} not found")
            return

        if not task.done:
            print(f"Task {id} is not completed")
            return

        task.done = False
        self._store.save(tasks)
        print(f"Undone: {task.description}")

    def delete(self, id: int) -> None:
        tasks = self._store.load()
        task = next((t for t in tasks if t.id == id), None)

        if task is None:
            print(f"Task {id} not found")
            return

        tasks.remove(task)
        # IDを振り直す
        for i, t in enumerate(tasks, start=1):
            t.id = i
        self._store.save(tasks)
        print(f"Deleted: {task.description}")

    def clear(self) -> None:
        tasks = self._store.load()
        done_tasks = [t for t in tasks if t.done]
        pending_tasks = [t for t in tasks if not t.done]

        if not done_tasks:
            print("No completed tasks to clear.")
            return

        # IDを振り直して保存
        for i, t in enumerate(pending_tasks, start=1):
            t.id = i
        self._store.save(pending_tasks)

        print(f"Cleared {len(done_tasks)} completed task(s).")

        if self._verbose:
            for t in done_tasks:
                print(f"  - {t.description}")


# --- CLI ---
def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog="todo",
        description="A simple TODO CLI tool",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
COMMANDS:
    add <task>    Add a new task
    list, ls      List all tasks
    done <id>     Mark a task as done
    undo <id>     Mark a task as not done
    delete <id>   Delete a task
    clear         Clear all completed tasks

EXAMPLES:
    todo add "Buy milk"
    todo list
    todo done 1
    todo list --verbose
""",
    )

    parser.add_argument(
        "-f", "--file",
        default="todo.txt",
        help="Use a custom file (default: todo.txt)",
    )
    parser.add_argument(
        "-v", "--verbose",
        action="store_true",
        help="Show verbose output",
    )
    parser.add_argument(
        "command",
        nargs="?",
        help="Command to run",
    )
    parser.add_argument(
        "args",
        nargs="*",
        help="Command arguments",
    )

    return parser


def main(args: Optional[List[str]] = None) -> None:
    parser = build_parser()
    parsed = parser.parse_args(args)

    if not parsed.command:
        parser.print_help()
        return

    store = TaskStore(parsed.file)
    commands = Commands(store, verbose=parsed.verbose)

    command = parsed.command.lower()

    if command == "add":
        if not parsed.args:
            print("Error: add requires a task description")
            return
        description = " ".join(parsed.args)
        commands.add(description)

    elif command in ("list", "ls"):
        commands.list()

    elif command == "done":
        if not parsed.args or not parsed.args[0].isdigit():
            print("Error: done requires a valid task ID")
            return
        commands.done(int(parsed.args[0]))

    elif command == "undo":
        if not parsed.args or not parsed.args[0].isdigit():
            print("Error: undo requires a valid task ID")
            return
        commands.undo(int(parsed.args[0]))

    elif command in ("delete", "rm"):
        if not parsed.args or not parsed.args[0].isdigit():
            print("Error: delete requires a valid task ID")
            return
        commands.delete(int(parsed.args[0]))

    elif command == "clear":
        commands.clear()

    elif command == "help":
        parser.print_help()

    else:
        print(f"Unknown command: {command}")
        parser.print_help()


# --- テスト ---
def run_tests() -> None:
    import tempfile

    print("--- Tests ---")

    with tempfile.NamedTemporaryFile(mode="w", suffix=".txt", delete=False) as f:
        temp_path = f.name

    try:
        store = TaskStore(temp_path)

        # 空のタスクリスト
        tasks = store.load()
        assert len(tasks) == 0, "empty load failed"

        # タスク追加
        store.save([
            Task(id=1, description="Task 1", done=False),
            Task(id=2, description="Task 2", done=True),
        ])

        tasks = store.load()
        assert len(tasks) == 2, "load count failed"
        assert tasks[0].description == "Task 1", "task 1 failed"
        assert tasks[0].done is False, "task 1 done failed"
        assert tasks[1].done is True, "task 2 done failed"

        # Task.from_line
        task = Task.from_line(1, "[ ] Test task")
        assert task.done is False, "from_line undone failed"
        assert task.description == "Test task", "from_line description failed"

        task = Task.from_line(1, "[x] Done task")
        assert task.done is True, "from_line done failed"

        # Task.to_line
        task = Task(id=1, description="Test", done=False)
        assert task.to_line() == "[ ] Test", "to_line undone failed"

        task.done = True
        assert task.to_line() == "[x] Test", "to_line done failed"

        print("All tests passed!")
    finally:
        Path(temp_path).unlink(missing_ok=True)


# --- メイン ---
if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "--test":
        run_tests()
    else:
        main()
