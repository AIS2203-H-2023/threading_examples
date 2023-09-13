
#ifndef THREADING_CPP_THREAD_POOL_HPP
#define THREADING_CPP_THREAD_POOL_HPP

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace ais2203 {

    class thread_pool {

    public:
        thread_pool() : done(false) {

            unsigned const thread_count = std::thread::hardware_concurrency();
            for (unsigned i = 0; i < thread_count; ++i) {
                threads.emplace_back(&thread_pool::worker_thread, this);
            }
        }

        void wait_for_tasks_to_finish() {

            std::unique_lock<std::mutex> lck(mutex);
            cv.wait(lck, [this] { return work_queue.empty(); });
            lck.unlock();

            finalize();
        }

        ~thread_pool() {
            finalize();
        }

        template<typename FunctionType>
        void submit(FunctionType f) {
            work_queue.push(std::function<void()>(f));
        }

    private:
        std::atomic_bool done;
        std::queue<std::function<void()>> work_queue;
        std::vector<std::thread> threads;
        std::mutex mutex;
        std::condition_variable cv;

        void worker_thread() {
            while (!done) {

                std::unique_lock<std::mutex> lock(mutex);
                if (!work_queue.empty()) {

                    auto task = work_queue.front();
                    work_queue.pop();
                    lock.unlock();

                    task();

                    cv.notify_one();

                } else {
                    std::this_thread::yield();
                }
            }
        }

        void finalize() {
            done = true;

            for (auto &thread : threads) {
                if (thread.joinable()) {
                    thread.join();
                }
            }
        }
    };

}// namespace ais2203

#endif//THREADING_CPP_THREAD_POOL_HPP
