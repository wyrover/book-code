/*!
* \file cactus_timer.h
* \brief 高精度定时器声明
*
* 高精度定时器声明
*
* \author wangyang
* \date 2015/01/30
* \version 1.0
*/
#ifndef __CACTUS_CACTUS_TIMER_H__
#define __CACTUS_CACTUS_TIMER_H__

#include <cactus/cactus.h>
#include "cactus/cactus_types.h"
#include <MMSystem.h>

namespace cactus
{
/*! @brief Timer
*
* 示例:
* Timer t;
* t.Start();
* // Your algorithm goes here
* t.Stop();
*
* std::cout << t.Elapsed() << " ms" << std::endl;
*/
struct CACTUS_API Timer {

    void Start()
    {
        QueryPerformanceCounter(&m_start);
    }

    void Stop()
    {
        QueryPerformanceCounter(&m_stop);
    }

    // Returns elapsed time in milliseconds (ms)
    double Elapsed()
    {
        return (m_stop.QuadPart - m_start.QuadPart - m_overhead) \
               * 1000.0 / m_freq.QuadPart;
    }

private:

    // Returns the overhead of the timer in ticks
    static LONGLONG GetOverhead()
    {
        Timer t;
        t.Start();
        t.Stop();
        return t.m_stop.QuadPart - t.m_start.QuadPart;
    }

    LARGE_INTEGER m_start;
    LARGE_INTEGER m_stop;
    static LARGE_INTEGER m_freq;
    static LONGLONG m_overhead;
};

__declspec(selectany) LARGE_INTEGER Timer::m_freq = (QueryPerformanceFrequency(&Timer::m_freq), Timer::m_freq);

__declspec(selectany) LONGLONG Timer::m_overhead = Timer::GetOverhead();
}

namespace cactus
{


class CACTUS_API CHighResolutionTimer
{
    typedef void (*TIMERCALLBACK)(DWORD);

private:
    DWORD m_dwUser;
    MMRESULT m_nIDTimer;
    UINT m_uDelay;
    TIMERCALLBACK m_pfnCallback;

    void KillTimer();

public:
    CHighResolutionTimer();

    virtual ~CHighResolutionTimer();

    static void CALLBACK TimeProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

    BOOL CreateTimer(DWORD dwUser, UINT uDelay, TIMERCALLBACK lpTimeProc);
};

} // namespace cactus

#endif // __CACTUS_CACTUS_TIMER_H__
