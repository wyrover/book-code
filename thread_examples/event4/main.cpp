// console_template.cpp : Defines the entry point for the console application.
//

#include <SDKDDKVer.h>
#include <Windows.h>
#include <process.h>
#include <conio.h>
#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <iostream>

using namespace std;

const int number_of_readers = 3;
const int number_of_writers = 4;
const int wait_time_writers = 20;
const int wait_time_readers = 20;
const int maximum_resourse = 50;
bool writerlock = 0;
int queue_canwrite = 0;
int queue_canread = 0;
int readers = 0;
int resourse = 0;

HANDLE can_read_event;
HANDLE can_write_event;
//HANDLE rw_mutex;

void start_read()
{
    InterlockedIncrement((LONG*)&queue_canread);

    if (writerlock || queue_canwrite) {
        WaitForSingleObject(can_read_event, INFINITE);
    }

    InterlockedIncrement((LONG*)&readers);
    InterlockedDecrement((LONG*)&queue_canread);
}

void stop_read()
{
    InterlockedDecrement((LONG*)&readers);

    if (!queue_canread) {
        ResetEvent(can_read_event);
        SetEvent(can_write_event);
    }
}

void start_write()
{
    InterlockedIncrement((LONG*)&queue_canwrite);

    if (readers || writerlock) {
        WaitForSingleObject(can_write_event, INFINITE);
    }

    InterlockedDecrement((LONG*)&queue_canwrite);
    InterlockedIncrement((LONG*)&writerlock);
}

void stop_write()
{
    InterlockedDecrement((LONG*)&writerlock);

    if (queue_canread) {
        SetEvent(can_read_event);
    } else {
        SetEvent(can_write_event);
    }
}

void read(void*parametr)
{
    int * number = (int *)parametr;

    while (resourse  < maximum_resourse) {
        Sleep(rand() % wait_time_readers);
        start_read();
        printf("Hello! I`m reader. My number is: %d; Resourse = %d\n", *number, resourse);
        stop_read();
    }
}

void write(void*parametr)
{
    int * number = (int *)parametr;

    while (resourse  < maximum_resourse) {
        Sleep(rand() % wait_time_writers);
        start_write();
        //WaitForSingleObject(rw_mutex, INFINITE);
        resourse++;
        printf("Hello! I`m writer. My number is: %d; Resourse = %d\n", *number, resourse);
        //ReleaseMutex(rw_mutex);
        stop_write();
    }
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    _tsetlocale(LC_ALL, _T(""));
    int rw_count = number_of_readers + number_of_writers;
    int *read_write_num = new int[number_of_readers + number_of_writers];

    for (int i = 0; i < rw_count; i++) {
        read_write_num[i] = i;
    }

    //rw_mutex = CreateMutex(NULL, FALSE, NULL);
    can_read_event = CreateEvent(NULL, TRUE, TRUE, NULL);
    can_write_event = CreateEvent(NULL, FALSE, TRUE, NULL);
    HANDLE hThread[number_of_readers + number_of_writers];

    for (int i = 0; i < rw_count; i++) {
        void *parametr = &read_write_num[i];

        if (i < number_of_readers)
            hThread[i] = (HANDLE)_beginthread(read, 0, parametr);
        else
            hThread[i] = (HANDLE)_beginthread(write, 0, parametr);
    }

    _getch();
    return 0;
}

