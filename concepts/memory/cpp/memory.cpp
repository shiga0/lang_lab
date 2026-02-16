// メモリ管理 - C++ 実装
//
// C++ は手動メモリ管理 + RAII + スマートポインタ。
// 所有権とライフタイムを明示的に管理する。

#include <iostream>
#include <memory>
#include <vector>
#include <string>

using namespace std;

// === スタックとヒープ ===
void stack_and_heap() {
    cout << "=== スタックとヒープ ===" << endl;

    // スタック割り当て（自動）
    int stack_var = 42;
    int stack_arr[10] = {0};
    cout << "Stack variable: " << stack_var << endl;
    cout << "Stack address: " << &stack_var << endl;

    // ヒープ割り当て（手動 - 非推奨）
    int* heap_var = new int(100);
    cout << "Heap variable: " << *heap_var << endl;
    cout << "Heap address: " << heap_var << endl;
    delete heap_var;  // 手動解放が必要

    // 配列
    int* heap_arr = new int[10];
    delete[] heap_arr;  // 配列は delete[]

    cout << endl;
}

// === unique_ptr (単独所有) ===
class Resource {
public:
    string name;
    Resource(const string& n) : name(n) {
        cout << "  Resource created: " << name << endl;
    }
    ~Resource() {
        cout << "  Resource destroyed: " << name << endl;
    }
    void use() const {
        cout << "  Using resource: " << name << endl;
    }
};

void unique_ptr_demo() {
    cout << "=== unique_ptr (単独所有) ===" << endl;

    // make_unique で生成（推奨）
    auto p1 = make_unique<Resource>("p1");
    p1->use();

    // 所有権の移動
    unique_ptr<Resource> p2 = move(p1);
    if (!p1) {
        cout << "  p1 is now null (ownership transferred)" << endl;
    }
    p2->use();

    // スコープ終了時に自動解放
    {
        auto temp = make_unique<Resource>("temp");
        temp->use();
        cout << "  Leaving inner scope..." << endl;
    }
    cout << "  Left inner scope (temp destroyed)" << endl;

    // 配列
    auto arr = make_unique<int[]>(5);
    for (int i = 0; i < 5; ++i) {
        arr[i] = i * 10;
    }
    cout << "  unique_ptr array: " << arr[2] << endl;

    cout << endl;
}

// === shared_ptr (共有所有) ===
void shared_ptr_demo() {
    cout << "=== shared_ptr (共有所有) ===" << endl;

    auto p1 = make_shared<Resource>("shared");
    cout << "  use_count after p1: " << p1.use_count() << endl;

    {
        auto p2 = p1;  // 共有
        cout << "  use_count after p2 = p1: " << p1.use_count() << endl;

        auto p3 = p1;  // さらに共有
        cout << "  use_count after p3 = p1: " << p1.use_count() << endl;
    }

    cout << "  use_count after leaving scope: " << p1.use_count() << endl;

    // reset
    p1.reset();
    cout << "  After reset, p1 is null: " << (p1 == nullptr) << endl;

    cout << endl;
}

// === weak_ptr (弱参照) ===
void weak_ptr_demo() {
    cout << "=== weak_ptr (弱参照) ===" << endl;

    weak_ptr<Resource> weak;

    {
        auto shared = make_shared<Resource>("weak-test");
        weak = shared;

        cout << "  Inside scope - expired: " << boolalpha << weak.expired() << endl;
        cout << "  use_count: " << weak.use_count() << endl;

        // lock で shared_ptr を取得
        if (auto locked = weak.lock()) {
            locked->use();
        }
    }

    cout << "  Outside scope - expired: " << weak.expired() << endl;

    // 循環参照の回避
    struct Node {
        string name;
        shared_ptr<Node> next;
        weak_ptr<Node> prev;  // weak_ptr で循環回避

        Node(const string& n) : name(n) {
            cout << "  Node created: " << name << endl;
        }
        ~Node() {
            cout << "  Node destroyed: " << name << endl;
        }
    };

    cout << "\n  Circular reference prevention:" << endl;
    {
        auto n1 = make_shared<Node>("Node1");
        auto n2 = make_shared<Node>("Node2");
        n1->next = n2;
        n2->prev = n1;  // weak_ptr なので循環参照にならない
    }
    cout << "  Both nodes destroyed properly" << endl;

    cout << endl;
}

// === RAII (Resource Acquisition Is Initialization) ===
class FileHandle {
public:
    FileHandle(const string& filename) : filename_(filename) {
        cout << "  Opening file: " << filename_ << endl;
        // 実際は fopen() など
    }

