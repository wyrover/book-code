// Program 14.5 Thread operations
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <threads.h>
#include <math.h>

#define thread_count 5                          // Number of task threads

thrd_t thread_id[thread_count];                 // Array of thread identifiers
size_t task = 0;                                // Integer identifying a task
struct timespec duration = {.tv_sec = 1, .tv_nsec = 0};   // Task work period
mtx_t task_mtx;                                 // Mutex for access to task

// Function to carry out a task
int execute_task(void *arg)
{
    mtx_lock(&task_mtx);
    size_t local_task = ++task;
    mtx_unlock(&task_mtx);
//  mtx_lock(&task_mtx);                          // mutex lock - blocks until acquired
//  printf_s("Task %zd started.\n", ++task);
    printf_s("Task %zd started.\n", local_task);
    thrd_sleep(&duration, NULL);                    // Just to make things take longer...
    double x = 0;

    for (int i = 0 ; i < 1000000000 ; ++i)
        x = sqrt(3.1415926);

    printf_s("  Task %zd finished\n", local_task);
//  printf_s("  Task %zd finished\n", task);
//  mtx_unlock(&task_mtx);                         // mutex unlock - for use by other threads
    return 0;
}

int main(void)
{
    if (thrd_error == mtx_init(&task_mtx, mtx_timed)) {
        fprintf_s(stderr, "Mutex creation failed.\n");
        thrd_exit(-2);
    }

    // Create the threads to carry out the tasks concurrently
    for (size_t i = 0 ; i < thread_count ; ++i)
        if (thrd_error == thrd_create(&(thread_id[i]), execute_task, NULL)) {
            fprintf_s(stderr, "Thread creation failed.\n");
            thrd_exit(-1);
        }

    // Join the additional threads to the main thread
    for (size_t j = 0 ; j < thread_count ; ++j)
        thrd_join(thread_id[j], NULL);

    return 0;
}
