/*!
* \file thread_helper.h
* \brief 线程回调UI对象方法帮助器声明 
* 
* 线程回调UI对象方法帮助器
* 
* \author wangyang 
* \date 2015/03/03 
* \version 2.0
*/
#ifndef __CACTUS_CACTUS_THREAD_HELPER_H__
#define __CACTUS_CACTUS_THREAD_HELPER_H__

#include <cactus/cactus.h>
#include <cactus/cactus_types.h>
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <list>

namespace cactus
{
typedef boost::function<void()> TThreadMethod;
typedef boost::function<void()> TThreadProcedure;
typedef boost::function0<void> WakeMainThreadCallback;

typedef struct tagSynchronizeRecord {
    HANDLE thread_handle;
    TThreadMethod method_callback;
} TSynchronizeRecord, *PSynchronizeRecord;

typedef struct tagSyncProc {
    PSynchronizeRecord sync_rec;
    HANDLE  signal;
} TSyncProc, *PSyncProc;

class CACTUS_API TThread
{
public:
    static void Synchronize(TThreadMethod method_callback);
    static void Synchronize(HANDLE thread_handle, TThreadMethod method_callback);    
    static BOOL CheckSynchronize(int timeout = 0);
private:
    static void Synchronize(PSynchronizeRecord sync_rec);
public:
    static DWORD MainThreadID;
    static HANDLE sync_event_;
    static boost::recursive_mutex mutex_;
    static WakeMainThreadCallback wake;
    static std::list<PSyncProc> sync_list_;
};

} // namespace cactus

#endif // __CACTUS_CACTUS_THREAD_HELPER_H__