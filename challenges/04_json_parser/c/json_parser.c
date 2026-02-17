// JSON Parser - C 実装
// 再帰下降パーサーによる JSON パース

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>

// === JSON 値の型 ===
typedef enum {
    JSON_NULL,
    JSON_BOOL,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
} JsonType;

// 前方宣言
typedef struct JsonValue JsonValue;
typedef struct JsonArray JsonArray;
typedef struct JsonObject JsonObject;

// === JSON 配列 ===
struct JsonArray {
    JsonValue **items;
    size_t size;
    size_t capacity;
};

// === JSON オブジェクト（キーバリューペア） ===
typedef struct {
    char *key;
    JsonValue *value;
} JsonPair;

struct JsonObject {
    JsonPair *pairs;
    size_t size;
    size_t capacity;
};

// === JSON 値 ===
struct JsonValue {
    JsonType type;
    union {
        bool boolean;
        double number;
        char *string;
        JsonArray *array;
        JsonObject *object;
    } data;
};

// === パーサー状態 ===
typedef struct {
    const char *input;
    size_t pos;
    size_t len;
    char error[256];
} Parser;

// === メモリ管理 ===

void json_free(JsonValue *value);

JsonValue* json_new(JsonType type) {
    JsonValue *v = malloc(sizeof(JsonValue));
    if (v == NULL) return NULL;
    v->type = type;
    memset(&v->data, 0, sizeof(v->data));
    return v;
}

JsonArray* array_new(void) {
    JsonArray *arr = malloc(sizeof(JsonArray));
    if (arr == NULL) return NULL;
    arr->items = NULL;
    arr->size = 0;
    arr->capacity = 0;
    return arr;
}

bool array_push(JsonArray *arr, JsonValue *value) {
    if (arr->size >= arr->capacity) {
        size_t new_cap = arr->capacity == 0 ? 4 : arr->capacity * 2;
        JsonValue **new_items = realloc(arr->items, new_cap * sizeof(JsonValue*));
        if (new_items == NULL) return false;
        arr->items = new_items;
        arr->capacity = new_cap;
    }
    arr->items[arr->size++] = value;
    return true;
}

void array_free(JsonArray *arr) {
    if (arr == NULL) return;
    for (size_t i = 0; i < arr->size; i++) {
        json_free(arr->items[i]);
    }
    free(arr->items);
    free(arr);
}

JsonObject* object_new(void) {
    JsonObject *obj = malloc(sizeof(JsonObject));
    if (obj == NULL) return NULL;
    obj->pairs = NULL;
    obj->size = 0;
    obj->capacity = 0;
    return obj;
}

bool object_set(JsonObject *obj, const char *key, JsonValue *value) {
    if (obj->size >= obj->capacity) {
        size_t new_cap = obj->capacity == 0 ? 4 : obj->capacity * 2;
        JsonPair *new_pairs = realloc(obj->pairs, new_cap * sizeof(JsonPair));
        if (new_pairs == NULL) return false;
        obj->pairs = new_pairs;
        obj->capacity = new_cap;
    }

    char *key_copy = strdup(key);
    if (key_copy == NULL) return false;

    obj->pairs[obj->size].key = key_copy;
    obj->pairs[obj->size].value = value;
    obj->size++;
    return true;
}

JsonValue* object_get(const JsonObject *obj, const char *key) {
    for (size_t i = 0; i < obj->size; i++) {
        if (strcmp(obj->pairs[i].key, key) == 0) {
            return obj->pairs[i].value;
        }
    }
    return NULL;
}

void object_free(JsonObject *obj) {
    if (obj == NULL) return;
    for (size_t i = 0; i < obj->size; i++) {
        free(obj->pairs[i].key);
        json_free(obj->pairs[i].value);
    }
    free(obj->pairs);
    free(obj);
}

void json_free(JsonValue *value) {
    if (value == NULL) return;

    switch (value->type) {
        case JSON_STRING:
            free(value->data.string);
            break;
        case JSON_ARRAY:
            array_free(value->data.array);
            break;
        case JSON_OBJECT:
            object_free(value->data.object);
            break;
        default:
            break;
    }
    free(value);
}

// === パーサーユーティリティ ===

void parser_init(Parser *p, const char *input) {
    p->input = input;
    p->pos = 0;
    p->len = strlen(input);
    p->error[0] = '\0';
}

void parser_error(Parser *p, const char *msg) {
    snprintf(p->error, sizeof(p->error), "Error at position %zu: %s", p->pos, msg);
}

char parser_peek(Parser *p) {
    if (p->pos >= p->len) return '\0';
    return p->input[p->pos];
}

