// オブジェクト指向 - C 実装
//
// C にはクラスがないが、構造体と関数ポインタで
// OOP のパターンを実現できる。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

// === カプセル化 ===
// ヘッダファイルでは不完全型として宣言し、実装を隠蔽

// Counter "クラス"
typedef struct Counter Counter;

// コンストラクタ
Counter* counter_new(int initial);
// デストラクタ
void counter_free(Counter *c);
// メソッド
void counter_increment(Counter *c);
void counter_decrement(Counter *c);
int counter_get(const Counter *c);
void counter_reset(Counter *c);

// 実装（通常は .c ファイルに隠蔽）
struct Counter {
    int value;
};

Counter* counter_new(int initial) {
    Counter *c = malloc(sizeof(Counter));
    if (c != NULL) {
        c->value = initial;
    }
    return c;
}

void counter_free(Counter *c) {
    free(c);
}

void counter_increment(Counter *c) {
    c->value++;
}

void counter_decrement(Counter *c) {
    c->value--;
}

int counter_get(const Counter *c) {
    return c->value;
}

void counter_reset(Counter *c) {
    c->value = 0;
}

void encapsulation_demo(void) {
    printf("--- カプセル化 ---\n");

    Counter *c = counter_new(0);

    counter_increment(c);
    counter_increment(c);
    printf("After 2 increments: %d\n", counter_get(c));

    counter_decrement(c);
    printf("After 1 decrement: %d\n", counter_get(c));

    counter_reset(c);
    printf("After reset: %d\n", counter_get(c));

    counter_free(c);

    printf("\n");
}

// === ポリモーフィズム（関数ポインタによる仮想関数）===

// Shape "インターフェース"
typedef struct Shape Shape;

struct Shape {
    // 仮想関数テーブル
    double (*area)(const Shape *self);
    double (*perimeter)(const Shape *self);
    void (*draw)(const Shape *self);
    void (*destroy)(Shape *self);
};

// Circle "クラス"
typedef struct {
    Shape base;  // 継承（先頭に配置）
    double radius;
} Circle;

double circle_area(const Shape *self) {
    const Circle *c = (const Circle*)self;
    return M_PI * c->radius * c->radius;
}

double circle_perimeter(const Shape *self) {
    const Circle *c = (const Circle*)self;
    return 2 * M_PI * c->radius;
}

void circle_draw(const Shape *self) {
    const Circle *c = (const Circle*)self;
    printf("Circle(radius=%.2f)\n", c->radius);
}

void circle_destroy(Shape *self) {
    free(self);
}

Shape* circle_new(double radius) {
    Circle *c = malloc(sizeof(Circle));
    if (c != NULL) {
        c->base.area = circle_area;
        c->base.perimeter = circle_perimeter;
        c->base.draw = circle_draw;
        c->base.destroy = circle_destroy;
        c->radius = radius;
    }
    return (Shape*)c;
}

// Rectangle "クラス"
typedef struct {
    Shape base;
    double width;
    double height;
} Rectangle;

double rectangle_area(const Shape *self) {
    const Rectangle *r = (const Rectangle*)self;
    return r->width * r->height;
}

double rectangle_perimeter(const Shape *self) {
    const Rectangle *r = (const Rectangle*)self;
    return 2 * (r->width + r->height);
}

void rectangle_draw(const Shape *self) {
    const Rectangle *r = (const Rectangle*)self;
    printf("Rectangle(%.2f x %.2f)\n", r->width, r->height);
}

void rectangle_destroy(Shape *self) {
    free(self);
}

Shape* rectangle_new(double width, double height) {
    Rectangle *r = malloc(sizeof(Rectangle));
    if (r != NULL) {
        r->base.area = rectangle_area;
        r->base.perimeter = rectangle_perimeter;
        r->base.draw = rectangle_draw;
        r->base.destroy = rectangle_destroy;
        r->width = width;
        r->height = height;
    }
    return (Shape*)r;
}

// 共通インターフェースで操作
void print_shape_info(const Shape *s) {
    s->draw(s);
    printf("  Area: %.2f\n", s->area(s));
    printf("  Perimeter: %.2f\n", s->perimeter(s));
}

void polymorphism_demo(void) {
    printf("--- ポリモーフィズム ---\n");

    Shape *shapes[] = {
        circle_new(5.0),
        rectangle_new(4.0, 6.0),
        circle_new(3.0)
    };
    int count = sizeof(shapes) / sizeof(shapes[0]);

    for (int i = 0; i < count; i++) {
        print_shape_info(shapes[i]);
        printf("\n");
    }

    // クリーンアップ
    for (int i = 0; i < count; i++) {
        shapes[i]->destroy(shapes[i]);
    }
}

// === 継承（構造体の埋め込み）===

// 基底クラス
typedef struct {
    char *name;
    int age;
} Animal;

