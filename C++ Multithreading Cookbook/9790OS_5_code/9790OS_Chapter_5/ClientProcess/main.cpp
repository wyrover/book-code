#pragma once

#include "..\concurrent_operations5\main.h"

int _tmain(void)
{
    char szBuffer[BUFFER_SIZE];
    DWORD dwRead = 0;
    DWORD dwWritten = 0;
    DWORD dwMode = PIPE_READMODE_MESSAGE;
    char szMessage[1024] = { 0 };
    wsprintfA(szMessage, "%u", GetCurrentProcessId());
    DWORD dwToWrite = (DWORD)(strlen(szMessage) * sizeof(char));
    HANDLE hPipe = NULL;

    while (true) {
        hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

        if (hPipe != INVALID_HANDLE_VALUE) {
            WaitNamedPipe(PIPE_NAME, INFINITE);
            SetNamedPipeHandleState(hPipe, &dwMode, NULL, NULL);
            break;
        }

        Sleep(100);
    }

    if (!WriteFile(hPipe, szMessage, dwToWrite, &dwWritten, NULL)) {
        printf("Error: [%u]\n", GetLastError());
        return system("pause");
    }

    printf("Request sent.\nReceiving task:\n");
    char szResult[1024] = { 0 };

    while (ReadFile(hPipe, szBuffer, BUFFER_SIZE * sizeof(char), &dwRead, NULL) && GetLastError() != ERROR_MORE_DATA) {
        if (szBuffer[0] == 'O' && szBuffer[1] == 'K' && ((DWORD)strtol(szBuffer + 2, NULL, 10) == GetCurrentProcessId())) {
            printf("Client process: [%s]\n", szBuffer + 2);
            HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, MUTEX_NAME);
            WaitForSingleObject(hMutex, INFINITE);
            HANDLE hMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MAPPING_NAME);
            QueueElement* pMapping = (QueueElement*)MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);

            if (pMapping != 0) {
                if (CalculateCramer(pMapping, szResult)) {
                    dwToWrite = (DWORD)(strlen(szResult) * sizeof(char));

                    if (!WriteFile(hPipe, szResult, dwToWrite, &dwWritten, NULL)) {
                        printf("Error: [%u]\n", GetLastError());
                        break;
                    } else {
                        printf("Result: %s\n", szResult);
                    }
                }
            }

            UnmapViewOfFile(pMapping);
            CloseHandle(hMapping);
            ReleaseMutex(hMutex);
            CloseHandle(hMutex);
        } else {
            printf("Error in connection [%u]\n", GetLastError());
        }
    }

    printf("\nSuccess!\nPress ENTER to exit.");
    scanf_s("%c", szBuffer);
    CloseHandle(hPipe);
    return 0;
}

bool CalculateCramer(QueueElement* pQElement, char* szResult)
{
    // We'll solve the linear system using Cramer's rule:
    double dDeterminant = (pQElement->iA1 * pQElement->iB2) - (pQElement->iB1 * pQElement->iA2);

    if (dDeterminant != 0) {
        double dX = ((pQElement->iC1 * pQElement->iB2) - (pQElement->iB1 * pQElement->iC2)) / dDeterminant;
        double dY = ((pQElement->iA1 * pQElement->iC2) - (pQElement->iC1 * pQElement->iA2)) / dDeterminant;
        sprintf_s(szResult, strlen(szResult) - 1, "  x = %8.4lf,\ty = %8.4lf\n", dX, dY);
    } else {
        sprintf_s(szResult, strlen(szResult) - 1, "  Determinant is zero.\n");
    }

    return true;
}
