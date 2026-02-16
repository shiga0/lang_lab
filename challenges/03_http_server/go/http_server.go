// HTTP Server - Go 実装
package main

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"strings"
)

func main() {
	fmt.Println("=== HTTP Server Demo ===\n")

	mux := http.NewServeMux()

	// ルート定義
	mux.HandleFunc("/", handleRoot)
	mux.HandleFunc("/hello/", handleHello)
	mux.HandleFunc("/json", handleJSON)
	mux.HandleFunc("/echo", handleEcho)
	mux.HandleFunc("/html", handleHTML)

	addr := ":8080"
	fmt.Printf("Server started at http://localhost%s\n", addr)
	fmt.Println("Try:")
	fmt.Println("  curl http://localhost:8080/")
	fmt.Println("  curl http://localhost:8080/hello/world")
	fmt.Println("  curl http://localhost:8080/json")
	fmt.Println("  curl 'http://localhost:8080/echo?name=Go&version=1.21'")
	fmt.Println("\nPress Ctrl+C to stop\n")

	if err := http.ListenAndServe(addr, mux); err != nil {
		log.Fatal(err)
	}
}

// ルートハンドラ
func handleRoot(w http.ResponseWriter, r *http.Request) {
	if r.URL.Path != "/" {
		http.NotFound(w, r)
		return
	}
	if r.Method != http.MethodGet {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}
	fmt.Fprintf(w, "Welcome to Go HTTP Server!")
}

// /hello/:name ハンドラ
func handleHello(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}

	// パスパラメータを抽出
	name := strings.TrimPrefix(r.URL.Path, "/hello/")
	if name == "" {
		http.Error(w, "Name is required", http.StatusBadRequest)
		return
	}

	fmt.Fprintf(w, "Hello, %s!", name)
}

// JSON ハンドラ
func handleJSON(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}

	response := map[string]interface{}{
		"message": "Hello, JSON!",
		"status":  "ok",
		"code":    200,
	}

	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(response)
}

// エコー ハンドラ (クエリパラメータを返す)
func handleEcho(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}

	params := make(map[string]string)
	for key, values := range r.URL.Query() {
		if len(values) > 0 {
			params[key] = values[0]
		}
	}

	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(params)
}

// HTML ハンドラ
func handleHTML(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}

	html := `<!DOCTYPE html>
<html>
<head>
    <title>Go Server</title>
</head>
<body>
    <h1>Hello from Go!</h1>
    <p>This is a simple HTTP server written in Go.</p>
</body>
</html>`

	w.Header().Set("Content-Type", "text/html; charset=utf-8")
	fmt.Fprint(w, html)
}

// --- ミドルウェアパターン ---

// ロギングミドルウェア
func loggingMiddleware(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		log.Printf("%s %s", r.Method, r.URL.Path)
		next.ServeHTTP(w, r)
	})
}

// リカバリーミドルウェア
func recoveryMiddleware(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		defer func() {
			if err := recover(); err != nil {
				log.Printf("Panic: %v", err)
				http.Error(w, "Internal Server Error", http.StatusInternalServerError)
			}
		}()
		next.ServeHTTP(w, r)
	})
}

// --- カスタムレスポンスライター ---

type ResponseWriter struct {
	http.ResponseWriter
	StatusCode int
}

func (rw *ResponseWriter) WriteHeader(code int) {
	rw.StatusCode = code
	rw.ResponseWriter.WriteHeader(code)
}

// --- JSON ヘルパー ---

func writeJSON(w http.ResponseWriter, status int, data interface{}) {
	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(status)
	json.NewEncoder(w).Encode(data)
}

func readJSON(r *http.Request, v interface{}) error {
	return json.NewDecoder(r.Body).Decode(v)
}
