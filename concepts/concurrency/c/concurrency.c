// 並行処理 - C 実装
//
// C11 で <threads.h> が標準化されたが、実装によってサポートが異なる。
// POSIX threads (pthread) が実質的な標準として広く使われる。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

// === 基本的なスレッド ===
void* simple_thread(void *arg) {
    int id = *(int*)arg;
    printf("Thread %d: Hello from thread!\n", id);
    sleep(1);
    printf("Thread %d: Goodbye!\n", id);
    return NULL;
}

void basic_thread_demo(void) {
    printf("--- 基本的なスレッド ---\n");

    pthread_t thread;
    int id = 1;

    // スレッド作成
    if (pthread_create(&thread, NULL, simple_thread, &id) != 0) {
        perror("pthread_create");
        return;
    }

    printf("Main: Thread created\n");

    // スレッド終了を待つ
    pthread_join(thread, NULL);

    printf("Main: Thread joined\n\n");
}

// === 複数スレッド ===
#define NUM_THREADS 4

void* numbered_thread(void *arg) {
    int id = *(int*)arg;
    printf("Thread %d started\n", id);
    usleep(100000 * (id + 1));  // 0.1秒 * (id+1)
    printf("Thread %d finished\n", id);
    return NULL;
}

void multiple_threads_demo(void) {
    printf("--- 複数スレッド ---\n");

    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    // スレッドを作成
    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, numbered_thread, &ids[i]);
    }

    // すべてのスレッドを待つ
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads finished\n\n");
}

// === Mutex ===
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
int shared_counter = 0;

void* increment_thread(void *arg) {
    int iterations = *(int*)arg;

    for (int i = 0; i < iterations; i++) {
        pthread_mutex_lock(&counter_mutex);
        shared_counter++;
        pthread_mutex_unlock(&counter_mutex);
    }

    return NULL;
}

void mutex_demo(void) {
    printf("--- Mutex ---\n");

    shared_counter = 0;
    int iterations = 100000;

    pthread_t t1, t2;
    pthread_create(&t1, NULL, increment_thread, &iterations);
    pthread_create(&t2, NULL, increment_thread, &iterations);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Expected: %d, Actual: %d\n", iterations * 2, shared_counter);
    printf("\n");
}

// === データ競合の例 ===
int unsafe_counter = 0;

void* unsafe_increment(void *arg) {
    int iterations = *(int*)arg;

    for (int i = 0; i < iterations; i++) {
        unsafe_counter++;  // 競合状態！
    }

    return NULL;
}

void race_condition_demo(void) {
    printf("--- データ競合（警告：安全でない例）---\n");

    unsafe_counter = 0;
    int iterations = 100000;

    pthread_t t1, t2;
    pthread_create(&t1, NULL, unsafe_increment, &iterations);
    pthread_create(&t2, NULL, unsafe_increment, &iterations);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Expected: %d, Actual: %d (may vary due to race condition)\n",
           iterations * 2, unsafe_counter);
    printf("\n");
}

// === 条件変数 ===
pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
bool ready = false;

void* producer(void *arg) {
    (void)arg;
    sleep(1);

    pthread_mutex_lock(&cond_mutex);
    printf("Producer: Preparing data...\n");
    ready = true;
    pthread_cond_signal(&cond_var);
    pthread_mutex_unlock(&cond_mutex);

    printf("Producer: Signal sent\n");
    return NULL;
}

void* consumer(void *arg) {
    (void)arg;

    pthread_mutex_lock(&cond_mutex);
    printf("Consumer: Waiting for data...\n");

    while (!ready) {
        pthread_cond_wait(&cond_var, &cond_mutex);
    }

    printf("Consumer: Data received!\n");
    pthread_mutex_unlock(&cond_mutex);

    return NULL;
}