    ~FileHandle() {
        cout << "  Closing file: " << filename_ << endl;
        // 実際は fclose() など
    }

    void write(const string& data) {
        cout << "  Writing to " << filename_ << ": " << data << endl;
    }

    // コピー禁止
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    // ムーブ許可
    FileHandle(FileHandle&& other) noexcept
        : filename_(move(other.filename_)) {
        other.filename_ = "";
    }

private:
    string filename_;
};

void raii_demo() {
    cout << "=== RAII ===" << endl;

    {
        FileHandle file("test.txt");
        file.write("Hello, World!");
        // スコープ終了時に自動クローズ
    }

    cout << "  File automatically closed" << endl;

    // 例外が発生してもリソースは解放される
    try {
        FileHandle file2("error.txt");
        throw runtime_error("Something went wrong");
    } catch (const exception& e) {
        cout << "  Exception caught: " << e.what() << endl;
    }
    cout << "  File2 also closed (exception safety)" << endl;

    cout << endl;
}

// === ムーブセマンティクス ===
class Buffer {
public:
    Buffer(size_t size) : size_(size), data_(new int[size]) {
        cout << "  Buffer created, size: " << size_ << endl;
    }

    ~Buffer() {
        delete[] data_;
        cout << "  Buffer destroyed" << endl;
    }

    // コピーコンストラクタ
    Buffer(const Buffer& other) : size_(other.size_), data_(new int[other.size_]) {
        copy(other.data_, other.data_ + size_, data_);
        cout << "  Buffer copied (deep copy)" << endl;
    }

    // ムーブコンストラクタ
    Buffer(Buffer&& other) noexcept : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr;
        cout << "  Buffer moved (no copy)" << endl;
    }

    // コピー代入
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new int[size_];
            copy(other.data_, other.data_ + size_, data_);
            cout << "  Buffer copy-assigned" << endl;
        }
        return *this;
    }

    // ムーブ代入
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
            cout << "  Buffer move-assigned" << endl;
        }
        return *this;
    }

    size_t size() const { return size_; }

private:
    size_t size_;
    int* data_;
};

void move_semantics_demo() {
    cout << "=== ムーブセマンティクス ===" << endl;

    Buffer b1(1000);
    Buffer b2 = move(b1);  // ムーブ（コピーなし）

    vector<Buffer> buffers;
    buffers.push_back(Buffer(500));  // ムーブ

    cout << endl;
}

// === メモリアライメント ===
void alignment_demo() {
    cout << "=== メモリアライメント ===" << endl;

    struct Unaligned {
        char a;
        int b;
        char c;
    };

    struct Aligned {
        int b;
        char a;
        char c;
    };

    cout << "sizeof(Unaligned): " << sizeof(Unaligned) << " bytes" << endl;
    cout << "sizeof(Aligned): " << sizeof(Aligned) << " bytes" << endl;

    // alignas
    alignas(16) int aligned_var;
    cout << "alignas(16) int address: " << &aligned_var << endl;

    // alignof
    cout << "alignof(int): " << alignof(int) << endl;
    cout << "alignof(double): " << alignof(double) << endl;

    cout << endl;
}

// === カスタムアロケータ ===
template<typename T>
class SimpleAllocator {
public:
    using value_type = T;

    SimpleAllocator() noexcept {
        cout << "  SimpleAllocator created" << endl;
    }

    template<typename U>
    SimpleAllocator(const SimpleAllocator<U>&) noexcept {}

    T* allocate(size_t n) {
        cout << "  Allocating " << n << " elements of size " << sizeof(T) << endl;
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, size_t n) {
        cout << "  Deallocating " << n << " elements" << endl;
        ::operator delete(p);
    }
};

void custom_allocator_demo() {
    cout << "=== カスタムアロケータ ===" << endl;

    vector<int, SimpleAllocator<int>> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    cout << "  Vector contents: ";
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;

    cout << endl;
}

// === placement new ===
void placement_new_demo() {
    cout << "=== placement new ===" << endl;

    // 事前に確保したバッファにオブジェクトを構築
    alignas(Resource) char buffer[sizeof(Resource)];

    // placement new
    Resource* r = new (buffer) Resource("placement");
    r->use();

    // 明示的にデストラクタを呼ぶ
    r->~Resource();

    cout << "  Object constructed in pre-allocated buffer" << endl;

    cout << endl;
}

int main() {
    cout << "=== Memory Management Demo ===" << endl << endl;

    stack_and_heap();
    unique_ptr_demo();
    shared_ptr_demo();
    weak_ptr_demo();
    raii_demo();
    move_semantics_demo();
    alignment_demo();
    custom_allocator_demo();
    placement_new_demo();

    return 0;
}
