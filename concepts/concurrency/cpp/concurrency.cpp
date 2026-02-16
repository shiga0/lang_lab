// 並行処理 - C++ 実装
//
// C++11 以降、標準ライブラリで並行処理をサポート。
// thread, mutex, future, atomic など。

#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <future>
#include <atomic>
#include <vector>
#include <queue>
#include <chrono>
#include <functional>

using namespace std;
using namespace chrono;

// === 基本的なスレッド ===
void basic_thread() {
    cout << "=== 基本的なスレッド ===" << endl;

    // ラムダでスレッド作成
    thread t1([]() {
        cout << "  Thread 1: Hello from thread!" << endl;
        cout << "  Thread 1 ID: " << this_thread::get_id() << endl;
    });

    // 関数でスレッド作成
    auto worker = [](int id, int count) {
        for (int i = 0; i < count; ++i) {
            cout << "  Worker " << id << ": iteration " << i << endl;
            this_thread::sleep_for(milliseconds(10));
        }
    };

    thread t2(worker, 2, 3);

    // join で完了を待つ
    t1.join();
    t2.join();

    // ハードウェア並行数
    cout << "  Hardware concurrency: " << thread::hardware_concurrency() << endl;

    cout << endl;
}

// === mutex ===
void mutex_demo() {
    cout << "=== mutex ===" << endl;

    mutex mtx;
    int counter = 0;

    auto increment = [&](int times) {
        for (int i = 0; i < times; ++i) {
            lock_guard<mutex> lock(mtx);  // RAII ロック
            ++counter;
        }
    };

    vector<thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(increment, 1000);
    }

    for (auto& t : threads) {
        t.join();
    }

    cout << "  Counter (4 threads x 1000): " << counter << endl;

    cout << endl;
}

// === unique_lock と scoped_lock ===
void advanced_locks() {
    cout << "=== unique_lock と scoped_lock ===" << endl;

    mutex mtx1, mtx2;

    // unique_lock: より柔軟なロック
    {
        unique_lock<mutex> lock(mtx1, defer_lock);  // ロック遅延
        // ... 何か処理 ...
        lock.lock();  // 明示的にロック
        cout << "  unique_lock: locked" << endl;
        // スコープ終了でアンロック
    }

    // scoped_lock: 複数の mutex を同時にロック（デッドロック回避）
    auto task1 = [&]() {
        scoped_lock lock(mtx1, mtx2);
        cout << "  Task1: locked both mutexes" << endl;
        this_thread::sleep_for(milliseconds(10));
    };

    auto task2 = [&]() {
        scoped_lock lock(mtx1, mtx2);  // 順序を変えてもデッドロックしない
        cout << "  Task2: locked both mutexes" << endl;
    };

    thread t1(task1);
    thread t2(task2);
    t1.join();
    t2.join();

    cout << endl;
}

// === shared_mutex (読み書きロック) ===
void shared_mutex_demo() {
    cout << "=== shared_mutex (読み書きロック) ===" << endl;

    shared_mutex rw_mutex;
    int data = 0;

    // 書き込み（排他ロック）
    auto writer = [&](int value) {
        unique_lock<shared_mutex> lock(rw_mutex);
        data = value;
        cout << "  Writer: set data to " << value << endl;
    };

    // 読み込み（共有ロック）
    auto reader = [&](int id) {
        shared_lock<shared_mutex> lock(rw_mutex);
        cout << "  Reader " << id << ": data = " << data << endl;
    };

    thread w(writer, 42);
    w.join();

    vector<thread> readers;
    for (int i = 0; i < 3; ++i) {
        readers.emplace_back(reader, i);
    }
    for (auto& r : readers) {
        r.join();
    }

    cout << endl;
}

// === condition_variable ===
void condition_variable_demo() {
    cout << "=== condition_variable ===" << endl;

    mutex mtx;
    condition_variable cv;
    queue<int> q;
    bool done = false;

    // プロデューサー
    thread producer([&]() {
        for (int i = 0; i < 5; ++i) {
            {
                lock_guard<mutex> lock(mtx);
                q.push(i);
                cout << "  Produced: " << i << endl;
            }
            cv.notify_one();
            this_thread::sleep_for(milliseconds(10));
        }
        {
            lock_guard<mutex> lock(mtx);
            done = true;
        }
        cv.notify_all();
    });

    // コンシューマー
    thread consumer([&]() {
        while (true) {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [&]() { return !q.empty() || done; });

            while (!q.empty()) {
                cout << "  Consumed: " << q.front() << endl;
                q.pop();
            }

            if (done && q.empty()) break;
        }
    });

    producer.join();
    consumer.join();

    cout << endl;
}

