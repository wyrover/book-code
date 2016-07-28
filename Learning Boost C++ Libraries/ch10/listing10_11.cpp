#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <iostream>

int main()
{
    int x = 0;
    static const int max = 1000000;
    boost::mutex mtx;
    auto thrFunc = [&x, &mtx]() {
        for (int i = 0; i < max; ++i) {
            boost::lock_guard<boost::mutex> lg(mtx);
            ++x;
        }
    };
    boost::thread t1(thrFunc);
    boost::thread t2(thrFunc);
    t1.join();
    t2.join();
    std::cout << "Value of x: " << x << '\n';
}

