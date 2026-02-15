//! JSON Parser - Rust 実装
//!
//! 再帰下降パーサーでJSONをパース

use std::collections::HashMap;
use std::iter::Peekable;
use std::str::Chars;

fn main() {
    println!("=== JSON Parser Demo ===\n");

    let examples = [
        r#"null"#,
        r#"true"#,
        r#"42"#,
        r#"3.14"#,
        r#""hello""#,
        r#"[1, 2, 3]"#,
        r#"{"name": "Rust", "version": 1.0}"#,
        r#"{"nested": {"array": [1, true, null]}}"#,
    ];

    for json in examples {
        println!("Input:  {}", json);
        match parse(json) {
            Ok(value) => println!("Parsed: {:?}\n", value),
            Err(e) => println!("Error:  {}\n", e),
        }
    }
}

/// JSON の値を表す列挙型
#[derive(Debug, Clone, PartialEq)]
pub enum JsonValue {
    Null,
    Bool(bool),
    Number(f64),
    String(String),
    Array(Vec<JsonValue>),
    Object(HashMap<String, JsonValue>),
}

/// パースエラー
#[derive(Debug, Clone, PartialEq)]
pub struct ParseError {
    pub message: String,
    pub position: usize,
}

impl std::fmt::Display for ParseError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "Parse error at position {}: {}", self.position, self.message)
    }
}

impl std::error::Error for ParseError {}

/// JSON 文字列をパースする
pub fn parse(input: &str) -> Result<JsonValue, ParseError> {
    let mut parser = Parser::new(input);
    let value = parser.parse_value()?;
    parser.skip_whitespace();

    if parser.chars.peek().is_some() {
        return Err(parser.error("Unexpected characters after JSON value"));
    }

    Ok(value)
}

/// パーサー
struct Parser<'a> {
    chars: Peekable<Chars<'a>>,
    position: usize,
}

impl<'a> Parser<'a> {
    fn new(input: &'a str) -> Self {
        Parser {
            chars: input.chars().peekable(),
            position: 0,
        }
    }

    fn error(&self, message: &str) -> ParseError {
        ParseError {
            message: message.to_string(),
            position: self.position,
        }
    }

    fn next(&mut self) -> Option<char> {
        let c = self.chars.next();
        if c.is_some() {
            self.position += 1;
        }
        c
    }

    fn peek(&mut self) -> Option<&char> {
        self.chars.peek()
    }

    fn skip_whitespace(&mut self) {
        while let Some(&c) = self.peek() {
            if c.is_whitespace() {
                self.next();
            } else {
                break;
            }
        }
    }

    fn parse_value(&mut self) -> Result<JsonValue, ParseError> {
        self.skip_whitespace();

        match self.peek() {
            None => Err(self.error("Unexpected end of input")),
            Some(&c) => match c {
                'n' => self.parse_null(),
                't' | 'f' => self.parse_bool(),
                '"' => self.parse_string(),
                '[' => self.parse_array(),
                '{' => self.parse_object(),
                '-' | '0'..='9' => self.parse_number(),
                _ => Err(self.error(&format!("Unexpected character: {}", c))),
            },
        }
    }

    fn parse_null(&mut self) -> Result<JsonValue, ParseError> {
        self.expect_keyword("null")?;
        Ok(JsonValue::Null)
    }

    fn parse_bool(&mut self) -> Result<JsonValue, ParseError> {
        if self.peek() == Some(&'t') {
            self.expect_keyword("true")?;
            Ok(JsonValue::Bool(true))
        } else {
            self.expect_keyword("false")?;
            Ok(JsonValue::Bool(false))
        }
    }

    fn expect_keyword(&mut self, keyword: &str) -> Result<(), ParseError> {
        for expected in keyword.chars() {
            match self.next() {
                Some(c) if c == expected => continue,
                Some(c) => {
                    return Err(self.error(&format!(
                        "Expected '{}' but got '{}'",
                        expected, c
                    )))
                }
                None => return Err(self.error("Unexpected end of input")),
            }
        }
        Ok(())
    }

