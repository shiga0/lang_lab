#!/usr/bin/env python3
"""並行処理 - Python 実装

Python の並行処理: threading, multiprocessing, asyncio
GIL (Global Interpreter Lock) の影響を理解することが重要。
"""

import asyncio
import threading
import multiprocessing
import queue
import time
from concurrent.futures import ThreadPoolExecutor, ProcessPoolExecutor
from typing import List

print("=== Concurrency Demo ===\n")


# --- threading 基礎 ---
def threading_basics() -> None:
    print("--- threading 基礎 ---")

    def worker(name: str, delay: float) -> None:
        print(f"  {name} started")
        time.sleep(delay)
        print(f"  {name} finished")

    # スレッドの作成と実行
    t1 = threading.Thread(target=worker, args=("Thread-1", 0.1))
    t2 = threading.Thread(target=worker, args=("Thread-2", 0.1))

    t1.start()
    t2.start()

    t1.join()
    t2.join()

    print()


# --- Lock (排他制御) ---
def lock_demo() -> None:
    print("--- Lock (排他制御) ---")

    counter = 0
    lock = threading.Lock()

    def increment(n: int) -> None:
        nonlocal counter
        for _ in range(n):
            with lock:
                counter += 1

    threads = [threading.Thread(target=increment, args=(10000,)) for _ in range(5)]
    for t in threads:
        t.start()
    for t in threads:
        t.join()

    print(f"  Counter (with lock): {counter}")

    # Lock なしだと競合する可能性
    counter_unsafe = 0

    def increment_unsafe(n: int) -> None:
        nonlocal counter_unsafe
        for _ in range(n):
            counter_unsafe += 1

    threads = [threading.Thread(target=increment_unsafe, args=(10000,)) for _ in range(5)]
    for t in threads:
        t.start()
    for t in threads:
        t.join()

    print(f"  Counter (no lock): {counter_unsafe} (may be less than 50000)")

    print()


# --- Queue (スレッドセーフなキュー) ---
def queue_demo() -> None:
    print("--- Queue ---")

    q: queue.Queue[int] = queue.Queue()

    def producer(items: List[int]) -> None:
        for item in items:
            q.put(item)
            print(f"  Produced: {item}")
            time.sleep(0.01)
        q.put(None)  # 終了シグナル

    def consumer() -> None:
        while True:
            item = q.get()
            if item is None:
                break
            print(f"  Consumed: {item}")
            q.task_done()

    prod = threading.Thread(target=producer, args=([1, 2, 3, 4, 5],))
    cons = threading.Thread(target=consumer)

    prod.start()
    cons.start()

    prod.join()
    cons.join()

    print()


# --- ThreadPoolExecutor ---
def thread_pool_demo() -> None:
    print("--- ThreadPoolExecutor ---")

    def fetch(url: str) -> str:
        time.sleep(0.1)  # 模擬的な I/O
        return f"Data from {url}"

    urls = ["url1", "url2", "url3", "url4"]

    # 並列実行
    with ThreadPoolExecutor(max_workers=4) as executor:
        results = list(executor.map(fetch, urls))

    for result in results:
        print(f"  {result}")

    # submit と Future
    print("\n  Using submit:")
    with ThreadPoolExecutor(max_workers=2) as executor:
        futures = [executor.submit(fetch, url) for url in urls[:2]]
        for future in futures:
            print(f"  {future.result()}")

    print()


# --- multiprocessing ---
def multiprocessing_demo() -> None:
    print("--- multiprocessing ---")

    def cpu_bound_task(n: int) -> int:
        """CPU バウンドな処理"""
        total = 0
        for i in range(n):
            total += i * i
        return total

    # GIL を回避して真の並列処理
    with ProcessPoolExecutor(max_workers=2) as executor:
        results = list(executor.map(cpu_bound_task, [100000, 100000]))

    print(f"  Results: {results}")

    print()


# --- asyncio 基礎 ---
async def asyncio_basics() -> None:
    print("--- asyncio 基礎 ---")

    async def fetch_data(name: str, delay: float) -> str:
        print(f"  {name} started")
        await asyncio.sleep(delay)
        print(f"  {name} finished")
        return f"Data from {name}"

    # 単一の await
    result = await fetch_data("Task-1", 0.1)
    print(f"  Result: {result}")

    # 並行実行
    print("\n  Concurrent execution:")
    results = await asyncio.gather(
        fetch_data("Task-A", 0.1),
        fetch_data("Task-B", 0.1),
        fetch_data("Task-C", 0.1),
    )
    print(f"  Results: {results}")

    print()


