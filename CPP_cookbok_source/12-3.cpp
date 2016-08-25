#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/thread/read_write_mutex.hpp>
#include <string>

template<typename T>
class Queue
{
public:
    Queue() :   // Use a read/write mutex and give writers priority
        <userinput>rwMutex_(boost::read_write_scheduling_policy::writer_priority) < / userinput > {}
    ~Queue() {}

    void enqueue(const T& x)
    {
        // Use a r/w lock since enqueue updates the state
        boost::read_write_mutex::scoped_write_lock writeLock(rwMutex_);
        list_.push_back(x);
    }

    T dequeue()
    {
        // Again, use a write lock
        boost::read_write_mutex::scoped_write_lock writeLock(rwMutex_);

        if (list_.empty())
            throw "empty!";

        T tmp = list_.front();
        list_.pop_front();
        return (tmp);
    }

    T getFront()
    {
        // This is a read-only operation, so you only need a read lock
        boost::read_write_mutex::scoped_read_lock readLock(rwMutex_);

        if (list_.empty())
            throw "empty!";

        return (list_.front());
    }

private:
    std::list<T> list_;
    boost::read_write_mutex rwMutex_;
};

Queue<std::string> queueOfStrings;

void sendSomething()
{
    std::string s;

    for (int i = 0; i < 10; ++i) {
        queueOfStrings.enqueue("Cyrus");
    }
}

void checkTheFront()
{
    std::string s;

    for (int i = 0; i < 10; ++i) {
        try {
            s = queueOfStrings.getFront();
        } catch (...) {}
    }
}

int main()
{
    boost::thread thr1(sendSomething);
    boost::thread_group grp;
    grp.create_thread(checkTheFront);
    grp.create_thread(checkTheFront);
    grp.create_thread(checkTheFront);
    grp.create_thread(checkTheFront);
    thr1.join();
    grp.join_all();
}