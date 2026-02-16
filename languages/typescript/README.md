# TypeScript

## 概要

TypeScript は Microsoft が開発した、JavaScript に静的型付けを追加した言語。
JavaScript のスーパーセットであり、既存の JS コードと互換性がある。

## 特徴

| 項目 | 内容 |
|------|------|
| 登場年 | 2012 |
| 開発者 | Microsoft (Anders Hejlsberg) |
| パラダイム | マルチパラダイム (OOP/FP/手続き) |
| 型システム | 静的・構造的部分型 |
| 実行環境 | JavaScript にトランスパイル |
| パッケージ管理 | npm / yarn / pnpm |

## Rust/Ruby/Go との比較

| 観点 | TypeScript | Rust | Ruby | Go |
|------|------------|------|------|-----|
| 型システム | 静的・構造的 | 静的・名前的 | 動的 | 静的・名前的 |
| 型推論 | あり | あり | - | あり |
| Null安全 | strictNullChecks | Option<T> | nil | nil |
| ジェネリクス | あり | Trait bounds | Duck typing | あり |
| 実行形式 | JS へトランスパイル | ネイティブ | インタプリタ | ネイティブ |
| 並行処理 | async/await, Promise | async/await, スレッド | Thread, Fiber | goroutine |
| エラー処理 | 例外 / Result パターン | Result<T, E> | 例外 | 多値戻り |

## 型システムの特徴

### 構造的部分型 (Structural Subtyping)

```typescript
// 名前ではなく構造で型を判定
interface Point {
  x: number;
  y: number;
}

function printPoint(p: Point) {
  console.log(`(${p.x}, ${p.y})`);
}

// Point を明示的に実装していなくても OK
const obj = { x: 10, y: 20, z: 30 };
printPoint(obj);  // OK: x, y を持っている
```

### ユニオン型とリテラル型

```typescript
type Status = "pending" | "success" | "error";
type Result = number | string | null;

function handle(status: Status): void {
  switch (status) {
    case "pending": // ...
    case "success": // ...
    case "error":   // ...
  }
}
```

### 型ガード

```typescript
function process(value: string | number) {
  if (typeof value === "string") {
    // ここでは value は string 型
    console.log(value.toUpperCase());
  } else {
    // ここでは value は number 型
    console.log(value.toFixed(2));
  }
}
```

## セットアップ

```bash
# Node.js がインストールされていることを確認
node --version

# TypeScript をインストール
npm install -g typescript

# または ts-node で直接実行
npm install -g ts-node

# プロジェクト初期化
npm init -y
npm install typescript --save-dev
npx tsc --init
```

## 実行方法

```bash
# コンパイルして実行
tsc example.ts
node example.js

# ts-node で直接実行
ts-node example.ts

# tsx で高速実行 (推奨)
npx tsx example.ts
```

## ファイル構成

```
project/
├── src/
│   └── index.ts
├── dist/           # コンパイル出力
├── package.json
├── tsconfig.json   # TypeScript 設定
└── node_modules/
```

## リソース

- [TypeScript Handbook](https://www.typescriptlang.org/docs/handbook/)
- [TypeScript Playground](https://www.typescriptlang.org/play)
- [Type Challenges](https://github.com/type-challenges/type-challenges)
