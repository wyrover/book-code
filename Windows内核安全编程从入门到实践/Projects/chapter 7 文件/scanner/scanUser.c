/*++

Copyright (c) 1999-2002  Microsoft Corporation

Module Name:

    scanUser.c

Abstract:

    This file contains the implementation for the main function of the
    user application piece of scanner.  This function is responsible for
    actually scanning file contents.

Environment:

    User mode

--*/

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <winioctl.h>
#include <string.h>
#include <crtdbg.h>
#include <assert.h>
#include "fltuser.h"
#include "scanuk.h"
#include "scanuser.h"
#include "dontuse.h"

//
//  Default and Maximum number of threads.
//

#define SCANNER_DEFAULT_REQUEST_COUNT       5
#define SCANNER_DEFAULT_THREAD_COUNT        2
#define SCANNER_MAX_THREAD_COUNT            64

UCHAR FoulString[] = "foul";

//
//  Context passed to worker threads
//

typedef struct _SCANNER_THREAD_CONTEXT {

    HANDLE Port;
    HANDLE Completion;

} SCANNER_THREAD_CONTEXT, *PSCANNER_THREAD_CONTEXT;


VOID
Usage(
    VOID
)
/*++

Routine Description

    Prints usage

Arguments

    None

Return Value

    None

--*/
{
    printf("Connects to the scanner filter and scans buffers \n");
    printf("Usage: scanuser [requests per thread] [number of threads(1-64)]\n");
}

BOOL
ScanBuffer(
    __in_bcount(BufferSize) PUCHAR Buffer,
    __in ULONG BufferSize
)
/*++

Routine Description

    Scans the supplied buffer for an instance of FoulString.

    Note: Pattern matching algorithm used here is just for illustration purposes,
    there are many better algorithms available for real world filters

Arguments

    Buffer      -   Pointer to buffer
    BufferSize  -   Size of passed in buffer

Return Value

    TRUE        -    Found an occurrence of the appropriate FoulString
    FALSE       -    Buffer is ok

--*/
{
    PUCHAR p;
    ULONG searchStringLength = sizeof(FoulString) - sizeof(UCHAR);

    for (p = Buffer;
         p <= (Buffer + BufferSize - searchStringLength);
         p++) {
        if (RtlEqualMemory(p, FoulString, searchStringLength)) {
            printf("Found a string\n");
            //
            //  Once we find our search string, we're not interested in seeing
            //  whether it appears again.
            //
            return TRUE;
        }
    }

    return FALSE;
}


DWORD
ScannerWorker(
    __in PSCANNER_THREAD_CONTEXT Context
)
/*++

Routine Description

    This is a worker thread that


Arguments

    Context  - This thread context has a pointer to the port handle we use to send/receive messages,
                and a completion port handle that was already associated with the comm. port by the caller

Return Value

    HRESULT indicating the status of thread exit.

--*/
{
    PSCANNER_NOTIFICATION notification;
    SCANNER_REPLY_MESSAGE replyMessage;
    PSCANNER_MESSAGE message;
    LPOVERLAPPED pOvlp;
    BOOL result;
    DWORD outSize;
    HRESULT hr;
    ULONG_PTR key;
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant

    while (TRUE) {
#pragma warning(pop)
        //
        //  获取已经完成的消息请求，并做进一步的内容扫描
        //
        result = GetQueuedCompletionStatus(Context->Completion, &outSize, &key, &pOvlp, INFINITE);
        //
        //  获取待扫描的消息。
        //  注意这里获取到的消息的顺序并不是投递消息的顺序，这也是IOCP模型高性能的原因之一。
        //  不过这些消息扫描请求都是独立的，不需要排序，直接处理获取到的这个随机的扫描请求就行了。
        //
        message = CONTAINING_RECORD(pOvlp, SCANNER_MESSAGE, Ovlp);

        if (!result) {
            hr = HRESULT_FROM_WIN32(GetLastError());
            break;
        }

        printf("Received message, size %d\n", pOvlp->InternalHigh);
        notification = &message->Notification;
        assert(notification->BytesToScan <= SCANNER_READ_BUFFER_SIZE);
        __analysis_assume(notification->BytesToScan <= SCANNER_READ_BUFFER_SIZE);
        //扫描内容是否有恶意特征值，扫描算法很简单，就是简单的匹配字符串
        result = ScanBuffer(notification->Contents, notification->BytesToScan);
        replyMessage.ReplyHeader.Status = 0;
        replyMessage.ReplyHeader.MessageId = message->MessageHeader.MessageId;
        //
        //  将扫描结果回复给微过滤驱动，由微过滤驱动根据扫描结果做最终的决定——是否放行I/O请求
        //
        //
        replyMessage.Reply.SafeToOpen = !result;
        printf("Replying message, SafeToOpen: %d\n", replyMessage.Reply.SafeToOpen);
        hr = FilterReplyMessage(Context->Port,
                                (PFILTER_REPLY_HEADER) &replyMessage,
                                sizeof(replyMessage));

        if (SUCCEEDED(hr)) {
            printf("Replied message\n");
        } else {
            printf("Scanner: Error replying message. Error = 0x%X\n", hr);
            break;
        }

        memset(&message->Ovlp, 0, sizeof(OVERLAPPED));
        //复用这个消息并重新投递
        hr = FilterGetMessage(Context->Port,
                              &message->MessageHeader,
                              FIELD_OFFSET(SCANNER_MESSAGE, Ovlp),
                              &message->Ovlp);

        if (hr != HRESULT_FROM_WIN32(ERROR_IO_PENDING)) {
            break;
        }
    }

    if (!SUCCEEDED(hr)) {
        if (hr == HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE)) {
            //
            //  端口已经断开了，退出线程
            //
            printf("Scanner: Port is disconnected, probably due to scanner filter unloading.\n");
        } else {
            printf("Scanner: Unknown error occured. Error = 0x%X\n", hr);
        }
    }

    free(message);
    return hr;
}


