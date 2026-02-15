//! CLI Tool - Rust 実装
//!
//! 標準ライブラリのみでシンプルな TODO CLI を実装

use std::env;
use std::fs::{self, File, OpenOptions};
use std::io::{BufRead, BufReader, Write};
use std::path::PathBuf;

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() < 2 {
        print_help();
        return;
    }

    let config = Config::parse(&args[1..]);

    match config {
        Ok(config) => {
            if let Err(e) = run(config) {
                eprintln!("Error: {}", e);
                std::process::exit(1);
            }
        }
        Err(e) => {
            eprintln!("Error: {}", e);
            print_help();
            std::process::exit(1);
        }
    }
}

fn print_help() {
    println!(
        r#"
todo - A simple TODO CLI tool

USAGE:
    todo <COMMAND> [OPTIONS]

COMMANDS:
    add <task>    Add a new task
    list          List all tasks
    done <id>     Mark a task as done
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
"#
    );
}

/// コマンドの種類
#[derive(Debug)]
enum Command {
    Add(String),
    List,
    Done(usize),
    Clear,
    Help,
}

/// 設定
#[derive(Debug)]
struct Config {
    command: Command,
    file_path: PathBuf,
    verbose: bool,
}

impl Config {
    fn parse(args: &[String]) -> Result<Self, String> {
        let mut file_path = PathBuf::from("todo.txt");
        let mut verbose = false;
        let mut remaining_args: Vec<&str> = Vec::new();

        let mut iter = args.iter().peekable();

        // オプションとコマンドを分離
        while let Some(arg) = iter.next() {
            match arg.as_str() {
                "-f" | "--file" => {
                    let path = iter.next().ok_or("--file requires a path")?;
                    file_path = PathBuf::from(path);
                }
                "-v" | "--verbose" => {
                    verbose = true;
                }
                _ => {
                    remaining_args.push(arg);
                }
            }
        }

        if remaining_args.is_empty() {
            return Err("No command specified".to_string());
        }

        let command = match remaining_args[0] {
            "add" => {
                if remaining_args.len() < 2 {
                    return Err("add requires a task description".to_string());
                }
                Command::Add(remaining_args[1..].join(" "))
            }
            "list" => Command::List,
            "done" => {
                if remaining_args.len() < 2 {
                    return Err("done requires a task ID".to_string());
                }
                let id: usize = remaining_args[1]
                    .parse()
                    .map_err(|_| "Invalid task ID")?;
                Command::Done(id)
            }
            "clear" => Command::Clear,
            "help" | "-h" | "--help" => Command::Help,
            other => return Err(format!("Unknown command: {}", other)),
        };

        Ok(Config {
            command,
            file_path,
            verbose,
        })
    }
}

/// タスク
#[derive(Debug, Clone)]
struct Task {
    id: usize,
    description: String,
    done: bool,
}

impl Task {
    fn from_line(id: usize, line: &str) -> Self {
        let done = line.starts_with("[x] ");
        let description = if done || line.starts_with("[ ] ") {
            line[4..].to_string()
        } else {
            line.to_string()
        };

        Task {
            id,
            description,
            done,
        }
    }

    fn to_line(&self) -> String {
        let prefix = if self.done { "[x]" } else { "[ ]" };
        format!("{} {}", prefix, self.description)
    }
}

fn run(config: Config) -> Result<(), String> {
    match &config.command {
        Command::Add(task) => add_task(&config, task),
        Command::List => list_tasks(&config),
        Command::Done(id) => mark_done(&config, *id),
        Command::Clear => clear_done(&config),
        Command::Help => {
            print_help();
            Ok(())
        }
    }
}

fn add_task(config: &Config, description: &str) -> Result<(), String> {
    let mut file = OpenOptions::new()
        .create(true)
        .append(true)
        .open(&config.file_path)
        .map_err(|e| format!("Failed to open file: {}", e))?;

    let task = Task {
        id: 0,
        description: description.to_string(),
        done: false,
    };

    writeln!(file, "{}", task.to_line())
        .map_err(|e| format!("Failed to write: {}", e))?;

    println!("Added: {}", description);

    if config.verbose {
        println!("  File: {:?}", config.file_path);
    }

    Ok(())
}

fn list_tasks(config: &Config) -> Result<(), String> {
    let tasks = load_tasks(&config.file_path)?;

    if tasks.is_empty() {
        println!("No tasks found.");
        return Ok(());
    }

    println!("Tasks:");
    for task in &tasks {
        let status = if task.done { "✓" } else { " " };
        println!("  {} [{}] {}", task.id, status, task.description);
    }

    if config.verbose {
        let done_count = tasks.iter().filter(|t| t.done).count();
        println!("\n  Total: {}, Done: {}, Pending: {}",
            tasks.len(), done_count, tasks.len() - done_count);
    }

    Ok(())
}

