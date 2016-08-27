/*======================================================================
Copyright (c) 2010,ｚｙｑ５９４５@126.com
All rights reserved.

文件名称：MsgThreadPool.h
摘    要：一个模板线程池类，没有使用信号灯，互斥体等同步机制

当前版本：1.00
作    者：ｚｙｑ５９４５@126.com（注意把中文字符换成英文字符）
完成日期：2010年6月26日
发布Blog：http://blog.csdn.net/zyq5945/
======================================================================*/

#pragma once
#include <Windows.h>
#include <process.h>
#include <vector>
#include <list>
#include <deque>

using namespace std;

#ifdef _DEBUG
    #define new DEBUG_NEW
#endif

#define ASSERT
#ifdef _DEBUG
    #include <atltrace.h>
    #define TRACE ATLTRACE
    #define VERIFY(f)          ASSERT(f)
    #define DEBUG_ONLY(f)      (f)
#else // _DEBUG
    #define TRACE              __noop
    #define VERIFY(f)          ((void)(f))
    #define DEBUG_ONLY(f)      ((void)0)
#endif


// 默认的开启工作线程个数线程
#define THREAD_NUM      10

// 析构的时候删除所有的未处理的任务
#define TM_DELETETASK       WM_USER
// 设置工作线程的个数
#define TM_SETTHREADNUM     WM_USER + 1
// 添加任务
#define TM_ADDTASK          WM_USER + 2
// 在工作线程中是处理任务
// 在管理线程中是给工作线程分配任务
#define TM_DISPOSETASK      WM_USER + 3
// 工作线程告诉管理线程该线程是空闲的
#define TM_KICKIDLE         WM_USER + 4


/*======================================================================
结构体名称：PoolStatus
结构体说明：线程池的状态结构体
======================================================================*/
typedef struct _PoolStatus {
    unsigned int uiTotalThread;     //总的线程数
    unsigned int uiIdleThread;      //空闲线程数
    unsigned int uiTask;            //任务链表中的任务个数
} PoolStatus;


/*======================================================================
类名称：template <typename T> class CMsgThreadPool
类说明：模板类的声明和实现不能分开,否则会产生链接错误
======================================================================*/
template <typename T>
class CMsgThreadPool
{
public:
    /*======================================================================
    函数名称：  CMsgThreadPool(void)
    函数参数：  无
      返回值：  无
    函数说明：  初始化类成员变量
    ======================================================================*/
    CMsgThreadPool(void)
    {
        m_ManagerTID = 0;
        m_TaskList.resize(0);
        m_dwThreadId.resize(0);
        m_dwIdleTId.resize(0);
    }

