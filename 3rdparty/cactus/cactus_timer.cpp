/*!
* \file cactus_timer.cpp
* \brief 高精度定时器实现
*
* 高精度定时器实现
*
* \author wangyang
* \date 2015/01/30
* \version 1.0
*/

#include "cactus/cactus_timer.h"

#include <MMSYSTEM.H>
#pragma comment(lib, "WINMM.LIB")

namespace cactus
{


BOOL CHighResolutionTimer::CreateTimer(DWORD dwUser, UINT uDelay, TIMERCALLBACK lpTimeProc)
{
    ASSERT(dwUser);
    ASSERT(lpTimeProc);
    m_dwUser = dwUser;
    m_pfnCallback = lpTimeProc;
    BOOL ret = FALSE;
    KillTimer();
    m_nIDTimer = timeSetEvent(uDelay, 0, TimeProc, (DWORD)this, TIME_PERIODIC | TIME_CALLBACK_FUNCTION);

    if (m_nIDTimer != NULL)
        ret = TRUE;

    return (ret);
}

void CALLBACK CHighResolutionTimer::TimeProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
    // dwUser contains ptr to Timer object
    CHighResolutionTimer * ptimer = (CHighResolutionTimer *) dwUser;
    // Call user-specified callback and pass back user specified data
    (ptimer->m_pfnCallback)(ptimer->m_dwUser);
}

CHighResolutionTimer::~CHighResolutionTimer()
{
    KillTimer();
}

CHighResolutionTimer::CHighResolutionTimer()
{
    m_nIDTimer = NULL;
    m_uDelay = 0;
    m_pfnCallback = 0;
}

void CHighResolutionTimer::KillTimer()
{
    if (m_nIDTimer != NULL) {
        timeKillEvent(m_nIDTimer);
        m_nIDTimer = NULL;
    }
}

}  // namespace cactus

