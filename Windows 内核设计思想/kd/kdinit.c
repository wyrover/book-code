/*
 * PROJECT:         ReactOS Kernel
 * LICENSE:         GPL - See COPYING in the top level directory
 * FILE:            ntoskrnl/kd64/kdinit.c
 * PURPOSE:         KD64 Initialization Code
 * PROGRAMMERS:     Alex Ionescu (alex.ionescu@reactos.org)
 *                  Stefan Ginsberg (stefan.ginsberg@reactos.org)
 */

/* INCLUDES ******************************************************************/

#include <ntoskrnl.h>
#define NDEBUG
#include <debug.h>
#include "kdapi.h"
#include <mm\ARM3\miarm.h>

//++ ree
#ifdef SIMULATE_DEBUG
#pragma comment(lib,"kernel32.lib")

#define PIPE_ACCESS_DUPLEX          0x00000003
#define PIPE_WAIT                   0x00000000
#define PIPE_READMODE_BYTE          0x00000000
#define PIPE_TYPE_BYTE              0x00000000

#define FILE_FLAG_FIRST_PIPE_INSTANCE   0x00080000

#define ERROR_PIPE_CONNECTED             535L

typedef struct _SECURITY_ATTRIBUTES {
    DWORD nLength;
    LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

typedef struct _OVERLAPPED {
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union {
        struct {
            DWORD Offset;
            DWORD OffsetHigh;
        };

        PVOID Pointer;
    };

    HANDLE  hEvent;
} OVERLAPPED, *LPOVERLAPPED;

HANDLE
WINAPI
CreateNamedPipeA(
    __in     LPCSTR lpName,
    __in     DWORD dwOpenMode,
    __in     DWORD dwPipeMode,
    __in     DWORD nMaxInstances,
    __in     DWORD nOutBufferSize,
    __in     DWORD nInBufferSize,
    __in     DWORD nDefaultTimeOut,
    __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes
);

BOOL
WINAPI
PeekNamedPipe(
    __in      HANDLE hNamedPipe,
    __out_bcount_part_opt(nBufferSize, *lpBytesRead) LPVOID lpBuffer,
    __in      DWORD nBufferSize,
    __out_opt LPDWORD lpBytesRead,
    __out_opt LPDWORD lpTotalBytesAvail,
    __out_opt LPDWORD lpBytesLeftThisMessage
);

BOOL
WINAPI
ReadFile(
    __in        HANDLE hFile,
    __out_bcount_part_opt(nNumberOfBytesToRead, *lpNumberOfBytesRead) __out_data_source(FILE) LPVOID lpBuffer,
    __in        DWORD nNumberOfBytesToRead,
    __out_opt   LPDWORD lpNumberOfBytesRead,
    __inout_opt LPOVERLAPPED lpOverlapped
);

BOOL
WINAPI
WriteFile(
    __in        HANDLE hFile,
    __in_bcount_opt(nNumberOfBytesToWrite) LPVOID lpBuffer,
    __in        DWORD nNumberOfBytesToWrite,
    __out_opt   LPDWORD lpNumberOfBytesWritten,
    __inout_opt LPOVERLAPPED lpOverlapped
);

BOOL
WINAPI
ConnectNamedPipe(
    __in        HANDLE hNamedPipe,
    __inout_opt LPOVERLAPPED lpOverlapped
);

BOOL
WINAPI
CloseHandle(
    __in HANDLE hObject
);

DWORD
WINAPI
GetLastError(
    VOID
);

VOID
WINAPI
Sleep(
    __in DWORD dwMilliseconds
);

BOOL WINAPI VirtualProtect(
    __in          LPVOID lpAddress,
    __in          SIZE_T dwSize,
    __in          DWORD flNewProtect,
    __out         LPDWORD lpflOldProtect
);

typedef LONG(WINAPI *PTOP_LEVEL_EXCEPTION_FILTER)(
    __in struct _EXCEPTION_POINTERS *ExceptionInfo
);
typedef PTOP_LEVEL_EXCEPTION_FILTER LPTOP_LEVEL_EXCEPTION_FILTER;

LPTOP_LEVEL_EXCEPTION_FILTER
WINAPI
SetUnhandledExceptionFilter(
    __in LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter
);

BOOL KdpVirtualProtect(IN ULONG Address, IN ULONG dwSize)
{
    DWORD dwOldProtect = 0;

    if (VirtualProtect((LPVOID)Address,             // Beginning of the set of pages to change
                       dwSize,                    // Length, in bytes, of the set of pages to change
                       PAGE_EXECUTE_READWRITE,    // What to change it to
                       &dwOldProtect              // Place to store the old setting
                      )) {
        return TRUE;
    }

    return FALSE;
}

ULONG_PTR PsNtosImageBase = 0x00400000;

HANDLE g_hPipe = NULL;

UCHAR SIMULATE_READ_PORT_UCHAR(IN PUCHAR Port)
{
    UCHAR chbuf = 0;
    DWORD avail = 0;

    if (PeekNamedPipe(g_hPipe, NULL, 0, NULL, &avail, NULL) /*&& avail > 0*/) {
        DWORD NumberOfBytesRead = 0;
        ReadFile(g_hPipe, &chbuf, 1, &NumberOfBytesRead, NULL);
    }

    return chbuf;
}

VOID SIMULATE_WRITE_PORT_UCHAR(IN UCHAR Value)
{
    DWORD dwWrite;
    WriteFile(g_hPipe, &Value, 1, &dwWrite, NULL);
}

/* 接管所有的异常 */
LONG WINAPI ExceptionFilter(
    __in struct _EXCEPTION_POINTERS *ExceptionInfo
)
{
    DbgBreakPoint(); /* 采用相同的处理, 向调试器报告异常。*/
    return EXCEPTION_EXECUTE_HANDLER;
}

VOID SIMULATE_INIT_PORT()
{
    g_hPipe = CreateNamedPipeA("\\\\.\\pipe\\com_1",
                               PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE,
                               PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                               1, 4096, 4096, 0, NULL);

    if (!ConnectNamedPipe(g_hPipe, NULL) && GetLastError() != ERROR_PIPE_CONNECTED) {
        CloseHandle(g_hPipe);
        g_hPipe = INVALID_HANDLE_VALUE;
    }

    SetUnhandledExceptionFilter(ExceptionFilter);
    Sleep(1000);
}
#else
ULONG_PTR PsNtosImageBase = 0x80400000;
#endif

BOOLEAN KdEnteredDebugger;
BOOLEAN KdIgnoreUmExceptions = FALSE;
KD_CONTEXT KdpContext = {20, 0};
ULONG KdpBootPhase = 0;
CHAR KdpMessageBuffer[0x1000] = {0};
ULONG NtGlobalFlag = 0;
ULONG KdpCurrentSymbolStart = 0;
ULONG KdpCurrentSymbolEnd = 0;
ULONG KdpNumInternalBreakpoints = 0;
ULONG NtBuildNumber = VER_PRODUCTBUILD | 0xC0000000;
ULONG KdpNextPacketIdToSend = INITIAL_PACKET_ID | SYNC_PACKET_ID;
ULONG KdpPacketIdExpected = INITIAL_PACKET_ID;
ULONG KeFeatureBits = 0;
UCHAR KdpKeNumberProcessors = 1;
//CCHAR KeNumberProcessors = 1;
//
// KeErrorMask - This is the value used to mask the error code passed to
//      memory management on page faults.
//
ULONG KeErrorMask = 1;

BOOLEAN BreakpointsSuspended = FALSE;
BOOLEAN KdpContextSent = FALSE;
BOOLEAN KdAutoEnableOnEvent = FALSE;
BOOLEAN KdPreviouslyEnabled = FALSE;
BOOLEAN KdPitchDebugger = FALSE;
BOOLEAN KdpControlCPending   = FALSE;
BOOLEAN KdpControlCPressed   = FALSE;
BOOLEAN KdDebuggerNotPresent = FALSE;
BOOLEAN KdDebuggerEnabled    = 0;

CHAR KdpPathBuffer[0x1000] = {0};
CCHAR KdNumberProcessors = 1;
USHORT KeProcessorLevel = 6;
PVOID KdpNtosImageBase = NULL;
PVOID KeUserExceptionDispatcher;
BREAKPOINT_ENTRY KdpBreakpointTable[BREAKPOINT_TABLE_SIZE] = {0};

EPROCESS KdDebugProcess = {0};
KTHREAD KdDebugThread = {0};
KPRCB KdDebugPrcb = {0};
PKPRCB KiProcessorBlock[MAXIMUM_PROCESSORS] = {&KdDebugPrcb, 0};
CPPORT KdDebugComPort = {COM1BASE, 115200, 0};

VOID
NTAPI
KdUpdateDataBlock(VOID)
{
    /* Update the KeUserCallbackDispatcher pointer */
    /*KdDebuggerDataBlock.KeUserCallbackDispatcher =
        (ULONG_PTR)KeUserCallbackDispatcher;*/
}

VOID
NTAPI
DebugLoadImageSymbols(
    IN PCHAR ImageName,
    IN PVOID BaseAddress
)
{
    STRING FileName;
    FileName.Length = strlen(ImageName);
    FileName.Buffer = ImageName;
    FileName.MaximumLength = FileName.Length + sizeof(CHAR);
    DbgLoadImageSymbols(&FileName, BaseAddress, 0xffffffff);
}

VOID
NTAPI
DebugUnLoadImageSymbols(
    IN PSTRING FileName,
    IN PKD_SYMBOLS_INFO SymbolInfo
)
{
    DebugService(BREAKPOINT_UNLOAD_SYMBOLS, FileName, SymbolInfo, NULL, NULL);
}

VOID KdStopDebugger()
{
    if (KdDebuggerEnabled) {
        KD_SYMBOLS_INFO SymbolInfo = {0};
        SymbolInfo.BaseOfDll = (PVOID) - 1;
        SymbolInfo.ProcessId = 0;
        DebugUnLoadImageSymbols(NULL, &SymbolInfo);
        KdpSuspendAllBreakPoints();
        KdDebuggerEnabled = FALSE;
        KdDebuggerNotPresent = TRUE;
        KiDebugRoutine = KdpStub;
    }
}

BOOLEAN
NTAPI
KdRegisterDebuggerDataBlock(IN ULONG Tag,
                            IN PDBGKD_DEBUG_DATA_HEADER32 DataHeader,
                            IN ULONG Size)
{
    //KIRQL OldIrql;
    PLIST_ENTRY NextEntry;
    PDBGKD_DEBUG_DATA_HEADER32 CurrentHeader;
    /* Acquire the Data Lock */
    //KeAcquireSpinLock(&KdpDataSpinLock, &OldIrql);
    /* Loop the debugger data list */
    NextEntry = KdpDebuggerDataListHead.Flink;

    while (NextEntry != &KdpDebuggerDataListHead) {
        /* Get the header for this entry */
        CurrentHeader = CONTAINING_RECORD(NextEntry,
                                          DBGKD_DEBUG_DATA_HEADER32,
                                          List);
        /*  Move to the next one */
        NextEntry = NextEntry->Flink;

        /* Check if we already have this data block */
        if ((CurrentHeader == DataHeader) || (CurrentHeader->OwnerTag == Tag)) {
            /* Release the lock and fail */
            //KeReleaseSpinLock(&KdpDataSpinLock, OldIrql);
            return FALSE;
        }
    }

    /* Setup the header */
    DataHeader->OwnerTag = Tag;
    DataHeader->Size = Size;
    /* Insert it into the list and release the lock */
    InsertTailList(&KdpDebuggerDataListHead, (PLIST_ENTRY)&DataHeader->List);
    //KeReleaseSpinLock(&KdpDataSpinLock, OldIrql);
    return TRUE;
}

/* FUNCTIONS *****************************************************************/

BOOLEAN
NTAPI
KdpInitSystem(IN ULONG BootPhase,
              IN PLOADER_PARAMETER_BLOCK LoaderBlock)
{
    KdpBootPhase = LoaderBlock ? 1 : 0;

    /* Check if the Debugger Data Block was already initialized */
    if (!KdpDebuggerDataListHead.Flink) {
        /* It wasn't...Initialize the KD Data Listhead */
        InitializeListHead(&KdpDebuggerDataListHead);
        /* Register the Debugger Data Block */
        KdRegisterDebuggerDataBlock(KDBG_TAG,
                                    &KdDebuggerDataBlock.Header,
                                    sizeof(KdDebuggerDataBlock));
        /* Fill out the KD Version Block */
        KdVersionBlock.MajorVersion = (USHORT)((DBGKD_MAJOR_NT << 8) | (NtBuildNumber >> 28));
        KdVersionBlock.MinorVersion = (USHORT)(NtBuildNumber & 0xFFFF);
#ifdef CONFIG_SMP
        /* This is an MP Build */
        KdVersionBlock.Flags |= DBGKD_VERS_FLAG_MP | DBGKD_VERS_FLAG_DATA;
#endif
        KdVersionBlock.MachineType = IMAGE_FILE_MACHINE_I386;
        KdVersionBlock.ThCallbackStack = FIELD_OFFSET(KTHREAD, CallbackStack);
        KdVersionBlock.NextCallback = FIELD_OFFSET(KCALLOUT_FRAME, CallbackStack);
#if defined(_X86_)
        KdVersionBlock.FramePointer = FIELD_OFFSET(KCALLOUT_FRAME, Ebp);
#endif
        KdVersionBlock.BreakpointWithStatus = (ULONG)DbgBreakPointWithStatus;
        /* Save Pointers to Loaded Module List and Debugger Data */
        KdVersionBlock.PsLoadedModuleList = (ULONG)(LONG_PTR)&PsLoadedModuleList;
        KdVersionBlock.DebuggerDataList = (ULONG)(LONG_PTR)&KdpDebuggerDataListHead;
        /* Set protocol limits */
        KdVersionBlock.ProtocolVersion = DbgKdMaximumStateChange -
                                         DbgKdMinimumStateChange;

        if (KdpBootPhase) {
            /* Link us in the KPCR */
            KeGetPcr()->KdVersionBlock =  &KdVersionBlock;
            /* Setup debug mapping PTE */
            MmDebugPte =  LoaderBlock->Extension->DumpHeader;
        }
    }

    /* Check if we have a loader block */
    if (LoaderBlock) {
        /* Get the image entry */
        PLDR_DATA_TABLE_ENTRY LdrEntry = CONTAINING_RECORD(
                                             LoaderBlock->LoadOrderListHead.Flink,
                                             LDR_DATA_TABLE_ENTRY,
                                             InLoadOrderLinks);
        /* Save the Kernel Base */
        PsNtosImageBase = (ULONG_PTR)LdrEntry->DllBase;
        KdVersionBlock.KernBase = (ULONG)(LONG_PTR)LdrEntry->DllBase;
    } else {
        /* Called from a bugcheck or a re-enable. Save the Kernel Base */
        KdVersionBlock.KernBase = (ULONG)(LONG_PTR)PsNtosImageBase;
    }

    /* Set the Kernel Base in the Data Block */
    KdDebuggerDataBlock.KernBase = (ULONG_PTR)KdVersionBlock.KernBase;
    KdDebuggerDataBlock.PsLoadedModuleList = &PsLoadedModuleList;
    KdDebuggerDataBlock.MmPfnDatabase = &MmPfnDatabase;
    KdDebuggerDataBlock.MmPageSize = PAGE_SIZE;
    KdDebuggerDataBlock.ObpRootDirectoryObject = &ObpRootDirectoryObject;
    KdDebuggerDataBlock.ObpTypeObjectType = &ObpTypeObjectType;
    KdDebuggerDataBlock.PspCidTable = &PspCidTable;
    KdDebuggerDataBlock.PsActiveProcessHead = &PsActiveProcessHead;
    KdDebuggerDataBlock.MmHighestUserAddress = &MmHighestUserAddress;
    KdDebuggerDataBlock.MmSystemRangeStart = &MmSystemRangeStart;
    KdDebuggerDataBlock.MmUserProbeAddress = &MmUserProbeAddress;
    KdDebuggerDataBlock.MmNonPagedPoolStart = &MmNonPagedPoolStart;
    KdDebuggerDataBlock.MmNonPagedPoolEnd = &MmNonPagedPoolEnd0;
    KdDebuggerDataBlock.MmPagedPoolStart = &MmPagedPoolStart;
    KdDebuggerDataBlock.MmPagedPoolEnd = &MmPagedPoolEnd;
    //KdDebuggerDataBlock.NtBuildLab = "20080427-r33159";
#ifdef SIMULATE_DEBUG
    SIMULATE_INIT_PORT();
#else
    KdSetTrap();
#endif
    KdDebugThread.ApcState.Process = &KdDebugProcess.Pcb;
    KdDebugPrcb.CurrentThread = &KdDebugThread;
    KiDebugRoutine = KdpTrap;
    KdDebugThread.KernelStack = 0x61ffc;
    KdDebugThread.InitialStack = 0x61ffc;
    KdDebugThread.StackLimit = 0x2000;
    KdDebugThread.Teb = 0x7ffde000;
    /* Enable the kernel debugger */
    KdDebuggerEnabled = TRUE;
    /* Initialize the port, return it */
    CpInitialize(&KdDebugComPort, (PUCHAR)COM1BASE, 115200);
    return TRUE;
}