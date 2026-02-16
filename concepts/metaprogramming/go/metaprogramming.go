// メタプログラミング - Go 実装
//
// Go のメタプログラミングは限定的
// reflect パッケージと go generate を使用

package main

import (
	"fmt"
	"reflect"
)

func main() {
	fmt.Println("=== Metaprogramming Demo ===\n")

	reflectBasics()
	reflectStructs()
	reflectModify()
	reflectMethods()
	tags()
	typeSwitch()
}

// --- reflect 基礎 ---
func reflectBasics() {
	fmt.Println("--- reflect 基礎 ---")

	// 型情報の取得
	var x int = 42
	var s string = "hello"
	var f float64 = 3.14

	fmt.Printf("x: Type=%v, Kind=%v, Value=%v\n",
		reflect.TypeOf(x), reflect.TypeOf(x).Kind(), reflect.ValueOf(x))
	fmt.Printf("s: Type=%v, Kind=%v, Value=%v\n",
		reflect.TypeOf(s), reflect.TypeOf(s).Kind(), reflect.ValueOf(s))
	fmt.Printf("f: Type=%v, Kind=%v, Value=%v\n",
		reflect.TypeOf(f), reflect.TypeOf(f).Kind(), reflect.ValueOf(f))

	// スライスの型情報
	slice := []int{1, 2, 3}
	t := reflect.TypeOf(slice)
	fmt.Printf("slice: Type=%v, Kind=%v, Elem=%v\n", t, t.Kind(), t.Elem())

	// マップの型情報
	m := map[string]int{}
	t = reflect.TypeOf(m)
	fmt.Printf("map: Type=%v, Key=%v, Elem=%v\n", t, t.Key(), t.Elem())

	fmt.Println()
}

// --- 構造体のリフレクション ---
func reflectStructs() {
	fmt.Println("--- 構造体のリフレクション ---")

	type Person struct {
		Name    string `json:"name" validate:"required"`
		Age     int    `json:"age" validate:"min=0"`
		Email   string `json:"email,omitempty"`
		private string // unexported
	}

	p := Person{Name: "Alice", Age: 30, Email: "alice@example.com"}
	t := reflect.TypeOf(p)
	v := reflect.ValueOf(p)

	fmt.Printf("Type: %v\n", t)
	fmt.Printf("NumField: %d\n", t.NumField())

	// フィールドの列挙
	for i := 0; i < t.NumField(); i++ {
		field := t.Field(i)
		value := v.Field(i)

		fmt.Printf("  %s: type=%v, value=%v, exported=%t\n",
			field.Name, field.Type, value, field.IsExported())

		// タグの取得
		if tag := field.Tag.Get("json"); tag != "" {
			fmt.Printf("    json tag: %s\n", tag)
		}
	}

	fmt.Println()
}

// --- 値の変更 ---
func reflectModify() {
	fmt.Println("--- 値の変更 ---")

	// ポインタ経由で変更
	x := 42
	v := reflect.ValueOf(&x).Elem() // Elem() でポインタ先を取得
	fmt.Printf("Before: %d\n", x)

	if v.CanSet() {
		v.SetInt(100)
	}
	fmt.Printf("After: %d\n", x)

	// 構造体のフィールドを変更
	type Config struct {
		Host string
		Port int
	}
	cfg := &Config{Host: "localhost", Port: 8080}
	v = reflect.ValueOf(cfg).Elem()

	hostField := v.FieldByName("Host")
	if hostField.CanSet() {
		hostField.SetString("0.0.0.0")
	}

	portField := v.FieldByName("Port")
	if portField.CanSet() {
		portField.SetInt(9000)
	}

	fmt.Printf("Config: %+v\n", cfg)

	fmt.Println()
}

// --- メソッドのリフレクション ---
func reflectMethods() {
	fmt.Println("--- メソッドのリフレクション ---")

	type Calculator struct {
		Value int
	}

	// メソッドを定義
	calc := Calculator{Value: 10}
	t := reflect.TypeOf(calc)

	fmt.Printf("Type: %v\n", t)
	fmt.Printf("NumMethod: %d\n", t.NumMethod())

	// メソッドの動的呼び出し
	v := reflect.ValueOf(&calc)
	method := v.MethodByName("Add")
	if method.IsValid() {
		args := []reflect.Value{reflect.ValueOf(5)}
		results := method.Call(args)
		fmt.Printf("Add(5) = %v\n", results[0].Int())
	}

	method = v.MethodByName("Multiply")
	if method.IsValid() {
		args := []reflect.Value{reflect.ValueOf(3)}
		results := method.Call(args)
		fmt.Printf("Multiply(3) = %v\n", results[0].Int())
	}

	fmt.Println()
}

// Calculator のメソッド
type Calculator struct {
	Value int
}

func (c *Calculator) Add(n int) int {
	c.Value += n
	return c.Value
}

func (c *Calculator) Multiply(n int) int {
	c.Value *= n
	return c.Value
}

// --- 構造体タグ ---
func tags() {
	fmt.Println("--- 構造体タグ ---")

	type User struct {
		ID        int    `db:"id" json:"id"`
		Name      string `db:"name" json:"name" validate:"required"`
		Email     string `db:"email" json:"email,omitempty" validate:"email"`
		CreatedAt string `db:"created_at" json:"-"` // json では無視
	}

	// タグパーサー
	t := reflect.TypeOf(User{})
	for i := 0; i < t.NumField(); i++ {
		field := t.Field(i)
		fmt.Printf("Field: %s\n", field.Name)
		fmt.Printf("  db:       %s\n", field.Tag.Get("db"))
		fmt.Printf("  json:     %s\n", field.Tag.Get("json"))
		fmt.Printf("  validate: %s\n", field.Tag.Get("validate"))
	}

	fmt.Println()
}

// --- 型スイッチ ---
func typeSwitch() {
	fmt.Println("--- 型スイッチ ---")

	printType := func(v interface{}) {
		switch x := v.(type) {
		case int:
			fmt.Printf("int: %d\n", x)
		case string:
			fmt.Printf("string: %s\n", x)
		case bool:
			fmt.Printf("bool: %t\n", x)
		case []int:
			fmt.Printf("[]int: %v\n", x)
		case map[string]int:
			fmt.Printf("map[string]int: %v\n", x)
		default:
			fmt.Printf("unknown: %T\n", x)
		}
	}

	printType(42)
	printType("hello")
	printType(true)
	printType([]int{1, 2, 3})
	printType(map[string]int{"a": 1})
	printType(3.14)

	fmt.Println()

	// go generate の説明
	fmt.Println("--- go generate ---")
	fmt.Println("Go のコード生成:")
	fmt.Println("  //go:generate stringer -type=Status")
	fmt.Println("  //go:generate mockgen -source=interface.go -destination=mock.go")
	fmt.Println("")
	fmt.Println("実行: go generate ./...")
}
