// メモリ管理 - C 実装
//
// C は完全手動メモリ管理。
// malloc/free でヒープメモリを管理する。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// === スタックとヒープ ===
void stack_and_heap(void) {
    printf("--- スタックとヒープ ---\n");

    // スタック（自動変数）
    int stack_var = 42;
    int stack_arr[5] = {1, 2, 3, 4, 5};

    printf("Stack variable: %d (addr: %p)\n", stack_var, (void*)&stack_var);
    printf("Stack array: %p\n", (void*)stack_arr);

    // ヒープ（動的割り当て）
    int *heap_var = malloc(sizeof(int));
    *heap_var = 100;

    int *heap_arr = malloc(5 * sizeof(int));
    for (int i = 0; i < 5; i++) {
        heap_arr[i] = i * 10;
    }

    printf("Heap variable: %d (addr: %p)\n", *heap_var, (void*)heap_var);
    printf("Heap array: %p\n", (void*)heap_arr);

    // 解放
    free(heap_var);
    free(heap_arr);

    printf("\n");
}

// === malloc / calloc / realloc / free ===
void allocation_functions(void) {
    printf("--- 割り当て関数 ---\n");

    // malloc: 未初期化メモリ
    printf("malloc:\n");
    int *p1 = malloc(3 * sizeof(int));
    if (p1 == NULL) {
        fprintf(stderr, "malloc failed\n");
        return;
    }
    printf("  Uninitialized: %d, %d, %d\n", p1[0], p1[1], p1[2]);
    p1[0] = 10; p1[1] = 20; p1[2] = 30;
    printf("  After init: %d, %d, %d\n", p1[0], p1[1], p1[2]);
    free(p1);

    // calloc: ゼロ初期化
    printf("calloc:\n");
    int *p2 = calloc(3, sizeof(int));
    if (p2 == NULL) {
        fprintf(stderr, "calloc failed\n");
        return;
    }
    printf("  Zero-initialized: %d, %d, %d\n", p2[0], p2[1], p2[2]);
    free(p2);

    // realloc: サイズ変更
    printf("realloc:\n");
    int *p3 = malloc(3 * sizeof(int));
    p3[0] = 1; p3[1] = 2; p3[2] = 3;
    printf("  Before: %d, %d, %d\n", p3[0], p3[1], p3[2]);

    p3 = realloc(p3, 5 * sizeof(int));
    if (p3 == NULL) {
        fprintf(stderr, "realloc failed\n");
        return;
    }
    p3[3] = 4; p3[4] = 5;
    printf("  After: %d, %d, %d, %d, %d\n", p3[0], p3[1], p3[2], p3[3], p3[4]);
    free(p3);

    printf("\n");
}

// === メモリリーク ===
void memory_leak_demo(void) {
    printf("--- メモリリーク（避けるべきパターン）---\n");

    // リークの例（コメントアウト）
    /*
    int *leaked = malloc(sizeof(int));
    *leaked = 42;
    // free(leaked); を忘れるとリーク
    */

    // 正しいパターン
    int *not_leaked = malloc(sizeof(int));
    *not_leaked = 42;
    printf("Allocated and will free: %d\n", *not_leaked);
    free(not_leaked);

    printf("Proper cleanup done.\n\n");
}

// === ダングリングポインタ ===
void dangling_pointer_demo(void) {
    printf("--- ダングリングポインタ（避けるべきパターン）---\n");

    // 危険な例（コメントアウト）
    /*
    int *p = malloc(sizeof(int));
    *p = 42;
    free(p);
    // p は無効だが、まだアドレスを保持
    // *p = 100;  // 未定義動作！
    */

    // 安全なパターン
    int *p = malloc(sizeof(int));
    *p = 42;
    printf("Before free: %d\n", *p);
    free(p);
    p = NULL;  // NULL にして無効化

    if (p == NULL) {
        printf("Pointer is now NULL (safe)\n");
    }

    printf("\n");
}

// === 二重解放 ===
void double_free_demo(void) {
    printf("--- 二重解放（避けるべきパターン）---\n");

    // 危険な例（コメントアウト）
    /*
    int *p = malloc(sizeof(int));
    free(p);
    free(p);  // 二重解放！未定義動作
    */

    // 安全なパターン
    int *p = malloc(sizeof(int));
    *p = 42;
    free(p);
    p = NULL;
    // free(NULL) は安全（何もしない）
    free(p);

    printf("Safe handling with NULL check.\n\n");
}

// === 構造体のメモリ管理 ===
typedef struct {
    char *name;
    int age;
} Person;

Person* person_new(const char *name, int age) {
    Person *p = malloc(sizeof(Person));
    if (p == NULL) return NULL;

    p->name = strdup(name);  // 文字列を複製
    if (p->name == NULL) {
        free(p);
        return NULL;
    }

    p->age = age;
    return p;
}

