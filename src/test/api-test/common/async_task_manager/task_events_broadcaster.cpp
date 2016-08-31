#include "task_events_broadcaster.h"

#include <iostream>

#include "task_manager.h"

static boost::condition_variable conditionVariable;

TaskEventsBroadcaster::TaskEventsBroadcaster(TaskManager *taskManager) : taskManager(taskManager)
{
    for (int i = 0; i < TaskStatistics::COUNT; ++i) {
        stats[i] = 0;
    }
}

void TaskEventsBroadcaster::taskStarted(unsigned taskId)
{
    boost::unique_lock<boost::mutex> lock(mutex);

    while (taskManager->getExecutionState() == TasksExecutionState::SUSPENDED) {
        std::cout << "Threads were suspended" << std::endl;
        conditionVariable.wait(lock);
    }

    //boost::mutex::scoped_lock scopedLock(mutex);
    ++stats[NB_STARTED_TASKS];
}

void TaskEventsBroadcaster::taskEnded(unsigned taskId)
{
    boost::mutex::scoped_lock scopedLock(mutex);
    ++stats[NB_FINISHED_TASKS];

    if (stats[NB_FINISHED_TASKS] == taskManager->getTaskCount()) {
        // All tasks have been executed.
        taskManager->finish();
    }
}

unsigned TaskEventsBroadcaster::getStatistics(TaskStatistics statType) const
{
    return stats[statType];
}

void TaskEventsBroadcaster::notifyTasks()
{
    conditionVariable.notify_all();
}