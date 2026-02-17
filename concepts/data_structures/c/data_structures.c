// データ構造 - C 実装
//
// C には組み込みのコンテナがないため、
// 基本的なデータ構造を手動で実装する。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// === 動的配列 (Vector) ===

typedef struct {
    int* data;
    size_t size;
    size_t capacity;
} IntVector;

void vector_init(IntVector* v) {
    v->data = malloc(4 * sizeof(int));
    v->size = 0;
    v->capacity = 4;
}

void vector_free(IntVector* v) {
    free(v->data);
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}

void vector_push(IntVector* v, int value) {
    if (v->size >= v->capacity) {
        v->capacity *= 2;
        v->data = realloc(v->data, v->capacity * sizeof(int));
    }
    v->data[v->size++] = value;
}

int vector_pop(IntVector* v) {
    if (v->size == 0) return -1;
    return v->data[--v->size];
}

int vector_get(IntVector* v, size_t index) {
    if (index >= v->size) return -1;
    return v->data[index];
}

void vector_set(IntVector* v, size_t index, int value) {
    if (index < v->size) {
        v->data[index] = value;
    }
}

void vector_print(IntVector* v) {
    printf("[");
    for (size_t i = 0; i < v->size; i++) {
        printf("%d", v->data[i]);
        if (i < v->size - 1) printf(", ");
    }
    printf("]");
}

void vector_demo(void) {
    printf("=== 動的配列 (Vector) ===\n");

    IntVector v;
    vector_init(&v);

    for (int i = 1; i <= 5; i++) {
        vector_push(&v, i);
    }

    printf("  vector: ");
    vector_print(&v);
    printf("\n");

    printf("  size = %zu, capacity = %zu\n", v.size, v.capacity);
    printf("  get(2) = %d\n", vector_get(&v, 2));

    vector_set(&v, 2, 100);
    printf("  after set(2, 100): ");
    vector_print(&v);
    printf("\n");

    printf("  pop() = %d\n", vector_pop(&v));
    printf("  after pop: ");
    vector_print(&v);
    printf("\n");

    vector_free(&v);
    printf("\n");
}

// === 単方向連結リスト ===

typedef struct SListNode {
    int value;
    struct SListNode* next;
} SListNode;

typedef struct {
    SListNode* head;
    size_t size;
} SinglyLinkedList;

void slist_init(SinglyLinkedList* list) {
    list->head = NULL;
    list->size = 0;
}

