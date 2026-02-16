// CLI Tool - Go 実装 (TODO管理ツール)
package main

import (
	"bufio"
	"flag"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	if len(os.Args) < 2 {
		printHelp()
		return
	}

	// グローバルフラグ
	var filePath string
	var verbose bool

	// サブコマンドより前のフラグを処理
	args := os.Args[1:]
	for i := 0; i < len(args); i++ {
		switch args[i] {
		case "-f", "--file":
			if i+1 < len(args) {
				filePath = args[i+1]
				args = append(args[:i], args[i+2:]...)
				i--
			}
		case "-v", "--verbose":
			verbose = true
			args = append(args[:i], args[i+1:]...)
			i--
		case "-h", "--help":
			printHelp()
			return
		}
	}

	if filePath == "" {
		filePath = "todo.txt"
	}

	if len(args) == 0 {
		printHelp()
		return
	}

	store := NewTaskStore(filePath)
	command := args[0]
	cmdArgs := args[1:]

	switch command {
	case "add":
		if len(cmdArgs) == 0 {
			fmt.Println("Error: add requires a task description")
			return
		}
		description := strings.Join(cmdArgs, " ")
		if err := runAdd(store, description, verbose); err != nil {
			fmt.Printf("Error: %v\n", err)
		}

	case "list", "ls":
		if err := runList(store, verbose); err != nil {
			fmt.Printf("Error: %v\n", err)
		}

	case "done":
		if len(cmdArgs) == 0 {
			fmt.Println("Error: done requires a task ID")
			return
		}
		id, err := strconv.Atoi(cmdArgs[0])
		if err != nil || id <= 0 {
			fmt.Println("Error: invalid task ID")
			return
		}
		if err := runDone(store, id, verbose); err != nil {
			fmt.Printf("Error: %v\n", err)
		}

	case "undo":
		if len(cmdArgs) == 0 {
			fmt.Println("Error: undo requires a task ID")
			return
		}
		id, err := strconv.Atoi(cmdArgs[0])
		if err != nil || id <= 0 {
			fmt.Println("Error: invalid task ID")
			return
		}
		if err := runUndo(store, id, verbose); err != nil {
			fmt.Printf("Error: %v\n", err)
		}

	case "delete", "rm":
		if len(cmdArgs) == 0 {
			fmt.Println("Error: delete requires a task ID")
			return
		}
		id, err := strconv.Atoi(cmdArgs[0])
		if err != nil || id <= 0 {
			fmt.Println("Error: invalid task ID")
			return
		}
		if err := runDelete(store, id, verbose); err != nil {
			fmt.Printf("Error: %v\n", err)
		}

	case "clear":
		if err := runClear(store, verbose); err != nil {
			fmt.Printf("Error: %v\n", err)
		}

	case "help":
		printHelp()

	default:
		fmt.Printf("Unknown command: %s\n", command)
		printHelp()
	}
}

func printHelp() {
	fmt.Println(`
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
`)
}

// --- Task ---
type Task struct {
	ID          int
	Description string
	Done        bool
}

func (t Task) ToLine() string {
	prefix := "[ ]"
	if t.Done {
		prefix = "[x]"
	}
	return fmt.Sprintf("%s %s", prefix, t.Description)
}

func TaskFromLine(id int, line string) Task {
	done := strings.HasPrefix(line, "[x]")
	description := line
	if strings.HasPrefix(line, "[x] ") || strings.HasPrefix(line, "[ ] ") {
		description = line[4:]
	}
	return Task{ID: id, Description: description, Done: done}
}

// --- TaskStore ---
type TaskStore struct {
	filePath string
}

func NewTaskStore(filePath string) *TaskStore {
	return &TaskStore{filePath: filePath}
}

func (s *TaskStore) Load() ([]Task, error) {
	file, err := os.Open(s.filePath)
	if os.IsNotExist(err) {
		return []Task{}, nil
	}
	if err != nil {
		return nil, err
	}
	defer file.Close()

	var tasks []Task
	scanner := bufio.NewScanner(file)
	id := 1
	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line != "" {
			tasks = append(tasks, TaskFromLine(id, line))
			id++
		}
	}
	return tasks, scanner.Err()
}

