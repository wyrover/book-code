/*++
Module Name:
    pdc.h
Abstract:

    This is the main source file for the Windows/NT PDC API demonstration
    program.  This include file defines macros, data structures and procedure
    prototypes for the code contained in the PDC.C source file.

--*/

//
// Include file for using the Win32 API
//

#include <windows.h>

//
// C Runtime include files used by the program.
//

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <malloc.h>
#include <sys\types.h>
#include <sys\stat.h>


//
// Macro definitions
//

//
// Useful rounding macros that the rounding amount is always a
// power of two.
//

#define ROUND_DOWN( Size, Amount ) ((DWORD)(Size) & ~((Amount) - 1))
#define ROUND_UP( Size, Amount ) (((DWORD)(Size) + ((Amount) - 1)) & ~((Amount) - 1))

//
// Pseudo keywords for documentation purposes.
//

#define IN
#define OUT
#define OPTIONAL

//
// Determine if an argument is present by testing a value of NULL
//

#define ARGUMENT_PRESENT( ArgumentPointer )    (\
    (LPSTR)(ArgumentPointer) != (LPSTR)(NULL) )



//
// Calculate the address of the base of the structure given its type, and an
// address of a field within the structure.
//

#define CONTAINING_RECORD( address, type, field ) ((type *)( \
                                                   (PCHAR)(address) - \
                                                   (PCHAR)(&((type *)0)->field)))


//
// Virtual Buffer data structure and function prototypes.
//

typedef struct _VIRTUAL_BUFFER {
    LPVOID Base;
    ULONG PageSize;
    LPVOID CommitLimit;
    LPVOID ReserveLimit;
} VIRTUAL_BUFFER, *PVIRTUAL_BUFFER;

BOOL
CreateVirtualBuffer(
    OUT PVIRTUAL_BUFFER Buffer,
    IN DWORD CommitSize,
    IN DWORD ReserveSize OPTIONAL
    );

BOOL
ExtendVirtualBuffer(
    IN PVIRTUAL_BUFFER Buffer,
    IN LPVOID Address
    );

BOOL
TrimVirtualBuffer(
    IN PVIRTUAL_BUFFER Buffer
    );

BOOL
FreeVirtualBuffer(
    IN PVIRTUAL_BUFFER Buffer
    );

int
VirtualBufferExceptionFilter(
    IN DWORD ExceptionCode,
    IN PEXCEPTION_POINTERS ExceptionInfo,
    IN OUT PVIRTUAL_BUFFER Buffer
    );


//
// Directory enumeration and file notification definitions.
//

typedef
VOID (*PDIRECTORY_ENUMERATE_ROUTINE)(
    LPSTR Path,
    PWIN32_FIND_DATA FindFileData,
    PVOID EnumerateParameter
    );

BOOL
EnumerateDirectoryTree(
    LPSTR DirectoryPath,
    PDIRECTORY_ENUMERATE_ROUTINE EnumerateRoutine,
    PVOID EnumerateParameter
    );

//
// Data structures private to the EnumerateDirectoryTree function.
//

typedef struct _ENUMERATE_DIRECTORY_STACK {
    LPSTR PathEnd;
    HANDLE FindHandle;
} ENUMERATE_DIRECTORY_STACK, *PENUMERATE_DIRECTORY_STACK;

#define MAX_DEPTH 256

typedef struct _ENUMERATE_DIRECTORY_STATE {
    DWORD Depth;
    ENUMERATE_DIRECTORY_STACK Stack[ MAX_DEPTH ];
    CHAR Path[ MAX_PATH ];
} ENUMERATE_DIRECTORY_STATE, *PENUMERATE_DIRECTORY_STATE;



//
//  Doubly-linked list manipulation routines.  Implemented as macros
//  but logically these are procedures.
//

//
//  VOID
//  InitializeListHead(
//      PLIST_ENTRY ListHead
//      );
//

#define InitializeListHead(ListHead) (\
    (ListHead)->Flink = (ListHead)->Blink = (ListHead) )

//
//  BOOLEAN
//  IsListEmpty(
//      PLIST_ENTRY ListHead
//      );
//

#define IsListEmpty(ListHead) (\
    ( ((ListHead)->Flink == (ListHead)) ? TRUE : FALSE ) )

//
//  PLIST_ENTRY
//  RemoveHeadList(
//      PLIST_ENTRY ListHead
//      );
//

#define RemoveHeadList(ListHead) \
    (ListHead)->Flink;\
    {\
        PLIST_ENTRY FirstEntry;\
        FirstEntry = (ListHead)->Flink;\
        FirstEntry->Flink->Blink = (ListHead);\
        (ListHead)->Flink = FirstEntry->Flink;\
    }

