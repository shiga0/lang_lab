// HTTP Server - C 実装
// シンプルな HTTP/1.1 サーバー

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define BUFFER_SIZE 4096
#define MAX_PATH_LEN 256
#define MAX_PARAM_LEN 128

// === HTTP リクエスト ===
typedef struct {
    char method[16];
    char path[MAX_PATH_LEN];
    char http_version[16];
} HttpRequest;

// === HTTP レスポンス ===
typedef struct {
    int status_code;
    const char *status_text;
    const char *content_type;
    char body[BUFFER_SIZE];
} HttpResponse;

// === ルーティング ===
typedef void (*RouteHandler)(const HttpRequest *req, HttpResponse *res);

typedef struct {
    const char *path;
    RouteHandler handler;
    bool has_param;  // :param を含むか
} Route;

// グローバル変数（シグナルハンドラ用）
static volatile bool running = true;
static int server_fd = -1;

// === ユーティリティ関数 ===

// 文字列のトリム
char* trim(char *str) {
    while (*str == ' ' || *str == '\t' || *str == '\r' || *str == '\n') {
        str++;
    }
    if (*str == 0) return str;

    char *end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n')) {
        end--;
    }
    *(end + 1) = 0;
    return str;
}

// パスからパラメータを抽出（例: /hello/world → world）
bool extract_param(const char *path, const char *pattern, char *param, size_t param_size) {
    // パターン: /hello/:name
    // パス: /hello/john

    const char *pattern_param = strstr(pattern, "/:");
    if (pattern_param == NULL) return false;

    size_t prefix_len = pattern_param - pattern;
    if (strncmp(path, pattern, prefix_len) != 0) return false;

    const char *param_start = path + prefix_len + 1;  // +1 for '/'
    if (*param_start == '\0') return false;

    strncpy(param, param_start, param_size - 1);
    param[param_size - 1] = '\0';

    // 次の '/' で切る
    char *slash = strchr(param, '/');
    if (slash != NULL) *slash = '\0';

    return true;
}

// === HTTP パース ===

bool parse_request(const char *buffer, HttpRequest *req) {
    // リクエストライン: "GET /path HTTP/1.1"
    char line[BUFFER_SIZE];
    const char *newline = strchr(buffer, '\r');
    if (newline == NULL) newline = strchr(buffer, '\n');
    if (newline == NULL) return false;

    size_t line_len = newline - buffer;
    if (line_len >= BUFFER_SIZE) return false;

    strncpy(line, buffer, line_len);
    line[line_len] = '\0';

    // パース
    char *token = strtok(line, " ");
    if (token == NULL) return false;
    strncpy(req->method, token, sizeof(req->method) - 1);

    token = strtok(NULL, " ");
    if (token == NULL) return false;
    strncpy(req->path, token, sizeof(req->path) - 1);

    token = strtok(NULL, " ");
    if (token != NULL) {
        strncpy(req->http_version, token, sizeof(req->http_version) - 1);
    }

    return true;
}

// === レスポンス生成 ===

void response_init(HttpResponse *res) {
    res->status_code = 200;
    res->status_text = "OK";
    res->content_type = "application/json";
    res->body[0] = '\0';
}

void response_json(HttpResponse *res, const char *json) {
    strncpy(res->body, json, BUFFER_SIZE - 1);
    res->body[BUFFER_SIZE - 1] = '\0';
}

void response_not_found(HttpResponse *res) {
    res->status_code = 404;
    res->status_text = "Not Found";
    response_json(res, "{\"error\": \"Not Found\"}");
}

void send_response(int client_fd, const HttpResponse *res) {
    char buffer[BUFFER_SIZE * 2];
    int len = snprintf(buffer, sizeof(buffer),
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        res->status_code, res->status_text,
        res->content_type,
        strlen(res->body),
        res->body
    );

    write(client_fd, buffer, len);
}

// === ルートハンドラ ===

void handle_root(const HttpRequest *req, HttpResponse *res) {
    (void)req;
    response_json(res, "{\"message\": \"Welcome to C HTTP Server!\"}");
}

