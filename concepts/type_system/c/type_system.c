// 型システム - C 実装
//
// C は静的・弱い型付け言語。
// コンパイル時に型チェックが行われるが、暗黙の型変換も許容される。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <float.h>

// === 基本型 ===
void basic_types(void) {
    printf("--- 基本型 ---\n");

    // 整数型
    char c = 'A';                    // 通常1バイト
    short s = 32767;                 // 通常2バイト以上
    int i = 2147483647;              // 通常4バイト以上
    long l = 9223372036854775807L;   // 通常4または8バイト以上
    long long ll = 9223372036854775807LL;  // 8バイト以上

    printf("char: %c (size: %zu)\n", c, sizeof(char));
    printf("short: %d (size: %zu)\n", s, sizeof(short));
    printf("int: %d (size: %zu)\n", i, sizeof(int));
    printf("long: %ld (size: %zu)\n", l, sizeof(long));
    printf("long long: %lld (size: %zu)\n", ll, sizeof(long long));

    // 符号なし整数
    unsigned int ui = 4294967295U;
    unsigned long ul = 18446744073709551615UL;
    printf("unsigned int: %u\n", ui);
    printf("unsigned long: %lu\n", ul);

    // 浮動小数点
    float f = 3.14159f;
    double d = 3.141592653589793;
    long double ld = 3.141592653589793238L;
    printf("float: %f (size: %zu)\n", f, sizeof(float));
    printf("double: %f (size: %zu)\n", d, sizeof(double));
    printf("long double: %Lf (size: %zu)\n", ld, sizeof(long double));

    // ブール (C99)
    _Bool b1 = 1;
    bool b2 = true;  // stdbool.h
    printf("_Bool: %d, bool: %d\n", b1, b2);

    printf("\n");
}

// === 固定幅整数型 (C99) ===
void fixed_width_types(void) {
    printf("--- 固定幅整数型 (C99) ---\n");

    // 正確なビット幅
    int8_t  i8  = INT8_MAX;
    int16_t i16 = INT16_MAX;
    int32_t i32 = INT32_MAX;
    int64_t i64 = INT64_MAX;

    printf("int8_t:  %d (max: %d)\n", i8, INT8_MAX);
    printf("int16_t: %d (max: %d)\n", i16, INT16_MAX);
    printf("int32_t: %d (max: %d)\n", i32, INT32_MAX);
    printf("int64_t: %lld (max: %lld)\n", (long long)i64, (long long)INT64_MAX);

    // 符号なし
    uint8_t  u8  = UINT8_MAX;
    uint16_t u16 = UINT16_MAX;
    uint32_t u32 = UINT32_MAX;
    uint64_t u64 = UINT64_MAX;

    printf("uint8_t:  %u\n", u8);
    printf("uint16_t: %u\n", u16);
    printf("uint32_t: %u\n", u32);
    printf("uint64_t: %llu\n", (unsigned long long)u64);

    // サイズ型
    size_t sz = sizeof(int);
    ptrdiff_t pd = 10;
    printf("size_t: %zu, ptrdiff_t: %td\n", sz, pd);

    printf("\n");
}

// === ポインタ型 ===
void pointer_types(void) {
    printf("--- ポインタ型 ---\n");

    int x = 42;
    int *p = &x;       // int へのポインタ
    int **pp = &p;     // int* へのポインタ

    printf("x = %d, &x = %p\n", x, (void*)&x);
    printf("*p = %d, p = %p\n", *p, (void*)p);
    printf("**pp = %d, *pp = %p, pp = %p\n", **pp, (void*)*pp, (void*)pp);

    // void ポインタ（汎用ポインタ）
    void *vp = &x;
    printf("void*: %p\n", vp);

    // 関数ポインタ
    int (*fp)(int, int) = NULL;
    printf("Function pointer size: %zu\n", sizeof(fp));

    // 配列とポインタ
    int arr[5] = {1, 2, 3, 4, 5};
    int *arrp = arr;  // 配列はポインタに減衰
    printf("arr[0] = %d, *arrp = %d, *(arrp+2) = %d\n",
           arr[0], *arrp, *(arrp + 2));

    printf("\n");
}

// === 構造体 ===
typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    char name[50];
    int age;
    Point location;
} Person;

void struct_types(void) {
    printf("--- 構造体 ---\n");

    // 初期化
    Point p1 = {3.0, 4.0};
    Point p2 = {.x = 10.0, .y = 20.0};  // 指示付き初期化

    printf("Point p1: (%.1f, %.1f)\n", p1.x, p1.y);
    printf("Point p2: (%.1f, %.1f)\n", p2.x, p2.y);

    // ネストした構造体
    Person person = {
        .name = "Alice",
        .age = 30,
        .location = {100.0, 200.0}
    };

    printf("Person: %s, %d years old at (%.1f, %.1f)\n",
           person.name, person.age,
           person.location.x, person.location.y);

    // 構造体のサイズ（アライメントに注意）
    printf("sizeof(Point): %zu\n", sizeof(Point));
    printf("sizeof(Person): %zu\n", sizeof(Person));

    printf("\n");
}

// === 共用体 ===
typedef union {
    int i;
    float f;
    char c[4];
} Value;

// タグ付き共用体パターン
typedef enum { VAL_INT, VAL_FLOAT, VAL_STRING } ValueType;

typedef struct {
    ValueType type;
    union {
        int i;
        float f;
        char *s;
    } data;
} TaggedValue;