void condition_variable_demo(void) {
    printf("--- 条件変数 ---\n");

    ready = false;

    pthread_t prod, cons;
    pthread_create(&cons, NULL, consumer, NULL);
    pthread_create(&prod, NULL, producer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    printf("\n");
}

// === スレッドプール（シンプルな実装）===
#define POOL_SIZE 3
#define QUEUE_SIZE 10

typedef struct {
    void (*function)(void*);
    void *arg;
} Task;

typedef struct {
    pthread_t threads[POOL_SIZE];
    Task queue[QUEUE_SIZE];
    int queue_head;
    int queue_tail;
    int queue_count;

    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;

    bool shutdown;
} ThreadPool;

void* pool_worker(void *arg) {
    ThreadPool *pool = (ThreadPool*)arg;

    while (true) {
        pthread_mutex_lock(&pool->mutex);

        while (pool->queue_count == 0 && !pool->shutdown) {
            pthread_cond_wait(&pool->not_empty, &pool->mutex);
        }

        if (pool->shutdown && pool->queue_count == 0) {
            pthread_mutex_unlock(&pool->mutex);
            break;
        }

        Task task = pool->queue[pool->queue_head];
        pool->queue_head = (pool->queue_head + 1) % QUEUE_SIZE;
        pool->queue_count--;

        pthread_cond_signal(&pool->not_full);
        pthread_mutex_unlock(&pool->mutex);

        task.function(task.arg);
    }

    return NULL;
}

void pool_init(ThreadPool *pool) {
    pool->queue_head = 0;
    pool->queue_tail = 0;
    pool->queue_count = 0;
    pool->shutdown = false;

    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->not_empty, NULL);
    pthread_cond_init(&pool->not_full, NULL);

    for (int i = 0; i < POOL_SIZE; i++) {
        pthread_create(&pool->threads[i], NULL, pool_worker, pool);
    }
}

void pool_submit(ThreadPool *pool, void (*function)(void*), void *arg) {
    pthread_mutex_lock(&pool->mutex);

    while (pool->queue_count == QUEUE_SIZE) {
        pthread_cond_wait(&pool->not_full, &pool->mutex);
    }

    pool->queue[pool->queue_tail].function = function;
    pool->queue[pool->queue_tail].arg = arg;
    pool->queue_tail = (pool->queue_tail + 1) % QUEUE_SIZE;
    pool->queue_count++;

    pthread_cond_signal(&pool->not_empty);
    pthread_mutex_unlock(&pool->mutex);
}

void pool_shutdown(ThreadPool *pool) {
    pthread_mutex_lock(&pool->mutex);
    pool->shutdown = true;
    pthread_cond_broadcast(&pool->not_empty);
    pthread_mutex_unlock(&pool->mutex);

    for (int i = 0; i < POOL_SIZE; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->not_empty);
    pthread_cond_destroy(&pool->not_full);
}

void example_task(void *arg) {
    int id = *(int*)arg;
    printf("Task %d executing on thread %lu\n", id, (unsigned long)pthread_self());
    usleep(100000);
}

void thread_pool_demo(void) {
    printf("--- スレッドプール ---\n");

    ThreadPool pool;
    pool_init(&pool);

    int task_ids[6];
    for (int i = 0; i < 6; i++) {
        task_ids[i] = i;
        pool_submit(&pool, example_task, &task_ids[i]);
    }

    sleep(1);  // タスク完了を待つ

    pool_shutdown(&pool);

    printf("Thread pool shut down\n\n");
}

// === スレッドローカルストレージ ===
__thread int thread_local_var = 0;

void* tls_thread(void *arg) {
    int id = *(int*)arg;
    thread_local_var = id * 100;
    printf("Thread %d: thread_local_var = %d\n", id, thread_local_var);
    return NULL;
}

void thread_local_demo(void) {
    printf("--- スレッドローカルストレージ ---\n");

    pthread_t threads[3];
    int ids[3] = {1, 2, 3};

    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, tls_thread, &ids[i]);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Main: thread_local_var = %d (main thread's value)\n", thread_local_var);
    printf("\n");
}

// === メイン ===
int main(void) {
    printf("=== Concurrency Demo ===\n\n");

    basic_thread_demo();
    multiple_threads_demo();
    mutex_demo();
    race_condition_demo();
    condition_variable_demo();
    thread_pool_demo();
    thread_local_demo();

    return 0;
}

/*
コンパイル:
    clang -std=c11 -pthread concurrency.c -o concurrency && ./concurrency
*/
