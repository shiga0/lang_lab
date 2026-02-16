// オブジェクト指向 - C++ 実装
//
// C++ はマルチパラダイム言語で、OOP を強力にサポート。
// クラス、継承、多態性、カプセル化。

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>

using namespace std;

// === クラスの基本 ===
class Person {
public:
    // コンストラクタ（メンバ初期化子リスト）
    Person(const string& name, int age)
        : name_(name), age_(age) {
        cout << "  Person created: " << name_ << endl;
    }

    // デフォルトコンストラクタ
    Person() : Person("Unknown", 0) {}

    // デストラクタ
    ~Person() {
        cout << "  Person destroyed: " << name_ << endl;
    }

    // ゲッター
    const string& name() const { return name_; }
    int age() const { return age_; }

    // セッター
    void set_age(int age) {
        if (age >= 0) {
            age_ = age;
        }
    }

    // メソッド
    string greet() const {
        return "Hello, I'm " + name_;
    }

    // static メンバ
    static int population() { return population_; }

private:
    string name_;
    int age_;
    inline static int population_ = 0;  // C++17
};

void class_basics() {
    cout << "=== クラスの基本 ===" << endl;

    Person alice("Alice", 30);
    cout << "  " << alice.greet() << endl;
    cout << "  Age: " << alice.age() << endl;

    alice.set_age(31);
    cout << "  New age: " << alice.age() << endl;

    cout << endl;
}

// === カプセル化とアクセス制御 ===
class BankAccount {
public:
    explicit BankAccount(double initial_balance)
        : balance_(initial_balance) {}

    void deposit(double amount) {
        if (amount > 0) {
            balance_ += amount;
            log("Deposited", amount);
        }
    }

    bool withdraw(double amount) {
        if (amount > 0 && amount <= balance_) {
            balance_ -= amount;
            log("Withdrawn", amount);
            return true;
        }
        return false;
    }

    double balance() const { return balance_; }

protected:
    void log(const string& action, double amount) {
        cout << "  " << action << ": $" << amount
             << " (Balance: $" << balance_ << ")" << endl;
    }

private:
    double balance_;
};

void encapsulation_demo() {
    cout << "=== カプセル化とアクセス制御 ===" << endl;

    BankAccount account(1000);
    account.deposit(500);
    account.withdraw(200);
    cout << "  Final balance: $" << account.balance() << endl;

    cout << endl;
}

// === 継承 ===
class Animal {
public:
    Animal(const string& name) : name_(name) {}
    virtual ~Animal() = default;

    const string& name() const { return name_; }

    virtual string speak() const {
        return name_ + " makes a sound";
    }

    void move() const {
        cout << "  " << name_ << " moves" << endl;
    }

protected:
    string name_;
};

class Dog : public Animal {
public:
    Dog(const string& name, const string& breed)
        : Animal(name), breed_(breed) {}

    string speak() const override {
        return name_ + " says: Woof!";
    }

    void fetch() const {
        cout << "  " << name_ << " fetches the ball" << endl;
    }

private:
    string breed_;
};

class Cat : public Animal {
public:
    Cat(const string& name) : Animal(name) {}

    string speak() const override {
        return name_ + " says: Meow!";
    }
};

void inheritance_demo() {
    cout << "=== 継承 ===" << endl;

    Dog dog("Buddy", "Golden Retriever");
    Cat cat("Whiskers");

    cout << "  " << dog.speak() << endl;
    dog.move();
    dog.fetch();

    cout << "  " << cat.speak() << endl;

    // ポリモーフィズム
    vector<unique_ptr<Animal>> animals;
    animals.push_back(make_unique<Dog>("Rex", "German Shepherd"));
    animals.push_back(make_unique<Cat>("Luna"));

    cout << "\n  Polymorphism:" << endl;
    for (const auto& animal : animals) {
        cout << "    " << animal->speak() << endl;
    }

    cout << endl;
}

// === 抽象クラス ===
class Shape {
public:
    virtual ~Shape() = default;

    // 純粋仮想関数
    virtual double area() const = 0;
    virtual double perimeter() const = 0;

    // 具象メソッド
    void describe() const {
        cout << "  Area: " << area()
             << ", Perimeter: " << perimeter() << endl;
    }
};

class Rectangle : public Shape {
public:
    Rectangle(double width, double height)
        : width_(width), height_(height) {}

    double area() const override {
        return width_ * height_;
    }

    double perimeter() const override {
        return 2 * (width_ + height_);
    }

private:
    double width_;
    double height_;
};

class Circle : public Shape {
public:
    explicit Circle(double radius) : radius_(radius) {}

    double area() const override {
        return M_PI * radius_ * radius_;
    }

    double perimeter() const override {
        return 2 * M_PI * radius_;
    }

private:
    double radius_;
};

void abstract_class_demo() {
    cout << "=== 抽象クラス ===" << endl;

    vector<unique_ptr<Shape>> shapes;
    shapes.push_back(make_unique<Rectangle>(10, 5));
    shapes.push_back(make_unique<Circle>(7));

    for (const auto& shape : shapes) {
        shape->describe();
    }

    cout << endl;
}

// === 多重継承 ===
class Printable {
public:
    virtual ~Printable() = default;
    virtual void print() const = 0;
};

class Serializable {
public:
    virtual ~Serializable() = default;
    virtual string serialize() const = 0;
};

class Document : public Printable, public Serializable {
public:
    Document(const string& content) : content_(content) {}

    void print() const override {
        cout << "  Printing: " << content_ << endl;
    }

    string serialize() const override {
        return "{\"content\": \"" + content_ + "\"}";
    }

private:
    string content_;
};

