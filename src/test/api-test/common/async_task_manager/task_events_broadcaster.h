#ifndef task_events_broadcaster_h__
#define task_events_broadcaster_h__

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
