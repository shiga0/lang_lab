// HTTP Server - TypeScript 実装 (Node.js)
//
// 標準ライブラリのみで HTTP サーバーを実装

import http from "http";
import { URL } from "url";

console.log("=== HTTP Server Demo ===\n");

// --- 型定義 ---
interface RouteHandler {
  (req: http.IncomingMessage, res: http.ServerResponse, params: Record<string, string>): void;
}

interface Route {
  method: string;
  pattern: RegExp;
  paramNames: string[];
  handler: RouteHandler;
}

// --- Router ---
class Router {
  private routes: Route[] = [];

  private addRoute(method: string, path: string, handler: RouteHandler): void {
    const paramNames: string[] = [];
    const pattern = path.replace(/:(\w+)/g, (_, name) => {
      paramNames.push(name);
      return "([^/]+)";
    });
    this.routes.push({
      method,
      pattern: new RegExp(`^${pattern}$`),
      paramNames,
      handler,
    });
  }

  get(path: string, handler: RouteHandler): void {
    this.addRoute("GET", path, handler);
  }

  post(path: string, handler: RouteHandler): void {
    this.addRoute("POST", path, handler);
  }

  match(method: string, path: string): { handler: RouteHandler; params: Record<string, string> } | null {
    for (const route of this.routes) {
      if (route.method !== method) continue;
      const match = path.match(route.pattern);
      if (match) {
        const params: Record<string, string> = {};
        route.paramNames.forEach((name, i) => {
          params[name] = match[i + 1];
        });
        return { handler: route.handler, params };
      }
    }
    return null;
  }
}

// --- レスポンスヘルパー ---
function sendJSON(res: http.ServerResponse, data: unknown, status = 200): void {
  res.writeHead(status, { "Content-Type": "application/json" });
  res.end(JSON.stringify(data));
}

function sendText(res: http.ServerResponse, text: string, status = 200): void {
  res.writeHead(status, { "Content-Type": "text/plain; charset=utf-8" });
  res.end(text);
}

function sendHTML(res: http.ServerResponse, html: string, status = 200): void {
  res.writeHead(status, { "Content-Type": "text/html; charset=utf-8" });
  res.end(html);
}

// --- ボディパーサー ---
async function parseBody(req: http.IncomingMessage): Promise<string> {
  return new Promise((resolve, reject) => {
    let body = "";
    req.on("data", (chunk) => (body += chunk));
    req.on("end", () => resolve(body));
    req.on("error", reject);
  });
}

async function parseJSON<T>(req: http.IncomingMessage): Promise<T> {
  const body = await parseBody(req);
  return JSON.parse(body);
}

// --- クエリパラメータ ---
function parseQuery(url: URL): Record<string, string> {
  const params: Record<string, string> = {};
  url.searchParams.forEach((value, key) => {
    params[key] = value;
  });
  return params;
}

// --- サーバー作成 ---
function createServer(): http.Server {
  const router = new Router();

  // ルート
  router.get("/", (req, res) => {
    sendText(res, "Welcome to TypeScript HTTP Server!");
  });

  // Hello with name
  router.get("/hello/:name", (req, res, params) => {
    sendText(res, `Hello, ${params.name}!`);
  });

  // JSON レスポンス
  router.get("/json", (req, res) => {
    sendJSON(res, {
      message: "Hello, JSON!",
      status: "ok",
      timestamp: new Date().toISOString(),
    });
  });

  // エコー (クエリパラメータ)
  router.get("/echo", (req, res) => {
    const url = new URL(req.url!, `http://${req.headers.host}`);
    const params = parseQuery(url);
    sendJSON(res, params);
  });

  // HTML
  router.get("/html", (req, res) => {
    const html = `<!DOCTYPE html>
<html>
<head>
    <title>TypeScript Server</title>
</head>
<body>
    <h1>Hello from TypeScript!</h1>
    <p>This is a simple HTTP server written in TypeScript.</p>
</body>
</html>`;
    sendHTML(res, html);
  });

  // POST エコー
  router.post("/echo", async (req, res) => {
    try {
      const body = await parseJSON(req);
      sendJSON(res, { received: body });
    } catch {
      sendJSON(res, { error: "Invalid JSON" }, 400);
    }
  });

  // API ユーザー一覧
  const users = [
    { id: 1, name: "Alice" },
    { id: 2, name: "Bob" },
    { id: 3, name: "Charlie" },
  ];

  router.get("/api/users", (req, res) => {
    sendJSON(res, users);
  });

  router.get("/api/users/:id", (req, res, params) => {
    const id = parseInt(params.id);
    const user = users.find((u) => u.id === id);
    if (user) {
      sendJSON(res, user);
    } else {
      sendJSON(res, { error: "User not found" }, 404);
    }
  });

  // サーバー作成
  const server = http.createServer(async (req, res) => {
    const url = new URL(req.url!, `http://${req.headers.host}`);
    const method = req.method ?? "GET";
    const pathname = url.pathname;

    // ロギング
    console.log(`${new Date().toISOString()} ${method} ${pathname}`);

    // ルーティング
    const match = router.match(method, pathname);
    if (match) {
      try {
        await match.handler(req, res, match.params);
      } catch (error) {
        console.error(error);
        sendJSON(res, { error: "Internal Server Error" }, 500);
      }
    } else {
      sendJSON(res, { error: "Not Found" }, 404);
    }
  });

  return server;
}

// --- ミドルウェアパターン ---
type Middleware = (
  req: http.IncomingMessage,
  res: http.ServerResponse,
  next: () => void
) => void;

function compose(...middlewares: Middleware[]) {
  return (req: http.IncomingMessage, res: http.ServerResponse, final: () => void) => {
    let index = 0;
    function next(): void {
      if (index < middlewares.length) {
        const middleware = middlewares[index++];
        middleware(req, res, next);
      } else {
        final();
      }
    }
    next();
  };
}

// ロギングミドルウェア
const loggingMiddleware: Middleware = (req, res, next) => {
  console.log(`[LOG] ${req.method} ${req.url}`);
  next();
};

// CORS ミドルウェア
const corsMiddleware: Middleware = (req, res, next) => {
  res.setHeader("Access-Control-Allow-Origin", "*");
  res.setHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE");
  res.setHeader("Access-Control-Allow-Headers", "Content-Type");
  next();
};

// --- メイン ---
function main(): void {
  const server = createServer();
  const port = 8080;

  server.listen(port, () => {
    console.log(`Server started at http://localhost:${port}`);
    console.log("Try:");
    console.log(`  curl http://localhost:${port}/`);
    console.log(`  curl http://localhost:${port}/hello/world`);
    console.log(`  curl http://localhost:${port}/json`);
    console.log(`  curl 'http://localhost:${port}/echo?name=TypeScript&version=5'`);
    console.log(`  curl http://localhost:${port}/api/users`);
    console.log(`  curl http://localhost:${port}/api/users/1`);
    console.log("\nPress Ctrl+C to stop\n");
  });
}

main();
