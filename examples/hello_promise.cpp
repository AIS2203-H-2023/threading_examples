
#include <thread>
#include <future>
#include <iostream>

int main() {

    std::promise<int> promise;
    auto future = promise.get_future();

    std::jthread t([&] {

      std::this_thread::sleep_for(std::chrono::milliseconds(1000));

      promise.set_value(42);

      std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    });

    auto result = future.get(); // blocks until promise has a value
    std::cout << "Answer to the universe is " << result << std::endl;

    return 0;

}