// ThreadPool2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ThreadPool2.h"
#include "ThreadPool.h"
#include <tracetool/tracetool.h>

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

class CJob : public IJobDesc
{
public:
    CJob(int number): m_number(number) {}
    int GetNumber()
    {
        return m_number;
    }
protected:
    int m_number;
};

//we write a very simple worker
class CWorker: public IWorker
{
    virtual void ProcessJob(IJobDesc* pJobDesc)
    {
        CJob* pJob = reinterpret_cast<CJob*>(pJobDesc);

        for (signed __int64 i = 0; i < 1E8; i++)
            if (i % (pJob->GetNumber()) == 0) TRACE_SEND_FORMAT("found one is %d.", i);
    }
};

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    int nRetCode = 0;

    // initialize MFC and print and error on failure
    if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)) {
        // TODO: change error code to suit your needs
        cerr << _T("Fatal Error: MFC initialization failed") << endl;
        nRetCode = 1;
    } else {
        // TODO: code your application's behavior here.
        CString strHello;
        strHello.LoadString(IDS_HELLO);
        std::wcout << (LPCTSTR)strHello << endl;
    }

    int a;
    cin >> a;
    cout << endl;
    //create a thread pool and start it
    CThreadPool pool;
    pool.Start(2, 20);
    CWorker worker;

    for (int j = 0; j < 8; j++) {
        CJob* pJob = new CJob(13797 + 13 * j);
        pool.ProcessJob(pJob, &worker);
    }

    cin >> a;
    pool.Stop();
    return nRetCode;
}