//
//  VOID
//  RemoveEntryList(
//      PLIST_ENTRY Entry
//      );
//

#define RemoveEntryList(Entry) {\
        PLIST_ENTRY _EX_Entry;\
        _EX_Entry = (Entry);\
        _EX_Entry->Blink->Flink = _EX_Entry->Flink;\
        _EX_Entry->Flink->Blink = _EX_Entry->Blink;\
        }

//
//  VOID
//  InsertTailList(
//      PLIST_ENTRY ListHead,
//      PLIST_ENTRY Entry
//      );
//

#define InsertTailList(ListHead,Entry) \
    (Entry)->Flink = (ListHead);\
    (Entry)->Blink = (ListHead)->Blink;\
    (ListHead)->Blink->Flink = (Entry);\
    (ListHead)->Blink = (Entry)

//
//  VOID
//  InsertHeadList(
//      PLIST_ENTRY ListHead,
//      PLIST_ENTRY Entry
//      );
//

#define InsertHeadList(ListHead,Entry) \
    (Entry)->Flink = (ListHead)->Flink;\
    (Entry)->Blink = (ListHead);\
    (ListHead)->Flink->Blink = (Entry);\
    (ListHead)->Flink = (Entry)



//
// Worker thread definitions
//

typedef
VOID
(*PWORKER_ROUTINE)(
  //  IN PWORK_QUEUE_ITEM WorkItem
    );

typedef struct _WORK_QUEUE {
    CRITICAL_SECTION CriticalSection;
    HANDLE Semaphore;
    LIST_ENTRY Queue;
    BOOL Terminating;
    PWORKER_ROUTINE WorkerRoutine;
    DWORD NumberOfWorkerThreads;
    HANDLE WorkerThreads[ 1 ];      // Variable length array
} WORK_QUEUE, *PWORK_QUEUE;

typedef struct _WORK_QUEUE_ITEM {
    LIST_ENTRY List;
    DWORD Reason;
    PWORK_QUEUE WorkQueue;
} WORK_QUEUE_ITEM, *PWORK_QUEUE_ITEM;

#define WORK_INITIALIZE_ITEM 0
#define WORK_ITEM            1
#define WORK_TERMINATE_ITEM  2

DWORD
WorkerThread(
    LPVOID lpThreadParameter
    );

PWORK_QUEUE
CreateWorkQueue(
    IN DWORD NumberOfWorkerThreads,
    IN PWORKER_ROUTINE WorkerRoutine
    );

BOOL
QueueWorkItem(
    IN OUT PWORK_QUEUE WorkQueue,
    IN PWORK_QUEUE_ITEM WorkItem
    );

VOID
DestroyWorkQueue(
    IN OUT PWORK_QUEUE WorkQueue
    );


//
// Main procedure prototypes
//

BOOL
ProcessCommandLineArguments(
    int argc,
    char *argv[]
    );

VOID
QueueSearchFile(
    LPSTR Path,
    PWIN32_FIND_DATA FindFileData,
    PVOID EnumerateParameter
    );

typedef struct _SEARCH_REQUEST_STATE {
    VIRTUAL_BUFFER Buffer;
    LPSTR CurrentOutput;
} SEARCH_REQUEST_STATE, *PSEARCH_REQUEST_STATE;


typedef struct _SEARCH_REQUEST {
    WORK_QUEUE_ITEM WorkItem;
    DWORD FileSize;
    LPSTR FileData;
    HANDLE File;
    OVERLAPPED OverlappedIO;
    CHAR FullPathName[ 1 ];         // Variable length array
} SEARCH_REQUEST, *PSEARCH_REQUEST;


VOID
ProcessRequest(
    IN PWORK_QUEUE_ITEM WorkItem
    );

VOID
ProcessReadFileCompletion(
    DWORD dwErrorCode,
    DWORD dwNumberOfBytesTransfered,
    LPOVERLAPPED lpOverlapped
    );

VOID
ProcessSearchFile(
    IN PSEARCH_REQUEST WorkerRequest,
    IN PSEARCH_REQUEST_STATE State
    );


//
// Global variables
//

DWORD TlsIndex;
INT NumberOfWorkerThreads;
BOOL Verbose;
BOOL IgnoreCase;
BOOL MappedFileIO;
BOOL ASyncIO;
BOOL SyncIO;
LPSTR DirectoryPath;
LPSTR SearchString;
DWORD SearchStringLength;
DWORD SearchedFileCount;
DWORD MatchedFileCount;
DWORD MatchedLineCount;
LONG  OutstandingIOOperations;
HANDLE IoCompletedEvent;

typedef
int
(_CRTAPI1 *PCOMPARE_ROUTINE)(
    IN const char *,
    IN const char *,
    IN size_t
    );

PCOMPARE_ROUTINE SearchFunction;
