#ifndef __BANKACCOUNT__
#define __BANKACCOUNT__

#include "main.h"

class CLock
{
public:
    CLock(TCHAR* szMutexName);
    ~CLock();
private:
    HANDLE hMutex;
};

inline CLock::CLock(TCHAR* szMutexName)
{
    hMutex = CreateMutex(NULL, FALSE, szMutexName);
    WaitForSingleObject(hMutex, INFINITE);
}

inline CLock::~CLock()
{
    ReleaseMutex(hMutex);
    CloseHandle(hMutex);
}

class CBankAccount;

class CParameters
{
public:
    CParameters(CBankAccount* fromAccount, CBankAccount* toAccount, double dAmount, bool bPrintOutput = true)
    {
        this->fromAccount = fromAccount;
        this->toAccount = toAccount;
        this->dAmount = dAmount;
        this->bPrintOutput = bPrintOutput;
    }
    CBankAccount* fromAccount;
    CBankAccount* toAccount;
    double dAmount;
    bool bPrintOutput;
};

class CBankAccount
{
public:
    CBankAccount(double dBalance) : dBalance(dBalance)
    {
        uID = NewId();
        *szLock = 0;
        _tcscpy_s(szLock, LockName());
    }
    CBankAccount(double dBalance, TCHAR* szLockName) : dBalance(dBalance)
    {
        uID = NewId();
        *szLock = 0;
        _tcscpy_s(szLock, szLockName);
    }
    static DWORD WINAPI Transfer(LPVOID lpParameter);
    double& Balance()
    {
        return dBalance;
    }
    unsigned AccountID() const
    {
        return uID;
    }
    TCHAR* LockName(void);
private:
    unsigned uID;
    double dBalance;
    TCHAR szLock[32];
    static unsigned NewId()
    {
        static unsigned uSeed = 61524;
        return uSeed++;
    }
};

TCHAR* CBankAccount::LockName(void)
{
    static int iCount = 0;
    static TCHAR szBuffer[32];

    if (*szLock == 0) {
        wsprintf(szBuffer, _T("_lock_%d_"), ++iCount);
    } else {
        return szLock;
    }

    return szBuffer;
}

DWORD WINAPI CBankAccount::Transfer(LPVOID lpParameter)
{
    CParameters* parameters = (CParameters*)lpParameter;
    Sleep(100);
    CLock* outerLock = new CLock(parameters->fromAccount->szLock);

    if (parameters->dAmount < parameters->fromAccount->dBalance) {
        CLock* innerLock = new CLock(parameters->toAccount->szLock);
        parameters->fromAccount->dBalance -= parameters->dAmount;
        parameters->toAccount->dBalance += parameters->dAmount;
        delete innerLock;
        delete outerLock;

        if (parameters->bPrintOutput) {
            _tprintf_s(_T("%ws\n%ws\t\t%8.2lf\n%ws\t%8u\n%ws\t%8.2lf\n%ws\t%8u\n%ws\t%8.2lf\n\n"),
                       L"Transfer succeeded.",
                       L"Amount:", parameters->dAmount,
                       L"From account:", parameters->fromAccount->AccountID(),
                       L"Balance:", parameters->fromAccount->Balance(),
                       L"To account:", parameters->toAccount->AccountID(),
                       L"Balance:", parameters->toAccount->Balance());
        }

        delete parameters;
        return 1;
    }

    delete outerLock;

    if (parameters->bPrintOutput) {
        _tprintf_s(_T("%ws\n%ws\t\t%8.2lf\n%ws\t%8u\n%ws\t%8.2lf\n%ws\t%8u\n%ws\t%8.2lf\n%ws\n\n"),
                   L"Transfer failed.",
                   L"Amount:", parameters->dAmount,
                   L"From account:", parameters->fromAccount->AccountID(),
                   L"Balance:", parameters->fromAccount->Balance(),
                   L"To account:", parameters->toAccount->AccountID(),
                   L"Balance:", parameters->toAccount->Balance(),
                   L"Not enough funds!");
    }

    delete parameters;
    return 0;
}

#endif
