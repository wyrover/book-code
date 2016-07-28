//
//  Mutex.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "Mutex.h"

#include "Thread.h"

#include <pthread.h>
#include <cstdlib>
#include <vector>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

struct MutexData {
    pthread_mutex_t m_mutex;
};

Mutex::Mutex()
: m_data(new MutexData)
{
    pthread_mutex_init(&m_data->m_mutex, NULL);
}

Mutex::~Mutex()
{
    if (m_data)
    {
        pthread_mutex_destroy(&m_data->m_mutex);
        delete m_data;
    }
}

void Mutex::lock()
{
    pthread_mutex_lock(&(m_data->m_mutex));
//    cout << " &&&& locked \n";
}

void Mutex::unlock()
{
//    cout << " ^^^^ unlocked \n";
    pthread_mutex_unlock(&(m_data->m_mutex));
}


/// ----

MutexAccess::MutexAccess(Mutex &m)
: m_mutex(m)
{
    m_mutex.lock();
}

MutexAccess::~MutexAccess()
{
    m_mutex.unlock();
}


/// ----

class MutexTestThread  : public Thread {
public:
    MutexTestThread(double &result, double incVal);
    ~MutexTestThread();

    void run();
private:
    double &m_result;
    double m_incValue;

    static Mutex m_globalMutex;
};

Mutex MutexTestThread::m_globalMutex;

MutexTestThread::MutexTestThread(double &result, double incVal)
: m_result(result),
  m_incValue(incVal)
{
}

MutexTestThread::~MutexTestThread()
{
}

void MutexTestThread::run()
{
    MutexAccess maccess(m_globalMutex); // mutex is locked here
    cout << " accessing data " << endl; cout.flush();
    if (m_result  > m_incValue)
    {
        m_result -= m_incValue;
    }
    else
    {
        m_incValue += m_incValue;
    }

    // mutex is automatically unlocked
}

int main_mutex()
{
    int nThreads = 10;

    vector<Thread*> threads(nThreads);
    double price = rand() % 25;

    for (int i=0; i<nThreads; ++i)
    {
        threads[i] = new MutexTestThread(price, (double)(rand() % 10));
        threads[i]->setJoinable(true);
        threads[i]->start();
    }
    for (int i=0; i<nThreads; ++i)
    {
        threads[i]->join();
    }

    cout << " final price is " << price << endl;
    return 0;
}