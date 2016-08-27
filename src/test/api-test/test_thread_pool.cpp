#include <catch.hpp>
#include <tracetool/tracetool.h>
#include "common/thread_pool/my_thread_pool.h"
#include <Windows.h>
#include <wtypes.h>
#include <atlstr.h>
#include <iostream>

TEST_CASE("Test for common/thread_pool/my_thread_pool", "[common/thread_pool/my_thread_pool]")
{
    CMyThreadPool m_pool;
    CString m_strStatus;
    SECTION("testCMyThreadPool", "[common/thread_pool/my_thread_pool]") {
        m_pool.Init();
        m_pool.SetThreadNum(10);
        SYSTEMTIME st;

        for (int i = 0; i < 100; i++) {
            CTask cTask;
            GetLocalTime(&st);
            st.wMilliseconds = st.wMilliseconds < 500 ?
                               st.wMilliseconds + i : st.wMilliseconds - i;
            static int s_nIndex = 0;
            cTask.m_nIndex = s_nIndex++;
            cTask.m_cTime = st;
            m_pool.AddTask(cTask);
        }

        PoolStatus cStatus;
        cStatus = m_pool.GetStatus();
        m_strStatus.Format(_T("总的线程数:%u\n空闲线程数:%u\n任务链表中的任务个数:%u\n"),
                           cStatus.uiTotalThread, cStatus.uiIdleThread, cStatus.uiTask);
        std::wcout << (LPCWSTR)m_strStatus << std::endl;
    }
}

