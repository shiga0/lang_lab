// HTTP Server - C++ 実装 (POSIX sockets)

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <functional>
#include <regex>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

// === Request ===
struct Request {
    string method;
    string path;
    map<string, string> headers;
    string body;
    map<string, string> path_params;
    map<string, string> query_params;

    static Request parse(const string& raw) {
        Request req;
        istringstream stream(raw);
        string line;

        // リクエストライン
        if (getline(stream, line)) {
            istringstream request_line(line);
            request_line >> req.method >> req.path;
        }

        // クエリパラメータを解析
        auto query_pos = req.path.find('?');
        if (query_pos != string::npos) {
            string query_string = req.path.substr(query_pos + 1);
            req.path = req.path.substr(0, query_pos);

            istringstream query_stream(query_string);
            string pair;
            while (getline(query_stream, pair, '&')) {
                auto eq_pos = pair.find('=');
                if (eq_pos != string::npos) {
                    req.query_params[pair.substr(0, eq_pos)] =
                        pair.substr(eq_pos + 1);
                }
            }
        }

        // ヘッダー
        while (getline(stream, line) && line != "\r" && !line.empty()) {
            if (line.back() == '\r') line.pop_back();
            auto colon_pos = line.find(':');
            if (colon_pos != string::npos) {
                string key = line.substr(0, colon_pos);
                string value = line.substr(colon_pos + 2);
                // ヘッダーキーを小文字に
                transform(key.begin(), key.end(), key.begin(), ::tolower);
                req.headers[key] = value;
            }
        }

        // ボディ
        if (auto it = req.headers.find("content-length"); it != req.headers.end()) {
            int length = stoi(it->second);
            req.body.resize(length);
            stream.read(&req.body[0], length);
        }

        return req;
    }
};

// === Response ===
class Response {
public:
    int status = 200;
    map<string, string> headers;
    string body;

    static const map<int, string> status_messages;

    Response& text(const string& content) {
        headers["Content-Type"] = "text/plain; charset=utf-8";
        body = content;
        return *this;
    }

    Response& html(const string& content) {
        headers["Content-Type"] = "text/html; charset=utf-8";
        body = content;
        return *this;
    }

    Response& json(const string& content) {
        headers["Content-Type"] = "application/json; charset=utf-8";
        body = content;
        return *this;
    }

    Response& set_status(int code) {
        status = code;
        return *this;
    }

    string to_string() const {
        ostringstream oss;

        string message = "OK";
        if (auto it = status_messages.find(status); it != status_messages.end()) {
            message = it->second;
        }

        oss << "HTTP/1.1 " << status << " " << message << "\r\n";

        auto headers_copy = headers;
        headers_copy["Content-Length"] = std::to_string(body.size());
        headers_copy["Connection"] = "close";

        for (const auto& [key, value] : headers_copy) {
            oss << key << ": " << value << "\r\n";
        }

        oss << "\r\n" << body;

        return oss.str();
    }
};

const map<int, string> Response::status_messages = {
    {200, "OK"},
    {201, "Created"},
    {400, "Bad Request"},
    {404, "Not Found"},
    {405, "Method Not Allowed"},
    {500, "Internal Server Error"}
};

// === Router ===
using Handler = function<Response(Request&)>;

class Router {
public:
    void get(const string& path, Handler handler) {
        routes_["GET"][path] = handler;
    }

    void post(const string& path, Handler handler) {
        routes_["POST"][path] = handler;
    }