// === async と future ===
void async_future_demo() {
    cout << "=== async と future ===" << endl;

    // async: 非同期タスク
    auto compute = [](int x) {
        this_thread::sleep_for(milliseconds(100));
        return x * x;
    };

    // launch::async で新スレッドで実行
    future<int> f1 = async(launch::async, compute, 10);

    // launch::deferred で遅延実行
    future<int> f2 = async(launch::deferred, compute, 20);

    cout << "  Waiting for results..." << endl;
    cout << "  f1 (async): " << f1.get() << endl;    // ブロックして結果取得
    cout << "  f2 (deferred): " << f2.get() << endl;  // get() 時に実行

    // 複数の async
    vector<future<int>> futures;
    for (int i = 1; i <= 4; ++i) {
        futures.push_back(async(launch::async, compute, i));
    }

    cout << "  Multiple results: ";
    for (auto& f : futures) {
        cout << f.get() << " ";
    }
    cout << endl;

    cout << endl;
}

// === promise ===
void promise_demo() {
    cout << "=== promise ===" << endl;

    promise<int> prom;
    future<int> fut = prom.get_future();

    thread t([&prom]() {
        this_thread::sleep_for(milliseconds(50));
        prom.set_value(42);
        cout << "  Promise fulfilled with 42" << endl;
    });

    cout << "  Waiting for promise..." << endl;
    cout << "  Got: " << fut.get() << endl;

    t.join();

    // 例外を伝播
    promise<int> prom2;
    future<int> fut2 = prom2.get_future();

    thread t2([&prom2]() {
        try {
            throw runtime_error("Something went wrong");
        } catch (...) {
            prom2.set_exception(current_exception());
        }
    });

    try {
        fut2.get();
    } catch (const exception& e) {
        cout << "  Caught exception: " << e.what() << endl;
    }

    t2.join();

    cout << endl;
}

// === atomic ===
void atomic_demo() {
    cout << "=== atomic ===" << endl;

    atomic<int> counter{0};

    auto increment = [&](int times) {
        for (int i = 0; i < times; ++i) {
            counter.fetch_add(1, memory_order_relaxed);
        }
    };

    vector<thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(increment, 10000);
    }

    for (auto& t : threads) {
        t.join();
    }

    cout << "  Atomic counter (4 x 10000): " << counter.load() << endl;

    // compare_exchange
    atomic<int> value{10};
    int expected = 10;
    bool success = value.compare_exchange_strong(expected, 20);
    cout << "  compare_exchange (10 -> 20): " << boolalpha << success << endl;
    cout << "  New value: " << value.load() << endl;

    // atomic_flag (最も基本的な atomic)
    atomic_flag flag = ATOMIC_FLAG_INIT;
    bool was_set = flag.test_and_set();
    cout << "  test_and_set: " << was_set << endl;
    flag.clear();

    cout << endl;
}

// === スレッドプール（簡易実装） ===
class ThreadPool {
public:
    ThreadPool(size_t num_threads) : stop_(false) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this]() {
                while (true) {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(mtx_);
                        cv_.wait(lock, [this]() {
                            return stop_ || !tasks_.empty();
                        });
                        if (stop_ && tasks_.empty()) return;
                        task = move(tasks_.front());
                        tasks_.pop();
                    }
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            lock_guard<mutex> lock(mtx_);
            stop_ = true;
        }
        cv_.notify_all();
        for (auto& worker : workers_) {
            worker.join();
        }
    }

    template<typename F>
    future<void> enqueue(F&& f) {
        auto task = make_shared<packaged_task<void()>>(forward<F>(f));
        future<void> result = task->get_future();
        {
            lock_guard<mutex> lock(mtx_);
            tasks_.emplace([task]() { (*task)(); });
        }
        cv_.notify_one();
        return result;
    }

private:
    vector<thread> workers_;
    queue<function<void()>> tasks_;
    mutex mtx_;
    condition_variable cv_;
    bool stop_;
};

void thread_pool_demo() {
    cout << "=== スレッドプール ===" << endl;

    ThreadPool pool(4);
    vector<future<void>> futures;

    for (int i = 0; i < 8; ++i) {
        futures.push_back(pool.enqueue([i]() {
            cout << "  Task " << i << " running on thread "
                 << this_thread::get_id() << endl;
            this_thread::sleep_for(milliseconds(50));
        }));
    }

    for (auto& f : futures) {
        f.get();
    }

    cout << "  All tasks completed" << endl;

    cout << endl;
}

// === call_once ===
void call_once_demo() {
    cout << "=== call_once ===" << endl;

    once_flag flag;
    auto initialize = []() {
        cout << "  Initialization (only once)" << endl;
    };

    auto worker = [&]() {
        call_once(flag, initialize);
        cout << "  Worker thread: " << this_thread::get_id() << endl;
    };

    vector<thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(worker);
    }

    for (auto& t : threads) {
        t.join();
    }

    cout << endl;
}

int main() {
    cout << "=== Concurrency Demo ===" << endl << endl;

    basic_thread();
    mutex_demo();
    advanced_locks();
    shared_mutex_demo();
    condition_variable_demo();
    async_future_demo();
    promise_demo();
    atomic_demo();
    thread_pool_demo();
    call_once_demo();

    return 0;
}
