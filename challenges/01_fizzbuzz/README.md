# Challenge 01: FizzBuzz

## 問題

1から100までの数字を出力する。ただし：
- 3で割り切れる場合は "Fizz"
- 5で割り切れる場合は "Buzz"
- 両方で割り切れる場合は "FizzBuzz"

## 期待される出力

```
1
2
Fizz
4
Buzz
Fizz
7
8
Fizz
Buzz
11
Fizz
13
14
FizzBuzz
...
```

## 学べること

| 言語 | 学べる概念 |
|------|-----------|
| Rust | match, 範囲, イテレータ |
| Ruby | 条件分岐, Range, each |

## 発展課題

1. ジェネリックに任意の範囲に対応
2. カスタムルール (7で割り切れたら "Bazz" など)
3. 並列処理版
