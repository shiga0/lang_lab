//! HTTP Server - Rust 実装
//!
//! 標準ライブラリのみでシンプルな HTTP サーバーを実装

use std::collections::HashMap;
use std::io::{BufRead, BufReader, Write};
use std::net::{TcpListener, TcpStream};

fn main() {
    println!("=== HTTP Server Demo ===\n");

    let addr = "127.0.0.1:8080";
    println!("Starting server at http://{}", addr);
    println!("Try:");
    println!("  curl http://localhost:8080/");
    println!("  curl http://localhost:8080/hello/world");
    println!("  curl http://localhost:8080/json");
    println!("\nPress Ctrl+C to stop\n");

    let listener = TcpListener::bind(addr).expect("Failed to bind");

    for stream in listener.incoming() {
        match stream {
            Ok(stream) => {
                handle_connection(stream);
            }
            Err(e) => {
                eprintln!("Connection error: {}", e);
            }
        }
    }
}

fn handle_connection(mut stream: TcpStream) {
    let buf_reader = BufReader::new(&stream);
    let request_line = buf_reader.lines().next();

    let request_line = match request_line {
        Some(Ok(line)) => line,
        _ => return,
    };

    println!("Request: {}", request_line);

    let response = route_request(&request_line);

    if let Err(e) = stream.write_all(response.as_bytes()) {
        eprintln!("Failed to write response: {}", e);
    }
}

fn route_request(request_line: &str) -> String {
    let parts: Vec<&str> = request_line.split_whitespace().collect();

    if parts.len() < 2 {
        return build_response(400, "Bad Request", "Invalid request");
    }

    let method = parts[0];
    let path = parts[1];

    if method != "GET" {
        return build_response(405, "Method Not Allowed", "Only GET is supported");
    }

    match_route(path)
}

fn match_route(path: &str) -> String {
    // ルーティング
    if path == "/" {
        return build_response(200, "OK", "Welcome to Rust HTTP Server!");
    }

    if path == "/json" {
        return build_json_response(200, r#"{"message": "Hello, JSON!", "status": "ok"}"#);
    }

    if path.starts_with("/hello/") {
        let name = &path[7..]; // "/hello/" の後の部分
        if name.is_empty() {
            return build_response(400, "Bad Request", "Name is required");
        }
        let body = format!("Hello, {}!", name);
        return build_response(200, "OK", &body);
    }

    if path == "/headers" {
        return build_response(200, "OK", "Use /headers endpoint to see request headers");
    }

    // 404
    build_response(404, "Not Found", &format!("Path '{}' not found", path))
}

fn build_response(status_code: u16, status_text: &str, body: &str) -> String {
    format!(
        "HTTP/1.1 {} {}\r\n\
         Content-Type: text/plain; charset=utf-8\r\n\
         Content-Length: {}\r\n\
         Connection: close\r\n\
         \r\n\
         {}",
        status_code,
        status_text,
        body.len(),
        body
    )
}

fn build_json_response(status_code: u16, body: &str) -> String {
    format!(
        "HTTP/1.1 {} OK\r\n\
         Content-Type: application/json; charset=utf-8\r\n\
         Content-Length: {}\r\n\
         Connection: close\r\n\
         \r\n\
         {}",
        status_code,
        body.len(),
        body
    )
}

/// HTTP リクエストをパースする
#[derive(Debug)]
pub struct Request {
    pub method: String,
    pub path: String,
    pub headers: HashMap<String, String>,
}

impl Request {
    pub fn parse(raw: &str) -> Option<Self> {
        let mut lines = raw.lines();

        // リクエストライン
        let request_line = lines.next()?;
        let parts: Vec<&str> = request_line.split_whitespace().collect();

        if parts.len() < 2 {
            return None;
        }

        let method = parts[0].to_string();
        let path = parts[1].to_string();

        // ヘッダー
        let mut headers = HashMap::new();
        for line in lines {
            if line.is_empty() {
                break;
            }
            if let Some((key, value)) = line.split_once(": ") {
                headers.insert(key.to_lowercase(), value.to_string());
            }
        }

        Some(Request {
            method,
            path,
            headers,
        })
    }
}

/// HTTP レスポンスを構築する
#[derive(Debug)]
pub struct Response {
    pub status_code: u16,
    pub status_text: String,
    pub headers: HashMap<String, String>,
    pub body: String,
}

impl Response {
    pub fn new(status_code: u16, status_text: &str) -> Self {
        Response {
            status_code,
            status_text: status_text.to_string(),
            headers: HashMap::new(),
            body: String::new(),
        }
    }

    pub fn with_body(mut self, body: &str) -> Self {
        self.body = body.to_string();
        self.headers
            .insert("Content-Length".to_string(), body.len().to_string());
        self
    }

    pub fn with_header(mut self, key: &str, value: &str) -> Self {
        self.headers.insert(key.to_string(), value.to_string());
        self
    }

    pub fn to_string(&self) -> String {
        let mut response = format!(
            "HTTP/1.1 {} {}\r\n",
            self.status_code, self.status_text
        );

        for (key, value) in &self.headers {
            response.push_str(&format!("{}: {}\r\n", key, value));
        }

        response.push_str("\r\n");
        response.push_str(&self.body);

        response
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_parse_request() {
        let raw = "GET /hello HTTP/1.1\r\nHost: localhost\r\nContent-Type: text/plain\r\n\r\n";
        let req = Request::parse(raw).unwrap();

        assert_eq!(req.method, "GET");
        assert_eq!(req.path, "/hello");
        assert_eq!(req.headers.get("host"), Some(&"localhost".to_string()));
    }

    #[test]
    fn test_route_root() {
        let response = match_route("/");
        assert!(response.contains("200 OK"));
        assert!(response.contains("Welcome"));
    }

    #[test]
    fn test_route_hello() {
        let response = match_route("/hello/world");
        assert!(response.contains("200 OK"));
        assert!(response.contains("Hello, world!"));
    }

    #[test]
    fn test_route_json() {
        let response = match_route("/json");
        assert!(response.contains("200"));
        assert!(response.contains("application/json"));
    }

    #[test]
    fn test_route_not_found() {
        let response = match_route("/unknown");
        assert!(response.contains("404"));
        assert!(response.contains("Not Found"));
    }

    #[test]
    fn test_response_builder() {
        let response = Response::new(200, "OK")
            .with_header("Content-Type", "text/plain")
            .with_body("Hello");

        let s = response.to_string();
        assert!(s.contains("HTTP/1.1 200 OK"));
        assert!(s.contains("Content-Type: text/plain"));
        assert!(s.contains("Hello"));
    }
}