void animal_init(Animal *a, const char *name, int age) {
    a->name = strdup(name);
    a->age = age;
}

void animal_cleanup(Animal *a) {
    free(a->name);
}

void animal_info(const Animal *a) {
    printf("Name: %s, Age: %d\n", a->name, a->age);
}

// 派生クラス
typedef struct {
    Animal base;  // 基底クラスを埋め込み
    char *breed;
} Dog;

Dog* dog_new(const char *name, int age, const char *breed) {
    Dog *d = malloc(sizeof(Dog));
    if (d != NULL) {
        animal_init(&d->base, name, age);
        d->breed = strdup(breed);
    }
    return d;
}

void dog_free(Dog *d) {
    if (d != NULL) {
        animal_cleanup(&d->base);
        free(d->breed);
        free(d);
    }
}

void dog_bark(const Dog *d) {
    printf("%s says: Woof!\n", d->base.name);
}

void dog_info(const Dog *d) {
    animal_info(&d->base);  // 基底クラスのメソッドを呼び出し
    printf("Breed: %s\n", d->breed);
}

void inheritance_demo(void) {
    printf("--- 継承（構造体の埋め込み）---\n");

    Dog *buddy = dog_new("Buddy", 3, "Golden Retriever");

    dog_info(buddy);
    dog_bark(buddy);

    // Animal* として扱える
    Animal *animal = &buddy->base;
    animal_info(animal);

    dog_free(buddy);

    printf("\n");
}

// === 抽象クラス / インターフェース ===

// Comparable インターフェース
typedef struct Comparable Comparable;

struct Comparable {
    int (*compare)(const Comparable *self, const Comparable *other);
};

// Integer "クラス"（Comparable を実装）
typedef struct {
    Comparable base;
    int value;
} Integer;

int integer_compare(const Comparable *self, const Comparable *other) {
    const Integer *a = (const Integer*)self;
    const Integer *b = (const Integer*)other;
    return a->value - b->value;
}

Integer integer_create(int value) {
    return (Integer){
        .base.compare = integer_compare,
        .value = value
    };
}

// 汎用ソート関数
void sort_comparables(Comparable **arr, size_t n) {
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            if (arr[j]->compare(arr[j], arr[j + 1]) > 0) {
                Comparable *tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

void interface_demo(void) {
    printf("--- インターフェース ---\n");

    Integer nums[] = {
        integer_create(5),
        integer_create(2),
        integer_create(8),
        integer_create(1)
    };

    Comparable *arr[] = {
        &nums[0].base,
        &nums[1].base,
        &nums[2].base,
        &nums[3].base
    };

    printf("Before sort: ");
    for (int i = 0; i < 4; i++) {
        printf("%d ", ((Integer*)arr[i])->value);
    }
    printf("\n");

    sort_comparables(arr, 4);

    printf("After sort:  ");
    for (int i = 0; i < 4; i++) {
        printf("%d ", ((Integer*)arr[i])->value);
    }
    printf("\n\n");
}

// === シングルトンパターン ===
typedef struct {
    int value;
    char name[50];
} Singleton;

Singleton* singleton_instance(void) {
    static Singleton instance = {0, "Singleton"};
    static bool initialized = false;

    if (!initialized) {
        instance.value = 42;
        initialized = true;
    }

    return &instance;
}

void singleton_demo(void) {
    printf("--- シングルトンパターン ---\n");

    Singleton *s1 = singleton_instance();
    Singleton *s2 = singleton_instance();

    printf("s1 address: %p, value: %d\n", (void*)s1, s1->value);
    printf("s2 address: %p, value: %d\n", (void*)s2, s2->value);
    printf("Same instance: %s\n", s1 == s2 ? "yes" : "no");

    s1->value = 100;
    printf("After s1->value = 100, s2->value = %d\n", s2->value);

    printf("\n");
}

// === ファクトリーパターン ===
typedef enum { CIRCLE, RECTANGLE, TRIANGLE } ShapeType;

Shape* shape_factory(ShapeType type) {
    switch (type) {
        case CIRCLE:
            return circle_new(1.0);
        case RECTANGLE:
            return rectangle_new(1.0, 1.0);
        default:
            return NULL;
    }
}

void factory_demo(void) {
    printf("--- ファクトリーパターン ---\n");

    Shape *s1 = shape_factory(CIRCLE);
    Shape *s2 = shape_factory(RECTANGLE);

    if (s1) {
        s1->draw(s1);
        s1->destroy(s1);
    }

    if (s2) {
        s2->draw(s2);
        s2->destroy(s2);
    }

    printf("\n");
}

// === メイン ===
int main(void) {
    printf("=== Object-Oriented Programming Demo ===\n\n");

    encapsulation_demo();
    polymorphism_demo();
    inheritance_demo();
    interface_demo();
    singleton_demo();
    factory_demo();

    return 0;
}
