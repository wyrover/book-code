#include "stdafx.h"
#include "CMySQL.h"

#define BLOCK_SIZE      4096
#define THREADS_NUMBER  3

typedef struct {
    char szQuery[BLOCK_SIZE];
    char szResult[BLOCK_SIZE];
    bool bIsRead;
} QUERYDATA, *PQUERYDATA;

CRITICAL_SECTION cs;
CMySQL* mySqlInstance = NULL;

DWORD WINAPI StartAddress(LPVOID lpParameter)
{
    PQUERYDATA pQueryData = (PQUERYDATA)lpParameter;
    EnterCriticalSection(&cs);

    if (mySqlInstance->ConnectInstance()) {
        if (pQueryData->bIsRead) {
            memset(pQueryData->szResult, 0, BLOCK_SIZE - 1);
            mySqlInstance->ReadData(pQueryData->szQuery, pQueryData->szResult, BLOCK_SIZE - 1);
        } else {
            mySqlInstance->WriteData(pQueryData->szQuery, pQueryData->szResult, BLOCK_SIZE - 1);
        }

        mySqlInstance->DisconnectInstance();
    }

    LeaveCriticalSection(&cs);
    return 0L;
}

int main()
{
    InitializeCriticalSection(&cs);
    mySqlInstance = CMySQL::CreateInstance("mysql.services.expert.its.me", "expertit_9790OS", "expertit_9790", "$dbpass_1342#");

    if (mySqlInstance) {
        HANDLE hThreads[THREADS_NUMBER];
        QUERYDATA queryData[THREADS_NUMBER] = {
            { "select address from clients where id = 3;", "", true },
            { "update clients set name = 'Merrill & Lynch' where id = 2;", "", false },
            { "select name from clients where id = 2;", "", true }
        };

        for (int iIndex = 0; iIndex < THREADS_NUMBER; iIndex++) {
            hThreads[iIndex] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartAddress, &queryData[iIndex], 0, 0);
        }

        WaitForMultipleObjects(THREADS_NUMBER, hThreads, TRUE, INFINITE);

        for (int iIndex = 0; iIndex < THREADS_NUMBER; iIndex++) {
            printf_s("%s\n", queryData[iIndex].szResult);
        }

        CMySQL::ReleaseInstance();
    }

    DeleteCriticalSection(&cs);
    return system("pause");
}