void multiple_inheritance_demo() {
    cout << "=== 多重継承 ===" << endl;

    Document doc("Hello, World!");
    doc.print();
    cout << "  Serialized: " << doc.serialize() << endl;

    cout << endl;
}

// === 仮想継承 (ダイヤモンド問題解決) ===
class Base {
public:
    Base(int value) : value_(value) {
        cout << "  Base constructed with: " << value_ << endl;
    }
    int value() const { return value_; }
protected:
    int value_;
};

class Left : virtual public Base {
public:
    Left(int value) : Base(value) {
        cout << "  Left constructed" << endl;
    }
};

class Right : virtual public Base {
public:
    Right(int value) : Base(value) {
        cout << "  Right constructed" << endl;
    }
};

class Diamond : public Left, public Right {
public:
    Diamond(int value)
        : Base(value),  // 仮想基底クラスは最派生クラスで初期化
          Left(value),
          Right(value) {
        cout << "  Diamond constructed" << endl;
    }
};

void virtual_inheritance_demo() {
    cout << "=== 仮想継承 (ダイヤモンド問題) ===" << endl;

    Diamond d(42);
    cout << "  Diamond::value(): " << d.value() << endl;
    // Base は1つだけ存在する

    cout << endl;
}

// === 演算子オーバーロード ===
class Vector2D {
public:
    double x, y;

    Vector2D(double x = 0, double y = 0) : x(x), y(y) {}

    Vector2D operator+(const Vector2D& other) const {
        return {x + other.x, y + other.y};
    }

    Vector2D operator-(const Vector2D& other) const {
        return {x - other.x, y - other.y};
    }

    Vector2D operator*(double scalar) const {
        return {x * scalar, y * scalar};
    }

    bool operator==(const Vector2D& other) const {
        return x == other.x && y == other.y;
    }

    // 出力ストリーム
    friend ostream& operator<<(ostream& os, const Vector2D& v) {
        return os << "(" << v.x << ", " << v.y << ")";
    }
};

void operator_overloading_demo() {
    cout << "=== 演算子オーバーロード ===" << endl;

    Vector2D v1(1, 2);
    Vector2D v2(3, 4);

    cout << "  v1 = " << v1 << endl;
    cout << "  v2 = " << v2 << endl;
    cout << "  v1 + v2 = " << (v1 + v2) << endl;
    cout << "  v1 - v2 = " << (v1 - v2) << endl;
    cout << "  v1 * 3 = " << (v1 * 3) << endl;
    cout << "  v1 == v1: " << boolalpha << (v1 == v1) << endl;

    cout << endl;
}

// === フレンド関数・クラス ===
class Secret {
public:
    Secret(int value) : value_(value) {}

private:
    int value_;

    friend class SecretReader;
    friend void reveal(const Secret& s);
};

class SecretReader {
public:
    int read(const Secret& s) const {
        return s.value_;  // private にアクセス可能
    }
};

void reveal(const Secret& s) {
    cout << "  Secret value: " << s.value_ << endl;
}

void friend_demo() {
    cout << "=== フレンド関数・クラス ===" << endl;

    Secret s(42);
    SecretReader reader;
    cout << "  Reader says: " << reader.read(s) << endl;
    reveal(s);

    cout << endl;
}

// === CRTP (Curiously Recurring Template Pattern) ===
template<typename Derived>
class Counter {
public:
    static int count() { return count_; }

protected:
    Counter() { ++count_; }
    ~Counter() { --count_; }

private:
    inline static int count_ = 0;
};

class Widget : public Counter<Widget> {
public:
    Widget(const string& name) : name_(name) {}
private:
    string name_;
};

class Gadget : public Counter<Gadget> {
public:
    Gadget(int id) : id_(id) {}
private:
    int id_;
};

void crtp_demo() {
    cout << "=== CRTP ===" << endl;

    Widget w1("W1");
    Widget w2("W2");
    Gadget g1(1);

    cout << "  Widget count: " << Widget::count() << endl;
    cout << "  Gadget count: " << Gadget::count() << endl;

    cout << endl;
}

// === Rule of Five ===
class Buffer {
public:
    // コンストラクタ
    explicit Buffer(size_t size)
        : size_(size), data_(new int[size]) {
        cout << "  Buffer constructed, size: " << size_ << endl;
    }

    // デストラクタ
    ~Buffer() {
        delete[] data_;
        cout << "  Buffer destroyed" << endl;
    }

    // コピーコンストラクタ
    Buffer(const Buffer& other)
        : size_(other.size_), data_(new int[other.size_]) {
        copy(other.data_, other.data_ + size_, data_);
        cout << "  Buffer copy constructed" << endl;
    }

    // コピー代入演算子
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new int[size_];
            copy(other.data_, other.data_ + size_, data_);
            cout << "  Buffer copy assigned" << endl;
        }
        return *this;
    }

    // ムーブコンストラクタ
    Buffer(Buffer&& other) noexcept
        : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr;
        cout << "  Buffer move constructed" << endl;
    }

    // ムーブ代入演算子
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
            cout << "  Buffer move assigned" << endl;
        }
        return *this;
    }

private:
    size_t size_;
    int* data_;
};

void rule_of_five_demo() {
    cout << "=== Rule of Five ===" << endl;

    Buffer b1(100);
    Buffer b2 = b1;           // コピーコンストラクタ
    Buffer b3 = move(b1);     // ムーブコンストラクタ

    cout << endl;
}

int main() {
    cout << "=== OOP Demo ===" << endl << endl;

    class_basics();
    encapsulation_demo();
    inheritance_demo();
    abstract_class_demo();
    multiple_inheritance_demo();
    virtual_inheritance_demo();
    operator_overloading_demo();
    friend_demo();
    crtp_demo();
    rule_of_five_demo();

    return 0;
}
