// CLI Tool - C 実装
// シンプルなタスク管理 CLI

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_TASKS 100
#define MAX_TASK_LEN 256
#define DEFAULT_FILE "tasks.txt"

// === タスク ===
typedef struct {
    int id;
    char description[MAX_TASK_LEN];
    bool done;
    time_t created_at;
} Task;

// === タスクリスト ===
typedef struct {
    Task tasks[MAX_TASKS];
    int count;
} TaskList;

// === オプション ===
typedef struct {
    bool verbose;
    const char *file;
    const char *command;
    const char *arg;
    bool show_help;
    bool show_version;
} Options;

// === ユーティリティ ===

void print_help(const char *prog_name) {
    printf("Usage: %s [OPTIONS] <COMMAND> [ARGS]\n\n", prog_name);
    printf("A simple task manager CLI\n\n");
    printf("Commands:\n");
    printf("  add <task>    Add a new task\n");
    printf("  list          List all tasks\n");
    printf("  done <id>     Mark a task as done\n");
    printf("  remove <id>   Remove a task\n");
    printf("  clear         Remove all completed tasks\n\n");
    printf("Options:\n");
    printf("  -f, --file <path>  Specify task file (default: %s)\n", DEFAULT_FILE);
    printf("  -v, --verbose      Enable verbose output\n");
    printf("  -h, --help         Show this help message\n");
    printf("  --version          Show version\n");
}

void print_version(void) {
    printf("todo 1.0.0\n");
}

// === 引数パース ===

bool parse_args(int argc, char *argv[], Options *opts) {
    opts->verbose = false;
    opts->file = DEFAULT_FILE;
    opts->command = NULL;
    opts->arg = NULL;
    opts->show_help = false;
    opts->show_version = false;

    int i = 1;
    while (i < argc) {
        const char *arg = argv[i];

        // オプション
        if (arg[0] == '-') {
            if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
                opts->show_help = true;
                return true;
            }
            if (strcmp(arg, "--version") == 0) {
                opts->show_version = true;
                return true;
            }
            if (strcmp(arg, "-v") == 0 || strcmp(arg, "--verbose") == 0) {
                opts->verbose = true;
                i++;
                continue;
            }
            if (strcmp(arg, "-f") == 0 || strcmp(arg, "--file") == 0) {
                if (i + 1 >= argc) {
                    fprintf(stderr, "Error: --file requires an argument\n");
                    return false;
                }
                opts->file = argv[++i];
                i++;
                continue;
            }

            fprintf(stderr, "Error: Unknown option: %s\n", arg);
            return false;
        }

        // コマンド
        if (opts->command == NULL) {
            opts->command = arg;
        } else if (opts->arg == NULL) {
            opts->arg = arg;
        }
        i++;
    }

    return true;
}

// === ファイル I/O ===

bool load_tasks(const char *filename, TaskList *list, bool verbose) {
    list->count = 0;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        if (verbose) {
            printf("Note: Task file not found, starting fresh\n");
        }
        return true;  // ファイルがなくてもOK
    }

    char line[512];
    while (fgets(line, sizeof(line), fp) && list->count < MAX_TASKS) {
        Task *task = &list->tasks[list->count];

        // フォーマット: id|done|timestamp|description
        char *token = strtok(line, "|");
        if (token == NULL) continue;
        task->id = atoi(token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        task->done = (atoi(token) == 1);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        task->created_at = (time_t)atol(token);

        token = strtok(NULL, "\n");
        if (token == NULL) continue;
        strncpy(task->description, token, MAX_TASK_LEN - 1);
        task->description[MAX_TASK_LEN - 1] = '\0';

        list->count++;
    }

    fclose(fp);

    if (verbose) {
        printf("Loaded %d tasks from %s\n", list->count, filename);
    }

    return true;
}

bool save_tasks(const char *filename, const TaskList *list, bool verbose) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open file for writing: %s\n", filename);
        return false;
    }

    for (int i = 0; i < list->count; i++) {
        const Task *task = &list->tasks[i];
        fprintf(fp, "%d|%d|%ld|%s\n",
            task->id,
            task->done ? 1 : 0,
            (long)task->created_at,
            task->description
        );
    }

    fclose(fp);

    if (verbose) {
        printf("Saved %d tasks to %s\n", list->count, filename);
    }

    return true;
}

// === タスク操作 ===

int get_next_id(const TaskList *list) {
    int max_id = 0;
    for (int i = 0; i < list->count; i++) {
        if (list->tasks[i].id > max_id) {
            max_id = list->tasks[i].id;
        }
    }
    return max_id + 1;
}

bool add_task(TaskList *list, const char *description) {
    if (list->count >= MAX_TASKS) {
        fprintf(stderr, "Error: Task list is full\n");
        return false;
    }

    Task *task = &list->tasks[list->count];
    task->id = get_next_id(list);
    strncpy(task->description, description, MAX_TASK_LEN - 1);
    task->description[MAX_TASK_LEN - 1] = '\0';
    task->done = false;
    task->created_at = time(NULL);

    list->count++;
    return true;
}