fn mark_done(config: &Config, id: usize) -> Result<(), String> {
    let mut tasks = load_tasks(&config.file_path)?;

    let task = tasks
        .iter_mut()
        .find(|t| t.id == id)
        .ok_or_else(|| format!("Task {} not found", id))?;

    if task.done {
        println!("Task {} is already done", id);
        return Ok(());
    }

    task.done = true;
    println!("Done: {}", task.description);

    save_tasks(&config.file_path, &tasks)?;

    Ok(())
}

fn clear_done(config: &Config) -> Result<(), String> {
    let tasks = load_tasks(&config.file_path)?;
    let (done, pending): (Vec<_>, Vec<_>) = tasks.iter().partition(|t| t.done);

    if done.is_empty() {
        println!("No completed tasks to clear.");
        return Ok(());
    }

    // pending のみを保存
    let pending: Vec<Task> = pending.into_iter().cloned().collect();
    save_tasks(&config.file_path, &pending)?;

    println!("Cleared {} completed task(s).", done.len());

    if config.verbose {
        for task in done {
            println!("  - {}", task.description);
        }
    }

    Ok(())
}

fn load_tasks(path: &PathBuf) -> Result<Vec<Task>, String> {
    if !path.exists() {
        return Ok(Vec::new());
    }

    let file = File::open(path)
        .map_err(|e| format!("Failed to open file: {}", e))?;

    let reader = BufReader::new(file);
    let mut tasks = Vec::new();

    for (i, line) in reader.lines().enumerate() {
        let line = line.map_err(|e| format!("Failed to read line: {}", e))?;
        if !line.trim().is_empty() {
            tasks.push(Task::from_line(i + 1, &line));
        }
    }

    Ok(tasks)
}

fn save_tasks(path: &PathBuf, tasks: &[Task]) -> Result<(), String> {
    let content: String = tasks
        .iter()
        .map(|t| t.to_line())
        .collect::<Vec<_>>()
        .join("\n");

    fs::write(path, content + "\n")
        .map_err(|e| format!("Failed to write file: {}", e))?;

    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_parse_add() {
        let args = vec!["add".to_string(), "Buy milk".to_string()];
        let config = Config::parse(&args).unwrap();

        match config.command {
            Command::Add(s) => assert_eq!(s, "Buy milk"),
            _ => panic!("Expected Add command"),
        }
    }

    #[test]
    fn test_parse_list() {
        let args = vec!["list".to_string()];
        let config = Config::parse(&args).unwrap();

        match config.command {
            Command::List => {}
            _ => panic!("Expected List command"),
        }
    }

    #[test]
    fn test_parse_done() {
        let args = vec!["done".to_string(), "3".to_string()];
        let config = Config::parse(&args).unwrap();

        match config.command {
            Command::Done(id) => assert_eq!(id, 3),
            _ => panic!("Expected Done command"),
        }
    }

    #[test]
    fn test_parse_verbose() {
        let args = vec!["--verbose".to_string(), "list".to_string()];
        let config = Config::parse(&args).unwrap();

        assert!(config.verbose);
    }

    #[test]
    fn test_parse_custom_file() {
        let args = vec![
            "--file".to_string(),
            "custom.txt".to_string(),
            "list".to_string(),
        ];
        let config = Config::parse(&args).unwrap();

        assert_eq!(config.file_path, PathBuf::from("custom.txt"));
    }

    #[test]
    fn test_task_from_line() {
        let task = Task::from_line(1, "[ ] Buy milk");
        assert!(!task.done);
        assert_eq!(task.description, "Buy milk");

        let task = Task::from_line(2, "[x] Done task");
        assert!(task.done);
        assert_eq!(task.description, "Done task");
    }

    #[test]
    fn test_task_to_line() {
        let task = Task {
            id: 1,
            description: "Test".to_string(),
            done: false,
        };
        assert_eq!(task.to_line(), "[ ] Test");

        let task = Task {
            id: 2,
            description: "Done".to_string(),
            done: true,
        };
        assert_eq!(task.to_line(), "[x] Done");
    }

    #[test]
    fn test_parse_error_no_command() {
        let args: Vec<String> = vec![];
        assert!(Config::parse(&args).is_err());
    }

    #[test]
    fn test_parse_error_unknown_command() {
        let args = vec!["unknown".to_string()];
        assert!(Config::parse(&args).is_err());
    }
}
