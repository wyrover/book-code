// IoCompletionPort.h : interface of the CIoCompletionPort class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

// CIoCompletionPort
//
/////////////////////////////////////////////////////////////////////////////

class CIoCompletionPort
{
public:
    CIoCompletionPort(DWORD dwThreadCount = 0) :
        m_hPort(NULL),
        m_nQueuedCount(0)
    { }

    virtual ~CIoCompletionPort()
    {
        if (m_hPort != NULL) {
            BOOL bRes = ::CloseHandle(m_hPort);
            ATLASSERT(bRes != FALSE);
            m_hPort = NULL;
        }
    }

    BOOL Create(DWORD dwThreadCount = 0)
    {
        ATLASSERT(m_hPort == NULL);
        return (m_hPort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, dwThreadCount)) != NULL;
    }

    BOOL Attach(HANDLE hDevice, ULONG_PTR ulKey)
    {
        ATLASSERT(m_hPort != NULL);
        return ::CreateIoCompletionPort(hDevice, m_hPort, ulKey, 0) == m_hPort;
    }

    BOOL GetQueuedCompletionStatus(LPDWORD pdwBytes, PULONG_PTR pulKey, LPOVERLAPPED *pOverlapped, DWORD dwTimeout)
    {
        ATLASSERT(m_hPort != NULL);
        BOOL bRes = ::GetQueuedCompletionStatus(m_hPort, pdwBytes, pulKey, pOverlapped, dwTimeout);

        if (bRes) {
            // Successfully retrieved queued packet
            ATLASSERT(m_nQueuedCount > 0);
            m_nQueuedCount--;
        } else {
            DWORD dwError = ::GetLastError();

            if (pOverlapped != NULL) {
                // Failed completed I/O request
                // dwError contains the reason for the failure
            } else {
                if (dwError == WAIT_TIMEOUT) {
                    // Timeout while waiting for the completed I/O
                } else {
                    // Bad call to GetQueuedCompletionStatus
                    // dwError contains the reason for the bad call
                }
            }
        }

        return bRes;
    }

    BOOL PostQueuedCompletionStatus(DWORD dwBytes, ULONG_PTR ulKey, LPOVERLAPPED pOverlapped)
    {
        ATLASSERT(m_hPort != NULL);
        BOOL bRes = ::PostQueuedCompletionStatus(m_hPort, dwBytes, ulKey, pOverlapped);
        ATLASSERT(bRes != FALSE);

        if (bRes)
            m_nQueuedCount++;

        return bRes;
    }

    INT GetQueuedCount() const
    {
        ATLASSERT(m_hPort != NULL);
        return m_nQueuedCount;
    }

private:
    HANDLE m_hPort;
    volatile INT m_nQueuedCount;
};
