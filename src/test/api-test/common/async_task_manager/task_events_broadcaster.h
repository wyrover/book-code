#ifndef task_events_broadcaster_h__
#define task_events_broadcaster_h__

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

#include <boost/thread/mutex.hpp>

class TaskManager;

enum TaskStatistics { NB_FINISHED_TASKS, NB_RUNNING_TASKS, NB_STARTED_TASKS, COUNT };

class TaskEventsBroadcaster
{
public:
    TaskEventsBroadcaster(TaskManager *taskManager);
    void taskStarted(unsigned taskId);
    void taskEnded(unsigned taskId);
    unsigned getStatistics(TaskStatistics statType) const;
    void notifyTasks();

private:
    unsigned stats[TaskStatistics::COUNT];
    TaskManager *taskManager;
    boost::mutex mutex;
};

#endif // task_events_broadcaster_h__