    /*======================================================================
    函数名称：  virtual ~CMsgThreadPool(void)
    函数参数：  无
      返回值：  无
    函数说明：  线程池退出后的清理工作
    ======================================================================*/
    virtual ~CMsgThreadPool(void)
    {
        // 删除所有未处理的任务
        VERIFY(PostTM((DWORD)m_ManagerTID,
                      TM_DELETETASK, 0, 0));
        // 通知所有的工作线程退出
        list<DWORD>::const_iterator it ;

        for (it = m_dwThreadId.begin();
             it != m_dwThreadId.end(); ++it) {
            VERIFY(PostTM(*it,
                          WM_QUIT, 0, 0));
        }

#if _MSC_VER >= 1500
        /*======================================================================
        MS VC++ 9.0 _MSC_VER = 1500
        MS VC++ 8.0 _MSC_VER = 1400
        MS VC++ 7.1 _MSC_VER = 1310
        MS VC++ 7.0 _MSC_VER = 1300
        MS VC++ 6.0 _MSC_VER = 1200
        MS VC++ 5.0 _MSC_VER = 1100
        说明:   下面代码仅在VS2008SP1中测试通过,如有需要请自行修改测试
        ======================================================================*/
        // 获取所有工作线程的句柄
        vector<HANDLE> hAllThread;

        while (m_dwThreadId.size() > 0) {
            DWORD dwTID = m_dwThreadId.front();
            m_dwThreadId.pop_front();
            HANDLE handle = OpenThread(THREAD_ALL_ACCESS,
                                       FALSE, dwTID);

            if (handle != NULL) {
                hAllThread.push_back(handle);
            }
        }

        // 等待所有工作线程退出
        if (hAllThread.size() > 0) {
            WaitForMultipleObjects(hAllThread.size(),
                                   (const HANDLE *)&hAllThread[0], TRUE, INFINITE);

            for (DWORD i = 0; i < hAllThread.size(); i++) {
                CloseHandle(hAllThread[i]);
            }
        }

        hAllThread.clear();
#endif
        // 通知管理线程退出
        VERIFY(PostTM((DWORD)m_ManagerTID,
                      WM_QUIT, 0, 0));
#if _MSC_VER >= 1500
        /*======================================================================
        MS VC++ 9.0 _MSC_VER = 1500
        MS VC++ 8.0 _MSC_VER = 1400
        MS VC++ 7.1 _MSC_VER = 1310
        MS VC++ 7.0 _MSC_VER = 1300
        MS VC++ 6.0 _MSC_VER = 1200
        MS VC++ 5.0 _MSC_VER = 1100
        说明:   下面代码仅在VS2008SP1中测试通过,如有需要请自行修改测试
        ======================================================================*/
        // 获取管理线程的句柄
        HANDLE handle = OpenThread(THREAD_ALL_ACCESS,
                                   FALSE, m_ManagerTID);

        // 等待管理线程退出
        if (handle != NULL) {
            WaitForSingleObject(handle, INFINITE);
            CloseHandle(handle);
        }

#endif
    }

public:
    /*======================================================================
    函数名称：  virtual void DoWork(T& t) = 0
    函数参数：  T& t
                    一个需要处理的任务的引用
      返回值：  无
    函数说明：  该函数需要在子类中实现具体的任务处理过程
    ======================================================================*/
    virtual void DoWork(T& t) = 0;                  // 在子类中要做的工作

public:
    /*======================================================================
    函数名称：  BOOL Init(DWORD dwThreadNum = THREAD_NUM)
    函数参数：  DWORD dwThreadNum = THREAD_NUM
                    初始化的工作线程个数,默认是10
      返回值：  该函数执行成功返回TRUE,否则FALSE
    函数说明：  用户接口函数,初始化线程池
    ======================================================================*/
    BOOL Init(DWORD dwThreadNum = THREAD_NUM)
    {
        ASSERT(dwThreadNum >= 1);
        ASSERT(m_ManagerTID == 0);
        // 创建管理线程
        HANDLE hManager = NULL;
        hManager = (HANDLE)_beginthreadex(NULL,
                                          0, &CMsgThreadPool::ManagerThread, this, 0, &m_ManagerTID);

        if (hManager == 0) {
            return FALSE;
        }

        CloseHandle(hManager);
        hManager = NULL;
        // 设置工作线程个数
        SetThreadNum(dwThreadNum);
        return TRUE;
    }

    /*======================================================================
    函数名称：  void SetThreadNum(DWORD dwThreadNum = THREAD_NUM)
    函数参数：  DWORD dwThreadNum = THREAD_NUM
                    初始化的工作线程个数,默认是10
      返回值：  无
    函数说明：  用户接口函数,设置工作线程的个数
    ======================================================================*/
    void SetThreadNum(DWORD dwThreadNum = THREAD_NUM)
    {
        ASSERT(dwThreadNum >= 1);
        ASSERT(m_ManagerTID != 0);
        VERIFY(PostTM((DWORD)m_ManagerTID,
                      TM_SETTHREADNUM, (WPARAM)dwThreadNum, 0));
        return ;
    }