void list_tasks(const TaskList *list) {
    if (list->count == 0) {
        printf("No tasks found.\n");
        return;
    }

    printf("Tasks:\n");
    printf("  ID  Status  Description\n");
    printf("  --  ------  -----------\n");

    for (int i = 0; i < list->count; i++) {
        const Task *task = &list->tasks[i];
        printf("  %2d  [%c]     %s\n",
            task->id,
            task->done ? 'x' : ' ',
            task->description
        );
    }

    // サマリー
    int done_count = 0;
    for (int i = 0; i < list->count; i++) {
        if (list->tasks[i].done) done_count++;
    }
    printf("\n%d/%d tasks completed\n", done_count, list->count);
}

bool mark_done(TaskList *list, int id) {
    for (int i = 0; i < list->count; i++) {
        if (list->tasks[i].id == id) {
            list->tasks[i].done = true;
            return true;
        }
    }
    return false;
}

bool remove_task(TaskList *list, int id) {
    for (int i = 0; i < list->count; i++) {
        if (list->tasks[i].id == id) {
            // シフト
            for (int j = i; j < list->count - 1; j++) {
                list->tasks[j] = list->tasks[j + 1];
            }
            list->count--;
            return true;
        }
    }
    return false;
}

int clear_completed(TaskList *list) {
    int removed = 0;
    int i = 0;
    while (i < list->count) {
        if (list->tasks[i].done) {
            remove_task(list, list->tasks[i].id);
            removed++;
        } else {
            i++;
        }
    }
    return removed;
}

// === コマンド実行 ===

int cmd_add(const Options *opts, TaskList *list) {
    if (opts->arg == NULL) {
        fprintf(stderr, "Error: add requires a task description\n");
        fprintf(stderr, "Usage: todo add <task>\n");
        return 1;
    }

    if (add_task(list, opts->arg)) {
        printf("Added: %s\n", opts->arg);
        if (opts->verbose) {
            printf("Task ID: %d\n", list->tasks[list->count - 1].id);
        }
        return 0;
    }
    return 1;
}

int cmd_list(const Options *opts, TaskList *list) {
    (void)opts;
    list_tasks(list);
    return 0;
}

int cmd_done(const Options *opts, TaskList *list) {
    if (opts->arg == NULL) {
        fprintf(stderr, "Error: done requires a task ID\n");
        fprintf(stderr, "Usage: todo done <id>\n");
        return 1;
    }

    int id = atoi(opts->arg);
    if (id <= 0) {
        fprintf(stderr, "Error: Invalid task ID: %s\n", opts->arg);
        return 1;
    }

    if (mark_done(list, id)) {
        printf("Marked task %d as done\n", id);
        return 0;
    } else {
        fprintf(stderr, "Error: Task %d not found\n", id);
        return 1;
    }
}

int cmd_remove(const Options *opts, TaskList *list) {
    if (opts->arg == NULL) {
        fprintf(stderr, "Error: remove requires a task ID\n");
        fprintf(stderr, "Usage: todo remove <id>\n");
        return 1;
    }

    int id = atoi(opts->arg);
    if (id <= 0) {
        fprintf(stderr, "Error: Invalid task ID: %s\n", opts->arg);
        return 1;
    }

    if (remove_task(list, id)) {
        printf("Removed task %d\n", id);
        return 0;
    } else {
        fprintf(stderr, "Error: Task %d not found\n", id);
        return 1;
    }
}

int cmd_clear(const Options *opts, TaskList *list) {
    (void)opts;
    int removed = clear_completed(list);
    printf("Cleared %d completed task(s)\n", removed);
    return 0;
}

// === メイン ===

int main(int argc, char *argv[]) {
    Options opts;

    if (!parse_args(argc, argv, &opts)) {
        return 1;
    }

    if (opts.show_help) {
        print_help(argv[0]);
        return 0;
    }

    if (opts.show_version) {
        print_version();
        return 0;
    }

    if (opts.command == NULL) {
        fprintf(stderr, "Error: No command specified\n");
        fprintf(stderr, "Run '%s --help' for usage\n", argv[0]);
        return 1;
    }

    // タスクをロード
    TaskList list;
    if (!load_tasks(opts.file, &list, opts.verbose)) {
        return 1;
    }

    // コマンド実行
    int result;
    if (strcmp(opts.command, "add") == 0) {
        result = cmd_add(&opts, &list);
    } else if (strcmp(opts.command, "list") == 0) {
        result = cmd_list(&opts, &list);
    } else if (strcmp(opts.command, "done") == 0) {
        result = cmd_done(&opts, &list);
    } else if (strcmp(opts.command, "remove") == 0) {
        result = cmd_remove(&opts, &list);
    } else if (strcmp(opts.command, "clear") == 0) {
        result = cmd_clear(&opts, &list);
    } else {
        fprintf(stderr, "Error: Unknown command: %s\n", opts.command);
        fprintf(stderr, "Run '%s --help' for usage\n", argv[0]);
        return 1;
    }

    // 変更があれば保存
    if (result == 0 && strcmp(opts.command, "list") != 0) {
        if (!save_tasks(opts.file, &list, opts.verbose)) {
            return 1;
        }
    }

    return result;
}

/*
コンパイルと実行:
    clang -std=c11 -Wall -O2 cli_tool.c -o todo && ./todo --help

使用例:
    ./todo add "Learn C programming"
    ./todo add "Build a project"
    ./todo list
    ./todo done 1
    ./todo list
    ./todo remove 2
    ./todo clear
    ./todo -v -f mytasks.txt add "Custom file task"
*/
