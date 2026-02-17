// メタプログラミング - C 実装
//
// C のメタプログラミングは主にプリプロセッサマクロで行う。
// コンパイル時のコード生成と条件付きコンパイルが可能。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// === オブジェクトマクロ ===
#define PI 3.14159265358979
#define MAX_SIZE 100
#define GREETING "Hello, World!"

void object_macros(void) {
    printf("--- オブジェクトマクロ ---\n");

    printf("PI = %f\n", PI);
    printf("MAX_SIZE = %d\n", MAX_SIZE);
    printf("GREETING = %s\n", GREETING);

    int arr[MAX_SIZE];
    printf("Array size: %zu\n", sizeof(arr) / sizeof(arr[0]));

    printf("\n");
}

// === 関数マクロ ===
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define SQUARE(x) ((x) * (x))
#define ABS(x) ((x) < 0 ? -(x) : (x))

// 副作用に注意が必要な例
#define UNSAFE_INCREMENT(x) ((x)++)

void function_macros(void) {
    printf("--- 関数マクロ ---\n");

    printf("MAX(3, 5) = %d\n", MAX(3, 5));
    printf("MIN(3, 5) = %d\n", MIN(3, 5));
    printf("SQUARE(4) = %d\n", SQUARE(4));
    printf("ABS(-7) = %d\n", ABS(-7));

    // マクロの落とし穴：副作用
    int x = 3;
    printf("Before: x = %d\n", x);
    int result = SQUARE(x++);  // x が2回インクリメントされる！
    printf("SQUARE(x++) = %d, After: x = %d\n", result, x);

    printf("\n");
}

// === 文字列化とトークン連結 ===
#define STRINGIFY(x) #x
#define CONCAT(a, b) a##b
#define MAKE_VAR(name, num) name##num

void stringify_concat(void) {
    printf("--- 文字列化とトークン連結 ---\n");

    // 文字列化
    printf("STRINGIFY(hello) = %s\n", STRINGIFY(hello));
    printf("STRINGIFY(1 + 2) = %s\n", STRINGIFY(1 + 2));

    // トークン連結
    int var1 = 100;
    int var2 = 200;
    printf("CONCAT(var, 1) = %d\n", CONCAT(var, 1));
    printf("MAKE_VAR(var, 2) = %d\n", MAKE_VAR(var, 2));

    printf("\n");
}

// === 可変引数マクロ ===
#define DEBUG_LOG(fmt, ...) \
    printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)

#define ERROR_LOG(fmt, ...) \
    fprintf(stderr, "[ERROR] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define TRACE(expr) \
    printf("TRACE: %s = %d\n", #expr, (expr))

void variadic_macros(void) {
    printf("--- 可変引数マクロ ---\n");

    DEBUG_LOG("Starting...");
    DEBUG_LOG("Value: %d", 42);
    DEBUG_LOG("x=%d, y=%d", 10, 20);

    int a = 5, b = 3;
    TRACE(a + b);
    TRACE(a * b);

    printf("\n");
}

// === 条件付きコンパイル ===
#define DEBUG_MODE 1
#define FEATURE_X 1
// #define FEATURE_Y 1  // コメントアウト

void conditional_compilation(void) {
    printf("--- 条件付きコンパイル ---\n");

    #if DEBUG_MODE
    printf("Debug mode is ON\n");
    #else
    printf("Debug mode is OFF\n");
    #endif

    #ifdef FEATURE_X
    printf("Feature X is enabled\n");
    #endif

    #ifndef FEATURE_Y
    printf("Feature Y is not defined\n");
    #endif

    // プラットフォーム検出
    #if defined(__APPLE__)
    printf("Platform: macOS\n");
    #elif defined(_WIN32)
    printf("Platform: Windows\n");
    #elif defined(__linux__)
    printf("Platform: Linux\n");
    #else
    printf("Platform: Unknown\n");
    #endif

    // コンパイラ検出
    #if defined(__clang__)
    printf("Compiler: Clang\n");
    #elif defined(__GNUC__)
    printf("Compiler: GCC\n");
    #elif defined(_MSC_VER)
    printf("Compiler: MSVC\n");
    #endif

    printf("\n");
}

// === 定義済みマクロ ===
void predefined_macros(void) {
    printf("--- 定義済みマクロ ---\n");

    printf("__FILE__ = %s\n", __FILE__);
    printf("__LINE__ = %d\n", __LINE__);
    printf("__DATE__ = %s\n", __DATE__);
    printf("__TIME__ = %s\n", __TIME__);
    printf("__func__ = %s\n", __func__);

    #ifdef __STDC_VERSION__
    printf("__STDC_VERSION__ = %ld\n", __STDC_VERSION__);
    #endif

    printf("\n");
}

// === X-Macro パターン ===
// データと振る舞いを一箇所で定義

#define COLOR_LIST \
    X(RED,   "red",   0xFF0000) \
    X(GREEN, "green", 0x00FF00) \
    X(BLUE,  "blue",  0x0000FF) \
    X(WHITE, "white", 0xFFFFFF) \
    X(BLACK, "black", 0x000000)

// enum 定義
typedef enum {
    #define X(name, str, hex) COLOR_##name,
    COLOR_LIST
    #undef X
    COLOR_COUNT
} Color;

// 名前の配列
static const char* color_names[] = {
    #define X(name, str, hex) str,
    COLOR_LIST
    #undef X
};

// 値の配列
static const int color_values[] = {
    #define X(name, str, hex) hex,
    COLOR_LIST
    #undef X
};

void xmacro_pattern(void) {
    printf("--- X-Macro パターン ---\n");

    printf("Color count: %d\n", COLOR_COUNT);

    for (int i = 0; i < COLOR_COUNT; i++) {
        printf("  %s = 0x%06X\n", color_names[i], color_values[i]);
    }

    printf("\n");
}

// === コード生成マクロ ===
#define DEFINE_VECTOR(type, name) \
    typedef struct { \
        type *data; \
        size_t size; \
        size_t capacity; \
    } name; \
    \
    static inline void name##_init(name *v) { \
        v->data = NULL; \
        v->size = 0; \
        v->capacity = 0; \
    } \
    \
    static inline bool name##_push(name *v, type value) { \
        if (v->size >= v->capacity) { \
            size_t new_cap = v->capacity == 0 ? 4 : v->capacity * 2; \
            type *new_data = realloc(v->data, new_cap * sizeof(type)); \
            if (new_data == NULL) return false; \
            v->data = new_data; \
            v->capacity = new_cap; \
        } \
        v->data[v->size++] = value; \
        return true; \
    } \
    \
    static inline void name##_free(name *v) { \
        free(v->data); \
        v->data = NULL; \
        v->size = 0; \
        v->capacity = 0; \
    }

