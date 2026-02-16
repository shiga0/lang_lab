// データ構造 - C++ 実装
//
// C++ STL には豊富なコンテナが用意されている。
// vector, list, deque, map, set, unordered_map など。

#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <forward_list>
#include <deque>
#include <stack>
#include <queue>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <memory>

using namespace std;

// === vector ===
void vector_demo() {
    cout << "=== vector ===" << endl;

    // 作成
    vector<int> v = {1, 2, 3, 4, 5};
    cout << "  vector: ";
    for (int x : v) cout << x << " ";
    cout << endl;

    // 操作
    v.push_back(6);
    v.insert(v.begin(), 0);
    cout << "  after push_back/insert: ";
    for (int x : v) cout << x << " ";
    cout << endl;

    // アクセス
    cout << "  v[2] = " << v[2] << endl;
    cout << "  v.at(2) = " << v.at(2) << endl;
    cout << "  front = " << v.front() << ", back = " << v.back() << endl;

    // サイズ
    cout << "  size = " << v.size() << ", capacity = " << v.capacity() << endl;

    // 削除
    v.pop_back();
    v.erase(v.begin());
    cout << "  after pop_back/erase: ";
    for (int x : v) cout << x << " ";
    cout << endl;

    cout << endl;
}

// === array ===
void array_demo() {
    cout << "=== array ===" << endl;

    // 固定長配列
    array<int, 5> arr = {1, 2, 3, 4, 5};

    cout << "  array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    cout << "  size = " << arr.size() << endl;
    cout << "  arr[2] = " << arr[2] << endl;

    // fill
    arr.fill(0);
    cout << "  after fill(0): ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    cout << endl;
}

// === list (双方向リスト) ===
void list_demo() {
    cout << "=== list (双方向リスト) ===" << endl;

    list<int> lst = {1, 2, 3};

    lst.push_front(0);
    lst.push_back(4);
    cout << "  list: ";
    for (int x : lst) cout << x << " ";
    cout << endl;

    lst.pop_front();
    lst.pop_back();
    cout << "  after pop_front/pop_back: ";
    for (int x : lst) cout << x << " ";
    cout << endl;

    // splice
    list<int> other = {10, 20};
    lst.splice(lst.end(), other);
    cout << "  after splice: ";
    for (int x : lst) cout << x << " ";
    cout << endl;

    cout << endl;
}

// === deque ===
void deque_demo() {
    cout << "=== deque ===" << endl;

    deque<int> d = {2, 3, 4};

    d.push_front(1);
    d.push_back(5);
    cout << "  deque: ";
    for (int x : d) cout << x << " ";
    cout << endl;

    cout << "  front = " << d.front() << ", back = " << d.back() << endl;

    d.pop_front();
    d.pop_back();
    cout << "  after pops: ";
    for (int x : d) cout << x << " ";
    cout << endl;

    cout << endl;
}

// === stack ===
void stack_demo() {
    cout << "=== stack ===" << endl;

    stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

    cout << "  top = " << s.top() << endl;
    cout << "  size = " << s.size() << endl;

    cout << "  popping: ";
    while (!s.empty()) {
        cout << s.top() << " ";
        s.pop();
    }
    cout << endl;

    cout << endl;
}

// === queue ===
void queue_demo() {
    cout << "=== queue ===" << endl;

    queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);

    cout << "  front = " << q.front() << ", back = " << q.back() << endl;

    cout << "  dequeuing: ";
    while (!q.empty()) {
        cout << q.front() << " ";
        q.pop();
    }
    cout << endl;

    cout << endl;
}

// === priority_queue ===
void priority_queue_demo() {
    cout << "=== priority_queue ===" << endl;

    // max heap (default)
    priority_queue<int> max_pq;
    max_pq.push(3);
    max_pq.push(1);
    max_pq.push(4);
    max_pq.push(1);
    max_pq.push(5);

    cout << "  max heap: ";
    while (!max_pq.empty()) {
        cout << max_pq.top() << " ";
        max_pq.pop();
    }
    cout << endl;

    // min heap
    priority_queue<int, vector<int>, greater<int>> min_pq;
    min_pq.push(3);
    min_pq.push(1);
    min_pq.push(4);
    min_pq.push(1);
    min_pq.push(5);

    cout << "  min heap: ";
    while (!min_pq.empty()) {
        cout << min_pq.top() << " ";
        min_pq.pop();
    }
    cout << endl;

    cout << endl;
}

// === map ===
void map_demo() {
    cout << "=== map ===" << endl;

    map<string, int> m;
    m["one"] = 1;
    m["two"] = 2;
    m["three"] = 3;

    cout << "  map contents:" << endl;
    for (const auto& [key, value] : m) {
        cout << "    " << key << " => " << value << endl;
    }

    // 検索
    cout << "  m[\"two\"] = " << m["two"] << endl;
    cout << "  m.count(\"four\") = " << m.count("four") << endl;

    if (auto it = m.find("three"); it != m.end()) {
        cout << "  found: " << it->first << " => " << it->second << endl;
    }

    // insert_or_assign (C++17)
    m.insert_or_assign("four", 4);

    cout << endl;
}