void person_free(Person *p) {
    if (p == NULL) return;
    free(p->name);  // 内部リソースを先に解放
    free(p);
}

void struct_memory(void) {
    printf("--- 構造体のメモリ管理 ---\n");

    Person *alice = person_new("Alice", 30);
    if (alice != NULL) {
        printf("Person: %s, %d years old\n", alice->name, alice->age);
        person_free(alice);
    }

    printf("Struct properly freed.\n\n");
}

// === 配列の動的確保 ===
typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} DynamicArray;

DynamicArray* array_new(size_t initial_capacity) {
    DynamicArray *arr = malloc(sizeof(DynamicArray));
    if (arr == NULL) return NULL;

    arr->data = malloc(initial_capacity * sizeof(int));
    if (arr->data == NULL) {
        free(arr);
        return NULL;
    }

    arr->size = 0;
    arr->capacity = initial_capacity;
    return arr;
}

bool array_push(DynamicArray *arr, int value) {
    if (arr->size >= arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
        int *new_data = realloc(arr->data, new_capacity * sizeof(int));
        if (new_data == NULL) return false;
        arr->data = new_data;
        arr->capacity = new_capacity;
    }
    arr->data[arr->size++] = value;
    return true;
}

void array_free(DynamicArray *arr) {
    if (arr == NULL) return;
    free(arr->data);
    free(arr);
}

void dynamic_array_demo(void) {
    printf("--- 動的配列 ---\n");

    DynamicArray *arr = array_new(4);
    if (arr == NULL) {
        fprintf(stderr, "Failed to create array\n");
        return;
    }

    for (int i = 0; i < 10; i++) {
        array_push(arr, i * 10);
    }

    printf("Array (size=%zu, capacity=%zu): ", arr->size, arr->capacity);
    for (size_t i = 0; i < arr->size; i++) {
        printf("%d ", arr->data[i]);
    }
    printf("\n");

    array_free(arr);
    printf("Dynamic array freed.\n\n");
}

// === メモリプール（シンプルな例） ===
#define POOL_SIZE 1024

typedef struct {
    char buffer[POOL_SIZE];
    size_t offset;
} MemoryPool;

void pool_init(MemoryPool *pool) {
    pool->offset = 0;
}

void* pool_alloc(MemoryPool *pool, size_t size) {
    // アライメント調整
    size_t aligned_size = (size + 7) & ~7;

    if (pool->offset + aligned_size > POOL_SIZE) {
        return NULL;  // プール枯渇
    }

    void *ptr = &pool->buffer[pool->offset];
    pool->offset += aligned_size;
    return ptr;
}

void pool_reset(MemoryPool *pool) {
    pool->offset = 0;  // 一括解放
}

void memory_pool_demo(void) {
    printf("--- メモリプール ---\n");

    MemoryPool pool;
    pool_init(&pool);

    int *a = pool_alloc(&pool, sizeof(int));
    int *b = pool_alloc(&pool, sizeof(int));
    int *arr = pool_alloc(&pool, 10 * sizeof(int));

    *a = 100;
    *b = 200;
    for (int i = 0; i < 10; i++) {
        arr[i] = i;
    }

    printf("a = %d, b = %d\n", *a, *b);
    printf("arr = ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    printf("Pool used: %zu / %d bytes\n", pool.offset, POOL_SIZE);

    pool_reset(&pool);
    printf("Pool reset.\n\n");
}

// === アライメント ===
void alignment_demo(void) {
    printf("--- アライメント ---\n");

    struct Example1 {
        char a;    // 1 byte
        int b;     // 4 bytes
        char c;    // 1 byte
    };

    struct Example2 {
        int b;     // 4 bytes
        char a;    // 1 byte
        char c;    // 1 byte
    };

    printf("struct Example1 (char, int, char):\n");
    printf("  sizeof: %zu\n", sizeof(struct Example1));
    printf("  offsetof a: %zu\n", __builtin_offsetof(struct Example1, a));
    printf("  offsetof b: %zu\n", __builtin_offsetof(struct Example1, b));
    printf("  offsetof c: %zu\n", __builtin_offsetof(struct Example1, c));

    printf("struct Example2 (int, char, char):\n");
    printf("  sizeof: %zu\n", sizeof(struct Example2));
    printf("  offsetof b: %zu\n", __builtin_offsetof(struct Example2, b));
    printf("  offsetof a: %zu\n", __builtin_offsetof(struct Example2, a));
    printf("  offsetof c: %zu\n", __builtin_offsetof(struct Example2, c));

    printf("\n");
}

// === メイン ===
int main(void) {
    printf("=== Memory Management Demo ===\n\n");

    stack_and_heap();
    allocation_functions();
    memory_leak_demo();
    dangling_pointer_demo();
    double_free_demo();
    struct_memory();
    dynamic_array_demo();
    memory_pool_demo();
    alignment_demo();

    return 0;
}