func (s *TaskStore) Save(tasks []Task) error {
	file, err := os.Create(s.filePath)
	if err != nil {
		return err
	}
	defer file.Close()

	for _, task := range tasks {
		fmt.Fprintln(file, task.ToLine())
	}
	return nil
}

// --- Commands ---
func runAdd(store *TaskStore, description string, verbose bool) error {
	tasks, err := store.Load()
	if err != nil {
		return err
	}

	task := Task{
		ID:          len(tasks) + 1,
		Description: description,
		Done:        false,
	}
	tasks = append(tasks, task)

	if err := store.Save(tasks); err != nil {
		return err
	}

	fmt.Printf("Added: %s\n", description)
	if verbose {
		fmt.Printf("  Total tasks: %d\n", len(tasks))
	}
	return nil
}

func runList(store *TaskStore, verbose bool) error {
	tasks, err := store.Load()
	if err != nil {
		return err
	}

	if len(tasks) == 0 {
		fmt.Println("No tasks found.")
		return nil
	}

	fmt.Println("Tasks:")
	for _, task := range tasks {
		status := " "
		if task.Done {
			status = "✓"
		}
		fmt.Printf("  %d [%s] %s\n", task.ID, status, task.Description)
	}

	if verbose {
		doneCount := 0
		for _, t := range tasks {
			if t.Done {
				doneCount++
			}
		}
		fmt.Printf("\n  Total: %d, Done: %d, Pending: %d\n",
			len(tasks), doneCount, len(tasks)-doneCount)
	}
	return nil
}

func runDone(store *TaskStore, id int, verbose bool) error {
	tasks, err := store.Load()
	if err != nil {
		return err
	}

	if id < 1 || id > len(tasks) {
		return fmt.Errorf("task %d not found", id)
	}

	task := &tasks[id-1]
	if task.Done {
		fmt.Printf("Task %d is already done\n", id)
		return nil
	}

	task.Done = true
	if err := store.Save(tasks); err != nil {
		return err
	}

	fmt.Printf("Done: %s\n", task.Description)
	return nil
}

func runUndo(store *TaskStore, id int, verbose bool) error {
	tasks, err := store.Load()
	if err != nil {
		return err
	}

	if id < 1 || id > len(tasks) {
		return fmt.Errorf("task %d not found", id)
	}

	task := &tasks[id-1]
	if !task.Done {
		fmt.Printf("Task %d is not completed\n", id)
		return nil
	}

	task.Done = false
	if err := store.Save(tasks); err != nil {
		return err
	}

	fmt.Printf("Undone: %s\n", task.Description)
	return nil
}

func runDelete(store *TaskStore, id int, verbose bool) error {
	tasks, err := store.Load()
	if err != nil {
		return err
	}

	if id < 1 || id > len(tasks) {
		return fmt.Errorf("task %d not found", id)
	}

	task := tasks[id-1]
	tasks = append(tasks[:id-1], tasks[id:]...)

	// IDを振り直す
	for i := range tasks {
		tasks[i].ID = i + 1
	}

	if err := store.Save(tasks); err != nil {
		return err
	}

	fmt.Printf("Deleted: %s\n", task.Description)
	return nil
}

func runClear(store *TaskStore, verbose bool) error {
	tasks, err := store.Load()
	if err != nil {
		return err
	}

	var pending []Task
	var done []Task
	for _, t := range tasks {
		if t.Done {
			done = append(done, t)
		} else {
			pending = append(pending, t)
		}
	}

	if len(done) == 0 {
		fmt.Println("No completed tasks to clear.")
		return nil
	}

	// IDを振り直す
	for i := range pending {
		pending[i].ID = i + 1
	}

	if err := store.Save(pending); err != nil {
		return err
	}

	fmt.Printf("Cleared %d completed task(s).\n", len(done))
	if verbose {
		for _, t := range done {
			fmt.Printf("  - %s\n", t.Description)
		}
	}
	return nil
}

// テスト用のフラグパーサー
func init() {
	flag.Usage = printHelp
}
