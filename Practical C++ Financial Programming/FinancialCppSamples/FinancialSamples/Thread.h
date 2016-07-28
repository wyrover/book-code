//
//  Thread.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__Thread__
#define __FinancialSamples__Thread__

struct ThreadData;

class Thread {
public:
    Thread();
    virtual ~Thread();
private:
    Thread(const Thread &p); // no copy allowed
    Thread &operator=(const Thread &p); // no assignment allowed

public:
    virtual void run() = 0;
    void start();
    void endThread();
    void setJoinable(bool yes);
    void join();

private:
    ThreadData *m_data;
    bool m_joinable;
};


#endif /* defined(__FinancialSamples__Thread__) */
