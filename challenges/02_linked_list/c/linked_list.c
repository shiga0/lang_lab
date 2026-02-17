// LinkedList - C 実装

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

// === Node 構造体 ===
typedef struct Node {
    int value;
    struct Node *next;
} Node;

// === LinkedList 構造体 ===
typedef struct {
    Node *head;
    Node *tail;
    size_t size;
} LinkedList;

// === 基本操作 ===

// リストの初期化
LinkedList* list_new(void) {
    LinkedList *list = malloc(sizeof(LinkedList));
    if (list == NULL) return NULL;

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

// ノードの作成
Node* node_new(int value) {
    Node *node = malloc(sizeof(Node));
    if (node == NULL) return NULL;

    node->value = value;
    node->next = NULL;
    return node;
}

// 空判定
bool list_is_empty(const LinkedList *list) {
    return list->size == 0;
}

// サイズ取得
size_t list_size(const LinkedList *list) {
    return list->size;
}

// 先頭に追加
bool list_push_front(LinkedList *list, int value) {
    Node *node = node_new(value);
    if (node == NULL) return false;

    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        node->next = list->head;
        list->head = node;
    }
    list->size++;
    return true;
}

// 末尾に追加
bool list_push_back(LinkedList *list, int value) {
    Node *node = node_new(value);
    if (node == NULL) return false;

    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
    list->size++;
    return true;
}

// 先頭を削除して返す
bool list_pop_front(LinkedList *list, int *out_value) {
    if (list->head == NULL) {
        return false;
    }

    Node *old_head = list->head;
    if (out_value != NULL) {
        *out_value = old_head->value;
    }

    list->head = list->head->next;
    if (list->head == NULL) {
        list->tail = NULL;
    }

    free(old_head);
    list->size--;
    return true;
}

// 先頭の値を参照
bool list_front(const LinkedList *list, int *out_value) {
    if (list->head == NULL) return false;
    if (out_value != NULL) {
        *out_value = list->head->value;
    }
    return true;
}

// 末尾の値を参照
bool list_back(const LinkedList *list, int *out_value) {
    if (list->tail == NULL) return false;
    if (out_value != NULL) {
        *out_value = list->tail->value;
    }
    return true;
}

