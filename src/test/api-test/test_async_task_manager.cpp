#include <catch.hpp>
#include <tracetool/tracetool.h>
#include "common/async_task_manager/task_manager.h"
#include <Windows.h>
#include <iostream>
#include <conio.h>

class SimpleTask : public Task
{
public:
    SimpleTask(unsigned id, TaskEventsBroadcaster *renderStatistics) : Task(id, renderStatistics), dummy(0.0f) {}
    void run()
    {
        tasksStatistics->taskStarted(id);
        std::cout << "Running task (" << id << ")" << std::endl;

        // Perform some heavy computation...
        for (int i = 0; i < 1000000; ++i) {
            dummy += sqrtf(i);
        }

        tasksStatistics->taskEnded(id);
    }

private:
    float dummy;
};

TEST_CASE("Test for common/async_task_manager/task_manager", "[common/async_task_manager/task_manager]")
{
    SECTION("testTaskManager", "[common/async_task_manager/task_manager]") {
        TaskManager *taskManager = new TaskManager();
        const unsigned maxTasks = 100;

        for (unsigned i = 0; i < maxTasks; ++i) {
            Task *task = new SimpleTask(i, taskManager->getTaskEventsBroadcaster());
            taskManager->postTask(task);
        }

        taskManager->execute();

        while (taskManager->getExecutionState() != TasksExecutionState::FINISHED) {
            //std::cout << "Running: " << taskManager->getProgress() << std::endl;
            int c = _getch();

            if (c == 'p') {
                std::cout << "Paused" << std::endl;
                taskManager->pause();
            } else if (c == 'r') {
                std::cout << "Resumed" << std::endl;
                taskManager->resume();
            }
        }

        delete taskManager;
    }
}