    /*======================================================================
    函数名称：  void AddTask(T& t)
    函数参数：  T& t
                    一个要处理的任务的引用
      返回值：  无
    函数说明：  用户接口函数,添加任务到线程池中
    ======================================================================*/
    void AddTask(T& t)
    {
        T* pT = new T;
        *pT = t;
        VERIFY(PostTM((DWORD)m_ManagerTID,
                      TM_ADDTASK, (WPARAM)pT, 0));
    }

    /*======================================================================
    函数名称：  PoolStatus GetStatus() const
    函数参数：  无
      返回值：  PoolStatus
                    线程池的状态结构体
    函数说明：  用户接口函数,获取线程池的状态
    ======================================================================*/
    PoolStatus GetStatus() const
    {
        PoolStatus cStatus;
        cStatus.uiTotalThread   =       m_dwThreadId.size();
        cStatus.uiIdleThread        =       m_dwIdleTId.size();
        cStatus.uiTask          =       m_TaskList.size();
        return cStatus;
    }

public:
    /*======================================================================
    函数名称：  DWORD GetManager() const
    函数参数：  无
      返回值：  DWORD
                    返回管理线程的线程ID
    函数说明：  该函数虽然是一个public类成员函数,但不是用户接口函数,
                主要是用来在工作线程中获取管理线程的ID
    ======================================================================*/
    DWORD GetManager() const
    {
        ASSERT((DWORD)m_ManagerTID != 0);
        return m_ManagerTID;
    }

private:
    /*======================================================================
    函数名称：  static unsigned int WINAPI ManagerThread(void* pParam)
    函数参数：  void* pParam
                    模板线程池的指针
      返回值：  unsigned int
    函数说明：  管理线程,维护工作线程队列和任务队列,并协调好它们的工作
    ======================================================================*/
    static unsigned int WINAPI ManagerThread(void* pParam)      // 管理线程
    {
        ASSERT(pParam != NULL);
        CMsgThreadPool* pPool = (CMsgThreadPool*)pParam;
        T* t = NULL;
        MSG msg;

        // 循环获取消息,析构的时候会收到WM_QUIT消息退出循环
        while (GetMessage(&msg, NULL, 0, 0)) {
            // 处理消息
            pPool->DisposeMsg(msg.message, msg.wParam, msg.lParam);
        }

        _endthreadex(0);
        return 0;
    }

    /*======================================================================
    函数名称：  static unsigned int WINAPI OperatorThread(void* pParam)
    函数参数：  void* pParam
                    模板线程池的指针
      返回值：  unsigned int
    函数说明：  工作线程,不断的获取任务并处理
                析构的时候会收到WM_QUIT消息退出循环
    ======================================================================*/
    static unsigned int WINAPI OperatorThread(void* pParam)     // 工作线程
    {
        ASSERT(pParam != NULL);
        DWORD dwThreadId = GetCurrentThreadId();
        CMsgThreadPool* pPool = (CMsgThreadPool*)pParam;
        // 告诉工作线程现在是空闲的
        VERIFY(PostTM(pPool->GetManager(),
                      TM_KICKIDLE, (WPARAM)0, (LPARAM)dwThreadId));
        T* t = NULL;
        MSG msg;

        // 循环获取消息,析构的时候会收到WM_QUIT消息退出循环
        while (GetMessage(&msg, NULL, 0, 0)) {
            switch (msg.message) {
            case TM_DISPOSETASK:
                t = (T *)msg.wParam;
                ASSERT(t != NULL);
                // 具体的任务处理过程
                pPool->DoWork(*t);
                delete t;
                t = NULL;
                // 告诉工作线程已经处理完一个任务,现在是空闲的
                VERIFY(PostTM(pPool->GetManager(),
                              TM_KICKIDLE, 0, (LPARAM)dwThreadId));
                break;

            default:
                break;
            }
        }

        _endthreadex(0);
        return dwThreadId;
    }

private:
    /*======================================================================
    函数名称：  BOOL CreateOperator(DWORD dwThreadNum)
    函数参数：  DWORD dwThreadNum
                    创建的工作线程个数,默认是10
      返回值：  该函数执行成功返回TRUE,否则FALSE
    函数说明：  创建工作线程
    ======================================================================*/
    BOOL CreateOperator(DWORD dwThreadNum)
    {
        HANDLE hOperator = NULL;
        unsigned int dwThreadId = 0;
        UINT i = 0;

        for (i = 0; i < dwThreadNum; i++) {
            // 创建工作线程
            hOperator = (HANDLE)_beginthreadex(NULL,
                                               0, &CMsgThreadPool::OperatorThread, this, 0, &dwThreadId);

            if (hOperator == 0) {
                return FALSE;
            }

            // 添加到工作线程链表
            m_dwThreadId.push_back(dwThreadId);
            CloseHandle(hOperator);
            hOperator = NULL;
        }

        return TRUE;
    }

