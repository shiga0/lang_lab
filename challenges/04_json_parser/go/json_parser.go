// JSON Parser - Go 実装 (再帰下降パーサー)
package main

import (
	"fmt"
	"strconv"
	"strings"
	"unicode"
)

func main() {
	fmt.Println("=== JSON Parser Demo ===\n")

	examples := []string{
		`null`,
		`true`,
		`42`,
		`3.14`,
		`"hello"`,
		`[1, 2, 3]`,
		`{"name": "Go", "version": 1.21}`,
		`{"nested": {"array": [1, true, null]}}`,
	}

	for _, json := range examples {
		fmt.Printf("Input:  %s\n", json)
		result, err := Parse(json)
		if err != nil {
			fmt.Printf("Error:  %s\n\n", err)
		} else {
			fmt.Printf("Parsed: %v\n\n", result)
		}
	}

	fmt.Println("--- Tests ---")
	runTests()
}

// --- JSON Value ---
type JSONValue interface{}

// --- Parser ---
type Parser struct {
	input    string
	position int
}

// --- ParseError ---
type ParseError struct {
	Message  string
	Position int
}

func (e *ParseError) Error() string {
	return fmt.Sprintf("parse error at position %d: %s", e.Position, e.Message)
}

// Parse parses a JSON string
func Parse(input string) (JSONValue, error) {
	p := &Parser{input: input, position: 0}
	p.skipWhitespace()

	value, err := p.parseValue()
	if err != nil {
		return nil, err
	}

	p.skipWhitespace()
	if p.position < len(p.input) {
		return nil, p.error("unexpected characters after JSON value")
	}

	return value, nil
}

func (p *Parser) error(message string) error {
	return &ParseError{Message: message, Position: p.position}
}

func (p *Parser) current() byte {
	if p.position >= len(p.input) {
		return 0
	}
	return p.input[p.position]
}

func (p *Parser) advance() byte {
	ch := p.current()
	p.position++
	return ch
}

func (p *Parser) skipWhitespace() {
	for p.position < len(p.input) && unicode.IsSpace(rune(p.input[p.position])) {
		p.position++
	}
}

func (p *Parser) parseValue() (JSONValue, error) {
	p.skipWhitespace()

	ch := p.current()
	switch {
	case ch == 'n':
		return p.parseNull()
	case ch == 't' || ch == 'f':
		return p.parseBool()
	case ch == '"':
		return p.parseString()
	case ch == '[':
		return p.parseArray()
	case ch == '{':
		return p.parseObject()
	case ch == '-' || (ch >= '0' && ch <= '9'):
		return p.parseNumber()
	default:
		return nil, p.error(fmt.Sprintf("unexpected character: %c", ch))
	}
}

func (p *Parser) parseNull() (JSONValue, error) {
	if err := p.expectKeyword("null"); err != nil {
		return nil, err
	}
	return nil, nil
}

func (p *Parser) parseBool() (JSONValue, error) {
	if p.current() == 't' {
		if err := p.expectKeyword("true"); err != nil {
			return nil, err
		}
		return true, nil
	}
	if err := p.expectKeyword("false"); err != nil {
		return nil, err
	}
	return false, nil
}

func (p *Parser) expectKeyword(keyword string) error {
	for _, ch := range keyword {
		if p.current() != byte(ch) {
			return p.error(fmt.Sprintf("expected '%c'", ch))
		}
		p.advance()
	}
	return nil
}

func (p *Parser) parseString() (JSONValue, error) {
	p.advance() // consume opening "

	var sb strings.Builder
	for {
		ch := p.advance()
		switch ch {
		case 0:
			return nil, p.error("unterminated string")
		case '"':
			return sb.String(), nil
		case '\\':
			escaped, err := p.parseEscape()
			if err != nil {
				return nil, err
			}
			sb.WriteRune(escaped)
		default:
			sb.WriteByte(ch)
		}
	}
}

func (p *Parser) parseEscape() (rune, error) {
	ch := p.advance()
	switch ch {
	case 'n':
		return '\n', nil
	case 't':
		return '\t', nil
	case 'r':
		return '\r', nil
	case '"':
		return '"', nil
	case '\\':
		return '\\', nil
	case '/':
		return '/', nil
	case 'u':
		// Unicode escape
		hex := make([]byte, 4)
		for i := 0; i < 4; i++ {
			hex[i] = p.advance()
		}
		code, err := strconv.ParseInt(string(hex), 16, 32)
		if err != nil {
			return 0, p.error("invalid unicode escape")
		}
		return rune(code), nil
	default:
		return 0, p.error(fmt.Sprintf("invalid escape: \\%c", ch))
	}
}

