//
//  Mutex.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__Mutex__
#define __FinancialSamples__Mutex__

struct MutexData;

class Mutex {
public:
    Mutex();
    ~Mutex();

    void lock();
    void unlock();
private:
    Mutex(const Mutex &p);  // copy not allowed
    Mutex &operator=(const Mutex &p);  // assignment not allowed

    MutexData *m_data;
};


class MutexAccess {
public:
    MutexAccess(Mutex &m);
    ~MutexAccess();
private:
    MutexAccess &operator=(const MutexAccess &p);
    MutexAccess(const MutexAccess &p);

    Mutex &m_mutex;
};


#endif /* defined(__FinancialSamples__Mutex__) */
