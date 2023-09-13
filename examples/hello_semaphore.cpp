
#include "ais2203/semaphore.hpp"

#include <iostream>
#include <thread>

int main() {

    std::mutex m;
    ais2203::semaphore semaphore(5);

    const int num_threads = 100;
    std::thread threads[num_threads];
    for (auto i = 0; i < num_threads; i++) {
        threads[i] = std::thread([&m, &semaphore, i] {
            semaphore.acquire();

            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            std::unique_lock<std::mutex> lock(m);
            std::cout << "hello from thread " << i << ", count=" << semaphore.getCount() << std::endl;
            lock.unlock();

            semaphore.release();
        });
    }

    for (auto &t : threads) {
        t.join();
    }

    return 0;
}