void handle_hello(const HttpRequest *req, HttpResponse *res) {
    char name[MAX_PARAM_LEN] = "World";
    extract_param(req->path, "/hello/:name", name, sizeof(name));

    char json[BUFFER_SIZE];
    snprintf(json, sizeof(json), "{\"message\": \"Hello, %s!\"}", name);
    response_json(res, json);
}

void handle_health(const HttpRequest *req, HttpResponse *res) {
    (void)req;
    response_json(res, "{\"status\": \"healthy\"}");
}

void handle_echo(const HttpRequest *req, HttpResponse *res) {
    char json[BUFFER_SIZE];
    snprintf(json, sizeof(json),
        "{\"method\": \"%s\", \"path\": \"%s\"}",
        req->method, req->path
    );
    response_json(res, json);
}

// === ルーティング ===

static Route routes[] = {
    {"/", handle_root, false},
    {"/health", handle_health, false},
    {"/echo", handle_echo, false},
    {"/hello/:name", handle_hello, true},
    {NULL, NULL, false}  // 終端
};

void route_request(const HttpRequest *req, HttpResponse *res) {
    // GETのみ対応
    if (strcmp(req->method, "GET") != 0) {
        res->status_code = 405;
        res->status_text = "Method Not Allowed";
        response_json(res, "{\"error\": \"Method Not Allowed\"}");
        return;
    }

    // ルートを検索
    for (int i = 0; routes[i].path != NULL; i++) {
        if (routes[i].has_param) {
            // パラメータ付きルート
            char param[MAX_PARAM_LEN];
            if (extract_param(req->path, routes[i].path, param, sizeof(param))) {
                routes[i].handler(req, res);
                return;
            }
        } else {
            // 完全一致
            if (strcmp(req->path, routes[i].path) == 0) {
                routes[i].handler(req, res);
                return;
            }
        }
    }

    // 見つからない
    response_not_found(res);
}

// === シグナルハンドラ ===

void signal_handler(int sig) {
    (void)sig;
    printf("\nShutting down server...\n");
    running = false;
    if (server_fd >= 0) {
        close(server_fd);
    }
}

// === サーバー ===

int create_server(int port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }

    // SO_REUSEADDR を設定
    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(fd);
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(fd);
        return -1;
    }

    if (listen(fd, 10) < 0) {
        perror("listen");
        close(fd);
        return -1;
    }

    return fd;
}

void handle_client(int client_fd) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);

    if (bytes_read <= 0) {
        return;
    }
    buffer[bytes_read] = '\0';

    HttpRequest req = {0};
    HttpResponse res;
    response_init(&res);

    if (parse_request(buffer, &req)) {
        printf("[%s] %s\n", req.method, req.path);
        route_request(&req, &res);
    } else {
        res.status_code = 400;
        res.status_text = "Bad Request";
        response_json(&res, "{\"error\": \"Bad Request\"}");
    }

    send_response(client_fd, &res);
}

void run_server(int port) {
    server_fd = create_server(port);
    if (server_fd < 0) {
        exit(1);
    }

    printf("=== C HTTP Server ===\n");
    printf("Listening on http://localhost:%d\n", port);
    printf("Press Ctrl+C to stop\n\n");
    printf("Endpoints:\n");
    printf("  GET /           - Welcome message\n");
    printf("  GET /health     - Health check\n");
    printf("  GET /echo       - Echo request info\n");
    printf("  GET /hello/:name - Greeting\n\n");

    // シグナルハンドラ設定
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    while (running) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            if (running) {
                perror("accept");
            }
            continue;
        }

        handle_client(client_fd);
        close(client_fd);
    }

    close(server_fd);
}

// === メイン ===

int main(int argc, char *argv[]) {
    int port = 8080;

    if (argc > 1) {
        port = atoi(argv[1]);
        if (port <= 0 || port > 65535) {
            fprintf(stderr, "Invalid port: %s\n", argv[1]);
            return 1;
        }
    }

    run_server(port);

    return 0;
}

/*
コンパイルと実行:
    clang -std=c11 -Wall -O2 http_server.c -o http_server && ./http_server

テスト:
    curl http://localhost:8080/
    curl http://localhost:8080/health
    curl http://localhost:8080/echo
    curl http://localhost:8080/hello/World
    curl http://localhost:8080/notfound
*/