void slist_free(SinglyLinkedList* list) {
    SListNode* current = list->head;
    while (current) {
        SListNode* next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->size = 0;
}

void slist_push_front(SinglyLinkedList* list, int value) {
    SListNode* node = malloc(sizeof(SListNode));
    node->value = value;
    node->next = list->head;
    list->head = node;
    list->size++;
}

void slist_push_back(SinglyLinkedList* list, int value) {
    SListNode* node = malloc(sizeof(SListNode));
    node->value = value;
    node->next = NULL;

    if (!list->head) {
        list->head = node;
    } else {
        SListNode* current = list->head;
        while (current->next) {
            current = current->next;
        }
        current->next = node;
    }
    list->size++;
}

int slist_pop_front(SinglyLinkedList* list) {
    if (!list->head) return -1;

    SListNode* node = list->head;
    int value = node->value;
    list->head = node->next;
    free(node);
    list->size--;
    return value;
}

void slist_print(SinglyLinkedList* list) {
    printf("[");
    SListNode* current = list->head;
    while (current) {
        printf("%d", current->value);
        if (current->next) printf(" -> ");
        current = current->next;
    }
    printf("]");
}

void singly_linked_list_demo(void) {
    printf("=== 単方向連結リスト ===\n");

    SinglyLinkedList list;
    slist_init(&list);

    slist_push_back(&list, 1);
    slist_push_back(&list, 2);
    slist_push_back(&list, 3);
    slist_push_front(&list, 0);

    printf("  list: ");
    slist_print(&list);
    printf("\n");

    printf("  pop_front() = %d\n", slist_pop_front(&list));
    printf("  after pop: ");
    slist_print(&list);
    printf("\n");

    printf("  size = %zu\n", list.size);

    slist_free(&list);
    printf("\n");
}

// === 双方向連結リスト ===

typedef struct DListNode {
    int value;
    struct DListNode* prev;
    struct DListNode* next;
} DListNode;

typedef struct {
    DListNode* head;
    DListNode* tail;
    size_t size;
} DoublyLinkedList;

void dlist_init(DoublyLinkedList* list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void dlist_free(DoublyLinkedList* list) {
    DListNode* current = list->head;
    while (current) {
        DListNode* next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void dlist_push_front(DoublyLinkedList* list, int value) {
    DListNode* node = malloc(sizeof(DListNode));
    node->value = value;
    node->prev = NULL;
    node->next = list->head;

    if (list->head) {
        list->head->prev = node;
    } else {
        list->tail = node;
    }
    list->head = node;
    list->size++;
}

void dlist_push_back(DoublyLinkedList* list, int value) {
    DListNode* node = malloc(sizeof(DListNode));
    node->value = value;
    node->next = NULL;
    node->prev = list->tail;

    if (list->tail) {
        list->tail->next = node;
    } else {
        list->head = node;
    }
    list->tail = node;
    list->size++;
}

int dlist_pop_front(DoublyLinkedList* list) {
    if (!list->head) return -1;

    DListNode* node = list->head;
    int value = node->value;
    list->head = node->next;

    if (list->head) {
        list->head->prev = NULL;
    } else {
        list->tail = NULL;
    }

    free(node);
    list->size--;
    return value;
}

int dlist_pop_back(DoublyLinkedList* list) {
    if (!list->tail) return -1;

    DListNode* node = list->tail;
    int value = node->value;
    list->tail = node->prev;

    if (list->tail) {
        list->tail->next = NULL;
    } else {
        list->head = NULL;
    }

    free(node);
    list->size--;
    return value;
}

void dlist_print(DoublyLinkedList* list) {
    printf("[");
    DListNode* current = list->head;
    while (current) {
        printf("%d", current->value);
        if (current->next) printf(" <-> ");
        current = current->next;
    }
    printf("]");
}

void doubly_linked_list_demo(void) {
    printf("=== 双方向連結リスト ===\n");

    DoublyLinkedList list;
    dlist_init(&list);

    dlist_push_back(&list, 2);
    dlist_push_back(&list, 3);
    dlist_push_front(&list, 1);
    dlist_push_back(&list, 4);

    printf("  list: ");
    dlist_print(&list);
    printf("\n");

    printf("  pop_front() = %d\n", dlist_pop_front(&list));
    printf("  pop_back() = %d\n", dlist_pop_back(&list));
    printf("  after pops: ");
    dlist_print(&list);
    printf("\n");

    dlist_free(&list);
    printf("\n");
}

// === スタック ===

typedef struct {
    int* data;
    size_t size;
    size_t capacity;
} Stack;

void stack_init(Stack* s) {
    s->data = malloc(16 * sizeof(int));
    s->size = 0;
    s->capacity = 16;
}

void stack_free(Stack* s) {
    free(s->data);
    s->data = NULL;
    s->size = 0;
    s->capacity = 0;
}

void stack_push(Stack* s, int value) {
    if (s->size >= s->capacity) {
        s->capacity *= 2;
        s->data = realloc(s->data, s->capacity * sizeof(int));
    }
    s->data[s->size++] = value;
}

int stack_pop(Stack* s) {
    if (s->size == 0) return -1;
    return s->data[--s->size];
}

int stack_peek(Stack* s) {
    if (s->size == 0) return -1;
    return s->data[s->size - 1];
}

bool stack_is_empty(Stack* s) {
    return s->size == 0;
}

void stack_demo(void) {
    printf("=== スタック ===\n");

    Stack s;
    stack_init(&s);

    stack_push(&s, 1);
    stack_push(&s, 2);
    stack_push(&s, 3);

    printf("  peek() = %d\n", stack_peek(&s));
    printf("  size = %zu\n", s.size);

    printf("  popping: ");
    while (!stack_is_empty(&s)) {
        printf("%d ", stack_pop(&s));
    }
    printf("\n");

    stack_free(&s);
    printf("\n");
}

// === キュー (リングバッファ) ===

typedef struct {
    int* data;
    size_t head;
    size_t tail;
    size_t size;
    size_t capacity;
} Queue;

void queue_init(Queue* q) {
    q->capacity = 16;
    q->data = malloc(q->capacity * sizeof(int));
    q->head = 0;
    q->tail = 0;
    q->size = 0;
}

void queue_free(Queue* q) {
    free(q->data);
    q->data = NULL;
    q->size = 0;
}

void queue_enqueue(Queue* q, int value) {
    if (q->size >= q->capacity) {
        // 簡略化のため、容量拡張は省略
        return;
    }
    q->data[q->tail] = value;
    q->tail = (q->tail + 1) % q->capacity;
    q->size++;
}

int queue_dequeue(Queue* q) {
    if (q->size == 0) return -1;
    int value = q->data[q->head];
    q->head = (q->head + 1) % q->capacity;
    q->size--;
    return value;
}

int queue_front(Queue* q) {
    if (q->size == 0) return -1;
    return q->data[q->head];
}

bool queue_is_empty(Queue* q) {
    return q->size == 0;
}

void queue_demo(void) {
    printf("=== キュー ===\n");

    Queue q;
    queue_init(&q);

    queue_enqueue(&q, 1);
    queue_enqueue(&q, 2);
    queue_enqueue(&q, 3);

    printf("  front() = %d\n", queue_front(&q));
    printf("  size = %zu\n", q.size);

    printf("  dequeuing: ");
    while (!queue_is_empty(&q)) {
        printf("%d ", queue_dequeue(&q));
    }
    printf("\n");

    queue_free(&q);
    printf("\n");
}

// === ハッシュテーブル ===

#define HASH_SIZE 16

typedef struct HashEntry {
    char* key;
    int value;
    struct HashEntry* next;
} HashEntry;

typedef struct {
    HashEntry* buckets[HASH_SIZE];
    size_t size;
} HashTable;

unsigned int hash_string(const char* str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % HASH_SIZE;
}

void hashtable_init(HashTable* ht) {
    for (int i = 0; i < HASH_SIZE; i++) {
        ht->buckets[i] = NULL;
    }
    ht->size = 0;
}

void hashtable_free(HashTable* ht) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashEntry* entry = ht->buckets[i];
        while (entry) {
            HashEntry* next = entry->next;
            free(entry->key);
            free(entry);
            entry = next;
        }
    }
    ht->size = 0;
}

void hashtable_put(HashTable* ht, const char* key, int value) {
    unsigned int idx = hash_string(key);
    HashEntry* entry = ht->buckets[idx];

    // 既存キーを検索
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return;
        }
        entry = entry->next;
    }

    // 新規エントリ
    entry = malloc(sizeof(HashEntry));
    entry->key = strdup(key);
    entry->value = value;
    entry->next = ht->buckets[idx];
    ht->buckets[idx] = entry;
    ht->size++;
}