    fn parse_string(&mut self) -> Result<JsonValue, ParseError> {
        self.next(); // consume opening "

        let mut s = String::new();

        loop {
            match self.next() {
                None => return Err(self.error("Unterminated string")),
                Some('"') => break,
                Some('\\') => {
                    // エスケープシーケンス
                    match self.next() {
                        Some('n') => s.push('\n'),
                        Some('t') => s.push('\t'),
                        Some('r') => s.push('\r'),
                        Some('"') => s.push('"'),
                        Some('\\') => s.push('\\'),
                        Some('/') => s.push('/'),
                        Some('u') => {
                            // Unicode エスケープ (簡易版)
                            let mut hex = String::new();
                            for _ in 0..4 {
                                match self.next() {
                                    Some(c) if c.is_ascii_hexdigit() => hex.push(c),
                                    _ => return Err(self.error("Invalid unicode escape")),
                                }
                            }
                            let code = u32::from_str_radix(&hex, 16)
                                .map_err(|_| self.error("Invalid unicode escape"))?;
                            let c = char::from_u32(code)
                                .ok_or_else(|| self.error("Invalid unicode code point"))?;
                            s.push(c);
                        }
                        Some(c) => return Err(self.error(&format!("Invalid escape: \\{}", c))),
                        None => return Err(self.error("Unterminated string")),
                    }
                }
                Some(c) => s.push(c),
            }
        }

        Ok(JsonValue::String(s))
    }

    fn parse_number(&mut self) -> Result<JsonValue, ParseError> {
        let mut num_str = String::new();

        // 負号
        if self.peek() == Some(&'-') {
            num_str.push(self.next().unwrap());
        }

        // 整数部
        match self.peek() {
            Some(&'0') => {
                num_str.push(self.next().unwrap());
            }
            Some(&c) if c.is_ascii_digit() => {
                while let Some(&c) = self.peek() {
                    if c.is_ascii_digit() {
                        num_str.push(self.next().unwrap());
                    } else {
                        break;
                    }
                }
            }
            _ => return Err(self.error("Expected digit")),
        }

        // 小数部
        if self.peek() == Some(&'.') {
            num_str.push(self.next().unwrap());
            let mut has_digit = false;
            while let Some(&c) = self.peek() {
                if c.is_ascii_digit() {
                    num_str.push(self.next().unwrap());
                    has_digit = true;
                } else {
                    break;
                }
            }
            if !has_digit {
                return Err(self.error("Expected digit after decimal point"));
            }
        }

        // 指数部
        if let Some(&c) = self.peek() {
            if c == 'e' || c == 'E' {
                num_str.push(self.next().unwrap());
                if let Some(&c) = self.peek() {
                    if c == '+' || c == '-' {
                        num_str.push(self.next().unwrap());
                    }
                }
                let mut has_digit = false;
                while let Some(&c) = self.peek() {
                    if c.is_ascii_digit() {
                        num_str.push(self.next().unwrap());
                        has_digit = true;
                    } else {
                        break;
                    }
                }
                if !has_digit {
                    return Err(self.error("Expected digit in exponent"));
                }
            }
        }

        let n: f64 = num_str
            .parse()
            .map_err(|_| self.error("Invalid number"))?;

        Ok(JsonValue::Number(n))
    }

    fn parse_array(&mut self) -> Result<JsonValue, ParseError> {
        self.next(); // consume [
        self.skip_whitespace();

        let mut arr = Vec::new();

        // 空配列
        if self.peek() == Some(&']') {
            self.next();
            return Ok(JsonValue::Array(arr));
        }

        loop {
            arr.push(self.parse_value()?);
            self.skip_whitespace();

            match self.peek() {
                Some(&',') => {
                    self.next();
                    self.skip_whitespace();
                }
                Some(&']') => {
                    self.next();
                    break;
                }
                _ => return Err(self.error("Expected ',' or ']'")),
            }
        }

        Ok(JsonValue::Array(arr))
    }

