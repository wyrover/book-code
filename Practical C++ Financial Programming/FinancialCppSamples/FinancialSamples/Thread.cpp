//
//  Thread.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "Thread.h"

#include <pthread.h>
#include <iostream>

using std::cout;
using std::endl;


struct ThreadData {
    pthread_t m_thread;
    pthread_attr_t m_attr;
};

namespace {

    void *thread_function(void *data)
    {
        Thread *t = reinterpret_cast<Thread*>(data);
        t->run();
        return nullptr;
    }
}

Thread::Thread()
: m_data(new ThreadData),
  m_joinable(false)
{
    pthread_attr_init(&m_data->m_attr);
}

Thread::~Thread()
{
    if (m_data)
    {
        delete m_data;
    }
}


void Thread::start()
{
    pthread_create(&m_data->m_thread, &m_data->m_attr, thread_function, this);
}

void Thread::endThread()
{
    pthread_exit(nullptr);
}

void Thread::setJoinable(bool yes)
{
    pthread_attr_setdetachstate(&m_data->m_attr,
                                yes ? PTHREAD_CREATE_JOINABLE : PTHREAD_CREATE_DETACHED);
    m_joinable = yes;
}

void Thread::run()
{
    cout << " no concreate implementation provided " << endl;
}

void Thread::join()
{
    if (!m_joinable)
    {
        cout << " thread cannot be joined " << endl;
    }
    else
    {
        void *result;
        pthread_join(m_data->m_thread, &result);
    }
}


// --- sample implementation

class TestThread : public Thread {
public:
    virtual void run();
};

void TestThread::run()
{
    cout << " this is a test implementation " << endl;
    endThread();
}

int main_thread()
{
    Thread *myThread = new TestThread;
    myThread->setJoinable(true);
    myThread->start();
    myThread->join();
    return 0;
}

