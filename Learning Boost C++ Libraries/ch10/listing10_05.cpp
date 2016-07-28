#define BOOST_THREAD_PROVIDES_FUTURE
#include <boost/thread.hpp>
#include <boost/thread/future.hpp>
#include <boost/chrono.hpp>
#include <ctime>
#include <cassert>
#include <cstdlib>
#include <iostream>

int main()
{
    boost::promise<void> promise;
    boost::future<void> future = promise.get_future();
    std::cout << "Main thread id="
              << boost::this_thread::get_id() << '\n';
    boost::thread thr([&promise]() {
        srand(time(0));
        int secs = 10 + rand() % 10;
        std::cout << "Thread " << boost::this_thread::get_id()
                  << " sleeping for "
                  << secs << " seconds\n";
        boost::this_thread::sleep_for(
            boost::chrono::seconds(secs));
        promise.set_value();
    });
    size_t timeout_count = 0;
    size_t secs = 2;

    while (future.wait_for(boost::chrono::seconds(secs))
           == boost::future_status::timeout) {
        std::cout << "Main thread timed out\n";
        ++timeout_count;
    }

    assert(future.is_ready());
    assert(future.get_state() == boost::future_state::ready);
    std::cout << "Timed out for " << timeout_count * secs
              << " seconds \n";
    thr.join();
}

