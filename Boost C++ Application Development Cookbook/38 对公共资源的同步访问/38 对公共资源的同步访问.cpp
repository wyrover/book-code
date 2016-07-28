#include <cassert>
#include <cstddef>

// 在秘笈37中包含了<boost/thread.hpp>, 它包含了Boost.Thread中的所有类
#include <boost/thread/thread.hpp>

namespace without_sync
{
int shared_i = 0;

void do_inc()
{
    for (std::size_t i = 0; i < 30000; ++i) {
        // 做一些工作
        // ...
        int i_snapshot = ++shared_i;
        // 做一些与i_snapshot有关的工作
        // ...
        (void)i_snapshot;
    }
}

void do_dec()
{
    for (std::size_t i = 0; i < 30000; ++i) {
        // 做一些工作
        // ...
        int i_snapshot = --shared_i;
        // 做一些与i_snapshot有关的工作
        // ...
        (void)i_snapshot;
    }
}

void run()
{
    boost::thread t1(&do_inc);
    boost::thread t2(&do_dec);
    t1.join();
    t2.join();
    //assert(shared_i == 0); // Oops!
    std::cout << "shared_i == " << shared_i;
}
} // namespace without_sync

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>

namespace with_sync
{
int shared_i = 0;
boost::mutex i_mutex;

void do_inc()
{
    for (std::size_t i = 0; i < 30000; ++i) {
        // 做一些工作
        // ...
        int i_snapshot;
        {
            // 临界区开始
            boost::lock_guard<boost::mutex> lock(i_mutex);
            i_snapshot = ++shared_i;
        }// 临界区结束
        // 做一些与i_snapshot有关的工作
        // ...
        (void)i_snapshot;
    }
}

void do_dec()
{
    for (std::size_t i = 0; i < 30000; ++i) {
        // 做一些工作
        // ...
        int i_snapshot;
        {
            // 临界区开始
            boost::lock_guard<boost::mutex> lock(i_mutex);
            i_snapshot = --shared_i;
        }// 临界区结束
        // 做一些与i_snapshot有关的工作
        // ...
        (void)i_snapshot;
    }
}
void run()
{
    boost::thread t1(&do_inc);
    boost::thread t2(&do_dec);
    t1.join();
    t2.join();
    assert(shared_i == 0);
    std::cout << "shared_i == " << shared_i;
}
} // namespace with_sync

#include <boost/thread/recursive_mutex.hpp>

int main()
{
    without_sync::run();
    std::cout << '\n';
    with_sync::run();
    std::cout << '\n';
    system("pause");
    return 0;
}