// JSON Parser - C++ 実装 (再帰下降パーサー)

#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>
#include <cctype>
#include <sstream>
#include <cassert>

using namespace std;

// === JSON値の型 ===
struct JsonValue;

using JsonNull = nullptr_t;
using JsonBool = bool;
using JsonNumber = double;
using JsonString = string;
using JsonArray = vector<JsonValue>;
using JsonObject = map<string, JsonValue>;

struct JsonValue : variant<JsonNull, JsonBool, JsonNumber, JsonString, JsonArray, JsonObject> {
    using variant::variant;

    bool is_null() const { return holds_alternative<JsonNull>(*this); }
    bool is_bool() const { return holds_alternative<JsonBool>(*this); }
    bool is_number() const { return holds_alternative<JsonNumber>(*this); }
    bool is_string() const { return holds_alternative<JsonString>(*this); }
    bool is_array() const { return holds_alternative<JsonArray>(*this); }
    bool is_object() const { return holds_alternative<JsonObject>(*this); }

    const JsonBool& as_bool() const { return get<JsonBool>(*this); }
    const JsonNumber& as_number() const { return get<JsonNumber>(*this); }
    const JsonString& as_string() const { return get<JsonString>(*this); }
    const JsonArray& as_array() const { return get<JsonArray>(*this); }
    const JsonObject& as_object() const { return get<JsonObject>(*this); }
};

// === ParseError ===
class ParseError : public runtime_error {
public:
    ParseError(const string& message, size_t position)
        : runtime_error("Parse error at position " + to_string(position) + ": " + message),
          position_(position) {}

    size_t position() const { return position_; }

private:
    size_t position_;
};

// === Tokenizer ===
class Tokenizer {
public:
    Tokenizer(const string& input) : input_(input), position_(0) {}

    size_t position() const { return position_; }

    char current() const {
        return position_ < input_.size() ? input_[position_] : '\0';
    }

    char peek(size_t offset = 0) const {
        size_t pos = position_ + offset;
        return pos < input_.size() ? input_[pos] : '\0';
    }

    char advance() {
        return position_ < input_.size() ? input_[position_++] : '\0';
    }

    void skip_whitespace() {
        while (isspace(current())) {
            advance();
        }
    }

    bool eof() const {
        return position_ >= input_.size();
    }

    void expect(char c) {
        if (current() != c) {
            throw ParseError(
                string("Expected '") + c + "' but got '" + current() + "'",
                position_
            );
        }
        advance();
    }

private:
    string input_;
    size_t position_;
};

// === Parser ===
class JsonParser {
public:
    JsonParser(const string& input) : tokenizer_(input) {}

    JsonValue parse() {
        tokenizer_.skip_whitespace();
        JsonValue value = parse_value();
        tokenizer_.skip_whitespace();

        if (!tokenizer_.eof()) {
            throw ParseError(
                "Unexpected characters after JSON value",
                tokenizer_.position()
            );
        }

        return value;
    }

private:
    JsonValue parse_value() {
        tokenizer_.skip_whitespace();
        char c = tokenizer_.current();

        if (c == '\0') {
            throw ParseError("Unexpected end of input", tokenizer_.position());
        } else if (c == 'n') {
            return parse_null();
        } else if (c == 't' || c == 'f') {
            return parse_bool();
        } else if (c == '"') {
            return parse_string();
        } else if (c == '[') {
            return parse_array();
        } else if (c == '{') {
            return parse_object();
        } else if (c == '-' || isdigit(c)) {
            return parse_number();
        } else {
            throw ParseError(
                string("Unexpected character: '") + c + "'",
                tokenizer_.position()
            );
        }
    }

    JsonNull parse_null() {
        expect_keyword("null");
        return nullptr;
    }

    JsonBool parse_bool() {
        if (tokenizer_.current() == 't') {
            expect_keyword("true");
            return true;
        } else {
            expect_keyword("false");
            return false;
        }
    }

    void expect_keyword(const string& keyword) {
        for (char expected : keyword) {
            char actual = tokenizer_.advance();
            if (actual != expected) {
                throw ParseError(
                    string("Expected '") + expected + "' but got '" + actual + "'",
                    tokenizer_.position()
                );
            }
        }
    }

