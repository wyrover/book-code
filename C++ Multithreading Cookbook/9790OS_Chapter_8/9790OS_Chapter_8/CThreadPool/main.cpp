#include "main.h"
#include "CBankAccount.h"
#include "CThreadPool.h"

int main(void)
{
    int i = 0;

    while (i++ < 10) {
        CBankAccount* a = new CBankAccount(200);
        CBankAccount* b = new CBankAccount(200);
        CThreadPool* pool = new CThreadPool(4);
        CParameters* params1 = new CParameters(a, b, 100);
        pool->RequestThread(CBankAccount::Transfer)->SetMaxWaitTime(6000)->Start(params1);
        CParameters* params2 = new CParameters(b, a, 50);
        pool->RequestThread(CBankAccount::Transfer)->Start(params2);
        pool->WaitAll();
        delete pool;
    }

#ifdef _DEBUG
    return system("pause");
#endif
    return 0;
}
