#ifndef stopwatch_h__
#define stopwatch_h__

#include <Windows.h>

class StopWatch
{
    DWORD dwStart;
    DWORD dwStop;
public:

    StopWatch()
    {
        start();
    }


    DWORD stop()
    {
        dwStop = ::GetTickCount();
        return getElapsedMilliSecUntilStoped();
    }


    void start()
    {
        dwStop = dwStart = ::GetTickCount();
    }


    DWORD getElapsedMilliSecUntilStoped()
    {
        return dwStop - dwStart;
    }


    DWORD getElapsedMilliSecUntilNow()
    {
        return ::GetTickCount() - dwStart;
    }
};

#endif // stopwatch_h__
