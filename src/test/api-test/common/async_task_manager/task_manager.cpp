#include "task_manager.h"

#include <boost/bind.hpp>

TaskManager::TaskManager() : state(NONE), work(service), taskCount(0), stats(this)
{
    const int nbThreads = boost::thread::hardware_concurrency();

    for (int i = 0; i < nbThreads; ++i) {
        threadpool.create_thread(boost::bind(&boost::asio::io_service::run, &service));
    }
}

TaskManager::~TaskManager()
{
}

void TaskManager::postTask(Task *task)
{
    boost::shared_ptr<Task> taskPtr(task);
    service.post(boost::bind(&Task::run, taskPtr));
    ++taskCount;
}

void TaskManager::execute()
{
    state = TasksExecutionState::RUNNING;
    // Stop accepting tasks
    service.stop();
}

void TaskManager::pause()
{
    state = TasksExecutionState::SUSPENDED;
}

void TaskManager::resume()
{
    state = TasksExecutionState::RUNNING;
    stats.notifyTasks();
}

void TaskManager::finish()
{
    state = TasksExecutionState::FINISHED;
}

float TaskManager::getProgress() const
{
    return stats.getStatistics(TaskStatistics::NB_FINISHED_TASKS) / (float)taskCount;
}

TaskEventsBroadcaster *TaskManager::getTaskEventsBroadcaster()
{
    return &stats;
}

TasksExecutionState TaskManager::getExecutionState() const
{
    return state;
}

unsigned TaskManager::getTaskCount() const
{
    return taskCount;
}