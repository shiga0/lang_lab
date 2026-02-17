// エラー処理 - C 実装
//
// C には例外がないため、戻り値とerrnoでエラーを表現する。
// エラー処理は呼び出し側の責任。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <assert.h>
#include <setjmp.h>

// === errno によるエラー処理 ===
void errno_demo(void) {
    printf("--- errno によるエラー処理 ---\n");

    // ファイルオープンの例
    FILE *fp = fopen("nonexistent_file.txt", "r");
    if (fp == NULL) {
        printf("fopen failed: %s (errno: %d)\n", strerror(errno), errno);
    }

    // 数値変換の例
    const char *str = "not_a_number";
    char *endptr;
    errno = 0;
    long val = strtol(str, &endptr, 10);

    if (errno != 0 || endptr == str) {
        printf("strtol failed for \"%s\"\n", str);
    } else {
        printf("strtol result: %ld\n", val);
    }

    // 正常な変換
    const char *valid = "12345";
    errno = 0;
    val = strtol(valid, &endptr, 10);
    if (errno == 0 && endptr != valid) {
        printf("strtol(\"%s\") = %ld\n", valid, val);
    }

    printf("\n");
}

// === 戻り値によるエラー処理 ===

// エラーコード定義
typedef enum {
    ERR_OK = 0,
    ERR_NULL_POINTER,
    ERR_OUT_OF_MEMORY,
    ERR_INVALID_ARGUMENT,
    ERR_NOT_FOUND,
    ERR_IO_ERROR
} ErrorCode;

const char* error_message(ErrorCode err) {
    switch (err) {
        case ERR_OK: return "Success";
        case ERR_NULL_POINTER: return "Null pointer";
        case ERR_OUT_OF_MEMORY: return "Out of memory";
        case ERR_INVALID_ARGUMENT: return "Invalid argument";
        case ERR_NOT_FOUND: return "Not found";
        case ERR_IO_ERROR: return "I/O error";
        default: return "Unknown error";
    }
}

// 結果を出力パラメータで返す
ErrorCode divide(int a, int b, int *result) {
    if (result == NULL) {
        return ERR_NULL_POINTER;
    }
    if (b == 0) {
        return ERR_INVALID_ARGUMENT;
    }
    *result = a / b;
    return ERR_OK;
}

void return_value_demo(void) {
    printf("--- 戻り値によるエラー処理 ---\n");

    int result;
    ErrorCode err;

    err = divide(10, 2, &result);
    if (err == ERR_OK) {
        printf("10 / 2 = %d\n", result);
    } else {
        printf("Error: %s\n", error_message(err));
    }

    err = divide(10, 0, &result);
    if (err == ERR_OK) {
        printf("10 / 0 = %d\n", result);
    } else {
        printf("10 / 0: Error: %s\n", error_message(err));
    }

    err = divide(10, 3, NULL);
    if (err == ERR_OK) {
        printf("Result: %d\n", result);
    } else {
        printf("NULL result: Error: %s\n", error_message(err));
    }

    printf("\n");
}

// === Result 型パターン ===
typedef struct {
    bool is_ok;
    union {
        int value;
        ErrorCode error;
    } data;
} IntResult;

IntResult int_result_ok(int value) {
    return (IntResult){.is_ok = true, .data.value = value};
}

IntResult int_result_err(ErrorCode error) {
    return (IntResult){.is_ok = false, .data.error = error};
}

IntResult safe_divide(int a, int b) {
    if (b == 0) {
        return int_result_err(ERR_INVALID_ARGUMENT);
    }
    return int_result_ok(a / b);
}

void result_type_demo(void) {
    printf("--- Result 型パターン ---\n");

    IntResult r1 = safe_divide(10, 2);
    if (r1.is_ok) {
        printf("10 / 2 = %d\n", r1.data.value);
    } else {
        printf("Error: %s\n", error_message(r1.data.error));
    }

    IntResult r2 = safe_divide(10, 0);
    if (r2.is_ok) {
        printf("10 / 0 = %d\n", r2.data.value);
    } else {
        printf("10 / 0: Error: %s\n", error_message(r2.data.error));
    }

    printf("\n");
}

// === Option 型パターン ===
typedef struct {
    bool has_value;
    int value;
} OptionInt;

OptionInt some_int(int value) {
    return (OptionInt){.has_value = true, .value = value};
}

OptionInt none_int(void) {
    return (OptionInt){.has_value = false};
}

OptionInt find_in_array(const int *arr, size_t len, int target) {
    for (size_t i = 0; i < len; i++) {
        if (arr[i] == target) {
            return some_int((int)i);
        }
    }
    return none_int();
}

