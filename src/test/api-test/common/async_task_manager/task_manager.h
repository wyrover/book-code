#ifndef task_manager_h__
#define task_manager_h__

#if _MSC_VER >= 1400
//  Following 8 lines: workaround for a bug in some older SDKs
#   pragma push_macro("_interlockedbittestandset")
#   pragma push_macro("_interlockedbittestandreset")
#   pragma push_macro("_interlockedbittestandset64")
#   pragma push_macro("_interlockedbittestandreset64")
#   define _interlockedbittestandset _local_interlockedbittestandset
#   define _interlockedbittestandreset _local_interlockedbittestandreset
#   define _interlockedbittestandset64 _local_interlockedbittestandset64
#   define _interlockedbittestandreset64 _local_interlockedbittestandreset64
#   include <intrin.h> // to force the header not to be included elsewhere
#   pragma pop_macro("_interlockedbittestandreset64")
#   pragma pop_macro("_interlockedbittestandset64")
#   pragma pop_macro("_interlockedbittestandreset")
#   pragma pop_macro("_interlockedbittestandset")
#endif

#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include "task.h"
#include "task_events_broadcaster.h"

enum TasksExecutionState { NONE, RUNNING, FINISHED, SUSPENDED };

class TaskManager
{
public:
    TaskManager();
    ~TaskManager();
    void postTask(Task *task);
    void execute();
    void pause();
    void resume();
    void finish();
    float getProgress() const;
    TaskEventsBroadcaster *getTaskEventsBroadcaster();
    TasksExecutionState getExecutionState() const;
    unsigned getTaskCount() const;

private:
    TasksExecutionState state;
    boost::asio::io_service service;
    boost::thread_group threadpool;
    boost::asio::io_service::work work;
    unsigned taskCount;
    TaskEventsBroadcaster stats;
};

#endif // task_manager_h__