// === unordered_map ===
void unordered_map_demo() {
    cout << "=== unordered_map ===" << endl;

    unordered_map<string, int> um = {
        {"apple", 100},
        {"banana", 200},
        {"cherry", 300}
    };

    cout << "  unordered_map contents:" << endl;
    for (const auto& [key, value] : um) {
        cout << "    " << key << " => " << value << endl;
    }

    cout << "  um[\"apple\"] = " << um["apple"] << endl;
    cout << "  bucket_count = " << um.bucket_count() << endl;
    cout << "  load_factor = " << um.load_factor() << endl;

    cout << endl;
}

// === set ===
void set_demo() {
    cout << "=== set ===" << endl;

    set<int> s = {3, 1, 4, 1, 5, 9, 2, 6};  // 重複除去、ソート済み

    cout << "  set: ";
    for (int x : s) cout << x << " ";
    cout << endl;

    s.insert(10);
    s.erase(4);

    cout << "  after insert/erase: ";
    for (int x : s) cout << x << " ";
    cout << endl;

    cout << "  contains 5: " << boolalpha << s.contains(5) << endl;

    // lower_bound, upper_bound
    cout << "  lower_bound(5) = " << *s.lower_bound(5) << endl;
    cout << "  upper_bound(5) = " << *s.upper_bound(5) << endl;

    cout << endl;
}

// === multiset / multimap ===
void multi_demo() {
    cout << "=== multiset / multimap ===" << endl;

    multiset<int> ms = {1, 2, 2, 3, 3, 3};
    cout << "  multiset: ";
    for (int x : ms) cout << x << " ";
    cout << endl;
    cout << "  count(3) = " << ms.count(3) << endl;

    multimap<string, int> mm;
    mm.insert({"a", 1});
    mm.insert({"a", 2});
    mm.insert({"b", 3});

    cout << "  multimap:" << endl;
    for (const auto& [k, v] : mm) {
        cout << "    " << k << " => " << v << endl;
    }

    cout << endl;
}

// === カスタム連結リスト ===
template<typename T>
class LinkedList {
public:
    struct Node {
        T value;
        unique_ptr<Node> next;
        Node(const T& v) : value(v), next(nullptr) {}
    };

    LinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

    void push_front(const T& value) {
        auto node = make_unique<Node>(value);
        node->next = move(head_);
        head_ = move(node);
        if (!tail_) tail_ = head_.get();
        ++size_;
    }

    void push_back(const T& value) {
        auto node = make_unique<Node>(value);
        auto* raw = node.get();
        if (tail_) {
            tail_->next = move(node);
            tail_ = raw;
        } else {
            head_ = move(node);
            tail_ = raw;
        }
        ++size_;
    }

    void pop_front() {
        if (head_) {
            head_ = move(head_->next);
            if (!head_) tail_ = nullptr;
            --size_;
        }
    }

    T& front() { return head_->value; }
    const T& front() const { return head_->value; }

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    void print() const {
        cout << "[";
        for (auto* node = head_.get(); node; node = node->next.get()) {
            cout << node->value;
            if (node->next) cout << " -> ";
        }
        cout << "]" << endl;
    }

private:
    unique_ptr<Node> head_;
    Node* tail_;
    size_t size_;
};

void custom_linked_list_demo() {
    cout << "=== カスタム連結リスト ===" << endl;

    LinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_front(0);

    cout << "  ";
    list.print();

    list.pop_front();
    cout << "  after pop_front: ";
    list.print();

    cout << endl;
}

// === 二分探索木 ===
template<typename T>
class BST {
public:
    struct Node {
        T value;
        unique_ptr<Node> left;
        unique_ptr<Node> right;
        Node(const T& v) : value(v), left(nullptr), right(nullptr) {}
    };

    void insert(const T& value) {
        insert_impl(root_, value);
    }

    bool search(const T& value) const {
        return search_impl(root_.get(), value);
    }

    void inorder(vector<T>& result) const {
        inorder_impl(root_.get(), result);
    }

private:
    unique_ptr<Node> root_;

    void insert_impl(unique_ptr<Node>& node, const T& value) {
        if (!node) {
            node = make_unique<Node>(value);
        } else if (value < node->value) {
            insert_impl(node->left, value);
        } else {
            insert_impl(node->right, value);
        }
    }

    bool search_impl(const Node* node, const T& value) const {
        if (!node) return false;
        if (value == node->value) return true;
        if (value < node->value) return search_impl(node->left.get(), value);
        return search_impl(node->right.get(), value);
    }

    void inorder_impl(const Node* node, vector<T>& result) const {
        if (!node) return;
        inorder_impl(node->left.get(), result);
        result.push_back(node->value);
        inorder_impl(node->right.get(), result);
    }
};

void bst_demo() {
    cout << "=== 二分探索木 ===" << endl;

    BST<int> bst;
    for (int v : {5, 3, 7, 1, 4, 6, 8}) {
        bst.insert(v);
    }

    vector<int> sorted;
    bst.inorder(sorted);
    cout << "  inorder: ";
    for (int x : sorted) cout << x << " ";
    cout << endl;

    cout << "  search(4): " << boolalpha << bst.search(4) << endl;
    cout << "  search(9): " << bst.search(9) << endl;

    cout << endl;
}

int main() {
    cout << "=== Data Structures Demo ===" << endl << endl;

    vector_demo();
    array_demo();
    list_demo();
    deque_demo();
    stack_demo();
    queue_demo();
    priority_queue_demo();
    map_demo();
    unordered_map_demo();
    set_demo();
    multi_demo();
    custom_linked_list_demo();
    bst_demo();

    return 0;
}