void union_types(void) {
    printf("--- 共用体 ---\n");

    Value v;
    v.i = 0x41424344;

    printf("As int: %d (0x%X)\n", v.i, v.i);
    printf("As float: %f\n", v.f);
    printf("As chars: %c%c%c%c\n", v.c[0], v.c[1], v.c[2], v.c[3]);
    printf("sizeof(Value): %zu\n", sizeof(Value));

    // タグ付き共用体
    TaggedValue tv1 = {.type = VAL_INT, .data.i = 42};
    TaggedValue tv2 = {.type = VAL_FLOAT, .data.f = 3.14f};
    TaggedValue tv3 = {.type = VAL_STRING, .data.s = "hello"};

    printf("\nTagged union:\n");
    printf("  VAL_INT: %d\n", tv1.data.i);
    printf("  VAL_FLOAT: %f\n", tv2.data.f);
    printf("  VAL_STRING: %s\n", tv3.data.s);

    printf("\n");
}

// === 列挙型 ===
typedef enum {
    RED,
    GREEN,
    BLUE
} Color;

typedef enum {
    HTTP_OK = 200,
    HTTP_NOT_FOUND = 404,
    HTTP_SERVER_ERROR = 500
} HttpStatus;

void enum_types(void) {
    printf("--- 列挙型 ---\n");

    Color c = GREEN;
    printf("Color GREEN = %d\n", c);

    HttpStatus status = HTTP_OK;
    printf("HTTP_OK = %d\n", status);
    printf("HTTP_NOT_FOUND = %d\n", HTTP_NOT_FOUND);

    printf("sizeof(Color): %zu\n", sizeof(Color));

    printf("\n");
}

// === 型修飾子 ===
void type_qualifiers(void) {
    printf("--- 型修飾子 ---\n");

    // const
    const int MAX_SIZE = 100;
    printf("const int: %d\n", MAX_SIZE);

    // const とポインタ
    int value = 42;
    const int *p1 = &value;     // 指す先が const
    int *const p2 = &value;     // ポインタ自体が const
    const int *const p3 = &value;  // 両方 const

    printf("const int*: *p1 = %d\n", *p1);
    printf("int* const: *p2 = %d\n", *p2);

    // volatile（最適化を抑制）
    volatile int flag = 0;
    printf("volatile int: %d\n", flag);

    // restrict (C99)（エイリアスがないことを示す）
    // void copy(int * restrict dest, const int * restrict src, size_t n);

    // static（関数内で値を保持）
    for (int i = 0; i < 3; i++) {
        static int count = 0;
        count++;
        printf("static count: %d\n", count);
    }

    printf("\n");
}

// === 型キャスト ===
void type_casting(void) {
    printf("--- 型キャスト ---\n");

    // 暗黙の型変換（危険な場合も）
    int i = 100;
    double d = i;  // int → double（安全）
    printf("Implicit int to double: %d → %f\n", i, d);

    double d2 = 3.999;
    int i2 = d2;   // double → int（切り捨て）
    printf("Implicit double to int: %f → %d\n", d2, i2);

    // 明示的キャスト
    int a = 10, b = 3;
    double result = (double)a / b;
    printf("Explicit cast: %d / %d = %f\n", a, b, result);

    // ポインタキャスト
    int x = 0x12345678;
    char *cp = (char*)&x;
    printf("Pointer cast (byte view): ");
    for (size_t j = 0; j < sizeof(int); j++) {
        printf("%02X ", (unsigned char)cp[j]);
    }
    printf("\n");

    printf("\n");
}

// === _Generic (C11) ===
#define type_name(x) _Generic((x), \
    int: "int", \
    float: "float", \
    double: "double", \
    char*: "char*", \
    int*: "int*", \
    default: "unknown")

// 型に応じた処理を選択
#define abs_val(x) _Generic((x), \
    int: abs(x), \
    float: fabsf(x), \
    double: fabs(x))

void generic_selection(void) {
    printf("--- _Generic (C11) ---\n");

    int i = 42;
    float f = 3.14f;
    double d = 2.718;
    char *s = "hello";
    int *p = &i;

    printf("type_name(i) = %s\n", type_name(i));
    printf("type_name(f) = %s\n", type_name(f));
    printf("type_name(d) = %s\n", type_name(d));
    printf("type_name(s) = %s\n", type_name(s));
    printf("type_name(p) = %s\n", type_name(p));

    // 型に応じた値の出力
    printf("\nType-specific output:\n");
    printf("  int i = %d\n", i);
    printf("  float f = %f\n", f);
    printf("  double d = %f\n", d);
    printf("  char* s = %s\n", s);

    printf("\n");
}

// === typedef による型定義 ===
typedef unsigned char byte;
typedef int (*BinaryOp)(int, int);

int add_fn(int a, int b) { return a + b; }
int mul_fn(int a, int b) { return a * b; }

void typedef_demo(void) {
    printf("--- typedef ---\n");

    byte b = 255;
    printf("byte: %d\n", b);

    BinaryOp op = add_fn;
    printf("BinaryOp add: %d\n", op(3, 4));

    op = mul_fn;
    printf("BinaryOp mul: %d\n", op(3, 4));

    // 配列型の typedef
    typedef int IntArray[5];
    IntArray arr = {1, 2, 3, 4, 5};
    printf("IntArray: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    printf("\n");
}

// === メイン ===
int main(void) {
    printf("=== Type System Demo ===\n\n");

    basic_types();
    fixed_width_types();
    pointer_types();
    struct_types();
    union_types();
    enum_types();
    type_qualifiers();
    type_casting();
    generic_selection();
    typedef_demo();

    return 0;
}
