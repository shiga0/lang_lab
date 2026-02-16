// LinkedList - C++ 実装

#include <iostream>
#include <memory>
#include <optional>
#include <vector>
#include <cassert>

using namespace std;

// === LinkedList ===
template<typename T>
class LinkedList {
public:
    struct Node {
        T value;
        unique_ptr<Node> next;

        Node(const T& v) : value(v), next(nullptr) {}
        Node(T&& v) : value(move(v)), next(nullptr) {}
    };

    // イテレータ
    class Iterator {
    public:
        using iterator_category = forward_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(Node* node) : node_(node) {}

        reference operator*() { return node_->value; }
        pointer operator->() { return &node_->value; }

        Iterator& operator++() {
            node_ = node_->next.get();
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            return node_ == other.node_;
        }

        bool operator!=(const Iterator& other) const {
            return node_ != other.node_;
        }

    private:
        Node* node_;
    };

    // const イテレータ
    class ConstIterator {
    public:
        using iterator_category = forward_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        ConstIterator(const Node* node) : node_(node) {}

        reference operator*() const { return node_->value; }
        pointer operator->() const { return &node_->value; }

        ConstIterator& operator++() {
            node_ = node_->next.get();
            return *this;
        }

        bool operator!=(const ConstIterator& other) const {
            return node_ != other.node_;
        }

    private:
        const Node* node_;
    };

    LinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

    // コピーコンストラクタ
    LinkedList(const LinkedList& other) : LinkedList() {
        for (const auto& value : other) {
            push_back(value);
        }
    }

    // ムーブコンストラクタ
    LinkedList(LinkedList&& other) noexcept
        : head_(move(other.head_)),
          tail_(other.tail_),
          size_(other.size_) {
        other.tail_ = nullptr;
        other.size_ = 0;
    }