    Response route(Request& req) {
        // 完全一致
        if (routes_.count(req.method) && routes_[req.method].count(req.path)) {
            return routes_[req.method][req.path](req);
        }

        // パターンマッチ
        if (routes_.count(req.method)) {
            for (const auto& [pattern, handler] : routes_[req.method]) {
                if (pattern.find(':') != string::npos) {
                    // パターンを正規表現に変換
                    string regex_pattern = "^" + pattern + "$";
                    vector<string> param_names;

                    // :name を (?<name>[^/]+) に変換
                    regex param_regex(":([a-zA-Z_][a-zA-Z0-9_]*)");
                    smatch match;
                    string temp = pattern;
                    while (regex_search(temp, match, param_regex)) {
                        param_names.push_back(match[1]);
                        temp = match.suffix();
                    }

                    regex_pattern = regex_replace(
                        regex_pattern,
                        param_regex,
                        "([^/]+)"
                    );

                    regex path_regex(regex_pattern);
                    smatch path_match;
                    if (regex_match(req.path, path_match, path_regex)) {
                        for (size_t i = 0; i < param_names.size(); ++i) {
                            req.path_params[param_names[i]] = path_match[i + 1];
                        }
                        return handler(req);
                    }
                }
            }
        }

        // 404
        Response res;
        return res.set_status(404).text("Not Found: " + req.path);
    }

private:
    map<string, map<string, Handler>> routes_;
};

// === HTTPServer ===
class HTTPServer {
public:
    HTTPServer(int port) : port_(port) {}

    void get(const string& path, Handler handler) {
        router_.get(path, handler);
    }

    void post(const string& path, Handler handler) {
        router_.post(path, handler);
    }

    void start() {
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            cerr << "Failed to create socket" << endl;
            return;
        }

        int opt = 1;
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port_);

        if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
            cerr << "Failed to bind" << endl;
            close(server_fd);
            return;
        }

        if (listen(server_fd, 10) < 0) {
            cerr << "Failed to listen" << endl;
            close(server_fd);
            return;
        }

        cout << "Server started at http://127.0.0.1:" << port_ << endl;
        cout << "Try:" << endl;
        cout << "  curl http://localhost:" << port_ << "/" << endl;
        cout << "  curl http://localhost:" << port_ << "/hello/world" << endl;
        cout << "  curl http://localhost:" << port_ << "/json" << endl;
        cout << "\nPress Ctrl+C to stop\n" << endl;

        while (true) {
            sockaddr_in client_addr{};
            socklen_t client_len = sizeof(client_addr);
            int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);

            if (client_fd < 0) {
                continue;
            }

            handle_client(client_fd);
        }

        close(server_fd);
    }

private:
    void handle_client(int client_fd) {
        char buffer[4096] = {0};
        ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);

        if (bytes_read > 0) {
            try {
                Request req = Request::parse(buffer);
                cout << req.method << " " << req.path << endl;

                Response res = router_.route(req);
                string response_str = res.to_string();

                write(client_fd, response_str.c_str(), response_str.size());
            } catch (const exception& e) {
                Response res;
                res.set_status(500).text("Internal Server Error");
                string response_str = res.to_string();
                write(client_fd, response_str.c_str(), response_str.size());
            }
        }

        close(client_fd);
    }

    int port_;
    Router router_;
};

// === JSON ヘルパー（簡易版）===
string json_object(const map<string, string>& obj) {
    ostringstream oss;
    oss << "{";
    bool first = true;
    for (const auto& [key, value] : obj) {
        if (!first) oss << ", ";
        oss << "\"" << key << "\": \"" << value << "\"";
        first = false;
    }
    oss << "}";
    return oss.str();
}

// === メイン ===
int main() {
    cout << "=== HTTP Server Demo ===" << endl << endl;

    HTTPServer server(8080);

    server.get("/", [](Request& req) {
        Response res;
        return res.text("Welcome to C++ HTTP Server!");
    });

    server.get("/hello/:name", [](Request& req) {
        Response res;
        string name = req.path_params["name"];
        return res.text("Hello, " + name + "!");
    });

    server.get("/json", [](Request& req) {
        Response res;
        return res.json(json_object({
            {"message", "Hello, JSON!"},
            {"status", "ok"}
        }));
    });

    server.get("/html", [](Request& req) {
        Response res;
        return res.html(R"(<!DOCTYPE html>
<html>
  <head><title>C++ Server</title></head>
  <body><h1>Hello from C++!</h1></body>
</html>)");
    });

    server.get("/echo", [](Request& req) {
        Response res;
        return res.json(json_object(req.query_params));
    });

    server.start();

    return 0;
}