// int 型の Vector を生成
DEFINE_VECTOR(int, IntVector)

// double 型の Vector を生成
DEFINE_VECTOR(double, DoubleVector)

void code_generation(void) {
    printf("--- コード生成マクロ ---\n");

    // IntVector の使用
    IntVector iv;
    IntVector_init(&iv);

    for (int i = 0; i < 5; i++) {
        IntVector_push(&iv, i * 10);
    }

    printf("IntVector: ");
    for (size_t i = 0; i < iv.size; i++) {
        printf("%d ", iv.data[i]);
    }
    printf("\n");

    IntVector_free(&iv);

    // DoubleVector の使用
    DoubleVector dv;
    DoubleVector_init(&dv);

    DoubleVector_push(&dv, 1.1);
    DoubleVector_push(&dv, 2.2);
    DoubleVector_push(&dv, 3.3);

    printf("DoubleVector: ");
    for (size_t i = 0; i < dv.size; i++) {
        printf("%.1f ", dv.data[i]);
    }
    printf("\n");

    DoubleVector_free(&dv);

    printf("\n");
}

// === _Static_assert (C11) ===
void static_assert_demo(void) {
    printf("--- _Static_assert (C11) ---\n");

    // コンパイル時アサーション
    _Static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");
    _Static_assert(sizeof(void*) == 8 || sizeof(void*) == 4,
                   "Pointer must be 32 or 64 bits");

    printf("sizeof(int) = %zu (>= 4 verified at compile time)\n", sizeof(int));
    printf("sizeof(void*) = %zu\n", sizeof(void*));

    printf("\n");
}

// === _Generic (C11) による型ディスパッチ ===
#define print_type(x) printf("Type of " #x ": %s\n", \
    _Generic((x), \
        int: "int", \
        float: "float", \
        double: "double", \
        char*: "char*", \
        default: "unknown"))

void generic_demo(void) {
    printf("--- _Generic (C11) ---\n");

    int i = 42;
    float f = 3.14f;
    double d = 2.718;
    char *s = "hello";

    print_type(i);
    print_type(f);
    print_type(d);
    print_type(s);

    printf("\n");
}

// === メイン ===
int main(void) {
    printf("=== Metaprogramming Demo ===\n\n");

    object_macros();
    function_macros();
    stringify_concat();
    variadic_macros();
    conditional_compilation();
    predefined_macros();
    xmacro_pattern();
    code_generation();
    static_assert_demo();
    generic_demo();

    return 0;
}