int _cdecl
main(
    __in int argc,
    __in_ecount(argc) char *argv[]
)
{
    DWORD requestCount = SCANNER_DEFAULT_REQUEST_COUNT;
    DWORD threadCount = SCANNER_DEFAULT_THREAD_COUNT;
    HANDLE threads[SCANNER_MAX_THREAD_COUNT];
    SCANNER_THREAD_CONTEXT context;
    HANDLE port, completion;
    PSCANNER_MESSAGE msg;
    DWORD threadId;
    HRESULT hr;
    DWORD i, j;
    //
    //  获取用户指定的参数
    //

    if (argc > 1) {
        requestCount = atoi(argv[1]);

        if (requestCount <= 0) {
            Usage();
            return 1;
        }

        if (argc > 2) {
            threadCount = atoi(argv[2]);
        }

        if (threadCount <= 0 || threadCount > 64) {
            Usage();
            return 1;
        }
    }

    //
    //  连接到通信服务端，返回一个通信客户端的句柄
    //
    printf("Scanner: Connecting to the filter ...\n");
    hr = FilterConnectCommunicationPort(ScannerPortName,
                                        0,
                                        NULL,
                                        0,
                                        NULL,
                                        &port);

    if (IS_ERROR(hr)) {
        printf("ERROR: Connecting to filter port: 0x%08x\n", hr);
        return 2;
    }

    //
    //  创建一个I/O完成端口，并关联到通信客户端句柄上。这样当微过滤驱动调用FltSendMessage完成一个等待消息时，
    //  将会向该I/O完成端口发送一个完成通知包，这样GetQueuedCompletionStatus函数将得到一个完成的请求消息。
    //
    completion = CreateIoCompletionPort(port,
                                        NULL,
                                        0,
                                        threadCount);

    if (completion == NULL) {
        printf("ERROR: Creating completion port: %d\n", GetLastError());
        CloseHandle(port);
        return 3;
    }

    printf("Scanner: Port = 0x%p Completion = 0x%p\n", port, completion);
    context.Port = port;
    context.Completion = completion;

    //
    //  创建工作线程
    //

    for (i = 0; i < threadCount; i++) {
        threads[i] = CreateThread(NULL,
                                  0,
                                  (LPTHREAD_START_ROUTINE)ScannerWorker,
                                  &context,
                                  0,
                                  &threadId);

        if (threads[i] == NULL) {
            hr = GetLastError();
            printf("ERROR: Couldn't create thread: %d\n", hr);
            goto main_cleanup;
        }

        for (j = 0; j < requestCount; j++) {
            //
            //  投递消息获取请求
            //
#pragma prefast(suppress:__WARNING_MEMORY_LEAK, "msg will not be leaked because it is freed in ScannerWorker")
            msg = malloc(sizeof(SCANNER_MESSAGE));

            if (msg == NULL) {
                hr = ERROR_NOT_ENOUGH_MEMORY;
                goto main_cleanup;
            }

            memset(&msg->Ovlp, 0, sizeof(OVERLAPPED));
            //
            //  FilterGetMessage实际上会将获取消息请求排队到通信客户端的消息等待链表中，
            //  直到微过滤驱动调用FltSendMessage完成消息请求
            //
            hr = FilterGetMessage(port,
                                  &msg->MessageHeader,
                                  FIELD_OFFSET(SCANNER_MESSAGE, Ovlp),
                                  &msg->Ovlp);

            if (hr != HRESULT_FROM_WIN32(ERROR_IO_PENDING)) {
                free(msg);
                goto main_cleanup;
            }
        }
    }

    hr = S_OK;
    //线程全部关闭后，退出程序
    WaitForMultipleObjectsEx(i, threads, TRUE, INFINITE, FALSE);
main_cleanup:
    printf("Scanner:  All done. Result = 0x%08x\n", hr);
    CloseHandle(port);
    CloseHandle(completion);
    return hr;
}

