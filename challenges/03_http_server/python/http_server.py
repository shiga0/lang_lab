#!/usr/bin/env python3
"""HTTP Server - Python 実装 (標準ライブラリのみ)"""

import socket
import json
import re
from typing import Dict, Callable, Any, Optional
from dataclasses import dataclass, field
from urllib.parse import parse_qs, urlparse

print("=== HTTP Server Demo ===\n")


# --- Request ---
@dataclass
class Request:
    method: str
    path: str
    headers: Dict[str, str] = field(default_factory=dict)
    body: Optional[str] = None
    path_params: Dict[str, str] = field(default_factory=dict)

    @property
    def query_params(self) -> Dict[str, list]:
        parsed = urlparse(self.path)
        return parse_qs(parsed.query)

    @classmethod
    def parse(cls, client: socket.socket) -> Optional["Request"]:
        try:
            data = b""
            while b"\r\n\r\n" not in data:
                chunk = client.recv(1024)
                if not chunk:
                    return None
                data += chunk

            header_part, _, body_start = data.partition(b"\r\n\r\n")
            lines = header_part.decode("utf-8").split("\r\n")

            if not lines:
                return None

            # リクエストライン
            request_line = lines[0].split(" ")
            if len(request_line) < 2:
                return None
            method, path = request_line[0], request_line[1]

            # ヘッダー
            headers: Dict[str, str] = {}
            for line in lines[1:]:
                if ": " in line:
                    key, value = line.split(": ", 1)
                    headers[key.lower()] = value

            # ボディ
            body = None
            content_length = headers.get("content-length")
            if content_length:
                length = int(content_length)
                body_data = body_start
                while len(body_data) < length:
                    body_data += client.recv(1024)
                body = body_data[:length].decode("utf-8")

            return cls(method=method, path=path, headers=headers, body=body)
        except Exception:
            return None


# --- Response ---
@dataclass
class Response:
    status: int = 200
    headers: Dict[str, str] = field(default_factory=dict)
    body: str = ""

    STATUS_MESSAGES = {
        200: "OK",
        201: "Created",
        400: "Bad Request",
        404: "Not Found",
        405: "Method Not Allowed",
        500: "Internal Server Error",
    }

    def __post_init__(self):
        if "Content-Type" not in self.headers:
            self.headers["Content-Type"] = "text/plain; charset=utf-8"

    def text(self, content: str) -> "Response":
        self.headers["Content-Type"] = "text/plain; charset=utf-8"
        self.body = content
        return self

    def json(self, data: Any) -> "Response":
        self.headers["Content-Type"] = "application/json; charset=utf-8"
        self.body = json.dumps(data, ensure_ascii=False)
        return self

    def html(self, content: str) -> "Response":
        self.headers["Content-Type"] = "text/html; charset=utf-8"
        self.body = content
        return self

    def to_bytes(self) -> bytes:
        self.headers["Content-Length"] = str(len(self.body.encode("utf-8")))
        self.headers["Connection"] = "close"

        status_message = self.STATUS_MESSAGES.get(self.status, "Unknown")
        response = f"HTTP/1.1 {self.status} {status_message}\r\n"
        for key, value in self.headers.items():
            response += f"{key}: {value}\r\n"
        response += "\r\n"
        response += self.body
        return response.encode("utf-8")


# --- Router ---
Handler = Callable[[Request], Response]


class Router:
    def __init__(self):
        self._routes: Dict[str, Dict[str, Handler]] = {"GET": {}, "POST": {}}

    def get(self, path: str):
        def decorator(handler: Handler) -> Handler:
            self._routes["GET"][path] = handler
            return handler
        return decorator

    def post(self, path: str):
        def decorator(handler: Handler) -> Handler:
            self._routes["POST"][path] = handler
            return handler
        return decorator

    def route(self, request: Request) -> Response:
        # パスからクエリ文字列を除去
        path = request.path.split("?")[0]

        # 完全一致
        if path in self._routes.get(request.method, {}):
            handler = self._routes[request.method][path]
            return handler(request)

        # パターンマッチ
        for pattern, handler in self._routes.get(request.method, {}).items():
            if ":" in pattern:
                # /hello/:name -> /hello/(?P<name>[^/]+)
                regex_pattern = re.sub(r":(\w+)", r"(?P<\1>[^/]+)", pattern)
                match = re.fullmatch(regex_pattern, path)
                if match:
                    request.path_params = match.groupdict()
                    return handler(request)

        # 404
        return Response(status=404).text(f"Not Found: {request.path}")


# --- Server ---
class HTTPServer:
    def __init__(self, port: int = 8080):
        self.port = port
        self.router = Router()

    def get(self, path: str):
        return self.router.get(path)

    def post(self, path: str):
        return self.router.post(path)

    def start(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
            server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            server.bind(("127.0.0.1", self.port))
            server.listen(5)

            print(f"Server started at http://127.0.0.1:{self.port}")
            print("Try:")
            print(f"  curl http://localhost:{self.port}/")
            print(f"  curl http://localhost:{self.port}/hello/world")
            print(f"  curl http://localhost:{self.port}/json")
            print("\nPress Ctrl+C to stop\n")

            try:
                while True:
                    client, _ = server.accept()
                    self._handle_client(client)
            except KeyboardInterrupt:
                print("\nShutting down...")

    def _handle_client(self, client: socket.socket):
        try:
            request = Request.parse(client)
            if request is None:
                return

            print(f"{request.method} {request.path}")

            response = self.router.route(request)
            client.sendall(response.to_bytes())
        except Exception as e:
            print(f"Error: {e}")
            error_response = Response(status=500).text("Internal Server Error")
            client.sendall(error_response.to_bytes())
        finally:
            client.close()


# --- メイン ---
if __name__ == "__main__":
    server = HTTPServer(port=8080)

    @server.get("/")
    def index(req: Request) -> Response:
        return Response().text("Welcome to Python HTTP Server!")

    @server.get("/hello/:name")
    def hello(req: Request) -> Response:
        name = req.path_params.get("name", "World")
        return Response().text(f"Hello, {name}!")

    @server.get("/json")
    def json_endpoint(req: Request) -> Response:
        return Response().json({"message": "Hello, JSON!", "status": "ok"})

    @server.get("/html")
    def html_endpoint(req: Request) -> Response:
        return Response().html("""<!DOCTYPE html>
<html>
  <head><title>Python Server</title></head>
  <body><h1>Hello from Python!</h1></body>
</html>""")

    @server.get("/echo")
    def echo(req: Request) -> Response:
        return Response().json(req.query_params)

    server.start()