    fn parse_object(&mut self) -> Result<JsonValue, ParseError> {
        self.next(); // consume {
        self.skip_whitespace();

        let mut obj = HashMap::new();

        // 空オブジェクト
        if self.peek() == Some(&'}') {
            self.next();
            return Ok(JsonValue::Object(obj));
        }

        loop {
            self.skip_whitespace();

            // キー
            if self.peek() != Some(&'"') {
                return Err(self.error("Expected string key"));
            }
            let key = match self.parse_string()? {
                JsonValue::String(s) => s,
                _ => unreachable!(),
            };

            self.skip_whitespace();

            // コロン
            if self.next() != Some(':') {
                return Err(self.error("Expected ':'"));
            }

            // 値
            let value = self.parse_value()?;
            obj.insert(key, value);

            self.skip_whitespace();

            match self.peek() {
                Some(&',') => {
                    self.next();
                }
                Some(&'}') => {
                    self.next();
                    break;
                }
                _ => return Err(self.error("Expected ',' or '}'")),
            }
        }

        Ok(JsonValue::Object(obj))
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_null() {
        assert_eq!(parse("null").unwrap(), JsonValue::Null);
    }

    #[test]
    fn test_bool() {
        assert_eq!(parse("true").unwrap(), JsonValue::Bool(true));
        assert_eq!(parse("false").unwrap(), JsonValue::Bool(false));
    }

    #[test]
    fn test_number() {
        assert_eq!(parse("42").unwrap(), JsonValue::Number(42.0));
        assert_eq!(parse("-17").unwrap(), JsonValue::Number(-17.0));
        assert_eq!(parse("3.14").unwrap(), JsonValue::Number(3.14));
        assert_eq!(parse("1e10").unwrap(), JsonValue::Number(1e10));
        assert_eq!(parse("2.5e-3").unwrap(), JsonValue::Number(2.5e-3));
    }

    #[test]
    fn test_string() {
        assert_eq!(
            parse(r#""hello""#).unwrap(),
            JsonValue::String("hello".to_string())
        );
        assert_eq!(
            parse(r#""hello\nworld""#).unwrap(),
            JsonValue::String("hello\nworld".to_string())
        );
        assert_eq!(
            parse(r#""tab\there""#).unwrap(),
            JsonValue::String("tab\there".to_string())
        );
    }

    #[test]
    fn test_array() {
        assert_eq!(parse("[]").unwrap(), JsonValue::Array(vec![]));
        assert_eq!(
            parse("[1, 2, 3]").unwrap(),
            JsonValue::Array(vec![
                JsonValue::Number(1.0),
                JsonValue::Number(2.0),
                JsonValue::Number(3.0),
            ])
        );
        assert_eq!(
            parse("[true, null]").unwrap(),
            JsonValue::Array(vec![JsonValue::Bool(true), JsonValue::Null])
        );
    }

    #[test]
    fn test_object() {
        assert_eq!(parse("{}").unwrap(), JsonValue::Object(HashMap::new()));

        let result = parse(r#"{"name": "Rust"}"#).unwrap();
        if let JsonValue::Object(obj) = result {
            assert_eq!(
                obj.get("name"),
                Some(&JsonValue::String("Rust".to_string()))
            );
        } else {
            panic!("Expected object");
        }
    }

    #[test]
    fn test_nested() {
        let json = r#"{"arr": [1, {"nested": true}]}"#;
        let result = parse(json).unwrap();

        if let JsonValue::Object(obj) = result {
            if let Some(JsonValue::Array(arr)) = obj.get("arr") {
                assert_eq!(arr.len(), 2);
                assert_eq!(arr[0], JsonValue::Number(1.0));
            } else {
                panic!("Expected array");
            }
        } else {
            panic!("Expected object");
        }
    }

    #[test]
    fn test_whitespace() {
        let json = r#"
            {
                "key"  :   "value"   ,
                "num"  :   42
            }
        "#;
        assert!(parse(json).is_ok());
    }

    #[test]
    fn test_errors() {
        assert!(parse("").is_err());
        assert!(parse("{").is_err());
        assert!(parse("[1,]").is_err());
        assert!(parse("undefined").is_err());
    }
}
