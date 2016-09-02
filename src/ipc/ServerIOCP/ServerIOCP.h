#ifndef _SERVER_IOCP_H_
#define _SERVER_IOCP_H_

//Disable deprecation warnings
#pragma warning(disable: 4996)

//Op codes for IOCP
#define OP_READ     0
#define OP_WRITE    1

#define WORKER_THREADS_PER_PROCESSOR 2

//Buffer Length
#define MAX_BUFFER_LEN 256

//Time out interval for wait calls
#define WAIT_TIMEOUT_INTERVAL 100

//Graceful shutdown Event
//For this simple implementation,
//We can use global variable as well.
//Wanted to demonstrate use of event
//for shutdown
HANDLE g_hShutdownEvent = NULL;

//Number of threads to be created.
int g_nThreads = 0;

//To store handle of worker threads
HANDLE *g_phWorkerThreads = NULL;

//Handle for Accept related thread
HANDLE g_hAcceptThread = NULL;

//Network Event for Accept
WSAEVENT                g_hAcceptEvent;

CRITICAL_SECTION g_csConsole; //When threads write to console we need mutual exclusion
CRITICAL_SECTION g_csClientList; //Need to protect the client list

//Global I/O completion port handle
HANDLE g_hIOCompletionPort = NULL;

class CClientContext  //To store and manage client related information
{
private:

    OVERLAPPED        *m_pol;
    WSABUF            *m_pwbuf;

    int               m_nTotalBytes;
    int               m_nSentBytes;

    SOCKET            m_Socket;  //accepted socket
    int               m_nOpCode; //will be used by the worker thread to decide what operation to perform
    char              m_szBuffer[MAX_BUFFER_LEN];

public:

    //Get/Set calls
    void SetOpCode(int n)
    {
        m_nOpCode = n;
    }

    int GetOpCode()
    {
        return m_nOpCode;
    }

    void SetTotalBytes(int n)
    {
        m_nTotalBytes = n;
    }

    int GetTotalBytes()
    {
        return m_nTotalBytes;
    }

    void SetSentBytes(int n)
    {
        m_nSentBytes = n;
    }

    void IncrSentBytes(int n)
    {
        m_nSentBytes += n;
    }

    int GetSentBytes()
    {
        return m_nSentBytes;
    }

    void SetSocket(SOCKET s)
    {
        m_Socket = s;
    }

    SOCKET GetSocket()
    {
        return m_Socket;
    }

    void SetBuffer(char *szBuffer)
    {
        strcpy(m_szBuffer, szBuffer);
    }

    void GetBuffer(char *szBuffer)
    {
        strcpy(szBuffer, m_szBuffer);
    }

    void ZeroBuffer()
    {
        ZeroMemory(m_szBuffer, MAX_BUFFER_LEN);
    }

    void SetWSABUFLength(int nLength)
    {
        m_pwbuf->len = nLength;
    }

    int GetWSABUFLength()
    {
        return m_pwbuf->len;
    }

    WSABUF* GetWSABUFPtr()
    {
        return m_pwbuf;
    }

    OVERLAPPED* GetOVERLAPPEDPtr()
    {
        return m_pol;
    }

    void ResetWSABUF()
    {
        ZeroBuffer();
        m_pwbuf->buf = m_szBuffer;
        m_pwbuf->len = MAX_BUFFER_LEN;
    }

    //Constructor
    CClientContext()
    {
        m_pol = new OVERLAPPED;
        m_pwbuf = new WSABUF;
        ZeroMemory(m_pol, sizeof(OVERLAPPED));
        m_Socket =  SOCKET_ERROR;
        ZeroMemory(m_szBuffer, MAX_BUFFER_LEN);
        m_pwbuf->buf = m_szBuffer;
        m_pwbuf->len = MAX_BUFFER_LEN;
        m_nOpCode = 0;
        m_nTotalBytes = 0;
        m_nSentBytes = 0;
    }

    //destructor
    ~CClientContext()
    {
        //Wait for the pending operations to complete
        while (!HasOverlappedIoCompleted(m_pol)) {
            Sleep(0);
        }

        closesocket(m_Socket);
        //Cleanup
        delete m_pol;
        delete m_pwbuf;
    }
};

//Vector to store pointers of dynamically allocated ClientContext.
//map class can also be used.
//Link list can also be created.
std::vector<CClientContext *> g_ClientContext;

//global functions
bool InitializeIOCP();
bool Initialize();
void CleanUp();
void DeInitialize();
DWORD WINAPI AcceptThread(LPVOID lParam);
void AcceptConnection(SOCKET ListenSocket);
bool AssociateWithIOCP(CClientContext   *pClientContext);
DWORD WINAPI WorkerThread(LPVOID lpParam);
void WriteToConsole(char *szFormat, ...);
void AddToClientList(CClientContext   *pClientContext);
void RemoveFromClientListAndFreeMemory(CClientContext   *pClientContext);
void CleanClientList();
int GetNoOfProcessors();

#endif