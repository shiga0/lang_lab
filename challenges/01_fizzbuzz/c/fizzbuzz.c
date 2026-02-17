// FizzBuzz - C 実装

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

// === 基本実装 ===
void fizzbuzz_basic(int n) {
    printf("--- Basic Implementation ---\n");

    for (int i = 1; i <= n; i++) {
        if (i % 15 == 0) {
            printf("FizzBuzz\n");
        } else if (i % 3 == 0) {
            printf("Fizz\n");
        } else if (i % 5 == 0) {
            printf("Buzz\n");
        } else {
            printf("%d\n", i);
        }
    }
    printf("\n");
}

// === 関数ポインタ版 ===
typedef const char* (*FizzBuzzRule)(int);

const char* rule_fizzbuzz(int i) {
    return (i % 15 == 0) ? "FizzBuzz" : NULL;
}

const char* rule_fizz(int i) {
    return (i % 3 == 0) ? "Fizz" : NULL;
}

const char* rule_buzz(int i) {
    return (i % 5 == 0) ? "Buzz" : NULL;
}

void fizzbuzz_function_pointer(int n) {
    printf("--- Function Pointer Style ---\n");

    FizzBuzzRule rules[] = {rule_fizzbuzz, rule_fizz, rule_buzz};
    size_t num_rules = sizeof(rules) / sizeof(rules[0]);

    for (int i = 1; i <= n; i++) {
        const char *result = NULL;
        for (size_t r = 0; r < num_rules; r++) {
            result = rules[r](i);
            if (result != NULL) break;
        }
        if (result != NULL) {
            printf("%s ", result);
        } else {
            printf("%d ", i);
        }
    }
    printf("\n\n");
}

// === 文字列連結スタイル ===
void fizzbuzz_concat(int n) {
    printf("--- String Concatenation Style ---\n");

    for (int i = 1; i <= n; i++) {
        char buffer[16] = "";

        if (i % 3 == 0) strcat(buffer, "Fizz");
        if (i % 5 == 0) strcat(buffer, "Buzz");

        if (buffer[0] == '\0') {
            printf("%d ", i);
        } else {
            printf("%s ", buffer);
        }
    }
    printf("\n\n");
}

// === ルックアップテーブル ===
void fizzbuzz_lookup(int n) {
    printf("--- Lookup Table Style ---\n");

    // 15周期のパターンをテーブル化
    // NULL = 数字を出力, それ以外 = 文字列を出力
    static const char *table[15] = {
        NULL, NULL, "Fizz",        // 1, 2, 3
        NULL, "Buzz", "Fizz",      // 4, 5, 6
        NULL, NULL, "Fizz",        // 7, 8, 9
        "Buzz", NULL, "Fizz",      // 10, 11, 12
        NULL, NULL, "FizzBuzz"     // 13, 14, 15
    };

    for (int i = 1; i <= n; i++) {
        const char *s = table[(i - 1) % 15];
        if (s != NULL) {
            printf("%s ", s);
        } else {
            printf("%d ", i);
        }
    }
    printf("\n\n");
}

// === ビット演算 ===
void fizzbuzz_bitwise(int n) {
    printf("--- Bitwise Style ---\n");

    for (int i = 1; i <= n; i++) {
        // divisible_by_3 = 1, divisible_by_5 = 2
        int flags = ((i % 3 == 0) << 0) | ((i % 5 == 0) << 1);

        switch (flags) {
            case 3: printf("FizzBuzz "); break;  // 両方
            case 1: printf("Fizz "); break;      // 3のみ
            case 2: printf("Buzz "); break;      // 5のみ
            default: printf("%d ", i); break;
        }
    }
    printf("\n\n");
}

// === 配列返却版（動的メモリ） ===
char** fizzbuzz_array(int n) {
    char **result = malloc(n * sizeof(char*));
    if (result == NULL) return NULL;

    for (int i = 0; i < n; i++) {
        result[i] = malloc(16 * sizeof(char));
        if (result[i] == NULL) {
            // クリーンアップ
            for (int j = 0; j < i; j++) free(result[j]);
            free(result);
            return NULL;
        }

        int num = i + 1;
        if (num % 15 == 0) {
            strcpy(result[i], "FizzBuzz");
        } else if (num % 3 == 0) {
            strcpy(result[i], "Fizz");
        } else if (num % 5 == 0) {
            strcpy(result[i], "Buzz");
        } else {
            snprintf(result[i], 16, "%d", num);
        }
    }
    return result;
}

void free_fizzbuzz_array(char **arr, int n) {
    if (arr == NULL) return;
    for (int i = 0; i < n; i++) {
        free(arr[i]);
    }
    free(arr);
}

void array_demo(void) {
    printf("--- Array Style (Dynamic Memory) ---\n");

    int n = 15;
    char **result = fizzbuzz_array(n);
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        printf("%s ", result[i]);
    }
    printf("\n\n");

    free_fizzbuzz_array(result, n);
}

// === マクロ版 ===
#define FIZZBUZZ_PRINT(i) \
    do { \
        if ((i) % 15 == 0) printf("FizzBuzz "); \
        else if ((i) % 3 == 0) printf("Fizz "); \
        else if ((i) % 5 == 0) printf("Buzz "); \
        else printf("%d ", (i)); \
    } while (0)

void fizzbuzz_macro(int n) {
    printf("--- Macro Style ---\n");

    for (int i = 1; i <= n; i++) {
        FIZZBUZZ_PRINT(i);
    }
    printf("\n\n");
}

// === 再帰版 ===
void fizzbuzz_recursive_helper(int current, int n) {
    if (current > n) return;

    if (current % 15 == 0) {
        printf("FizzBuzz ");
    } else if (current % 3 == 0) {
        printf("Fizz ");
    } else if (current % 5 == 0) {
        printf("Buzz ");
    } else {
        printf("%d ", current);
    }

    fizzbuzz_recursive_helper(current + 1, n);
}

void fizzbuzz_recursive(int n) {
    printf("--- Recursive Style ---\n");
    fizzbuzz_recursive_helper(1, n);
    printf("\n\n");
}

// === テスト ===
void run_tests(void) {
    printf("--- Tests ---\n");

    int n = 15;
    char **result = fizzbuzz_array(n);
    assert(result != NULL);

    // 基本ケース
    assert(strcmp(result[0], "1") == 0);
    assert(strcmp(result[1], "2") == 0);

    // Fizz (3の倍数)
    assert(strcmp(result[2], "Fizz") == 0);   // 3
    assert(strcmp(result[5], "Fizz") == 0);   // 6

    // Buzz (5の倍数)
    assert(strcmp(result[4], "Buzz") == 0);   // 5
    assert(strcmp(result[9], "Buzz") == 0);   // 10

    // FizzBuzz (15の倍数)
    assert(strcmp(result[14], "FizzBuzz") == 0);  // 15

    free_fizzbuzz_array(result, n);

    printf("All tests passed!\n");
}

int main(void) {
    printf("=== FizzBuzz Demo ===\n\n");

    fizzbuzz_basic(15);
    fizzbuzz_function_pointer(15);
    fizzbuzz_concat(15);
    fizzbuzz_lookup(15);
    fizzbuzz_bitwise(15);
    array_demo();
    fizzbuzz_macro(15);
    fizzbuzz_recursive(15);
    run_tests();

    return 0;
}
