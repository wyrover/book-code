/*

Module Name:
    tmpheap.c

Abstract:
    Test program for the MP heap package.

*/
#include "windows.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "mpheap.h"

DWORD
WINAPI
ThreadStartup(
    LPVOID Parameter
    );

LPVOID Malloc(DWORD dwBytes);
VOID Free(LPVOID lpMem);

BOOL Suicide = FALSE;

HANDLE MpHeap;

LPVOID
Malloc(
    DWORD dwBytes
    )
{
    return(MpHeapAlloc(MpHeap,0,dwBytes));
}

VOID
Free(
    LPVOID lpMem
    )
{
    MpHeapFree(MpHeap,lpMem);
}

// int
// _CRTAPI1
main (argc, argv)
    int argc;
    char *argv[];
{
    ULONG NumThreads;
    ULONG i;
    HANDLE *h;
    DWORD ThreadId;
    DWORD Parallelism;
    DWORD Seconds;
    LPMPHEAP_STATISTICS HeapStats;
    DWORD StatSize;
    DWORD Error;

    if (argc != 4) {
        fprintf(stderr, "Usage: tmpheap NumThreads HeapParallelism Seconds\n");
        exit(1);
    }
    NumThreads = atoi(argv[1]);
    Parallelism = atoi(argv[2]);
    Seconds = atoi(argv[3]);

    MpHeap = MpHeapCreate(0, 0, Parallelism);
    if (MpHeap == NULL) {
        fprintf(stderr, "MpHeapCreate failed error %d\n",GetLastError);
        exit(1);
    }
    h = Malloc(NumThreads * sizeof(HANDLE));
    if (h==NULL) {
        fprintf(stderr, "Malloc thread handle array failed\n");
        exit(1);
    }
    for (i=0; i<NumThreads; i++) {
        h[i] = CreateThread(NULL,
                            0,
                            ThreadStartup,
                            NULL,
                            0,
                            &ThreadId);
        if (h==NULL) {
            fprintf(stderr, "CreateThread %d failed %d\n",i,GetLastError());
            exit(1);
        }
    }

    Sleep(Seconds * 1000);
    Suicide = TRUE;
    WaitForMultipleObjects(NumThreads, h, TRUE, INFINITE);
    StatSize = Parallelism * sizeof(MPHEAP_STATISTICS);
    HeapStats = (LPMPHEAP_STATISTICS)LocalAlloc(LMEM_FIXED, StatSize);
    if (HeapStats != NULL) {
        Error = MpHeapGetStatistics(MpHeap,&StatSize, HeapStats);
        if (Error==ERROR_SUCCESS) {
            for (i=0; i<StatSize/sizeof(MPHEAP_STATISTICS);i++) {
                printf("HEAP %d\n",i);
                printf("  Allocations:      %8d\n",HeapStats[i].TotalAllocates);
                if (0 == HeapStats[i].TotalAllocates) continue;  // avoid divide by 0
                printf("  Contention :      %8d (%d%%)\n",HeapStats[i].Contention,
                        100*HeapStats[i].Contention/HeapStats[i].TotalAllocates);
                printf("  Lookaside Allocs: %8d (%d%%)\n",
                        HeapStats[i].LookasideAllocates,
                        100*HeapStats[i].LookasideAllocates/HeapStats[i].TotalAllocates);
                printf("  Frees:            %8d\n",HeapStats[i].TotalFrees);
                printf("  Lookaside Frees:  %8d (%d%%)\n",
                        HeapStats[i].LookasideFrees,
                        100*HeapStats[i].LookasideFrees/HeapStats[i].TotalFrees);
                printf("  Delayed Frees:    %8d (%d%%)\n",
                        HeapStats[i].DelayedFrees,
                        100*HeapStats[i].DelayedFrees/HeapStats[i].TotalFrees);
            }
        } else {
            fprintf(stderr, "MpHeapStatistics failed with error %d\n",Error);
        }
        MpHeapDestroy(MpHeap);
        LocalFree(HeapStats);
    }

    return(0);
}

DWORD
WINAPI
ThreadStartup(
    LPVOID Parameter
    )
{
    DWORD Seed = GetCurrentThreadId();
    PULONG Buffer = NULL;
    DWORD BufferSize;
    DWORD Delay;
    DWORD i,j;
    DWORD Fill;
    PULONG Last = NULL;
    BOOL AllocAgain;
    DWORD MaxSize=0;
    DWORD CurrentSize=0;
    DWORD CurrentAllocs = 0;
    DWORD MaxAllocs = 0;

    //
    // Loop allocating/filling/freeing random chunks
    // of memory.
    //
    Fill = GetCurrentThreadId();
    while (!Suicide) {
        //
        // Decide whether to allocate a new chunk or free the
        // last chunk.
        //
        AllocAgain = (BOOL)((rand() & 0xff) > 0x80);
        if (AllocAgain) {
            BufferSize = (rand() & 0xfff) + sizeof(PUCHAR) + sizeof(ULONG);

            CurrentAllocs++;
            if (CurrentAllocs > MaxAllocs) {
                MaxAllocs = CurrentAllocs;
            }
            Buffer = Malloc(BufferSize);
            if (Buffer == NULL) {
                fprintf(stderr,"malloc of %d returned NULL\n",BufferSize);
                return(0);
            }
            CurrentSize += BufferSize;
            if (CurrentSize > MaxSize) {
                if ((CurrentSize >> 18) != (MaxSize >> 18)) {
                    printf("Thread %x up to %d bytes %d allocs\n",
                            GetCurrentThreadId(),
                            CurrentSize,
                            CurrentAllocs);
                }
                MaxSize = CurrentSize;
            }
            if ((CurrentSize > MaxSize) &&
                (CurrentSize-MaxSize > 0x10000)) {
            }
            for (j=0; j<BufferSize/sizeof(ULONG); j++) {
                Buffer[j] = Fill;
            }
            Buffer[0] = (ULONG)Last;
            Buffer[1] = BufferSize;
            Last = Buffer;
        } else if (Buffer != NULL) {
            Last = (PULONG)Buffer[0];
            CurrentSize -= Buffer[1];
            --CurrentAllocs;
            Free(Buffer);
            Buffer = Last;
        }

        Delay = rand() & 0x1ff;
        for (i=0; i< Delay; i++) {
            Fill = GetCurrentThreadId();
        }
        if ((rand() & 0xffff) < (int)CurrentAllocs) {
            //
            // Magic number, free EVERYTHING!
            //
            while (Buffer != NULL) {
                Last = (PULONG)Buffer[0];
                CurrentSize -= Buffer[1];
                --CurrentAllocs;
                Free(Buffer);
                Buffer = Last;
            }
        }
    }
    return(0);
}