void option_type_demo(void) {
    printf("--- Option 型パターン ---\n");

    int arr[] = {1, 3, 5, 7, 9};
    size_t len = sizeof(arr) / sizeof(arr[0]);

    OptionInt result = find_in_array(arr, len, 5);
    if (result.has_value) {
        printf("Found 5 at index %d\n", result.value);
    } else {
        printf("5 not found\n");
    }

    result = find_in_array(arr, len, 4);
    if (result.has_value) {
        printf("Found 4 at index %d\n", result.value);
    } else {
        printf("4 not found\n");
    }

    printf("\n");
}

// === assert ===
void assert_demo(void) {
    printf("--- assert ---\n");

    int x = 42;
    assert(x > 0);  // 成功
    printf("x = %d (assertion passed)\n", x);

    // assert(x < 0);  // これは失敗してプログラムが終了する

    // NDEBUG を定義すると assert が無効化される
    #ifdef NDEBUG
    printf("NDEBUG is defined, assertions are disabled\n");
    #else
    printf("Assertions are enabled\n");
    #endif

    printf("\n");
}

// === setjmp/longjmp（非ローカルジャンプ）===
jmp_buf jump_buffer;

void deep_function(int level) {
    printf("At level %d\n", level);
    if (level == 3) {
        printf("Error at level 3! Jumping back...\n");
        longjmp(jump_buffer, 1);  // setjmp の位置に戻る
    }
    deep_function(level + 1);
}

void setjmp_demo(void) {
    printf("--- setjmp/longjmp ---\n");

    int result = setjmp(jump_buffer);

    if (result == 0) {
        // 最初の呼び出し
        printf("Starting deep call...\n");
        deep_function(0);
        printf("This won't be printed\n");
    } else {
        // longjmp から戻った
        printf("Recovered from error (result = %d)\n", result);
    }

    printf("\n");
}

// === クリーンアップパターン ===
typedef struct {
    int *data;
    size_t size;
} Buffer;

ErrorCode buffer_create(Buffer **out, size_t size) {
    if (out == NULL) {
        return ERR_NULL_POINTER;
    }

    Buffer *buf = malloc(sizeof(Buffer));
    if (buf == NULL) {
        return ERR_OUT_OF_MEMORY;
    }

    buf->data = malloc(size * sizeof(int));
    if (buf->data == NULL) {
        free(buf);  // クリーンアップ
        return ERR_OUT_OF_MEMORY;
    }

    buf->size = size;
    *out = buf;
    return ERR_OK;
}

void buffer_destroy(Buffer *buf) {
    if (buf == NULL) return;
    free(buf->data);
    free(buf);
}

ErrorCode complex_operation(void) {
    Buffer *buf1 = NULL;
    Buffer *buf2 = NULL;
    ErrorCode err;

    // リソース 1 を確保
    err = buffer_create(&buf1, 100);
    if (err != ERR_OK) {
        goto cleanup;  // クリーンアップへジャンプ
    }

    // リソース 2 を確保
    err = buffer_create(&buf2, 200);
    if (err != ERR_OK) {
        goto cleanup;
    }

    // 何か処理...
    printf("Both buffers created successfully\n");
    err = ERR_OK;

cleanup:
    // すべてのリソースをクリーンアップ
    buffer_destroy(buf2);
    buffer_destroy(buf1);
    return err;
}

void cleanup_pattern_demo(void) {
    printf("--- クリーンアップパターン（goto）---\n");

    ErrorCode err = complex_operation();
    if (err == ERR_OK) {
        printf("Operation succeeded\n");
    } else {
        printf("Operation failed: %s\n", error_message(err));
    }

    printf("\n");
}

// === defer 風マクロ（GCC拡張）===
#ifdef __GNUC__
#define DEFER(fn) __attribute__((cleanup(fn)))

static void free_ptr(void *p) {
    free(*(void**)p);
}

void defer_demo(void) {
    printf("--- defer 風マクロ (GCC拡張) ---\n");

    DEFER(free_ptr) char *str = malloc(100);
    if (str != NULL) {
        strcpy(str, "Auto-freed string");
        printf("String: %s\n", str);
    }
    // スコープを抜けると自動的に free される

    printf("String freed automatically on scope exit\n\n");
}
#else
void defer_demo(void) {
    printf("--- defer 風マクロ ---\n");
    printf("Not available (requires GCC extensions)\n\n");
}
#endif

// === メイン ===
int main(void) {
    printf("=== Error Handling Demo ===\n\n");

    errno_demo();
    return_value_demo();
    result_type_demo();
    option_type_demo();
    assert_demo();
    setjmp_demo();
    cleanup_pattern_demo();
    defer_demo();

    return 0;
}