    /*======================================================================
    函数名称：  void DestryOperator(DWORD dwThreadNum)
    函数参数：  DWORD dwThreadNum
                    销毁的工作线程个数
      返回值：  无
    函数说明：  销毁工作线程.
                注:当没有空闲线程的时候不销毁工作线程
    ======================================================================*/
    void DestryOperator(DWORD dwThreadNum)
    {
        DWORD dwCount = dwThreadNum < m_dwIdleTId.size() ?
                        dwThreadNum : m_dwIdleTId.size();

        for (DWORD i = 0; i < dwCount; i++) {
            DWORD dwThreadID = m_dwIdleTId.front();
            // 通知空闲线程退出
            VERIFY(PostTM(dwThreadID,
                          WM_QUIT, 0, 0));
            m_dwIdleTId.pop_front();
            // 删除其在工作线程链表中记录
            m_dwThreadId.remove(dwThreadID);
        }
    }

    /*======================================================================
    函数名称：  void DisposeMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
    函数参数：  UINT uMsg
                    消息类型
                WPARAM wParam
                LPARAM lParam
      返回值：  无
    函数说明：  管理线程通过消息维护工作线程队列和任务队列,
                并协调好它们的工作
    ======================================================================*/
    void DisposeMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        ASSERT((DWORD)m_ManagerTID != 0);

        switch (uMsg) {
        // 工作线程空闲
        case TM_KICKIDLE:
            OnKickIdle(wParam, lParam);
            break;

        // 添加新任务
        case TM_ADDTASK:
            OnAddTask(wParam, lParam);
            break;

        // 分配任务给空闲的工作线程
        case TM_DISPOSETASK:
            OnTmDisposeTask(wParam, lParam);
            break;

        // 删除所有的未处理的任务
        case TM_DELETETASK:
            OnDeleteTask(wParam, lParam);
            break;

        // 设置工作线程的个数
        case TM_SETTHREADNUM:
            OnSetThreadNum(wParam, lParam);
            break;

        default:
            break;
        }
    }

