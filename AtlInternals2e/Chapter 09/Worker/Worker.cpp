// Worker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
using namespace std;

[event_source(native)]
class Worker
{
public:
    __event void WorkStarted(LPCSTR pszProject);
    __event void WorkProgressing(LPCSTR pszProject);
    __event void WorkCompleted(LPCSTR pszProject);
};

[event_receiver(native)]
class Boss
{
public:
    void ListenTo(Worker* pWorker)
    {
        __hook(&Worker::WorkStarted, pWorker, OnWorkStarted);
    }

    void StopListening(Worker* pWorker)
    {
        __unhook(&Worker::WorkStarted, pWorker, OnWorkStarted);
    }

    void OnWorkStarted(LPCSTR pszProject)
    {
        string strMsg = "Worker started project " + string(pszProject);
        ::MessageBox(NULL, strMsg.c_str(), "Boss Handler", MB_OK | MB_ICONINFORMATION);
    }
};

int _tmain(int argc, _TCHAR* argv[])
{
    Worker w;
    Boss b;
    b.ListenTo(&w);
    w.WorkStarted("Segway Personal Transporter");
    return 0;
}