bool hashtable_get(HashTable* ht, const char* key, int* value) {
    unsigned int idx = hash_string(key);
    HashEntry* entry = ht->buckets[idx];

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            *value = entry->value;
            return true;
        }
        entry = entry->next;
    }
    return false;
}

bool hashtable_remove(HashTable* ht, const char* key) {
    unsigned int idx = hash_string(key);
    HashEntry* entry = ht->buckets[idx];
    HashEntry* prev = NULL;

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (prev) {
                prev->next = entry->next;
            } else {
                ht->buckets[idx] = entry->next;
            }
            free(entry->key);
            free(entry);
            ht->size--;
            return true;
        }
        prev = entry;
        entry = entry->next;
    }
    return false;
}

void hashtable_demo(void) {
    printf("=== ハッシュテーブル ===\n");

    HashTable ht;
    hashtable_init(&ht);

    hashtable_put(&ht, "one", 1);
    hashtable_put(&ht, "two", 2);
    hashtable_put(&ht, "three", 3);

    int value;
    if (hashtable_get(&ht, "two", &value)) {
        printf("  ht[\"two\"] = %d\n", value);
    }

    printf("  size = %zu\n", ht.size);

    if (!hashtable_get(&ht, "four", &value)) {
        printf("  ht[\"four\"] not found\n");
    }

    hashtable_put(&ht, "two", 22);
    hashtable_get(&ht, "two", &value);
    printf("  after update: ht[\"two\"] = %d\n", value);

    hashtable_remove(&ht, "one");
    printf("  after remove \"one\": size = %zu\n", ht.size);

    hashtable_free(&ht);
    printf("\n");
}

