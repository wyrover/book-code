#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/thread/once.hpp>

// Some sort of connection class that should only be initialized once
struct Conn {
    static void init()
    {
        ++i_;
    }
    static boost::once_flag init_;
    static int i_;
    // ...
};

int Conn::i_ = 0;
boost::once_flag Conn::init_ = BOOST_ONCE_INIT;

void worker()
{
    boost::call_once(Conn::init, Conn::init_);
    // Do the real work...
}

Conn c;  // You probably don't want to use a global, so see the
// next Recipe

int main()
{
    boost::thread_group grp;

    for (int i = 0; i < 100; ++i)
        grp.create_thread(worker);

    grp.join_all();
    std::cout << c.i_ << '\n'; // c.i_ = 1
}