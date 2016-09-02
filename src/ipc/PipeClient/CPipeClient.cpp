
#include "process.h"
#include "CPipeClient.h"


CPipeClient::CPipeClient(void)
{
}

CPipeClient::CPipeClient(std::wstring& sName) : m_sPipeName(sName),
    m_hThread(NULL),
    m_nEvent(AU_INIT)
{
    m_buffer = (wchar_t*)calloc(AU_DATA_BUF, sizeof(wchar_t));
    Init();
}

CPipeClient::~CPipeClient(void)
{
    delete m_buffer;
    m_buffer = NULL;
}

int CPipeClient::GetEvent() const
{
    return m_nEvent;
}

void CPipeClient::SetEvent(int nEventID)
{
    m_nEvent = nEventID;
}

HANDLE CPipeClient::GetThreadHandle()
{
    return m_hThread;
}

HANDLE CPipeClient::GetPipeHandle()
{
    return m_hPipe;
}

void CPipeClient::SetData(std::wstring& sData)
{
    memset(&m_buffer[0], 0, AU_DATA_BUF);
    //memcpy(&m_buffer[0], sData.c_str(), __min(AU_DATA_BUF, sData.size()));
    wcsncpy(&m_buffer[0], sData.c_str(), __min(AU_DATA_BUF, sData.size()));
}

// Get data from buffer
void CPipeClient::GetData(std::wstring& sData)
{
    sData.clear(); // Clear old data, if any
    sData.append(m_buffer);
}

void CPipeClient::Init()
{
    if (m_sPipeName.empty()) {
        // Invalid pipe name
        return;
    }

    Run();
}

void CPipeClient::Run()
{
    UINT uiThreadId = 0;
    m_hThread = (HANDLE)::_beginthreadex(NULL,
                                         NULL,
                                         PipeThreadProc,
                                         this,
                                         CREATE_SUSPENDED,
                                         &uiThreadId);

    if (NULL == m_hThread) {
        OnEvent(AU_ERROR);
    } else {
        SetEvent(AU_INIT);
        ::ResumeThread(m_hThread);
    }
}

UINT32 __stdcall CPipeClient::PipeThreadProc(void* pParam)
{
    CPipeClient* pPipe = reinterpret_cast<CPipeClient*>(pParam);

    if (pPipe == NULL)
        return 1L;

    pPipe->OnEvent(AU_THRD_RUN);

    while (true) {
        int nEventID = pPipe->GetEvent();

        if (nEventID == AU_ERROR || nEventID == AU_TERMINATE) {
            // Close pipe comm
            pPipe->Close();
            break;
        }

        switch (nEventID) {
        case AU_INIT: {
            pPipe->ConnectToServer();
            break;
        }

        case AU_IOREAD: {
            if (pPipe->Read())
                pPipe->OnEvent(AU_READ);
            else
                pPipe->OnEvent(AU_ERROR);

            break;
        }

        case AU_IOWRITE: {
            if (pPipe->Write())
                pPipe->OnEvent(AU_WRITE);
            else
                pPipe->OnEvent(AU_ERROR);
        }
        break;

        case AU_CLOSE: {
            pPipe->OnEvent(AU_CLOSE);
            break;
        }

        case AU_IOWRITECLOSE: {
            if (pPipe->Write())
                pPipe->OnEvent(AU_CLOSE);
            else
                pPipe->OnEvent(AU_ERROR);

            break;
        }

        case AU_IOPENDING:
        default:
            Sleep(10);
            continue;
        };

        Sleep(10);
    };

    return 0;
}

void CPipeClient::ConnectToServer()
{
    OnEvent(AU_CLNT_TRY);
    m_hPipe = ::CreateFile(
                  m_sPipeName.c_str(),      // pipe name
                  GENERIC_READ | GENERIC_WRITE, // read and write access
                  0,              // no sharing
                  NULL,           // default security attributes
                  OPEN_EXISTING,  // opens existing pipe
                  0,              // default attributes
                  NULL);          // no template file

    if (INVALID_HANDLE_VALUE == m_hPipe) {
        //SetEventData("Could not connect to pipe server");
        OnEvent(AU_ERROR);
    } else {
        OnEvent(AU_CLNT_CONN);
    }
}

void CPipeClient::OnEvent(int nEventID)
{
    switch (nEventID) {
    case AU_THRD_RUN:
        LOG << "Thread running" << std::endl;
        break;

    case AU_INIT:
        LOG << "Initializing pipe comm" << std::endl;
        break;

    case AU_CLNT_TRY:
        LOG << "Trying to connect to pipe server" << std::endl;
        break;

    case AU_CLNT_CONN: {
        LOG << "Connected to server" << std::endl;
        SetEvent(AU_IOREAD);
        break;
    }

    case AU_READ: {
        std::wstring sData;
        GetData(sData);
        LOG << "Message from server: " << sData << std::endl;
        sData.clear();
        sData.append(_T("cl\x03c6se"));
        SetData(sData);
        SetEvent(AU_IOWRITECLOSE);
        break;
    }

    case AU_WRITE:
        LOG << "Wrote data to pipe" << std::endl;
        SetEvent(AU_IOREAD);
        break;

    case AU_ERROR:
        LOG << "ERROR: Pipe error" << std::endl;
        SetEvent(AU_ERROR);
        break;

    case AU_CLOSE:
        LOG << "Closing pipe" << std::endl;
        SetEvent(AU_TERMINATE);
        break;
    };
}

void CPipeClient::Close()
{
    ::CloseHandle(m_hPipe);
    m_hPipe = NULL;
}

bool CPipeClient::Read()
{
    DWORD drBytes = 0;
    BOOL bFinishedRead = FALSE;
    int read = 0;

    do {
        bFinishedRead = ::ReadFile(
                            m_hPipe,            // handle to pipe
                            &m_buffer[read],    // buffer to receive data
                            AU_DATA_BUF,        // size of buffer
                            &drBytes,           // number of bytes read
                            NULL);              // not overlapped I/O

        if (!bFinishedRead && ERROR_MORE_DATA != GetLastError()) {
            bFinishedRead = FALSE;
            break;
        }

        read += drBytes;
    } while (!bFinishedRead);

    if (FALSE == bFinishedRead || 0 == drBytes) {
        //SetEventData("ReadFile failed");
        return false;
    }

    return true;
}

bool CPipeClient::Write()
{
    DWORD dwBytes;
    BOOL bResult = ::WriteFile(
                       m_hPipe,                  // handle to pipe
                       m_buffer,                 // buffer to write from
                       ::wcslen(m_buffer) * sizeof(wchar_t) + 1,   // number of bytes to write, include the NULL
                       &dwBytes,                 // number of bytes written
                       NULL);                  // not overlapped I/O

    if (FALSE == bResult || wcslen(m_buffer)*sizeof(wchar_t) + 1 != dwBytes) {
        //SetEventData("WriteFile failed");
        return false;
    }

    return true;
}

