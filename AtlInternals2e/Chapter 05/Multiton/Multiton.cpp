// Multiton.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <atlbase.h>

class CAccount
{
public:
    void Audit() { }
    void Open() { }
    void Close() { }
    double Deposit(double dAmount)
    {
        return 0;
    }
    double Withdraw(double dAmount)
    {
        return 0;
    }
};

static CAccount s_account;
static CComAutoCriticalSection s_lock;

class CTeller
{
public:
    void Deposit(double dAmount)
    {
        s_lock.Lock();
        s_account.Deposit(dAmount);
        s_lock.Unlock();
    }
};

int _tmain(int argc, _TCHAR* argv[])
{
    return 0;
}