// === 二分探索木 ===

typedef struct BSTNode {
    int value;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

typedef struct {
    BSTNode* root;
    size_t size;
} BST;

void bst_init(BST* tree) {
    tree->root = NULL;
    tree->size = 0;
}

BSTNode* bst_insert_node(BSTNode* node, int value) {
    if (!node) {
        BSTNode* new_node = malloc(sizeof(BSTNode));
        new_node->value = value;
        new_node->left = NULL;
        new_node->right = NULL;
        return new_node;
    }

    if (value < node->value) {
        node->left = bst_insert_node(node->left, value);
    } else {
        node->right = bst_insert_node(node->right, value);
    }
    return node;
}

void bst_insert(BST* tree, int value) {
    tree->root = bst_insert_node(tree->root, value);
    tree->size++;
}

bool bst_search_node(BSTNode* node, int value) {
    if (!node) return false;
    if (value == node->value) return true;
    if (value < node->value) {
        return bst_search_node(node->left, value);
    }
    return bst_search_node(node->right, value);
}

bool bst_search(BST* tree, int value) {
    return bst_search_node(tree->root, value);
}

void bst_inorder_node(BSTNode* node) {
    if (!node) return;
    bst_inorder_node(node->left);
    printf("%d ", node->value);
    bst_inorder_node(node->right);
}

void bst_inorder(BST* tree) {
    bst_inorder_node(tree->root);
}

void bst_free_node(BSTNode* node) {
    if (!node) return;
    bst_free_node(node->left);
    bst_free_node(node->right);
    free(node);
}

void bst_free(BST* tree) {
    bst_free_node(tree->root);
    tree->root = NULL;
    tree->size = 0;
}

int bst_min(BST* tree) {
    if (!tree->root) return -1;
    BSTNode* node = tree->root;
    while (node->left) {
        node = node->left;
    }
    return node->value;
}

int bst_max(BST* tree) {
    if (!tree->root) return -1;
    BSTNode* node = tree->root;
    while (node->right) {
        node = node->right;
    }
    return node->value;
}

void bst_demo(void) {
    printf("=== 二分探索木 ===\n");

    BST tree;
    bst_init(&tree);

    int values[] = {5, 3, 7, 1, 4, 6, 8};
    for (int i = 0; i < 7; i++) {
        bst_insert(&tree, values[i]);
    }

    printf("  inorder: ");
    bst_inorder(&tree);
    printf("\n");

    printf("  search(4) = %s\n", bst_search(&tree, 4) ? "true" : "false");
    printf("  search(9) = %s\n", bst_search(&tree, 9) ? "true" : "false");

    printf("  min = %d, max = %d\n", bst_min(&tree), bst_max(&tree));

    bst_free(&tree);
    printf("\n");
}

// === ヒープ (優先度キュー) ===

typedef struct {
    int* data;
    size_t size;
    size_t capacity;
} MinHeap;

void heap_init(MinHeap* h) {
    h->capacity = 16;
    h->data = malloc(h->capacity * sizeof(int));
    h->size = 0;
}

void heap_free(MinHeap* h) {
    free(h->data);
    h->data = NULL;
    h->size = 0;
}

void heap_swap(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void heap_bubble_up(MinHeap* h, size_t idx) {
    while (idx > 0) {
        size_t parent = (idx - 1) / 2;
        if (h->data[idx] >= h->data[parent]) break;
        heap_swap(&h->data[idx], &h->data[parent]);
        idx = parent;
    }
}

void heap_bubble_down(MinHeap* h, size_t idx) {
    while (true) {
        size_t smallest = idx;
        size_t left = 2 * idx + 1;
        size_t right = 2 * idx + 2;

        if (left < h->size && h->data[left] < h->data[smallest]) {
            smallest = left;
        }
        if (right < h->size && h->data[right] < h->data[smallest]) {
            smallest = right;
        }

        if (smallest == idx) break;
        heap_swap(&h->data[idx], &h->data[smallest]);
        idx = smallest;
    }
}

void heap_push(MinHeap* h, int value) {
    if (h->size >= h->capacity) {
        h->capacity *= 2;
        h->data = realloc(h->data, h->capacity * sizeof(int));
    }
    h->data[h->size] = value;
    heap_bubble_up(h, h->size);
    h->size++;
}

int heap_pop(MinHeap* h) {
    if (h->size == 0) return -1;
    int value = h->data[0];
    h->data[0] = h->data[--h->size];
    heap_bubble_down(h, 0);
    return value;
}

int heap_peek(MinHeap* h) {
    if (h->size == 0) return -1;
    return h->data[0];
}

void heap_demo(void) {
    printf("=== ヒープ (優先度キュー) ===\n");

    MinHeap h;
    heap_init(&h);

    int values[] = {3, 1, 4, 1, 5, 9, 2, 6};
    for (int i = 0; i < 8; i++) {
        heap_push(&h, values[i]);
    }

    printf("  min heap pop order: ");
    while (h.size > 0) {
        printf("%d ", heap_pop(&h));
    }
    printf("\n");

    heap_free(&h);
    printf("\n");
}

// === ビット配列 (ビットセット) ===

typedef struct {
    unsigned char* data;
    size_t size;       // ビット数
    size_t byte_size;
} BitSet;

void bitset_init(BitSet* bs, size_t size) {
    bs->size = size;
    bs->byte_size = (size + 7) / 8;
    bs->data = calloc(bs->byte_size, 1);
}

void bitset_free(BitSet* bs) {
    free(bs->data);
    bs->data = NULL;
}

void bitset_set(BitSet* bs, size_t index) {
    if (index >= bs->size) return;
    bs->data[index / 8] |= (1 << (index % 8));
}

void bitset_clear(BitSet* bs, size_t index) {
    if (index >= bs->size) return;
    bs->data[index / 8] &= ~(1 << (index % 8));
}

bool bitset_get(BitSet* bs, size_t index) {
    if (index >= bs->size) return false;
    return (bs->data[index / 8] & (1 << (index % 8))) != 0;
}

size_t bitset_count(BitSet* bs) {
    size_t count = 0;
    for (size_t i = 0; i < bs->size; i++) {
        if (bitset_get(bs, i)) count++;
    }
    return count;
}

void bitset_demo(void) {
    printf("=== ビットセット ===\n");

    BitSet bs;
    bitset_init(&bs, 16);

    bitset_set(&bs, 0);
    bitset_set(&bs, 3);
    bitset_set(&bs, 7);
    bitset_set(&bs, 15);

    printf("  set bits: ");
    for (size_t i = 0; i < bs.size; i++) {
        if (bitset_get(&bs, i)) {
            printf("%zu ", i);
        }
    }
    printf("\n");

    printf("  count = %zu\n", bitset_count(&bs));

    bitset_clear(&bs, 3);
    printf("  after clear(3): count = %zu\n", bitset_count(&bs));

    bitset_free(&bs);
    printf("\n");
}

int main(void) {
    printf("=== Data Structures Demo ===\n\n");

    vector_demo();
    singly_linked_list_demo();
    doubly_linked_list_demo();
    stack_demo();
    queue_demo();
    hashtable_demo();
    bst_demo();
    heap_demo();
    bitset_demo();

    return 0;
}
