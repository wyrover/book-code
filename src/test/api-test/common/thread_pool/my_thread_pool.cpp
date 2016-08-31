/*======================================================================
Copyright (c) 2010,ｚｙｑ５９４５@126.com
All rights reserved.

文件名称：MyThreadPool.cpp
摘    要：任务类和模板实例化线程池类的函数实现

当前版本：1.00
作    者：ｚｙｑ５９４５@126.com（注意把中文字符换成英文字符）
完成日期：2010年6月26日
发布Blog：http://blog.csdn.net/zyq5945/
======================================================================*/




#include "my_thread_pool.h"
#include <atlstr.h>

/*======================================================================
函数名称：  CTask::CTask()
函数参数：  无
  返回值：  无
函数说明：  构造函数
======================================================================*/
CTask::CTask()
{
    m_nIndex = 0;
    RtlZeroMemory(&m_cTime, sizeof(SYSTEMTIME));
}

/*======================================================================
函数名称：  CTask::CTask(const CTask& rTask)
函数参数：  无
  返回值：  无
函数说明：  拷贝构造函数
======================================================================*/
CTask::CTask(const CTask& rTask)
{
    m_nIndex    =   rTask.m_nIndex;
    m_cTime     =   rTask.m_cTime;
}

/*======================================================================
函数名称：  CTask& CTask::operator =(const CTask& rTask)
函数参数：  无
  返回值：  无
函数说明：  等号操作符
======================================================================*/
CTask& CTask::operator =(const CTask& rTask)
{
    m_nIndex    =   rTask.m_nIndex;
    m_cTime     =   rTask.m_cTime;
    return *this;
}



/*======================================================================
函数名称：  CMyThreadPool::CMyThreadPool(void)
函数参数：  无
  返回值：  无
函数说明：  模板实例化线程池构造函数
======================================================================*/
CMyThreadPool::CMyThreadPool(void)
{
}

/*======================================================================
函数名称：  CMyThreadPool::~CMyThreadPool(void)
函数参数：  无
  返回值：  无
函数说明：  模板实例化线程池析构函数
======================================================================*/
CMyThreadPool::~CMyThreadPool(void)
{
}

/*======================================================================
函数名称：  void CMyThreadPool::DoWork(CTask& rTask)
函数参数：  T& t
                一个需要处理的任务的引用
返回值：    无
函数说明：  具体的任务处理过程
            注:请勿保存传递过来的参数指针
======================================================================*/
void CMyThreadPool::DoWork(CTask& rTask)
{
    CString strMsg;
    SYSTEMTIME st;
    st = rTask.m_cTime;
    strMsg.Format(_T("\tIndex:%08d\t%02d:%02d:%02d.%03d\t\tTID:%08x\n"),
                  rTask.m_nIndex, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, GetCurrentThreadId());
    // 在调试窗口输出处理的任务
    OutputDebugString(strMsg);
    // 模拟线程处理消耗时间
    DWORD dwSleep = st.wMilliseconds > 500 ? st.wMilliseconds : 500;
    Sleep(dwSleep);
}
