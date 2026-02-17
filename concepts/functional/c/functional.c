// 関数型プログラミング - C 実装
//
// C は関数型言語ではないが、関数ポインタを使って
// 高階関数やコールバックパターンを実現できる。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// === 関数ポインタの基本 ===

// 関数ポインタの型定義
typedef int (*IntFunc)(int);
typedef int (*BinaryIntFunc)(int, int);
typedef void (*VoidCallback)(void);
typedef int (*Predicate)(int);
typedef int (*Comparator)(const void*, const void*);

int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }

int double_it(int x) { return x * 2; }
int square(int x) { return x * x; }
int add_one(int x) { return x + 1; }

void function_pointer_demo(void) {
    printf("=== 関数ポインタの基本 ===\n");

    // 関数ポインタへの代入
    BinaryIntFunc operation = add;
    printf("  add(10, 5) = %d\n", operation(10, 5));

    operation = multiply;
    printf("  multiply(10, 5) = %d\n", operation(10, 5));

    // 関数ポインタの配列
    BinaryIntFunc ops[] = {add, subtract, multiply};
    const char* op_names[] = {"add", "subtract", "multiply"};

    for (int i = 0; i < 3; i++) {
        printf("  %s(10, 3) = %d\n", op_names[i], ops[i](10, 3));
    }

    printf("\n");
}

// === 高階関数 ===

// apply: 関数を値に適用
int apply(int x, IntFunc f) {
    return f(x);
}

// compose: 関数合成 f(g(x))
int compose(IntFunc f, IntFunc g, int x) {
    return f(g(x));
}

// map: 配列の各要素に関数を適用
void map_int(int* arr, int len, IntFunc f, int* result) {
    for (int i = 0; i < len; i++) {
        result[i] = f(arr[i]);
    }
}

// filter: 条件を満たす要素のみ抽出
int filter_int(int* arr, int len, Predicate pred, int* result) {
    int count = 0;
    for (int i = 0; i < len; i++) {
        if (pred(arr[i])) {
            result[count++] = arr[i];
        }
    }
    return count;
}

// reduce (fold): 配列を単一の値に畳み込む
int reduce_int(int* arr, int len, BinaryIntFunc f, int initial) {
    int acc = initial;
    for (int i = 0; i < len; i++) {
        acc = f(acc, arr[i]);
    }
    return acc;
}

// for_each: 各要素に対してアクションを実行
typedef void (*IntAction)(int);

void for_each_int(int* arr, int len, IntAction action) {
    for (int i = 0; i < len; i++) {
        action(arr[i]);
    }
}

int is_even(int x) { return x % 2 == 0; }
int is_positive(int x) { return x > 0; }

void print_int(int x) { printf("%d ", x); }

void higher_order_demo(void) {
    printf("=== 高階関数 ===\n");

    int numbers[] = {1, 2, 3, 4, 5};
    int len = 5;
    int result[5];

    // apply
    printf("  apply(5, double_it) = %d\n", apply(5, double_it));

    // compose
    printf("  compose(double_it, add_one, 5) = %d\n", compose(double_it, add_one, 5));

    // map
    map_int(numbers, len, double_it, result);
    printf("  map (*2): ");
    for_each_int(result, len, print_int);
    printf("\n");

    map_int(numbers, len, square, result);
    printf("  map (^2): ");
    for_each_int(result, len, print_int);
    printf("\n");

    // filter
    int filtered[5];
    int filtered_len = filter_int(numbers, len, is_even, filtered);
    printf("  filter (even): ");
    for_each_int(filtered, filtered_len, print_int);
    printf("\n");

    // reduce
    int sum = reduce_int(numbers, len, add, 0);
    printf("  reduce (sum): %d\n", sum);

    int product = reduce_int(numbers, len, multiply, 1);
    printf("  reduce (product): %d\n", product);

    printf("\n");
}

// === コールバック ===

typedef void (*EventCallback)(const char* event, void* data);

// イベントエミッタの模倣
typedef struct {
    EventCallback callbacks[10];
    int count;
} EventEmitter;

void emitter_init(EventEmitter* e) {
    e->count = 0;
}

void emitter_on(EventEmitter* e, EventCallback cb) {
    if (e->count < 10) {
        e->callbacks[e->count++] = cb;
    }
}

void emitter_emit(EventEmitter* e, const char* event, void* data) {
    for (int i = 0; i < e->count; i++) {
        e->callbacks[i](event, data);
    }
}

