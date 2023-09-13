#include "ais2203/random_gen.hpp"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <queue>
#include <thread>

namespace {

    const int MAX_QUEUE_SIZE = 10;
    const int NUM_PRODUCERS = 2;
    const int NUM_CONSUMERS = 5;
    const int FINAL_NUM_RESULTS = 50;

    std::queue<int> queue;
    std::vector<int> result;
    std::atomic<bool> stop = false;

    std::mutex queue_mutex;
    std::mutex result_mutex;

    std::condition_variable read_condition, write_condition;
    std::condition_variable result_condition;

    void producer() {

        while (!stop) {

            std::unique_lock<std::mutex> lck(queue_mutex);
            write_condition.wait(lck, [&] { return queue.size() < MAX_QUEUE_SIZE; });

            if (!stop) {
                //simulate work
                auto sleep = std::chrono::milliseconds(ais2203::rand(1, 5));
                std::this_thread::sleep_for(sleep);//1-5 ms sleep
                queue.push(0);

                lck.unlock();
                read_condition.notify_all();
            }
        }
    }

    void consumer() {

        while (!stop) {

            std::unique_lock<std::mutex> lck(queue_mutex);
            read_condition.wait(lck, [&] { return !queue.empty(); });

            auto value = queue.back();
            queue.pop();

            lck.unlock();
            write_condition.notify_all();

            // simulate work
            auto sleep = std::chrono::milliseconds(ais2203::rand(1, 10));
            std::this_thread::sleep_for(sleep);//1-10 ms sleep

            std::lock_guard<std::mutex> l(result_mutex);
            if (result.size() < FINAL_NUM_RESULTS) {
                result.emplace_back(1);
                result_condition.notify_one();
            }
        }
    }

}// namespace

int main() {

    std::vector<std::jthread> producerThreads;
    std::vector<std::jthread> consumerThreads;

    producerThreads.reserve(NUM_PRODUCERS);
    consumerThreads.reserve(NUM_CONSUMERS);

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        producerThreads.emplace_back(&producer);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        consumerThreads.emplace_back(&consumer);
    }

    {
        std::unique_lock<std::mutex> lck(result_mutex);
        result_condition.wait(lck, [] {
            return result.size() == FINAL_NUM_RESULTS;
        });
    }
    stop = true;

    return result.size() != FINAL_NUM_RESULTS;
}
