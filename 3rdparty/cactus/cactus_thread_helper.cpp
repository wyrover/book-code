/*!
* \file thread_helper.cpp
* \brief 线程回调UI对象方法帮助器实现 
* 
* 线程回调UI对象方法帮助器
* 
* \author wangyang 
* \date 2015/03/03 
* \version 1.0
*/

#include <cactus/cactus_thread_helper.h>
namespace cactus
{
DWORD TThread::MainThreadID = 0;
HANDLE TThread::sync_event_ = INVALID_HANDLE_VALUE;
boost::recursive_mutex TThread::mutex_;
WakeMainThreadCallback TThread::wake = NULL;
std::list<PSyncProc> TThread::sync_list_;


void TThread::Synchronize(PSynchronizeRecord sync_rec)
{
    PSyncProc sync_proc_ptr;

    if (::GetCurrentThreadId() == MainThreadID) {
        if (sync_rec->method_callback)
            sync_rec->method_callback();
    } else {
        sync_proc_ptr = new TSyncProc();
        sync_proc_ptr->signal = ::CreateEvent(NULL, TRUE, FALSE, NULL);     
        sync_proc_ptr->sync_rec = sync_rec;
		boost::recursive_mutex::scoped_lock lock(mutex_);
        sync_list_.push_back(sync_proc_ptr);
		lock.unlock();
        ResetEvent(sync_proc_ptr->signal);       
      
		if (TThread::wake) {
			TThread::wake();
		}
        WaitForSingleObject(sync_proc_ptr->signal, INFINITE);
		CloseHandle(sync_proc_ptr->signal);
		delete sync_proc_ptr;
		
    }
}

void TThread::Synchronize(HANDLE thread_handle, TThreadMethod method_callback)
{
    TSynchronizeRecord sync_rec;
    sync_rec.thread_handle = thread_handle;
    sync_rec.method_callback = method_callback;
    TThread::Synchronize(&sync_rec);
}

void TThread::Synchronize(TThreadMethod method_callback)
{
    TSynchronizeRecord sync_rec;    
    sync_rec.method_callback = method_callback;
    TThread::Synchronize(&sync_rec);
}

BOOL TThread::CheckSynchronize(int timeout /*= 0*/)
{
    PSyncProc local_sync_proc;
    std::list<PSyncProc> local_sync_list;

    if (::GetCurrentThreadId() != MainThreadID)
        return FALSE;

    //if (timeout > 0) {
    //    if (::WaitForSingleObject(sync_event_, timeout) == WAIT_OBJECT_0)
    //        ::ResetEvent(sync_event_);
    //} else {
    //    ::ResetEvent(sync_event_);
    //}
	
    boost::recursive_mutex::scoped_lock lock(mutex_);
    local_sync_list.swap(sync_list_);
    lock.unlock();

    if (local_sync_list.size() > 0) {
        std::list<PSyncProc>::iterator i;

        for (i = local_sync_list.begin(); i != local_sync_list.end();) {
            local_sync_proc = (*i);
            local_sync_list.erase(i++);
		
            if (local_sync_proc->sync_rec->method_callback) {
                local_sync_proc->sync_rec->method_callback();
            }
       
			::SetEvent(local_sync_proc->signal);            
        }
    }

    return TRUE;
}


} // namespace cactus