void on_event1(const char* event, void* data) {
    printf("  Listener 1: event=%s, data=%d\n", event, *(int*)data);
}

void on_event2(const char* event, void* data) {
    printf("  Listener 2: event=%s, data=%d\n", event, *(int*)data);
}

void callback_demo(void) {
    printf("=== コールバック ===\n");

    EventEmitter emitter;
    emitter_init(&emitter);

    emitter_on(&emitter, on_event1);
    emitter_on(&emitter, on_event2);

    int data = 42;
    emitter_emit(&emitter, "click", &data);

    printf("\n");
}

// === クロージャの模倣 (構造体 + 関数) ===

// カウンタークロージャ
typedef struct {
    int count;
} Counter;

typedef struct {
    Counter* state;
    int (*call)(Counter*);
} CounterClosure;

int counter_call(Counter* state) {
    return ++(state->count);
}

CounterClosure make_counter(Counter* state) {
    state->count = 0;
    CounterClosure closure = {state, counter_call};
    return closure;
}

// 加算器クロージャ
typedef struct {
    int addend;
} Adder;

typedef struct {
    Adder* state;
    int (*call)(Adder*, int);
} AdderClosure;

int adder_call(Adder* state, int x) {
    return x + state->addend;
}

AdderClosure make_adder(Adder* state, int n) {
    state->addend = n;
    AdderClosure closure = {state, adder_call};
    return closure;
}

// 乗算器クロージャ
typedef struct {
    int factor;
} Multiplier;

typedef struct {
    Multiplier* state;
    int (*call)(Multiplier*, int);
} MultiplierClosure;

int multiplier_call(Multiplier* state, int x) {
    return x * state->factor;
}

MultiplierClosure make_multiplier(Multiplier* state, int factor) {
    state->factor = factor;
    MultiplierClosure closure = {state, multiplier_call};
    return closure;
}

void closure_demo(void) {
    printf("=== クロージャの模倣 ===\n");

    // カウンター
    Counter counter_state;
    CounterClosure counter = make_counter(&counter_state);
    printf("  counter() = %d\n", counter.call(counter.state));
    printf("  counter() = %d\n", counter.call(counter.state));
    printf("  counter() = %d\n", counter.call(counter.state));

    // 独立したカウンター
    Counter counter2_state;
    CounterClosure counter2 = make_counter(&counter2_state);
    printf("  counter2() = %d\n", counter2.call(counter2.state));

    // 加算器
    Adder adder_state;
    AdderClosure add_five = make_adder(&adder_state, 5);
    printf("  add_five(10) = %d\n", add_five.call(add_five.state, 10));
    printf("  add_five(20) = %d\n", add_five.call(add_five.state, 20));

    // 乗算器
    Multiplier mult_state;
    MultiplierClosure triple = make_multiplier(&mult_state, 3);
    printf("  triple(7) = %d\n", triple.call(triple.state, 7));

    printf("\n");
}

// === 関数を返す関数 ===

IntFunc get_operation(char op) {
    switch (op) {
        case 'd': return double_it;
        case 's': return square;
        case '+': return add_one;
        default:  return NULL;
    }
}

void function_factory_demo(void) {
    printf("=== 関数を返す関数 ===\n");

    IntFunc op_double = get_operation('d');
    IntFunc op_square = get_operation('s');

    printf("  get_operation('d')(5) = %d\n", op_double(5));
    printf("  get_operation('s')(5) = %d\n", op_square(5));

    printf("\n");
}

// === qsort でのコンパレータ ===

int compare_int_asc(const void* a, const void* b) {
    return *(const int*)a - *(const int*)b;
}

int compare_int_desc(const void* a, const void* b) {
    return *(const int*)b - *(const int*)a;
}

typedef struct {
    char name[32];
    int age;
} Person;

int compare_person_by_age(const void* a, const void* b) {
    const Person* pa = (const Person*)a;
    const Person* pb = (const Person*)b;
    return pa->age - pb->age;
}

int compare_person_by_name(const void* a, const void* b) {
    const Person* pa = (const Person*)a;
    const Person* pb = (const Person*)b;
    return strcmp(pa->name, pb->name);
}