    JsonString parse_string() {
        tokenizer_.advance();  // consume opening "
        string result;

        while (true) {
            char c = tokenizer_.advance();

            if (c == '\0') {
                throw ParseError("Unterminated string", tokenizer_.position());
            } else if (c == '"') {
                break;
            } else if (c == '\\') {
                result += parse_escape_sequence();
            } else {
                result += c;
            }
        }

        return result;
    }

    char parse_escape_sequence() {
        char c = tokenizer_.advance();

        switch (c) {
            case 'n': return '\n';
            case 't': return '\t';
            case 'r': return '\r';
            case '"': return '"';
            case '\\': return '\\';
            case '/': return '/';
            case 'u': {
                // Unicode escape
                string hex;
                for (int i = 0; i < 4; ++i) {
                    hex += tokenizer_.advance();
                }
                int codepoint = stoi(hex, nullptr, 16);
                // 簡易実装: ASCII 範囲のみ対応
                return static_cast<char>(codepoint);
            }
            default:
                throw ParseError(
                    string("Invalid escape sequence: \\") + c,
                    tokenizer_.position()
                );
        }
    }

    JsonNumber parse_number() {
        string num_str;

        // 負号
        if (tokenizer_.current() == '-') {
            num_str += tokenizer_.advance();
        }

        // 整数部
        if (tokenizer_.current() == '0') {
            num_str += tokenizer_.advance();
        } else if (isdigit(tokenizer_.current())) {
            while (isdigit(tokenizer_.current())) {
                num_str += tokenizer_.advance();
            }
        } else {
            throw ParseError("Expected digit", tokenizer_.position());
        }

        // 小数部
        if (tokenizer_.current() == '.') {
            num_str += tokenizer_.advance();
            if (!isdigit(tokenizer_.current())) {
                throw ParseError("Expected digit after decimal point", tokenizer_.position());
            }
            while (isdigit(tokenizer_.current())) {
                num_str += tokenizer_.advance();
            }
        }

        // 指数部
        if (tokenizer_.current() == 'e' || tokenizer_.current() == 'E') {
            num_str += tokenizer_.advance();
            if (tokenizer_.current() == '+' || tokenizer_.current() == '-') {
                num_str += tokenizer_.advance();
            }
            if (!isdigit(tokenizer_.current())) {
                throw ParseError("Expected digit in exponent", tokenizer_.position());
            }
            while (isdigit(tokenizer_.current())) {
                num_str += tokenizer_.advance();
            }
        }

        return stod(num_str);
    }

    JsonArray parse_array() {
        tokenizer_.advance();  // consume [
        tokenizer_.skip_whitespace();

        JsonArray arr;

        if (tokenizer_.current() == ']') {
            tokenizer_.advance();
            return arr;
        }

        while (true) {
            arr.push_back(parse_value());
            tokenizer_.skip_whitespace();

            char c = tokenizer_.current();
            if (c == ',') {
                tokenizer_.advance();
                tokenizer_.skip_whitespace();
            } else if (c == ']') {
                tokenizer_.advance();
                break;
            } else {
                throw ParseError("Expected ',' or ']'", tokenizer_.position());
            }
        }

        return arr;
    }

    JsonObject parse_object() {
        tokenizer_.advance();  // consume {
        tokenizer_.skip_whitespace();

        JsonObject obj;

        if (tokenizer_.current() == '}') {
            tokenizer_.advance();
            return obj;
        }

        while (true) {
            tokenizer_.skip_whitespace();

            if (tokenizer_.current() != '"') {
                throw ParseError("Expected string key", tokenizer_.position());
            }
            string key = parse_string();

            tokenizer_.skip_whitespace();
            tokenizer_.expect(':');

            JsonValue value = parse_value();
            obj[key] = value;

            tokenizer_.skip_whitespace();

            char c = tokenizer_.current();
            if (c == ',') {
                tokenizer_.advance();
            } else if (c == '}') {
                tokenizer_.advance();
                break;
            } else {
                throw ParseError("Expected ',' or '}'", tokenizer_.position());
            }
        }

        return obj;
    }

    Tokenizer tokenizer_;
};

// === 便利関数 ===
JsonValue parse_json(const string& input) {
    return JsonParser(input).parse();
}

