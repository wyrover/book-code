#include <iostream>
#include <boost/thread/thread.hpp>
#include <string>

// A simple queue class; don't do this, use std::queue
template<typename T>
class Queue
{
public:
    Queue() {}
    ~Queue() {}

    void enqueue(const T& x)
    {
        // Lock the mutex for this queue
        boost::mutex::scoped_lock lock(mutex_);
        list_.push_back(x);
        // A scoped_lock is automatically destroyed (and thus unlocked)
        // when it goes out of scope
    }

    T dequeue()
    {
        boost::mutex::scoped_lock lock(mutex_);

        if (list_.empty())
            throw "empty!";     // This leaves the current scope, so the

        T tmp = list_.front();  // lock is released
        list_.pop_front();
        return (tmp);
    } // Again: when scope ends, mutex_ is unlocked

private:
    std::list<T> list_;
    boost::mutex mutex_;
};

Queue<std::string> queueOfStrings;

void sendSomething()
{
    std::string s;

    for (int i = 0; i < 10; ++i) {
        queueOfStrings.enqueue("Cyrus");
    }
}

void recvSomething()
{
    std::string s;

    for (int i = 0; i < 10; ++i) {
        try {
            s = queueOfStrings.dequeue();
        } catch (...) {}
    }
}

int main()
{
    boost::thread thr1(sendSomething);
    boost::thread thr2(recvSomething);
    thr1.join();
    thr2.join();
}