// 検索
Node* list_find(const LinkedList *list, int value) {
    Node *current = list->head;
    while (current != NULL) {
        if (current->value == value) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// 値が含まれているか
bool list_contains(const LinkedList *list, int value) {
    return list_find(list, value) != NULL;
}

// 値を削除
bool list_delete(LinkedList *list, int value) {
    if (list->head == NULL) {
        return false;
    }

    // 先頭の場合
    if (list->head->value == value) {
        int dummy;
        return list_pop_front(list, &dummy);
    }

    // それ以外
    Node *prev = list->head;
    Node *current = list->head->next;

    while (current != NULL) {
        if (current->value == value) {
            prev->next = current->next;
            if (current == list->tail) {
                list->tail = prev;
            }
            free(current);
            list->size--;
            return true;
        }
        prev = current;
        current = current->next;
    }

    return false;
}

// 反転
void list_reverse(LinkedList *list) {
    if (list->size <= 1) {
        return;
    }

    Node *prev = NULL;
    Node *current = list->head;
    list->tail = list->head;

    while (current != NULL) {
        Node *next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    list->head = prev;
}

// クリア
void list_clear(LinkedList *list) {
    Node *current = list->head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

// 解放
void list_free(LinkedList *list) {
    if (list == NULL) return;
    list_clear(list);
    free(list);
}

// === イテレータ ===
// C言語ではコールバック関数による反復が一般的

typedef void (*ListIterFunc)(int value, void *context);

void list_foreach(const LinkedList *list, ListIterFunc func, void *context) {
    Node *current = list->head;
    while (current != NULL) {
        func(current->value, context);
        current = current->next;
    }
}

// 配列に変換
int* list_to_array(const LinkedList *list, size_t *out_size) {
    if (list->size == 0) {
        if (out_size != NULL) *out_size = 0;
        return NULL;
    }

    int *arr = malloc(list->size * sizeof(int));
    if (arr == NULL) return NULL;

    size_t i = 0;
    Node *current = list->head;
    while (current != NULL) {
        arr[i++] = current->value;
        current = current->next;
    }

    if (out_size != NULL) *out_size = list->size;
    return arr;
}

// 表示
void list_print(const LinkedList *list) {
    printf("LinkedList([");
    Node *current = list->head;
    while (current != NULL) {
        printf("%d", current->value);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("])\n");
}

// === map / filter / reduce ===

// map: 各要素に関数を適用した新しいリストを返す
typedef int (*MapFunc)(int value);

LinkedList* list_map(const LinkedList *list, MapFunc func) {
    LinkedList *new_list = list_new();
    if (new_list == NULL) return NULL;

    Node *current = list->head;
    while (current != NULL) {
        if (!list_push_back(new_list, func(current->value))) {
            list_free(new_list);
            return NULL;
        }
        current = current->next;
    }

    return new_list;
}

// filter: 条件を満たす要素のみの新しいリストを返す
typedef bool (*FilterFunc)(int value);

LinkedList* list_filter(const LinkedList *list, FilterFunc func) {
    LinkedList *new_list = list_new();
    if (new_list == NULL) return NULL;

    Node *current = list->head;
    while (current != NULL) {
        if (func(current->value)) {
            if (!list_push_back(new_list, current->value)) {
                list_free(new_list);
                return NULL;
            }
        }
        current = current->next;
    }

    return new_list;
}

// reduce: 畳み込み
typedef int (*ReduceFunc)(int accumulator, int value);

int list_reduce(const LinkedList *list, ReduceFunc func, int initial) {
    int acc = initial;
    Node *current = list->head;
    while (current != NULL) {
        acc = func(acc, current->value);
        current = current->next;
    }
    return acc;
}

// === デモ用関数 ===

void print_value(int value, void *context) {
    (void)context;
    printf("%d ", value);
}

int double_value(int value) {
    return value * 2;
}

bool greater_than_2(int value) {
    return value > 2;
}

int sum(int acc, int value) {
    return acc + value;
}

// === デモ ===

void demo(void) {
    printf("=== LinkedList Demo ===\n\n");

    printf("--- Basic Operations ---\n");

    LinkedList *list = list_new();
    printf("Empty list: ");
    list_print(list);

    list_push_front(list, 3);
    list_push_front(list, 2);
    list_push_front(list, 1);
    printf("After push_front 3, 2, 1: ");
    list_print(list);

    list_push_back(list, 4);
    list_push_back(list, 5);
    printf("After push_back 4, 5: ");
    list_print(list);

    printf("Size: %zu\n", list_size(list));

    int front, back;
    if (list_front(list, &front)) {
        printf("Front: %d\n", front);
    }
    if (list_back(list, &back)) {
        printf("Back: %d\n", back);
    }

    printf("\n--- Pop and Delete ---\n");

    int popped;
    if (list_pop_front(list, &popped)) {
        printf("pop_front: %d\n", popped);
    }
    printf("After pop_front: ");
    list_print(list);

    if (list_delete(list, 4)) {
        printf("delete(4): 4\n");
    }
    printf("After delete(4): ");
    list_print(list);

    printf("\n--- Iteration ---\n");
    list_clear(list);
    for (int x = 1; x <= 5; x++) {
        list_push_back(list, x);
    }
    printf("List: ");
    list_print(list);

    printf("foreach: ");
    list_foreach(list, print_value, NULL);
    printf("\n");

    printf("map (x * 2): ");
    LinkedList *doubled = list_map(list, double_value);
    list_print(doubled);
    list_free(doubled);

    printf("filter (x > 2): ");
    LinkedList *filtered = list_filter(list, greater_than_2);
    list_print(filtered);
    list_free(filtered);

    printf("sum: %d\n", list_reduce(list, sum, 0));
    printf("3 in list: %s\n", list_contains(list, 3) ? "true" : "false");

    printf("\n--- Reverse ---\n");
    printf("Before reverse: ");
    list_print(list);

    list_reverse(list);
    printf("After reverse: ");
    list_print(list);

    list_free(list);
    printf("\n");
}

// === テスト ===

void run_tests(void) {
    printf("--- Tests ---\n");

    LinkedList *list = list_new();
    assert(list != NULL);

    // 空リスト
    assert(list_is_empty(list));
    assert(list_size(list) == 0);

    // push_front
    list_push_front(list, 1);
    list_push_front(list, 2);
    list_push_front(list, 3);

    size_t size;
    int *arr = list_to_array(list, &size);
    assert(size == 3);
    assert(arr[0] == 3 && arr[1] == 2 && arr[2] == 1);
    free(arr);

    // push_back
    list_push_back(list, 10);
    arr = list_to_array(list, &size);
    assert(size == 4);
    assert(arr[0] == 3 && arr[1] == 2 && arr[2] == 1 && arr[3] == 10);
    free(arr);

    // front / back
    int front, back;
    assert(list_front(list, &front) && front == 3);
    assert(list_back(list, &back) && back == 10);

    // pop_front
    int popped;
    assert(list_pop_front(list, &popped) && popped == 3);
    arr = list_to_array(list, &size);
    assert(size == 3);
    assert(arr[0] == 2 && arr[1] == 1 && arr[2] == 10);
    free(arr);

    // delete
    assert(list_delete(list, 1));
    arr = list_to_array(list, &size);
    assert(size == 2);
    assert(arr[0] == 2 && arr[1] == 10);
    free(arr);

    // reverse
    list_push_back(list, 20);
    list_reverse(list);
    arr = list_to_array(list, &size);
    assert(size == 3);
    assert(arr[0] == 20 && arr[1] == 10 && arr[2] == 2);
    free(arr);

    // contains
    assert(list_contains(list, 10));
    assert(!list_contains(list, 999));

    list_free(list);

    printf("All tests passed!\n");
}

int main(void) {
    demo();
    run_tests();
    return 0;
}