private:
    /*======================================================================
    函数名称：  static BOOL PostTM(DWORD dwID, UINT uMsg, WPARAM wParam, LPARAM lParam)
    函数参数：  DWORD dwID
                    需要发送的线程ID
                UINT uMsg
                    需要发送的消息类型
                WPARAM wParam
                LPARAM lParam
      返回值：  该函数执行成功返回TRUE,否则FALSE
    函数说明：  仅仅是对PostThreadMessage进行一次封装,
                在本机测试第一次用PostThreadMessage发送的时候会失败
    ======================================================================*/
    static BOOL PostTM(DWORD dwID, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
#define TRY_COUNT       100
        UINT nCount = 0;

        while (nCount++ <= TRY_COUNT) {
            if (PostThreadMessage(dwID, uMsg, wParam, lParam)) {
                return TRUE;
            }

#ifdef _DEBUG
            static DWORD dwCount = 0;
            TRACE(_T("\t TRY_COUNT:%d Error:%d\n"), dwCount++, GetLastError());
#endif
            Sleep(200);
        }

        // 发送消息失败，麻烦大了！！！
        ASSERT(FALSE);
        return FALSE;
    }

    /*======================================================================
    函数名称：  void OnTmDisposeTask(WPARAM wParam, LPARAM lParam)
    函数参数：  WPARAM wParam
                LPARAM lParam
      返回值：  无
    函数说明：  给工作线程分配任务
    ======================================================================*/
    void OnTmDisposeTask(WPARAM wParam, LPARAM lParam)
    {
        T* pT = NULL;
        DWORD dwThreadId = 0;

        while ((m_TaskList.size() != 0)
               && (m_dwIdleTId.size() != 0)) {
            // 从任务链表中取出任务
            pT = m_TaskList.front();
            ASSERT(pT != NULL);
            // 从空闲工作线程链表中取出其ID
            dwThreadId = m_dwIdleTId.front();
            // 把任务发送给工作线程
            VERIFY(PostTM(dwThreadId,
                          TM_DISPOSETASK, (WPARAM)pT, 0));
            m_TaskList.pop_front();
            m_dwIdleTId.pop_front();
        }
    }

    /*======================================================================
    函数名称：  void OnDeleteTask(WPARAM wParam, LPARAM lParam)
    函数参数：  WPARAM wParam
                LPARAM lParam
    返回值：    无
    函数说明：  删除所有的未处理的任务
    ======================================================================*/
    void OnDeleteTask(WPARAM wParam, LPARAM lParam)
    {
        T* pT = NULL;

        while (m_TaskList.size() > 0) {
            pT = m_TaskList.front();
            ASSERT(pT != NULL);
            delete pT;
            m_TaskList.pop_front();
        }
    }

    /*======================================================================
    函数名称：  void OnKickIdle(WPARAM wParam, LPARAM lParam)
    函数参数：  WPARAM wParam
                LPARAM lParam
      返回值：  无
    函数说明：  工作线程告诉管理线程该线程是空闲的
    ======================================================================*/
    void OnKickIdle(WPARAM wParam, LPARAM lParam)
    {
        // 把线程加入空闲线程队列
        m_dwIdleTId.push_back((DWORD)lParam);
        VERIFY(PostTM((DWORD)m_ManagerTID,
                      TM_DISPOSETASK, 0, 0));
    }

    /*======================================================================
    函数名称：  void OnAddTask(WPARAM wParam, LPARAM lParam)
    函数参数：  WPARAM wParam
                LPARAM lParam
      返回值：  无
    函数说明：  添加任务到任务链表中
    ======================================================================*/
    void OnAddTask(WPARAM wParam, LPARAM lParam)
    {
        // 添加任务到任务链表中
        ASSERT(wParam != 0);
        // 把任务加入任务队列
        m_TaskList.push_back((T*)wParam);
        VERIFY(PostTM((DWORD)m_ManagerTID,
                      TM_DISPOSETASK, 0, 0));
    }

    /*======================================================================
    函数名称：  void OnSetThreadNum(WPARAM wParam, LPARAM lParam)
    函数参数：  WPARAM wParam
                LPARAM lParam
      返回值：  无
    函数说明：  设置工作线程的个数
    ======================================================================*/
    void OnSetThreadNum(WPARAM wParam, LPARAM lParam)
    {
        DWORD dwThreadNum = (DWORD)wParam;

        if (dwThreadNum > m_dwThreadId.size()) {
            VERIFY(CreateOperator(dwThreadNum - m_dwThreadId.size()));
        } else if (dwThreadNum < m_dwThreadId.size()) {
            DestryOperator(m_dwThreadId.size() - dwThreadNum);
        }
    }

private:
    unsigned int m_ManagerTID;                      // 管理者线程ID
    list<DWORD> m_dwThreadId;                       // 工作线程的线程ID
    deque<T*> m_TaskList;                           // 任务链
    deque<DWORD> m_dwIdleTId;                       // 空闲线程
};