void qsort_demo(void) {
    printf("=== qsort コンパレータ ===\n");

    // int のソート
    int numbers[] = {3, 1, 4, 1, 5, 9, 2, 6};
    int len = 8;

    qsort(numbers, len, sizeof(int), compare_int_asc);
    printf("  sorted asc: ");
    for_each_int(numbers, len, print_int);
    printf("\n");

    qsort(numbers, len, sizeof(int), compare_int_desc);
    printf("  sorted desc: ");
    for_each_int(numbers, len, print_int);
    printf("\n");

    // 構造体のソート
    Person people[] = {
        {"Charlie", 30},
        {"Alice", 25},
        {"Bob", 35}
    };
    int plen = 3;

    qsort(people, plen, sizeof(Person), compare_person_by_age);
    printf("  sorted by age:\n");
    for (int i = 0; i < plen; i++) {
        printf("    %s: %d\n", people[i].name, people[i].age);
    }

    qsort(people, plen, sizeof(Person), compare_person_by_name);
    printf("  sorted by name:\n");
    for (int i = 0; i < plen; i++) {
        printf("    %s: %d\n", people[i].name, people[i].age);
    }

    printf("\n");
}

// === bsearch ===

void bsearch_demo(void) {
    printf("=== bsearch ===\n");

    int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int len = 10;

    int key = 5;
    int* found = bsearch(&key, numbers, len, sizeof(int), compare_int_asc);
    if (found) {
        printf("  Found %d at index %ld\n", key, found - numbers);
    }

    key = 11;
    found = bsearch(&key, numbers, len, sizeof(int), compare_int_asc);
    if (found) {
        printf("  Found %d\n", key);
    } else {
        printf("  %d not found\n", key);
    }

    printf("\n");
}

// === ジェネレータパターン ===

typedef struct {
    int current;
    int end;
} RangeGenerator;

typedef struct {
    int a;
    int b;
} FibGenerator;

int range_next(RangeGenerator* gen, int* value) {
    if (gen->current > gen->end) {
        return 0;  // 終了
    }
    *value = gen->current++;
    return 1;
}

int fib_next(FibGenerator* gen, int* value) {
    *value = gen->a;
    int tmp = gen->a + gen->b;
    gen->a = gen->b;
    gen->b = tmp;
    return 1;
}

void generator_demo(void) {
    printf("=== ジェネレータパターン ===\n");

    // Range generator
    RangeGenerator range = {1, 5};
    int value;
    printf("  range(1, 5): ");
    while (range_next(&range, &value)) {
        printf("%d ", value);
    }
    printf("\n");

    // Fibonacci generator
    FibGenerator fib = {0, 1};
    printf("  fibonacci: ");
    for (int i = 0; i < 10; i++) {
        fib_next(&fib, &value);
        printf("%d ", value);
    }
    printf("\n");

    printf("\n");
}

// === パイプライン (関数合成チェーン) ===

typedef int (*Pipeline[10])(int);

int run_pipeline(int x, IntFunc* funcs, int count) {
    for (int i = 0; i < count; i++) {
        x = funcs[i](x);
    }
    return x;
}

void pipeline_demo(void) {
    printf("=== パイプライン ===\n");

    IntFunc pipeline[] = {add_one, double_it, square};
    int result = run_pipeline(5, pipeline, 3);
    printf("  pipeline(add_one, double_it, square)(5) = %d\n", result);
    // (5 + 1) = 6 -> 6 * 2 = 12 -> 12 * 12 = 144

    IntFunc pipeline2[] = {square, add_one};
    result = run_pipeline(5, pipeline2, 2);
    printf("  pipeline(square, add_one)(5) = %d\n", result);
    // 5 * 5 = 25 -> 25 + 1 = 26

    printf("\n");
}

// === 部分適用の模倣 ===

typedef struct {
    BinaryIntFunc func;
    int first_arg;
} PartialApplication;

int apply_partial(PartialApplication* pa, int second_arg) {
    return pa->func(pa->first_arg, second_arg);
}

void partial_application_demo(void) {
    printf("=== 部分適用 ===\n");

    // add(10, ?) を部分適用
    PartialApplication add_ten = {add, 10};
    printf("  add_ten(5) = %d\n", apply_partial(&add_ten, 5));
    printf("  add_ten(20) = %d\n", apply_partial(&add_ten, 20));

    // multiply(3, ?) を部分適用
    PartialApplication triple = {multiply, 3};
    printf("  triple(7) = %d\n", apply_partial(&triple, 7));

    printf("\n");
}

int main(void) {
    printf("=== Functional Programming Demo ===\n\n");

    function_pointer_demo();
    higher_order_demo();
    callback_demo();
    closure_demo();
    function_factory_demo();
    qsort_demo();
    bsearch_demo();
    generator_demo();
    pipeline_demo();
    partial_application_demo();

    return 0;
}
