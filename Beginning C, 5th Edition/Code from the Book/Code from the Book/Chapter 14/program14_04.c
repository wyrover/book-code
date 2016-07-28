// Program 14.4 Joining threads
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

typedef struct MyData {
    thrd_t id;                                  // get_data thread ID
    int a;
    int b;
} MyData;

// Get data function
int get_data(void *pdata)
{
    MyData* pd = (MyData*)pdata;
    printf_s("The get_data thread received: data.a=%d and data.b=%d\n", pd->a, pd->b);
    int mult = 0;
    printf_s("Enter an integer multiplier:\n");
    scanf_s("%d", &mult);
    pd->a *= mult;
    printf_s("The get_data thread makes it: data.a=%d and data.b=%d\n", pd->a, pd->b);
    return 0;
}

// Process data function
int process_data(void * pdata)
{
    MyData* pd = (MyData*)pdata;
    int result = 0;

    if (thrd_error == thrd_join(pd->id, &result)) {
        fprintf_s(stderr, "get_data thread join by process_data failed.\n");
        thrd_exit(-1);                            // Terminate this thread with error results code
    }

    if (result == -1) {
        fprintf_s(stderr, "process_data thread reading data failed.\n");
        thrd_exit(-2);                            // Terminate this thread with error results code
    }

    printf_s("The process_data thread received: data.a=%d and data.b=%d\n", pd->a, pd->b);
    thrd_exit(0);                               // Terminate with success results code
}

int main(void)
{
    thrd_t process_id;                          // process_data thread ID
    MyData mydata = { .a = 123, .b = 345};
    printf_s("Before starting the get_data thread: mydata.a=%d and mydata. b=%d\n", mydata.a, mydata.b);

    switch (thrd_create(&mydata.id, get_data, &mydata)) {
    case thrd_success:
        printf_s("get_data thread started.\n");
        break;

    case thrd_nomem:
        fprintf_s(stderr, "Failed to allocate get_data thread memory.\n");

    case thrd_error:
        fprintf_s(stderr, "Failed to start get_data thread.\n");
        exit(1);
    }

    switch (thrd_create(&process_id, process_data, &mydata)) {
    case thrd_success:
        printf_s("process_data thread started.\n");
        break;

    case thrd_nomem:
        fprintf_s(stderr, "Failed to allocate process_data thread memory.\n");

    case thrd_error:
        fprintf_s(stderr, "Failed to start process_data thread.\n");
        exit(1);
    }

    thrd_join(process_id, NULL);
    printf_s("After both threads finish executing: mydata.a=%d and mydata. b=%d\n", mydata.a, mydata.b);
    return 0;
}
