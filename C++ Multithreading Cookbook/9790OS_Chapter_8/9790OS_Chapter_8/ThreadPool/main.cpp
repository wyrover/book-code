#include "main.h"
#include "CBankAccount.h"
#include "CThreadPool.h"

double Random(double dMin, double dMax)
{
    double dValue = (double)rand() / RAND_MAX;
    return dMin + dValue * (dMax - dMin);
}

int main(void)
{
    srand((unsigned)time(NULL));
#define ACCOUNTS_COUNT 12
    CBankAccount* accounts[ACCOUNTS_COUNT];

    for (unsigned uIndex = 0; uIndex < ACCOUNTS_COUNT; uIndex++) {
        accounts[uIndex] = new CBankAccount(Random(10, 1000));
    }

    CThreadPool* pool = new CThreadPool();
#define TASK_COUNT 5

    for (unsigned uIndex = 0; uIndex < TASK_COUNT; uIndex++) {
        int nFirstIndex = rand() % ACCOUNTS_COUNT;
        int nSecondIndex = -1;

        while ((nSecondIndex = rand() % ACCOUNTS_COUNT) == nFirstIndex) {}

        CParameters* params = new CParameters(accounts[nFirstIndex], accounts[nSecondIndex], Random(50, 200));
        pool->RequestThread(CBankAccount::Transfer)->Start(params);
    }

    pool->WaitAll();
    delete pool;
#ifdef _DEBUG
    return system("pause");
#endif
    return 0;
}
