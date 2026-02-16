// オブジェクト指向 - Go 実装
//
// Go はクラスがないが、構造体とメソッドで OOP を実現
// インターフェースは暗黙的に実装される

package main

import (
	"fmt"
	"math"
)

func main() {
	fmt.Println("=== OOP Demo ===\n")

	structsAndMethods()
	interfaces()
	composition()
	embedding()
	polymorphism()
}

// --- 構造体とメソッド ---
func structsAndMethods() {
	fmt.Println("--- 構造体とメソッド ---")

	// 構造体定義
	type Person struct {
		Name string
		Age  int
	}

	// インスタンス化
	p1 := Person{Name: "Alice", Age: 30}
	p2 := Person{"Bob", 25}           // フィールド順
	p3 := &Person{Name: "Charlie"}    // ポインタ

	fmt.Printf("p1: %+v\n", p1)
	fmt.Printf("p2: %+v\n", p2)
	fmt.Printf("p3: %+v\n", *p3)

	// メソッド呼び出し
	user := User{Name: "Alice", Email: "alice@example.com"}
	fmt.Printf("\nUser: %s\n", user.Greet())

	// ポインタレシーバでの変更
	user.SetName("Alicia")
	fmt.Printf("After SetName: %s\n", user.Name)

	fmt.Println()
}

// User 構造体とメソッド
type User struct {
	Name  string
	Email string
}

// 値レシーバ (読み取り専用)
func (u User) Greet() string {
	return fmt.Sprintf("Hello, I'm %s (%s)", u.Name, u.Email)
}

// ポインタレシーバ (変更可能)
func (u *User) SetName(name string) {
	u.Name = name
}

// --- インターフェース ---
func interfaces() {
	fmt.Println("--- インターフェース ---")

	// インターフェース定義
	type Shape interface {
		Area() float64
		Perimeter() float64
	}

	// 構造体がインターフェースを満たす (暗黙的)
	shapes := []Shape{
		Rectangle{Width: 10, Height: 5},
		Circle{Radius: 7},
	}

	for _, s := range shapes {
		fmt.Printf("%T: Area=%.2f, Perimeter=%.2f\n",
			s, s.Area(), s.Perimeter())
	}

	// 型アサーション
	var s Shape = Circle{Radius: 5}
	if c, ok := s.(Circle); ok {
		fmt.Printf("Circle radius: %.2f\n", c.Radius)
	}

	fmt.Println()
}

// Rectangle
type Rectangle struct {
	Width, Height float64
}

func (r Rectangle) Area() float64 {
	return r.Width * r.Height
}

func (r Rectangle) Perimeter() float64 {
	return 2 * (r.Width + r.Height)
}

// Circle
type Circle struct {
	Radius float64
}

func (c Circle) Area() float64 {
	return math.Pi * c.Radius * c.Radius
}

func (c Circle) Perimeter() float64 {
	return 2 * math.Pi * c.Radius
}

// Engine for composition demo
type Engine struct {
	Horsepower int
}

func (e Engine) Start() string {
	return fmt.Sprintf("Engine started (%d HP)", e.Horsepower)
}

// Car with composition
type Car struct {
	Brand  string
	Engine Engine // コンポジション
}

// --- コンポジション (継承の代わり) ---
func composition() {
	fmt.Println("--- コンポジション ---")

	// Go には継承がない
	// 代わりにコンポジションを使う

	car := Car{
		Brand:  "Tesla",
		Engine: Engine{Horsepower: 500},
	}

	fmt.Printf("Car: %s\n", car.Brand)
	fmt.Printf("Engine: %s\n", car.Engine.Start())

	fmt.Println()
}

// --- 埋め込み (Embedding) ---
func embedding() {
	fmt.Println("--- 埋め込み (Embedding) ---")

	// 埋め込みで疑似継承
	admin := Admin{
		Person2: Person2{Name: "Admin", Age: 40},
		Level:   5,
	}

	// 埋め込まれたフィールドとメソッドに直接アクセス
	fmt.Printf("Name: %s\n", admin.Name) // admin.Person.Name と同じ
	fmt.Printf("Greet: %s\n", admin.Greet())
	fmt.Printf("AdminGreet: %s\n", admin.AdminGreet())

	// インターフェースの埋め込み
	var g Greeter = admin
	fmt.Printf("Interface: %s\n", g.Greet())

	fmt.Println()
}

type Person2 struct {
	Name string
	Age  int
}

func (p Person2) Greet() string {
	return fmt.Sprintf("Hello, I'm %s", p.Name)
}

type Admin struct {
	Person2     // 埋め込み
	Level   int
}

func (a Admin) AdminGreet() string {
	return fmt.Sprintf("Hello, I'm admin %s (level %d)", a.Name, a.Level)
}

type Greeter interface {
	Greet() string
}

// Animal interface for polymorphism demo
type Animal interface {
	Speak() string
}

type Dog struct{ Name string }
type Cat struct{ Name string }
type Robot struct{ Name string }

func (d Dog) Speak() string   { return "Woof!" }
func (c Cat) Speak() string   { return "Meow!" }
func (r Robot) Speak() string { return "Beep boop!" }

// --- ポリモーフィズム ---
func polymorphism() {
	fmt.Println("--- ポリモーフィズム ---")

	// インターフェースによるポリモーフィズム
	animals := []Animal{
		Dog{Name: "Buddy"},
		Cat{Name: "Whiskers"},
		Robot{Name: "R2D2"},
	}

	for _, a := range animals {
		fmt.Printf("%T: %s\n", a, a.Speak())
	}

	// 空インターフェース (any)
	printAny := func(v any) {
		fmt.Printf("any: %v (%T)\n", v, v)
	}

	printAny(42)
	printAny("hello")
	printAny(true)
}
