// condition_variable example
#include <condition_variable>// std::condition_variable
#include <iostream>          // std::cout
#include <mutex>             // std::mutex, std::unique_lock
#include <thread>            // std::thread
#include <vector>

namespace {
    std::mutex mtx;
    std::condition_variable cv;
    bool ready = false;

    void print_id(int id) {
        std::unique_lock<std::mutex> lck(mtx);
        while (!ready) cv.wait(lck);
        // ...
        std::cout << "thread " << id << '\n';
    }

    void go() {
        {
            std::unique_lock<std::mutex> lck(mtx);
            ready = true;
        }
        cv.notify_all();
    }
}// namespace

int main() {

    int numThreads = 10;
    std::vector<std::jthread> threads;
    threads.reserve(numThreads);

    for (int i = 0; i < numThreads; ++i)
        threads.emplace_back(print_id, i);

    std::cout << numThreads << " threads ready to race...\n";
    go();// go!

    return 0;
}
