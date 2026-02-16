// エラー処理 - Go 実装
//
// Go は例外を使わず、多値返却でエラーを返す
// "Errors are values"

package main

import (
	"errors"
	"fmt"
	"io"
	"os"
)

func main() {
	fmt.Println("=== Error Handling Demo ===\n")

	basicErrorHandling()
	customErrors()
	errorWrapping()
	panicAndRecover()
	deferUsage()
}

// --- 基本的なエラー処理 ---
func basicErrorHandling() {
	fmt.Println("--- 基本的なエラー処理 ---")

	// エラーを返す関数
	result, err := divide(10, 2)
	if err != nil {
		fmt.Printf("Error: %v\n", err)
	} else {
		fmt.Printf("10 / 2 = %d\n", result)
	}

	// エラーが発生する場合
	result, err = divide(10, 0)
	if err != nil {
		fmt.Printf("Error: %v\n", err)
	} else {
		fmt.Printf("10 / 0 = %d\n", result)
	}

	// ファイル操作の例
	file, err := os.Open("nonexistent.txt")
	if err != nil {
		fmt.Printf("File error: %v\n", err)
	} else {
		defer file.Close()
	}

	fmt.Println()
}

func divide(a, b int) (int, error) {
	if b == 0 {
		return 0, errors.New("division by zero")
	}
	return a / b, nil
}

// --- カスタムエラー ---
func customErrors() {
	fmt.Println("--- カスタムエラー ---")

	// error インターフェースを実装
	// type error interface {
	//     Error() string
	// }

	err := &ValidationError{
		Field:   "email",
		Message: "invalid format",
	}
	fmt.Printf("Validation error: %v\n", err)
	fmt.Printf("  Field: %s\n", err.Field)

	// センチネルエラー
	_, err2 := findUser(0)
	if errors.Is(err2, ErrNotFound) {
		fmt.Println("User not found (sentinel error)")
	}

	fmt.Println()
}

// カスタムエラー型
type ValidationError struct {
	Field   string
	Message string
}

func (e *ValidationError) Error() string {
	return fmt.Sprintf("validation error on %s: %s", e.Field, e.Message)
}

// センチネルエラー (定義済みエラー)
var ErrNotFound = errors.New("not found")
var ErrPermissionDenied = errors.New("permission denied")

func findUser(id int) (string, error) {
	if id == 0 {
		return "", ErrNotFound
	}
	return "user", nil
}

// --- エラーのラップ ---
func errorWrapping() {
	fmt.Println("--- エラーのラップ (Go 1.13+) ---")

	// %w でエラーをラップ
	err := loadConfig()
	if err != nil {
		fmt.Printf("Config error: %v\n", err)

		// errors.Is でアンラップしてチェック
		if errors.Is(err, os.ErrNotExist) {
			fmt.Println("  -> File does not exist")
		}

		// errors.Unwrap でアンラップ
		unwrapped := errors.Unwrap(err)
		fmt.Printf("  -> Unwrapped: %v\n", unwrapped)
	}

	// errors.As で型アサーション
	err2 := validateInput("")
	var validErr *ValidationError
	if errors.As(err2, &validErr) {
		fmt.Printf("Validation failed on field: %s\n", validErr.Field)
	}

	fmt.Println()
}

func loadConfig() error {
	_, err := os.Open("config.json")
	if err != nil {
		return fmt.Errorf("failed to load config: %w", err)
	}
	return nil
}

func validateInput(input string) error {
	if input == "" {
		return &ValidationError{Field: "input", Message: "cannot be empty"}
	}
	return nil
}

// --- panic と recover ---
func panicAndRecover() {
	fmt.Println("--- panic と recover ---")

	// recover で panic を捕捉
	result := safeOperation()
	fmt.Printf("Safe operation result: %v\n", result)

	// panic が起きても実行継続
	fmt.Println("Continuing after recovered panic...")

	fmt.Println()
}

func safeOperation() (result string) {
	defer func() {
		if r := recover(); r != nil {
			fmt.Printf("  Recovered from panic: %v\n", r)
			result = "recovered"
		}
	}()

	// panic を発生させる
	riskyOperation()
	return "success"
}

func riskyOperation() {
	panic("something went wrong!")
}

// --- defer ---
func deferUsage() {
	fmt.Println("--- defer ---")

	// defer は LIFO 順
	fmt.Println("defer order (LIFO):")
	defer fmt.Println("  first defer")
	defer fmt.Println("  second defer")
	defer fmt.Println("  third defer")

	// リソースのクリーンアップ
	fmt.Println("\nResource cleanup example:")
	if err := processFile(); err != nil {
		fmt.Printf("  Error: %v\n", err)
	}

	// defer と名前付き返り値
	fmt.Println("\nDefer with named return:")
	result := deferredReturn()
	fmt.Printf("  Result: %d\n", result)

	fmt.Println()
}

func processFile() error {
	fmt.Println("  Opening resource...")

	// ダミーのクローズ可能オブジェクト
	r := &dummyCloser{}
	defer func() {
		fmt.Println("  Closing resource (defer)")
		r.Close()
	}()

	fmt.Println("  Processing...")
	return nil
}

type dummyCloser struct{}

func (d *dummyCloser) Close() error {
	return nil
}

func deferredReturn() (result int) {
	defer func() {
		result += 10 // 名前付き返り値を変更
	}()
	return 5 // defer 後に返り値が変更される
}

// --- io.EOF パターン ---
func init() {
	// io.EOF は正常終了を示すエラー
	_ = io.EOF
}
