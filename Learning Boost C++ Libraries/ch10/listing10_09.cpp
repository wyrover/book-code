#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <ctime>
#include <cstdlib>

int main()
{
    int duration = 10 + rand() % 10;
    srand(time(0));
    std::cout << "Main thread id="
              << std::this_thread::get_id() << '\n';
    std::future<int> future =
        std::async(std::launch::async,
    [](int secs) -> int {
        std::cout << "Thread " << std::this_thread::get_id()
        << " sleeping for "
        << secs << " seconds\n";
        std::this_thread::sleep_for(
            std::chrono::seconds(secs));
        return secs;
    }, duration);
    size_t timeout_count = 0, secs = 2;

    while (future.wait_for(std::chrono::seconds(secs))
           == std::future_status::timeout) {
        std::cout << "Main thread timed out\n";
        ++timeout_count;
    }

    std::cout << "Launched task slept for "
              << future.get() << '\n';
    std::cout << "Timed out for " << timeout_count * secs
              << " seconds \n";
}

