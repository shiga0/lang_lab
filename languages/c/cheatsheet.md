# C チートシート

## 基本構文

### Hello World
```c
#include <stdio.h>

int main(void) {
    printf("Hello, World!\n");
    return 0;
}
```

### 変数と型
```c
// 基本型
int i = 42;
double d = 3.14;
char c = 'A';
_Bool b = 1;  // C99、または stdbool.h で bool

// 固定幅整数型（C99, stdint.h）
#include <stdint.h>
int32_t x = 42;
uint64_t y = 100;

// const
const int MAX = 100;

// ポインタ
int value = 10;
int *ptr = &value;
int arr[5] = {1, 2, 3, 4, 5};
```

### 文字列
```c
#include <string.h>

// 文字列リテラル（読み取り専用）
const char *s1 = "Hello";

// 文字配列（変更可能）
char s2[20] = "Hello";
strcat(s2, " World");

// 文字列操作
strlen(s1);              // 長さ
strcmp(s1, s2);          // 比較
strcpy(dest, src);       // コピー
strncpy(dest, src, n);   // n文字コピー
strchr(s1, 'e');         // 文字検索
strstr(s1, "llo");       // 部分文字列検索
```

### 配列
```c
// 固定長配列
int arr[5] = {1, 2, 3, 4, 5};
int matrix[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

// 配列のサイズ
size_t len = sizeof(arr) / sizeof(arr[0]);

// 可変長配列（C99、スタック上）
void func(int n) {
    int vla[n];
}
```

## 制御構文

### if/else
```c
if (x > 0) {
    // ...
} else if (x < 0) {
    // ...
} else {
    // ...
}

// 三項演算子
int max = (a > b) ? a : b;
```

### switch
```c
switch (value) {
    case 1:
        // ...
        break;
    case 2:
    case 3:
        // ...
        break;
    default:
        // ...
}
```

### ループ
```c
// for
for (int i = 0; i < 10; i++) {
    // ...
}

// while
while (condition) {
    // ...
}

// do-while
do {
    // ...
} while (condition);

// break / continue
for (int i = 0; i < 10; i++) {
    if (i == 5) continue;
    if (i == 8) break;
}
```

## 関数

### 基本
```c
int add(int a, int b) {
    return a + b;
}

// プロトタイプ宣言
int multiply(int a, int b);

// 実装
int multiply(int a, int b) {
    return a * b;
}

// void 関数
void greet(const char *name) {
    printf("Hello, %s!\n", name);
}
```

### 関数ポインタ
```c
// 関数ポインタの宣言
int (*operation)(int, int);

// 代入と呼び出し
operation = add;
int result = operation(3, 4);  // 7

// typedef で簡潔に
typedef int (*BinaryOp)(int, int);
BinaryOp op = add;

// コールバック
void apply(int *arr, size_t len, int (*f)(int)) {
    for (size_t i = 0; i < len; i++) {
        arr[i] = f(arr[i]);
    }
}
```

### 可変引数
```c
#include <stdarg.h>

int sum(int count, ...) {
    va_list args;
    va_start(args, count);

    int total = 0;
    for (int i = 0; i < count; i++) {
        total += va_arg(args, int);
    }

    va_end(args);
    return total;
}

// 使用例
sum(3, 10, 20, 30);  // 60
```

## 構造体

### 基本
```c
struct Point {
    double x;
    double y;
};

// 使用
struct Point p1 = {1.0, 2.0};
struct Point p2 = {.x = 3.0, .y = 4.0};  // 指示付き初期化（C99）

p1.x = 5.0;

// typedef で簡潔に
typedef struct {
    double x;
    double y;
} Point;

Point p3 = {1.0, 2.0};
```

### 構造体とポインタ
```c
void move_point(Point *p, double dx, double dy) {
    p->x += dx;
    p->y += dy;
}

Point p = {0, 0};
move_point(&p, 1.0, 2.0);
```

### ネスト
```c
typedef struct {
    Point center;
    double radius;
} Circle;

Circle c = {
    .center = {0.0, 0.0},
    .radius = 5.0
};
```

## 共用体と列挙型

### union
```c
typedef union {
    int i;
    float f;
    char c;
} Value;

Value v;
v.i = 42;
printf("%d\n", v.i);
v.f = 3.14f;  // v.i は無効になる
```

### enum
```c
typedef enum {
    RED,
    GREEN,
    BLUE
} Color;

Color c = RED;

// 値を指定
typedef enum {
    HTTP_OK = 200,
    HTTP_NOT_FOUND = 404,
    HTTP_SERVER_ERROR = 500
} HttpStatus;
```

## メモリ管理

### malloc / free
```c
#include <stdlib.h>

// 単一要素
int *p = malloc(sizeof(int));
if (p == NULL) {
    // エラー処理
}
*p = 42;
free(p);

// 配列
int *arr = malloc(10 * sizeof(int));
for (int i = 0; i < 10; i++) {
    arr[i] = i;
}
free(arr);

// calloc（ゼロ初期化）
int *zeroed = calloc(10, sizeof(int));
free(zeroed);

// realloc
arr = realloc(arr, 20 * sizeof(int));
```