// === JSON 文字列化 ===
string stringify(const JsonValue& value, int indent = 0);

string stringify_impl(const JsonValue& value, int indent, int current_indent) {
    string pad(current_indent, ' ');
    string inner_pad(current_indent + indent, ' ');

    if (value.is_null()) {
        return "null";
    } else if (value.is_bool()) {
        return value.as_bool() ? "true" : "false";
    } else if (value.is_number()) {
        ostringstream oss;
        oss << value.as_number();
        return oss.str();
    } else if (value.is_string()) {
        return "\"" + value.as_string() + "\"";
    } else if (value.is_array()) {
        const auto& arr = value.as_array();
        if (arr.empty()) return "[]";

        ostringstream oss;
        oss << "[";
        if (indent > 0) oss << "\n";
        for (size_t i = 0; i < arr.size(); ++i) {
            if (indent > 0) oss << inner_pad;
            oss << stringify_impl(arr[i], indent, current_indent + indent);
            if (i < arr.size() - 1) oss << ",";
            if (indent > 0) oss << "\n";
        }
        if (indent > 0) oss << pad;
        oss << "]";
        return oss.str();
    } else if (value.is_object()) {
        const auto& obj = value.as_object();
        if (obj.empty()) return "{}";

        ostringstream oss;
        oss << "{";
        if (indent > 0) oss << "\n";
        size_t i = 0;
        for (const auto& [key, val] : obj) {
            if (indent > 0) oss << inner_pad;
            oss << "\"" << key << "\": ";
            oss << stringify_impl(val, indent, current_indent + indent);
            if (i < obj.size() - 1) oss << ",";
            if (indent > 0) oss << "\n";
            ++i;
        }
        if (indent > 0) oss << pad;
        oss << "}";
        return oss.str();
    }

    return "";
}

string stringify(const JsonValue& value, int indent) {
    return stringify_impl(value, indent, 0);
}

// === デモ ===
void demo() {
    vector<string> examples = {
        "null",
        "true",
        "42",
        "3.14",
        "\"hello\"",
        "[1, 2, 3]",
        "{\"name\": \"C++\", \"version\": 20}",
        "{\"nested\": {\"array\": [1, true, null]}}"
    };

    for (const auto& json : examples) {
        cout << "Input:  " << json << endl;
        try {
            JsonValue result = parse_json(json);
            cout << "Parsed: " << stringify(result) << endl;
        } catch (const ParseError& e) {
            cout << "Error:  " << e.what() << endl;
        }
        cout << endl;
    }
}

// === テスト ===
void run_tests() {
    cout << "--- Tests ---" << endl;

    // null
    assert(parse_json("null").is_null());

    // bool
    assert(parse_json("true").as_bool() == true);
    assert(parse_json("false").as_bool() == false);

    // number
    assert(parse_json("42").as_number() == 42);
    assert(parse_json("-17").as_number() == -17);
    assert(parse_json("3.14").as_number() == 3.14);
    assert(parse_json("1e10").as_number() == 1e10);

    // string
    assert(parse_json("\"hello\"").as_string() == "hello");
    assert(parse_json("\"hello\\nworld\"").as_string() == "hello\nworld");

    // array
    assert(parse_json("[]").as_array().empty());
    assert(parse_json("[1, 2, 3]").as_array().size() == 3);

    // object
    assert(parse_json("{}").as_object().empty());
    auto obj = parse_json("{\"name\": \"C++\"}").as_object();
    assert(obj["name"].as_string() == "C++");

    // nested
    auto nested = parse_json("{\"arr\": [1, {\"nested\": true}]}");
    assert(nested.as_object().at("arr").as_array()[1].as_object().at("nested").as_bool() == true);

    // whitespace
    auto ws = parse_json("  { \"key\" : \"value\" }  ");
    assert(ws.as_object().at("key").as_string() == "value");

    // errors
    try {
        parse_json("");
        assert(false && "empty input should fail");
    } catch (const ParseError&) {}

    try {
        parse_json("{");
        assert(false && "unclosed object should fail");
    } catch (const ParseError&) {}

    cout << "All tests passed!" << endl;
}

int main() {
    cout << "=== JSON Parser Demo ===" << endl << endl;

    demo();
    run_tests();

    return 0;
}