# --- asyncio タスク ---
async def asyncio_tasks() -> None:
    print("--- asyncio タスク ---")

    async def background_task(name: str) -> str:
        await asyncio.sleep(0.1)
        return f"Completed: {name}"

    # タスクの作成
    task1 = asyncio.create_task(background_task("Task-1"))
    task2 = asyncio.create_task(background_task("Task-2"))

    # 他の処理をしながらタスクが実行される
    print("  Tasks created, doing other work...")
    await asyncio.sleep(0.05)
    print("  Still working...")

    # 結果を待つ
    results = await asyncio.gather(task1, task2)
    print(f"  Results: {results}")

    print()


# --- asyncio キャンセル ---
async def asyncio_cancellation() -> None:
    print("--- asyncio キャンセル ---")

    async def long_running_task() -> str:
        try:
            await asyncio.sleep(10)
            return "completed"
        except asyncio.CancelledError:
            print("  Task was cancelled")
            raise

    task = asyncio.create_task(long_running_task())

    # 0.1秒後にキャンセル
    await asyncio.sleep(0.1)
    task.cancel()

    try:
        await task
    except asyncio.CancelledError:
        print("  Caught cancellation")

    print()


# --- asyncio タイムアウト ---
async def asyncio_timeout() -> None:
    print("--- asyncio タイムアウト ---")

    async def slow_operation() -> str:
        await asyncio.sleep(1)
        return "done"

    try:
        result = await asyncio.wait_for(slow_operation(), timeout=0.1)
        print(f"  Result: {result}")
    except asyncio.TimeoutError:
        print("  Operation timed out")

    print()


# --- asyncio セマフォ ---
async def asyncio_semaphore() -> None:
    print("--- asyncio セマフォ ---")

    semaphore = asyncio.Semaphore(2)  # 同時に2つまで

    async def limited_task(name: str) -> str:
        async with semaphore:
            print(f"  {name} acquired semaphore")
            await asyncio.sleep(0.1)
            print(f"  {name} releasing semaphore")
            return name

    results = await asyncio.gather(
        limited_task("Task-1"),
        limited_task("Task-2"),
        limited_task("Task-3"),
        limited_task("Task-4"),
    )
    print(f"  Results: {results}")

    print()


# --- async ジェネレータ ---
async def async_generators() -> None:
    print("--- async ジェネレータ ---")

    async def async_range(start: int, end: int):
        for i in range(start, end):
            await asyncio.sleep(0.02)
            yield i

    print("  async for:")
    async for i in async_range(0, 5):
        print(f"    {i}")

    # async リスト内包表記
    result = [i async for i in async_range(0, 5)]
    print(f"  async comprehension: {result}")

    print()


# --- Event ---
async def asyncio_event() -> None:
    print("--- asyncio Event ---")

    event = asyncio.Event()

    async def waiter(name: str) -> None:
        print(f"  {name} waiting for event...")
        await event.wait()
        print(f"  {name} got event!")

    async def setter() -> None:
        await asyncio.sleep(0.1)
        print("  Setting event")
        event.set()

    await asyncio.gather(
        waiter("Waiter-1"),
        waiter("Waiter-2"),
        setter(),
    )

    print()


# --- GIL の説明 ---
def gil_explanation() -> None:
    print("--- GIL (Global Interpreter Lock) ---")

    print("""
  GIL (Global Interpreter Lock):
  - CPython の実装上の制約
  - 一度に1つのスレッドしか Python バイトコードを実行できない
  - CPU バウンドな処理では並列化の恩恵を受けにくい

  回避策:
  1. multiprocessing: 別プロセスで実行 (GIL は各プロセスに1つ)
  2. asyncio: I/O 待ちの間に他のタスクを実行
  3. C 拡張: GIL を解放して処理
  4. 別の実装: PyPy, Jython, etc.

  GIL が問題にならないケース:
  - I/O バウンドな処理 (ネットワーク、ファイル)
  - time.sleep() 中は GIL が解放される
  - numpy など GIL を解放する C 拡張
""")


# --- 実行 ---
async def async_main() -> None:
    await asyncio_basics()
    await asyncio_tasks()
    await asyncio_cancellation()
    await asyncio_timeout()
    await asyncio_semaphore()
    await async_generators()
    await asyncio_event()


if __name__ == "__main__":
    threading_basics()
    lock_demo()
    queue_demo()
    thread_pool_demo()
    multiprocessing_demo()

    asyncio.run(async_main())

    gil_explanation()
