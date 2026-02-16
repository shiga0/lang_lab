// CLI Tool - C++ 実装 (TODO管理ツール)

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <optional>
#include <algorithm>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

// === Task ===
struct Task {
    int id;
    string description;
    bool done;

    string to_line() const {
        return (done ? "[x] " : "[ ] ") + description;
    }

    static Task from_line(int id, const string& line) {
        Task task;
        task.id = id;

        if (line.substr(0, 4) == "[x] ") {
            task.done = true;
            task.description = line.substr(4);
        } else if (line.substr(0, 4) == "[ ] ") {
            task.done = false;
            task.description = line.substr(4);
        } else {
            task.done = false;
            task.description = line;
        }

        return task;
    }
};

// === TaskStore ===
class TaskStore {
public:
    TaskStore(const string& file_path) : file_path_(file_path) {}

    vector<Task> load() const {
        vector<Task> tasks;

        if (!fs::exists(file_path_)) {
            return tasks;
        }

        ifstream file(file_path_);
        string line;
        int id = 1;

        while (getline(file, line)) {
            if (!line.empty()) {
                tasks.push_back(Task::from_line(id++, line));
            }
        }

        return tasks;
    }

    void save(const vector<Task>& tasks) const {
        ofstream file(file_path_);

        for (const auto& task : tasks) {
            file << task.to_line() << "\n";
        }
    }

private:
    string file_path_;
};

// === Commands ===
class Commands {
public:
    Commands(TaskStore& store, bool verbose)
        : store_(store), verbose_(verbose) {}

    void add(const string& description) {
        auto tasks = store_.load();
        tasks.push_back(Task{
            static_cast<int>(tasks.size()) + 1,
            description,
            false
        });
        store_.save(tasks);
        cout << "Added: " << description << endl;

        if (verbose_) {
            cout << "  [verbose] Total tasks: " << tasks.size() << endl;
        }
    }

    void list() {
        auto tasks = store_.load();

        if (tasks.empty()) {
            cout << "No tasks found." << endl;
            return;
        }

        cout << "Tasks:" << endl;
        for (const auto& task : tasks) {
            char status = task.done ? 'x' : ' ';
            cout << "  " << task.id << " [" << status << "] " << task.description << endl;
        }

        if (verbose_) {
            int done_count = count_if(tasks.begin(), tasks.end(),
                                       [](const Task& t) { return t.done; });
            cout << "\n  Total: " << tasks.size()
                 << ", Done: " << done_count
                 << ", Pending: " << (tasks.size() - done_count) << endl;
        }
    }

    void done(int id) {
        auto tasks = store_.load();

        auto it = find_if(tasks.begin(), tasks.end(),
                          [id](const Task& t) { return t.id == id; });

        if (it == tasks.end()) {
            cout << "Task " << id << " not found" << endl;
            return;
        }

        if (it->done) {
            cout << "Task " << id << " is already done" << endl;
            return;
        }

        it->done = true;
        store_.save(tasks);
        cout << "Done: " << it->description << endl;
    }

    void undo(int id) {
        auto tasks = store_.load();

        auto it = find_if(tasks.begin(), tasks.end(),
                          [id](const Task& t) { return t.id == id; });

        if (it == tasks.end()) {
            cout << "Task " << id << " not found" << endl;
            return;
        }

        if (!it->done) {
            cout << "Task " << id << " is not completed" << endl;
            return;
        }

        it->done = false;
        store_.save(tasks);
        cout << "Undone: " << it->description << endl;
    }

    void remove(int id) {
        auto tasks = store_.load();

        auto it = find_if(tasks.begin(), tasks.end(),
                          [id](const Task& t) { return t.id == id; });

        if (it == tasks.end()) {
            cout << "Task " << id << " not found" << endl;
            return;
        }

        string description = it->description;
        tasks.erase(it);

        // ID を振り直す
        for (size_t i = 0; i < tasks.size(); ++i) {
            tasks[i].id = static_cast<int>(i) + 1;
        }

        store_.save(tasks);
        cout << "Deleted: " << description << endl;
    }

    void clear() {
        auto tasks = store_.load();

        vector<Task> done_tasks;
        vector<Task> pending_tasks;

        for (const auto& task : tasks) {
            if (task.done) {
                done_tasks.push_back(task);
            } else {
                pending_tasks.push_back(task);
            }
        }

        if (done_tasks.empty()) {
            cout << "No completed tasks to clear." << endl;
            return;
        }

        // ID を振り直す
        for (size_t i = 0; i < pending_tasks.size(); ++i) {
            pending_tasks[i].id = static_cast<int>(i) + 1;
        }

        store_.save(pending_tasks);
        cout << "Cleared " << done_tasks.size() << " completed task(s)." << endl;

        if (verbose_) {
            for (const auto& task : done_tasks) {
                cout << "  - " << task.description << endl;
            }
        }
    }

private:
    TaskStore& store_;
    bool verbose_;
};

// === CLI ===
struct Options {
    string file = "todo.txt";
    bool verbose = false;
    string command;
    vector<string> args;
};

void print_help() {
    cout << R"(
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

)" << endl;
}

Options parse_args(int argc, char* argv[]) {
    Options opts;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];

        if (arg == "-f" || arg == "--file") {
            if (i + 1 < argc) {
                opts.file = argv[++i];
            }
        } else if (arg == "-v" || arg == "--verbose") {
            opts.verbose = true;
        } else if (arg == "-h" || arg == "--help") {
            opts.command = "help";
        } else if (opts.command.empty()) {
            opts.command = arg;
        } else {
            opts.args.push_back(arg);
        }
    }

    return opts;
}

int main(int argc, char* argv[]) {
    Options opts = parse_args(argc, argv);

    if (opts.command.empty() || opts.command == "help") {
        print_help();
        return 0;
    }

    TaskStore store(opts.file);
    Commands commands(store, opts.verbose);

    if (opts.command == "add") {
        if (opts.args.empty()) {
            cout << "Error: add requires a task description" << endl;
            return 1;
        }

        // 引数を結合
        string description;
        for (size_t i = 0; i < opts.args.size(); ++i) {
            if (i > 0) description += " ";
            description += opts.args[i];
        }

        commands.add(description);
    } else if (opts.command == "list" || opts.command == "ls") {
        commands.list();
    } else if (opts.command == "done") {
        if (opts.args.empty()) {
            cout << "Error: done requires a task ID" << endl;
            return 1;
        }
        commands.done(stoi(opts.args[0]));
    } else if (opts.command == "undo") {
        if (opts.args.empty()) {
            cout << "Error: undo requires a task ID" << endl;
            return 1;
        }
        commands.undo(stoi(opts.args[0]));
    } else if (opts.command == "delete" || opts.command == "rm") {
        if (opts.args.empty()) {
            cout << "Error: delete requires a task ID" << endl;
            return 1;
        }
        commands.remove(stoi(opts.args[0]));
    } else if (opts.command == "clear") {
        commands.clear();
    } else {
        cout << "Unknown command: " << opts.command << endl;
        print_help();
        return 1;
    }

    return 0;
}