    // コピー代入
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            LinkedList tmp(other);
            swap(*this, tmp);
        }
        return *this;
    }

    // ムーブ代入
    LinkedList& operator=(LinkedList&& other) noexcept {
        if (this != &other) {
            head_ = move(other.head_);
            tail_ = other.tail_;
            size_ = other.size_;
            other.tail_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    // 先頭に追加
    void push_front(const T& value) {
        auto node = make_unique<Node>(value);
        node->next = move(head_);
        if (!tail_) {
            tail_ = node.get();
        }
        head_ = move(node);
        ++size_;
    }

    // 末尾に追加
    void push_back(const T& value) {
        auto node = make_unique<Node>(value);
        auto* raw_ptr = node.get();

        if (tail_) {
            tail_->next = move(node);
            tail_ = raw_ptr;
        } else {
            head_ = move(node);
            tail_ = raw_ptr;
        }
        ++size_;
    }

    // 先頭を削除
    optional<T> pop_front() {
        if (!head_) return nullopt;

        T value = move(head_->value);
        head_ = move(head_->next);

        if (!head_) {
            tail_ = nullptr;
        }

        --size_;
        return value;
    }

    // 先頭の値を参照
    optional<reference_wrapper<T>> front() {
        if (!head_) return nullopt;
        return head_->value;
    }

    optional<reference_wrapper<const T>> front() const {
        if (!head_) return nullopt;
        return head_->value;
    }

    // 末尾の値を参照
    optional<reference_wrapper<T>> back() {
        if (!tail_) return nullopt;
        return tail_->value;
    }

    optional<reference_wrapper<const T>> back() const {
        if (!tail_) return nullopt;
        return tail_->value;
    }

    // 検索
    bool contains(const T& value) const {
        for (const auto& v : *this) {
            if (v == value) return true;
        }
        return false;
    }

    // 削除
    bool remove(const T& value) {
        if (!head_) return false;

        // 先頭の場合
        if (head_->value == value) {
            pop_front();
            return true;
        }

        // それ以外
        Node* prev = head_.get();
        Node* current = head_->next.get();

        while (current) {
            if (current->value == value) {
                if (current == tail_) {
                    tail_ = prev;
                }
                prev->next = move(current->next);
                --size_;
                return true;
            }
            prev = current;
            current = current->next.get();
        }

        return false;
    }

    // 反転
    void reverse() {
        if (size_ <= 1) return;

        unique_ptr<Node> prev = nullptr;
        unique_ptr<Node> current = move(head_);
        tail_ = current.get();

        while (current) {
            unique_ptr<Node> next = move(current->next);
            current->next = move(prev);
            prev = move(current);
            current = move(next);
        }

        head_ = move(prev);
    }

    // クリア
    void clear() {
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

    // サイズ
    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    // イテレータ
    Iterator begin() { return Iterator(head_.get()); }
    Iterator end() { return Iterator(nullptr); }
    ConstIterator begin() const { return ConstIterator(head_.get()); }
    ConstIterator end() const { return ConstIterator(nullptr); }

    // vector に変換
    vector<T> to_vector() const {
        vector<T> result;
        result.reserve(size_);
        for (const auto& v : *this) {
            result.push_back(v);
        }
        return result;
    }

    // swap
    friend void swap(LinkedList& a, LinkedList& b) noexcept {
        using std::swap;
        swap(a.head_, b.head_);
        swap(a.tail_, b.tail_);
        swap(a.size_, b.size_);
    }

private:
    unique_ptr<Node> head_;
    Node* tail_;
    size_t size_;
};

// 出力演算子
template<typename T>
ostream& operator<<(ostream& os, const LinkedList<T>& list) {
    os << "[";
    bool first = true;
    for (const auto& v : list) {
        if (!first) os << " -> ";
        os << v;
        first = false;
    }
    os << "]";
    return os;
}

// === デモ ===
void basic_operations() {
    cout << "--- Basic Operations ---" << endl;

    LinkedList<int> list;
    cout << "Empty list: " << list << endl;

    list.push_front(3);
    list.push_front(2);
    list.push_front(1);
    cout << "After push_front 3, 2, 1: " << list << endl;

    list.push_back(4);
    list.push_back(5);
    cout << "After push_back 4, 5: " << list << endl;

    cout << "Size: " << list.size() << endl;
    cout << "Front: " << list.front()->get() << endl;
    cout << "Back: " << list.back()->get() << endl;

    cout << endl;
}

void pop_and_remove() {
    cout << "--- Pop and Remove ---" << endl;

    LinkedList<int> list;
    for (int i = 1; i <= 5; ++i) {
        list.push_back(i);
    }
    cout << "List: " << list << endl;

    auto popped = list.pop_front();
    cout << "pop_front: " << *popped << endl;
    cout << "After pop_front: " << list << endl;

    list.remove(4);
    cout << "After remove(4): " << list << endl;

    cout << endl;
}

void iteration_demo() {
    cout << "--- Iteration ---" << endl;

    LinkedList<int> list;
    for (int i = 1; i <= 5; ++i) {
        list.push_back(i);
    }
    cout << "List: " << list << endl;

    // Range-based for
    cout << "Values: ";
    for (const auto& v : list) {
        cout << v << " ";
    }
    cout << endl;

    // 変換
    cout << "Squared: ";
    for (auto& v : list) {
        cout << v * v << " ";
    }
    cout << endl;

    // contains
    cout << "contains(3): " << boolalpha << list.contains(3) << endl;
    cout << "contains(10): " << list.contains(10) << endl;

    cout << endl;
}

void reverse_demo() {
    cout << "--- Reverse ---" << endl;

    LinkedList<int> list;
    for (int i = 1; i <= 5; ++i) {
        list.push_back(i);
    }

    cout << "Before reverse: " << list << endl;
    list.reverse();
    cout << "After reverse: " << list << endl;

    cout << endl;
}

void copy_move_demo() {
    cout << "--- Copy and Move ---" << endl;

    LinkedList<int> list1;
    list1.push_back(1);
    list1.push_back(2);
    list1.push_back(3);

    // コピー
    LinkedList<int> list2 = list1;
    cout << "Original: " << list1 << endl;
    cout << "Copy: " << list2 << endl;

    // ムーブ
    LinkedList<int> list3 = move(list1);
    cout << "After move (original empty): " << list1 << endl;
    cout << "Moved: " << list3 << endl;

    cout << endl;
}

// === テスト ===
void run_tests() {
    cout << "--- Tests ---" << endl;

    LinkedList<int> list;

    // 空リスト
    assert(list.empty());
    assert(list.size() == 0);

    // push_front
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    assert(list.to_vector() == vector<int>({3, 2, 1}));

    // push_back
    list.push_back(10);
    assert(list.to_vector() == vector<int>({3, 2, 1, 10}));

    // front / back
    assert(list.front()->get() == 3);
    assert(list.back()->get() == 10);

    // pop_front
    assert(list.pop_front() == 3);
    assert(list.to_vector() == vector<int>({2, 1, 10}));

    // remove
    assert(list.remove(1) == true);
    assert(list.to_vector() == vector<int>({2, 10}));

    // reverse
    list.push_back(20);
    list.reverse();
    assert(list.to_vector() == vector<int>({20, 10, 2}));

    // contains
    assert(list.contains(10) == true);
    assert(list.contains(999) == false);

    cout << "All tests passed!" << endl;
}

int main() {
    cout << "=== LinkedList Demo ===" << endl << endl;

    basic_operations();
    pop_and_remove();
    iteration_demo();
    reverse_demo();
    copy_move_demo();
    run_tests();

    return 0;
}