func (p *Parser) parseNumber() (JSONValue, error) {
	start := p.position

	// 負号
	if p.current() == '-' {
		p.advance()
	}

	// 整数部
	if p.current() == '0' {
		p.advance()
	} else if p.current() >= '1' && p.current() <= '9' {
		for p.current() >= '0' && p.current() <= '9' {
			p.advance()
		}
	} else {
		return nil, p.error("expected digit")
	}

	isFloat := false

	// 小数部
	if p.current() == '.' {
		isFloat = true
		p.advance()
		if p.current() < '0' || p.current() > '9' {
			return nil, p.error("expected digit after decimal point")
		}
		for p.current() >= '0' && p.current() <= '9' {
			p.advance()
		}
	}

	// 指数部
	if p.current() == 'e' || p.current() == 'E' {
		isFloat = true
		p.advance()
		if p.current() == '+' || p.current() == '-' {
			p.advance()
		}
		if p.current() < '0' || p.current() > '9' {
			return nil, p.error("expected digit in exponent")
		}
		for p.current() >= '0' && p.current() <= '9' {
			p.advance()
		}
	}

	numStr := p.input[start:p.position]
	if isFloat {
		return strconv.ParseFloat(numStr, 64)
	}
	return strconv.Atoi(numStr)
}

func (p *Parser) parseArray() (JSONValue, error) {
	p.advance() // consume [
	p.skipWhitespace()

	arr := []JSONValue{}

	// 空配列
	if p.current() == ']' {
		p.advance()
		return arr, nil
	}

	for {
		value, err := p.parseValue()
		if err != nil {
			return nil, err
		}
		arr = append(arr, value)

		p.skipWhitespace()
		switch p.current() {
		case ',':
			p.advance()
			p.skipWhitespace()
		case ']':
			p.advance()
			return arr, nil
		default:
			return nil, p.error("expected ',' or ']'")
		}
	}
}

func (p *Parser) parseObject() (JSONValue, error) {
	p.advance() // consume {
	p.skipWhitespace()

	obj := map[string]JSONValue{}

	// 空オブジェクト
	if p.current() == '}' {
		p.advance()
		return obj, nil
	}

	for {
		p.skipWhitespace()

		// キー
		if p.current() != '"' {
			return nil, p.error("expected string key")
		}
		keyValue, err := p.parseString()
		if err != nil {
			return nil, err
		}
		key := keyValue.(string)

		p.skipWhitespace()
		if p.current() != ':' {
			return nil, p.error("expected ':'")
		}
		p.advance()

		// 値
		value, err := p.parseValue()
		if err != nil {
			return nil, err
		}
		obj[key] = value

		p.skipWhitespace()
		switch p.current() {
		case ',':
			p.advance()
		case '}':
			p.advance()
			return obj, nil
		default:
			return nil, p.error("expected ',' or '}'")
		}
	}
}

// --- Tests ---
func runTests() {
	// null
	result, err := Parse("null")
	if err != nil || result != nil {
		panic("null failed")
	}

	// bool
	result, err = Parse("true")
	if err != nil || result != true {
		panic("true failed")
	}
	result, err = Parse("false")
	if err != nil || result != false {
		panic("false failed")
	}

	// number
	result, err = Parse("42")
	if err != nil || result != 42 {
		panic("integer failed")
	}
	result, err = Parse("3.14")
	if err != nil {
		panic("float failed")
	}

	// string
	result, err = Parse(`"hello"`)
	if err != nil || result != "hello" {
		panic("string failed")
	}
	result, err = Parse(`"hello\nworld"`)
	if err != nil || result != "hello\nworld" {
		panic("escape failed")
	}

	// array
	result, err = Parse("[]")
	arr, ok := result.([]JSONValue)
	if err != nil || !ok || len(arr) != 0 {
		panic("empty array failed")
	}
	result, err = Parse("[1, 2, 3]")
	arr, ok = result.([]JSONValue)
	if err != nil || !ok || len(arr) != 3 {
		panic("array failed")
	}

	// object
	result, err = Parse("{}")
	obj, ok := result.(map[string]JSONValue)
	if err != nil || !ok || len(obj) != 0 {
		panic("empty object failed")
	}
	result, err = Parse(`{"name": "Go"}`)
	obj, ok = result.(map[string]JSONValue)
	if err != nil || !ok || obj["name"] != "Go" {
		panic("object failed")
	}

	// nested
	result, err = Parse(`{"arr": [1, {"nested": true}]}`)
	if err != nil {
		panic("nested failed")
	}

	// whitespace
	result, err = Parse(`  { "key" : "value" }  `)
	if err != nil {
		panic("whitespace failed")
	}

	// errors
	_, err = Parse("")
	if err == nil {
		panic("empty input should fail")
	}
	_, err = Parse("{")
	if err == nil {
		panic("unclosed object should fail")
	}

	fmt.Println("All tests passed!")
}
