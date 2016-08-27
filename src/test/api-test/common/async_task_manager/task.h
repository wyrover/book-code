#ifndef task_h__
#define task_h__

class TaskEventsBroadcaster;

class Task
{
public:
    Task(unsigned id, TaskEventsBroadcaster *tasksStatistics) : id(id), tasksStatistics(tasksStatistics) {}
    virtual void run() = 0;

protected:
    unsigned id;
    TaskEventsBroadcaster *tasksStatistics;
};

#endif // task_h__
