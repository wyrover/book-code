#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <list>
#include <string>

class Request
{
    /*...*/
};

// A simple job queue class; don't do this, use std::queue
template<typename T>
class JobQueue
{
public:
    JobQueue() {}
    ~JobQueue() {}

    void submitJob(const T& x)
    {
        boost::mutex::scoped_lock lock(mutex_);
        list_.push_back(x);
        workToBeDone_.notify_one();
    }

    T getJob()
    {
        boost::mutex::scoped_lock lock(mutex_);
        workToBeDone_.wait(lock); // Wait until this condition is
        // satisfied, then lock the mutex
        T tmp = list_.front();
        list_.pop_front();
        return (tmp);
    }

private:
    std::list<T> list_;
    boost::mutex mutex_;
    boost::condition workToBeDone_;
};

JobQueue<Request> myJobQueue;

void boss()
{
    for (;;) {
        // Get the request from somewhere
        Request req;
        myJobQueue.submitJob(req);
    }
}

void worker()
{
    for (;;) {
        Request r(myJobQueue.getJob());
        // Do something with the job...
    }
}

int main()
{
    boost::thread thr1(boss);
    boost::thread thr2(worker);
    boost::thread thr3(worker);
    thr1.join();
    thr2.join();
    thr3.join();
}