char parser_advance(Parser *p) {
    if (p->pos >= p->len) return '\0';
    return p->input[p->pos++];
}

void parser_skip_whitespace(Parser *p) {
    while (p->pos < p->len && isspace((unsigned char)p->input[p->pos])) {
        p->pos++;
    }
}

bool parser_match(Parser *p, const char *str) {
    size_t len = strlen(str);
    if (p->pos + len > p->len) return false;
    if (strncmp(p->input + p->pos, str, len) == 0) {
        p->pos += len;
        return true;
    }
    return false;
}

// === パーサー ===

JsonValue* parse_value(Parser *p);

JsonValue* parse_null(Parser *p) {
    if (parser_match(p, "null")) {
        return json_new(JSON_NULL);
    }
    parser_error(p, "Expected 'null'");
    return NULL;
}

JsonValue* parse_bool(Parser *p) {
    JsonValue *v = json_new(JSON_BOOL);
    if (v == NULL) return NULL;

    if (parser_match(p, "true")) {
        v->data.boolean = true;
        return v;
    }
    if (parser_match(p, "false")) {
        v->data.boolean = false;
        return v;
    }

    free(v);
    parser_error(p, "Expected 'true' or 'false'");
    return NULL;
}

JsonValue* parse_number(Parser *p) {
    char buffer[64];
    size_t i = 0;

    // 負号
    if (parser_peek(p) == '-') {
        buffer[i++] = parser_advance(p);
    }

    // 整数部
    if (!isdigit((unsigned char)parser_peek(p))) {
        parser_error(p, "Expected digit");
        return NULL;
    }

    while (isdigit((unsigned char)parser_peek(p)) && i < sizeof(buffer) - 1) {
        buffer[i++] = parser_advance(p);
    }

    // 小数部
    if (parser_peek(p) == '.') {
        buffer[i++] = parser_advance(p);
        while (isdigit((unsigned char)parser_peek(p)) && i < sizeof(buffer) - 1) {
            buffer[i++] = parser_advance(p);
        }
    }

    // 指数部
    if (parser_peek(p) == 'e' || parser_peek(p) == 'E') {
        buffer[i++] = parser_advance(p);
        if (parser_peek(p) == '+' || parser_peek(p) == '-') {
            buffer[i++] = parser_advance(p);
        }
        while (isdigit((unsigned char)parser_peek(p)) && i < sizeof(buffer) - 1) {
            buffer[i++] = parser_advance(p);
        }
    }

    buffer[i] = '\0';

    JsonValue *v = json_new(JSON_NUMBER);
    if (v == NULL) return NULL;
    v->data.number = strtod(buffer, NULL);
    return v;
}

JsonValue* parse_string(Parser *p) {
    if (parser_advance(p) != '"') {
        parser_error(p, "Expected '\"'");
        return NULL;
    }

    char buffer[4096];
    size_t i = 0;

    while (p->pos < p->len && i < sizeof(buffer) - 1) {
        char c = parser_advance(p);

        if (c == '"') {
            buffer[i] = '\0';
            JsonValue *v = json_new(JSON_STRING);
            if (v == NULL) return NULL;
            v->data.string = strdup(buffer);
            if (v->data.string == NULL) {
                free(v);
                return NULL;
            }
            return v;
        }

        if (c == '\\') {
            c = parser_advance(p);
            switch (c) {
                case '"':  buffer[i++] = '"';  break;
                case '\\': buffer[i++] = '\\'; break;
                case '/':  buffer[i++] = '/';  break;
                case 'b':  buffer[i++] = '\b'; break;
                case 'f':  buffer[i++] = '\f'; break;
                case 'n':  buffer[i++] = '\n'; break;
                case 'r':  buffer[i++] = '\r'; break;
                case 't':  buffer[i++] = '\t'; break;
                default:
                    parser_error(p, "Invalid escape sequence");
                    return NULL;
            }
        } else {
            buffer[i++] = c;
        }
    }

    parser_error(p, "Unterminated string");
    return NULL;
}

JsonValue* parse_array(Parser *p) {
    if (parser_advance(p) != '[') {
        parser_error(p, "Expected '['");
        return NULL;
    }

    JsonValue *v = json_new(JSON_ARRAY);
    if (v == NULL) return NULL;

    v->data.array = array_new();
    if (v->data.array == NULL) {
        free(v);
        return NULL;
    }

    parser_skip_whitespace(p);

    // 空配列
    if (parser_peek(p) == ']') {
        parser_advance(p);
        return v;
    }

    // 要素をパース
    while (true) {
        JsonValue *item = parse_value(p);
        if (item == NULL) {
            json_free(v);
            return NULL;
        }

        if (!array_push(v->data.array, item)) {
            json_free(item);
            json_free(v);
            return NULL;
        }

        parser_skip_whitespace(p);

        if (parser_peek(p) == ']') {
            parser_advance(p);
            return v;
        }

        if (parser_advance(p) != ',') {
            parser_error(p, "Expected ',' or ']'");
            json_free(v);
            return NULL;
        }

        parser_skip_whitespace(p);
    }
}

