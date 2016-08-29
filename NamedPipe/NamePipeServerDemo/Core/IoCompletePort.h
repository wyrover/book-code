#pragma once

#include <windows.h>
class CIOCompletionPort
{
private:
    HANDLE m_hIOCP;

public:

    CIOCompletionPort() : m_hIOCP(NULL)
    {
    }

    ~CIOCompletionPort()
    {
        Close();
    }

    BOOL Create(DWORD dwNumberOfConcurrentThreads = 0)
    {
        m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, dwNumberOfConcurrentThreads);
        return (m_hIOCP != NULL);
    }

    BOOL Close()
    {
        // Ensure that the handle of the I/O completion port is closed
        if(m_hIOCP != NULL && CloseHandle(m_hIOCP))
        {
            m_hIOCP = NULL;
            return TRUE;
        }

        return FALSE;
    }

    BOOL AssociateDevice(HANDLE hDevice, ULONG_PTR CompKey)
    {
        HANDLE h = CreateIoCompletionPort(hDevice, m_hIOCP, CompKey, 0);
        return h == m_hIOCP;
    }

    BOOL QueuePacket(ULONG_PTR CompKey, DWORD dwNumBytes = 0,
                     OVERLAPPED* po = NULL)
    {
        return PostQueuedCompletionStatus(m_hIOCP, dwNumBytes, CompKey, po);
    }

    BOOL DequeuePacket(ULONG_PTR* pCompKey, PDWORD pdwNumBytes,
                       OVERLAPPED** ppo, DWORD dwMilliseconds = INFINITE)
    {
        return GetQueuedCompletionStatus(m_hIOCP, pdwNumBytes, pCompKey, ppo,
                                         dwMilliseconds);
    }
};