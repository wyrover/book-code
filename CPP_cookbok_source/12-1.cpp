#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>

struct MyThreadFunc {
    void operator()()
    {
        // Do something long-running...
    }
} threadFun;

int main()
{
    boost::thread myThread(threadFun); // Create a thread that starts
    // running threadFun
    boost::thread::yield();  // Give up the main thread's timeslice
    // so the child thread can get some work
    // done.
    // Go do some other work...
    myThread.join();  // The current (i.e., main) thread will wait
    // for myThread to finish before it returns
}