JsonValue* parse_object(Parser *p) {
    if (parser_advance(p) != '{') {
        parser_error(p, "Expected '{'");
        return NULL;
    }

    JsonValue *v = json_new(JSON_OBJECT);
    if (v == NULL) return NULL;

    v->data.object = object_new();
    if (v->data.object == NULL) {
        free(v);
        return NULL;
    }

    parser_skip_whitespace(p);

    // 空オブジェクト
    if (parser_peek(p) == '}') {
        parser_advance(p);
        return v;
    }

    // キーバリューペアをパース
    while (true) {
        parser_skip_whitespace(p);

        // キー
        JsonValue *key_val = parse_string(p);
        if (key_val == NULL) {
            json_free(v);
            return NULL;
        }
        char *key = key_val->data.string;
        key_val->data.string = NULL;
        json_free(key_val);

        parser_skip_whitespace(p);

        // コロン
        if (parser_advance(p) != ':') {
            free(key);
            parser_error(p, "Expected ':'");
            json_free(v);
            return NULL;
        }

        // 値
        JsonValue *val = parse_value(p);
        if (val == NULL) {
            free(key);
            json_free(v);
            return NULL;
        }

        if (!object_set(v->data.object, key, val)) {
            free(key);
            json_free(val);
            json_free(v);
            return NULL;
        }
        free(key);

        parser_skip_whitespace(p);

        if (parser_peek(p) == '}') {
            parser_advance(p);
            return v;
        }

        if (parser_advance(p) != ',') {
            parser_error(p, "Expected ',' or '}'");
            json_free(v);
            return NULL;
        }
    }
}

JsonValue* parse_value(Parser *p) {
    parser_skip_whitespace(p);

    char c = parser_peek(p);

    if (c == 'n') return parse_null(p);
    if (c == 't' || c == 'f') return parse_bool(p);
    if (c == '"') return parse_string(p);
    if (c == '[') return parse_array(p);
    if (c == '{') return parse_object(p);
    if (c == '-' || isdigit((unsigned char)c)) return parse_number(p);

    parser_error(p, "Unexpected character");
    return NULL;
}

// === 公開 API ===

JsonValue* json_parse(const char *input) {
    Parser p;
    parser_init(&p, input);

    JsonValue *v = parse_value(&p);
    if (v == NULL) {
        fprintf(stderr, "%s\n", p.error);
    }
    return v;
}

// === 表示 ===

void json_print_indent(int depth) {
    for (int i = 0; i < depth * 2; i++) {
        putchar(' ');
    }
}

void json_print_helper(const JsonValue *v, int depth) {
    if (v == NULL) {
        printf("null");
        return;
    }

    switch (v->type) {
        case JSON_NULL:
            printf("null");
            break;

        case JSON_BOOL:
            printf("%s", v->data.boolean ? "true" : "false");
            break;

        case JSON_NUMBER:
            printf("%g", v->data.number);
            break;

        case JSON_STRING:
            printf("\"%s\"", v->data.string);
            break;

        case JSON_ARRAY:
            printf("[\n");
            for (size_t i = 0; i < v->data.array->size; i++) {
                json_print_indent(depth + 1);
                json_print_helper(v->data.array->items[i], depth + 1);
                if (i < v->data.array->size - 1) printf(",");
                printf("\n");
            }
            json_print_indent(depth);
            printf("]");
            break;

        case JSON_OBJECT:
            printf("{\n");
            for (size_t i = 0; i < v->data.object->size; i++) {
                json_print_indent(depth + 1);
                printf("\"%s\": ", v->data.object->pairs[i].key);
                json_print_helper(v->data.object->pairs[i].value, depth + 1);
                if (i < v->data.object->size - 1) printf(",");
                printf("\n");
            }
            json_print_indent(depth);
            printf("}");
            break;
    }
}

void json_print(const JsonValue *v) {
    json_print_helper(v, 0);
    printf("\n");
}

// === デモ ===