### 構造体の動的確保
```c
typedef struct Node {
    int value;
    struct Node *next;
} Node;

Node *create_node(int value) {
    Node *node = malloc(sizeof(Node));
    if (node != NULL) {
        node->value = value;
        node->next = NULL;
    }
    return node;
}

void free_list(Node *head) {
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}
```

## ファイル I/O

### 基本操作
```c
#include <stdio.h>

// 読み込み
FILE *fp = fopen("file.txt", "r");
if (fp == NULL) {
    perror("fopen");
    return 1;
}

char buffer[256];
while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    printf("%s", buffer);
}
fclose(fp);

// 書き込み
FILE *fp = fopen("output.txt", "w");
fprintf(fp, "Hello, %s!\n", "World");
fclose(fp);
```

### バイナリファイル
```c
// 書き込み
int data[] = {1, 2, 3, 4, 5};
FILE *fp = fopen("data.bin", "wb");
fwrite(data, sizeof(int), 5, fp);
fclose(fp);

// 読み込み
int buffer[5];
fp = fopen("data.bin", "rb");
fread(buffer, sizeof(int), 5, fp);
fclose(fp);
```

## プリプロセッサ

### マクロ
```c
// オブジェクトマクロ
#define PI 3.14159
#define MAX_SIZE 100

// 関数マクロ
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SQUARE(x) ((x) * (x))

// 可変引数マクロ（C99）
#define DEBUG_LOG(fmt, ...) \
    fprintf(stderr, "[DEBUG] " fmt "\n", ##__VA_ARGS__)
```

### 条件付きコンパイル
```c
#ifdef DEBUG
    printf("Debug mode\n");
#endif

#ifndef HEADER_H
#define HEADER_H
// ヘッダー内容
#endif

#if defined(_WIN32)
    // Windows
#elif defined(__APPLE__)
    // macOS
#else
    // Linux など
#endif
```

### インクルードガード
```c
// header.h
#ifndef HEADER_H
#define HEADER_H

// 宣言

#endif
```

## エラー処理

### errno
```c
#include <errno.h>
#include <string.h>

FILE *fp = fopen("nonexistent.txt", "r");
if (fp == NULL) {
    fprintf(stderr, "Error: %s\n", strerror(errno));
    return 1;
}
```

### 戻り値によるエラー
```c
typedef enum {
    SUCCESS = 0,
    ERROR_NULL_POINTER,
    ERROR_OUT_OF_MEMORY,
    ERROR_INVALID_ARGUMENT
} ErrorCode;

ErrorCode divide(int a, int b, int *result) {
    if (result == NULL) {
        return ERROR_NULL_POINTER;
    }
    if (b == 0) {
        return ERROR_INVALID_ARGUMENT;
    }
    *result = a / b;
    return SUCCESS;
}

// 使用
int result;
ErrorCode err = divide(10, 2, &result);
if (err != SUCCESS) {
    // エラー処理
}
```

## アサーション

```c
#include <assert.h>

void process(int *ptr) {
    assert(ptr != NULL);
    // ...
}

// NDEBUG を定義すると assert が無効化
// #define NDEBUG
```

## 型修飾子

```c
// const
const int x = 10;
const int *p1;        // ポインタが指す値が const
int *const p2;        // ポインタ自体が const
const int *const p3;  // 両方 const

// volatile（最適化を抑制）
volatile int flag;

// restrict（C99、ポインタのエイリアスがないことを示す）
void copy(int *restrict dest, const int *restrict src, size_t n);

// static（関数内で状態を保持）
int counter(void) {
    static int count = 0;
    return ++count;
}
```

## _Generic（C11）

```c
#define type_name(x) _Generic((x), \
    int: "int", \
    double: "double", \
    char *: "string", \
    default: "unknown")

printf("%s\n", type_name(42));       // "int"
printf("%s\n", type_name(3.14));     // "double"
printf("%s\n", type_name("hello"));  // "string"
```

## マルチスレッド（C11）

```c
#include <threads.h>

int thread_func(void *arg) {
    int *value = (int *)arg;
    printf("Thread received: %d\n", *value);
    return 0;
}

int main(void) {
    thrd_t thread;
    int arg = 42;

    thrd_create(&thread, thread_func, &arg);
    thrd_join(thread, NULL);

    return 0;
}
```

### mutex
```c
#include <threads.h>

mtx_t mutex;
int counter = 0;

int increment(void *arg) {
    for (int i = 0; i < 1000; i++) {
        mtx_lock(&mutex);
        counter++;
        mtx_unlock(&mutex);
    }
    return 0;
}

int main(void) {
    mtx_init(&mutex, mtx_plain);

    thrd_t t1, t2;
    thrd_create(&t1, increment, NULL);
    thrd_create(&t2, increment, NULL);

    thrd_join(t1, NULL);
    thrd_join(t2, NULL);

    mtx_destroy(&mutex);
    printf("Counter: %d\n", counter);  // 2000
    return 0;
}
```

## 実行例

```bash
# コンパイルと実行
clang -std=c11 -Wall -O2 main.c -o main && ./main

# デバッグビルド
clang -std=c11 -g -fsanitize=address main.c -o main

# 警告を厳格に
clang -std=c11 -Wall -Wextra -Werror -pedantic main.c -o main

# 静的解析
clang --analyze main.c
```
