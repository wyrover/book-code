#ifndef task_scheduler_h__
#define task_scheduler_h__

/**
 * @file
 * @brief windows计划任务相关
 */

#include <Windows.h>
//#include <initguid.h>
#include <mstask.h>

namespace cactus
{


/**
 * @brief windows计划任务接口
 */
class ZLTaskScheduler
{
public:
    ///> 创建简单的开机启动任务
    static BOOL CreateSimpleLogonTaskScheduler(LPCTSTR lpTaskName, LPCTSTR lpAppFilePath);
    static BOOL CreateTaskScheduler(
        LPCTSTR lpTaskName,
        LPCTSTR lpAppFilePath,
        DWORD   dwTaskFlag,
        TASK_TRIGGER &stTrigger,
        LPCTSTR lpAccountName,
        LPCTSTR lpPassword
    );
    static BOOL DeleteTaskScheduler(LPCTSTR lpTaskName);

private:
    static void ZLTaskScheduler::_InitLogonTaskTrigger(TASK_TRIGGER &stTrigger);
};


// 以下是实现部分


} ///> end of namespace zl

#endif // task_scheduler_h__