void demo(void) {
    printf("=== JSON Parser Demo ===\n\n");

    // 基本型
    printf("--- Basic Types ---\n");

    JsonValue *v1 = json_parse("null");
    printf("null: ");
    json_print(v1);
    json_free(v1);

    JsonValue *v2 = json_parse("true");
    printf("true: ");
    json_print(v2);
    json_free(v2);

    JsonValue *v3 = json_parse("42.5");
    printf("42.5: ");
    json_print(v3);
    json_free(v3);

    JsonValue *v4 = json_parse("\"hello\"");
    printf("\"hello\": ");
    json_print(v4);
    json_free(v4);

    printf("\n--- Array ---\n");

    JsonValue *v5 = json_parse("[1, 2, 3, \"four\", true, null]");
    printf("Array:\n");
    json_print(v5);
    json_free(v5);

    printf("\n--- Object ---\n");

    const char *json_obj =
        "{"
        "  \"name\": \"John\","
        "  \"age\": 30,"
        "  \"active\": true,"
        "  \"skills\": [\"C\", \"Python\", \"Rust\"],"
        "  \"address\": {"
        "    \"city\": \"Tokyo\","
        "    \"zip\": \"100-0001\""
        "  }"
        "}";

    JsonValue *v6 = json_parse(json_obj);
    printf("Object:\n");
    json_print(v6);

    // 値の取得
    printf("\nAccessing values:\n");
    JsonValue *name = object_get(v6->data.object, "name");
    if (name && name->type == JSON_STRING) {
        printf("  name: %s\n", name->data.string);
    }

    JsonValue *age = object_get(v6->data.object, "age");
    if (age && age->type == JSON_NUMBER) {
        printf("  age: %g\n", age->data.number);
    }

    JsonValue *skills = object_get(v6->data.object, "skills");
    if (skills && skills->type == JSON_ARRAY) {
        printf("  skills: [");
        for (size_t i = 0; i < skills->data.array->size; i++) {
            JsonValue *skill = skills->data.array->items[i];
            if (skill->type == JSON_STRING) {
                printf("%s", skill->data.string);
                if (i < skills->data.array->size - 1) printf(", ");
            }
        }
        printf("]\n");
    }

    json_free(v6);
    printf("\n");
}

// === テスト ===

void run_tests(void) {
    printf("--- Tests ---\n");

    // null
    JsonValue *v = json_parse("null");
    assert(v != NULL && v->type == JSON_NULL);
    json_free(v);

    // bool
    v = json_parse("true");
    assert(v != NULL && v->type == JSON_BOOL && v->data.boolean == true);
    json_free(v);

    v = json_parse("false");
    assert(v != NULL && v->type == JSON_BOOL && v->data.boolean == false);
    json_free(v);

    // number
    v = json_parse("42");
    assert(v != NULL && v->type == JSON_NUMBER && v->data.number == 42.0);
    json_free(v);

    v = json_parse("-3.14");
    assert(v != NULL && v->type == JSON_NUMBER && v->data.number == -3.14);
    json_free(v);

    v = json_parse("1e10");
    assert(v != NULL && v->type == JSON_NUMBER && v->data.number == 1e10);
    json_free(v);

    // string
    v = json_parse("\"hello\"");
    assert(v != NULL && v->type == JSON_STRING);
    assert(strcmp(v->data.string, "hello") == 0);
    json_free(v);

    v = json_parse("\"hello\\nworld\"");
    assert(v != NULL && v->type == JSON_STRING);
    assert(strcmp(v->data.string, "hello\nworld") == 0);
    json_free(v);

    // array
    v = json_parse("[]");
    assert(v != NULL && v->type == JSON_ARRAY && v->data.array->size == 0);
    json_free(v);

    v = json_parse("[1, 2, 3]");
    assert(v != NULL && v->type == JSON_ARRAY && v->data.array->size == 3);
    assert(v->data.array->items[0]->data.number == 1.0);
    assert(v->data.array->items[1]->data.number == 2.0);
    assert(v->data.array->items[2]->data.number == 3.0);
    json_free(v);

    // object
    v = json_parse("{}");
    assert(v != NULL && v->type == JSON_OBJECT && v->data.object->size == 0);
    json_free(v);

    v = json_parse("{\"key\": \"value\"}");
    assert(v != NULL && v->type == JSON_OBJECT && v->data.object->size == 1);
    JsonValue *val = object_get(v->data.object, "key");
    assert(val != NULL && val->type == JSON_STRING);
    assert(strcmp(val->data.string, "value") == 0);
    json_free(v);

    // nested
    v = json_parse("{\"arr\": [1, {\"nested\": true}]}");
    assert(v != NULL && v->type == JSON_OBJECT);
    JsonValue *arr = object_get(v->data.object, "arr");
    assert(arr != NULL && arr->type == JSON_ARRAY && arr->data.array->size == 2);
    json_free(v);

    printf("All tests passed!\n");
}

int main(void) {
    demo();
    run_tests();
    return 0;
}
