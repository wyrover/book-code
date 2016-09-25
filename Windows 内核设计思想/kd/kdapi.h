#pragma once

#include "serial_port.h"
#include "windbgkd.h"

typedef ULONG KDP_STATUS;
typedef ULONG(*PFNDBGPRNT)(const char *Format, ...);

#ifndef KDDEBUG
    #define KDDBGPRINT(...)
#else
    #define KDDBGPRINT KdpDbgPrint
#endif

#define KD_BREAKPOINT_MAX  32
#define BREAKPOINT_TABLE_SIZE 32
#define KD_BREAKPOINT_TYPE UCHAR
#define KD_DEFAULT_LOG_BUFFER_SIZE 0x1000
#define MiGetPteAddress(va) ((PMMPTE)(((((ULONG)(va)) >> 12) << 2) + PTE_BASE))


#define PDBGKD_WAIT_STATE_CHANGE PDBGKD_WAIT_STATE_CHANGE32
#define PDBGKD_WRITE_MEMORY PDBGKD_WRITE_MEMORY32
#define PDBGKD_READ_WRITE_IO PDBGKD_READ_WRITE_IO32
#define PDBGKD_READ_WRITE_IO_EXTENDED PDBGKD_READ_WRITE_IO_EXTENDED32
#define DBGKD_WAIT_STATE_CHANGE DBGKD_WAIT_STATE_CHANGE32
#define PDBGKD_WRITE_BREAKPOINT PDBGKD_WRITE_BREAKPOINT32
#define PDBGKD_READ_MEMORY PDBGKD_READ_MEMORY32
#define PDBGKD_GET_VERSION PDBGKD_GET_VERSION32
#define DBGKD_GET_VERSION DBGKD_GET_VERSION32

enum {
    KDP_PACKET_RECEIVED = 0,
    KDP_PACKET_TIMEOUT = 1,
    KDP_PACKET_RESEND = 2
};

typedef enum _KDP_BREAKPOINT_FLAGS {
    KdpBreakpointActive = 1,
    KdpBreakpointPending = 2,
    KdpBreakpointSuspended = 4,
    KdpBreakpointExpired = 8
} KDP_BREAKPOINT_FLAGS;


KDP_STATUS
NTAPI
KdpReceivePacketLeader(
    OUT PULONG PacketLeader);

typedef struct _DBGKD_PAGEIN {
    ULONG   Address;
    ULONG   ContinueStatus;
} DBGKD_PAGEIN, *PDBGKD_PAGEIN;

typedef struct _DBGKD_MANIPULATE_STATE {
    ULONG ApiNumber;
    USHORT ProcessorLevel;
    USHORT Processor;
    NTSTATUS ReturnStatus;
    union {
        DBGKD_READ_MEMORY32 ReadMemory;
        DBGKD_WRITE_MEMORY32 WriteMemory;
        DBGKD_GET_CONTEXT GetContext;
        DBGKD_SET_CONTEXT SetContext;
        DBGKD_WRITE_BREAKPOINT32 WriteBreakPoint;
        DBGKD_RESTORE_BREAKPOINT RestoreBreakPoint;
        DBGKD_CONTINUE Continue;
        DBGKD_CONTINUE2 Continue2;
        DBGKD_READ_WRITE_IO32 ReadWriteIo;
        DBGKD_READ_WRITE_IO_EXTENDED32 ReadWriteIoExtended;
        DBGKD_QUERY_SPECIAL_CALLS QuerySpecialCalls;
        DBGKD_SET_SPECIAL_CALL32 SetSpecialCall;
        DBGKD_SET_INTERNAL_BREAKPOINT32 SetInternalBreakpoint;
        DBGKD_GET_INTERNAL_BREAKPOINT32 GetInternalBreakpoint;
        DBGKD_GET_VERSION32 GetVersion;
        DBGKD_BREAKPOINTEX BreakPointEx;
        DBGKD_PAGEIN PageIn;
        DBGKD_READ_WRITE_MSR ReadWriteMsr;
        DBGKD_GET_SET_BUS_DATA GetSetBusData;
        DBGKD_QUERY_MEMORY QueryMemory;
    } u;
} DBGKD_MANIPULATE_STATE, *PDBGKD_MANIPULATE_STATE;


typedef struct _BREAKPOINT_ENTRY {
    ULONG Flags;
    ULONG DirectoryTableBase;
    PVOID Address;
    KD_BREAKPOINT_TYPE Content;
} BREAKPOINT_ENTRY, *PBREAKPOINT_ENTRY;

typedef
BOOLEAN
(NTAPI *PKDEBUG_ROUTINE)(
    IN PKTRAP_FRAME TrapFrame,
    IN PKEXCEPTION_FRAME ExceptionFrame,
    IN PEXCEPTION_RECORD ExceptionRecord,
    IN PCONTEXT ContextRecord,
    IN KPROCESSOR_MODE PreviousMode,
    IN BOOLEAN SecondChance
);

typedef
BOOLEAN
(NTAPI *PKDEBUG_SWITCH_ROUTINE)(
    IN PEXCEPTION_RECORD ExceptionRecord,
    IN PCONTEXT Context,
    IN BOOLEAN SecondChance
);

extern KD_BREAKPOINT_TYPE KdpBreakpointInstruction;
extern BREAKPOINT_ENTRY KdpBreakpointTable[BREAKPOINT_TABLE_SIZE];
extern KD_CONTEXT KdpContext;
extern BOOLEAN KdpControlCPressed;
extern BOOLEAN KdAutoEnableOnEvent;
extern BOOLEAN KdPreviouslyEnabled;
extern EPROCESS KdDebugProcess;
extern KTHREAD KdDebugThread;
extern ULONG KdpStallScaleFactor;
extern ULONG KeErrorMask;
extern BOOLEAN KdpContextSent;
extern ULONG KdpNumInternalBreakpoints;
extern PVOID KdpNtosImageBase;
extern BOOLEAN BreakpointsSuspended;
extern UCHAR KdpKeNumberProcessors;
extern ULONG KdpBootPhase;
extern PKDEBUG_ROUTINE KiDebugRoutine;
extern PFNDBGPRNT KdpDbgPrint;
extern DBGKD_GET_VERSION KdVersionBlock;
extern PKDEBUG_ROUTINE KiDebugRoutine;
extern PKDEBUG_SWITCH_ROUTINE KiDebugSwitchRoutine;
extern BOOLEAN KdBlockEnable;
extern ULONG KdComponentTableSize;
extern ULONG Kd_WIN2000_Mask;
extern PULONG KdComponentTable[104];
extern CHAR KdpMessageBuffer[0x1000], KdpPathBuffer[0x1000];
extern BREAKPOINT_ENTRY KdpBreakpointTable[KD_BREAKPOINT_MAX];
extern ULONG_PTR KdpCurrentSymbolStart, KdpCurrentSymbolEnd;
extern ULONG TraceDataBuffer[40];
extern ULONG TraceDataBufferPosition;
extern LIST_ENTRY KdpDebuggerDataListHead;
extern PVOID MmNonPagedPoolEnd0;

extern KDDEBUGGER_DATA32 KdDebuggerDataBlock;

VOID
__cdecl
KdpDprintf(
    IN PCHAR Format,
    ...
);

FORCEINLINE
PKPRCB
NTAPI
KdpKeGetCurrentPrcb(VOID)
{
    if (KdpBootPhase == 0) return KiProcessorBlock[0];

#if (_MSC_FULL_VER >= 13012035)
    return (PKPRCB)(ULONG_PTR) __readfsdword(FIELD_OFFSET(KPCR, Prcb));
#else
    __asm {  mov eax, _PCR KPCR.Prcb     }
#endif
}

FORCEINLINE
struct _KTHREAD *
NTAPI KdpKeGetCurrentThread(VOID)
{
    if (KdpBootPhase == 0) return &KdDebugThread;

#if (_MSC_FULL_VER >= 13012035)
    return (struct _KTHREAD *)(ULONG_PTR) __readfsdword(FIELD_OFFSET(KIPCR, PrcbData.CurrentThread));
#else
    __asm {  mov eax, fs:[0] KPCR.PrcbData.CurrentThread }
#endif
}

FORCEINLINE
PEPROCESS
NTAPI
KdpPsGetCurrentProcess(VOID)
{
    if (KdpBootPhase == 0) return &KdDebugProcess;

    /* Return the current thread's process */
    return (PEPROCESS)((PETHREAD)(((PKIPCR)KeGetPcr())->PrcbData.CurrentThread))->Tcb.ApcState.Process;
}

FORCEINLINE
UCHAR
_declspec(naked)
NTAPI
KD_READ_PORT_UCHAR(
    IN PUCHAR Port
)
{
    _asm {
        mov     dx, [esp+4]
        in      al, dx
        retn    4
    }
}

VOID
NTAPI
KdpSendBuffer(
    IN PVOID Buffer,
    IN ULONG Size
);

KDP_STATUS
NTAPI
KdpReceiveBuffer(
    OUT PVOID Buffer,
    IN  ULONG Size
);

VOID
NTAPI
KdSetTrap(
    VOID
);

VOID
NTAPI
KdpCommandString(IN PSTRING NameString,
                 IN PSTRING CommandString,
                 IN KPROCESSOR_MODE PreviousMode,
                 IN PCONTEXT ContextRecord,
                 IN PKTRAP_FRAME TrapFrame,
                 IN PKEXCEPTION_FRAME ExceptionFrame);

NTSTATUS
NTAPI
KdpPrint(IN ULONG ComponentId,
         IN ULONG Level,
         IN LPSTR String,
         IN USHORT Length,
         IN KPROCESSOR_MODE PreviousMode,
         IN PKTRAP_FRAME TrapFrame,
         IN PKEXCEPTION_FRAME ExceptionFrame,
         OUT PBOOLEAN Handled);

VOID
NTAPI
KdpSetContextState(IN PDBGKD_WAIT_STATE_CHANGE WaitStateChange,
                   IN PCONTEXT Context);

NTSTATUS
NTAPI
KdpSysReadControlSpace(IN ULONG Processor,
                       IN ULONG BaseAddress,
                       IN PVOID Buffer,
                       IN ULONG Length,
                       OUT PULONG ActualLength);

NTSTATUS
NTAPI
KdpSysReadIoSpace(IN ULONG InterfaceType,
                  IN ULONG BusNumber,
                  IN ULONG AddressSpace,
                  IN ULONG IoAddress,
                  IN PVOID DataValue,
                  IN ULONG DataSize,
                  OUT PULONG ActualDataSize);

NTSTATUS
NTAPI
KdpSysWriteIoSpace(IN ULONG InterfaceType,
                   IN ULONG BusNumber,
                   IN ULONG AddressSpace,
                   IN ULONG IoAddress,
                   IN PVOID DataValue,
                   IN ULONG DataSize,
                   OUT PULONG ActualDataSize);

BOOLEAN
NTAPI
KdpPollBreakInWithPortLock(VOID);

VOID
NTAPI
KdpSendByte(IN BYTE Byte);

KDP_STATUS
NTAPI
KdpReceiveByte(OUT PBYTE OutByte);

KDP_STATUS
NTAPI
KdpPollBreakIn(VOID);

VOID
NTAPI
KdpSuspendAllBreakPoints(VOID);

BOOLEAN
NTAPI
KdIsThisAKdTrap(
    IN PEXCEPTION_RECORD ExceptionRecord,
    IN PCONTEXT Context,
    IN KPROCESSOR_MODE PreviousMode
);


BOOLEAN
NTAPI
KdpStub(
    IN PKTRAP_FRAME TrapFrame,
    IN PKEXCEPTION_FRAME ExceptionFrame,
    IN PEXCEPTION_RECORD ExceptionRecord,
    IN PCONTEXT ContextRecord,
    IN KPROCESSOR_MODE PreviousMode,
    IN BOOLEAN SecondChanceException
);

BOOLEAN
NTAPI
KdpTrap(
    IN PKTRAP_FRAME TrapFrame,
    IN PKEXCEPTION_FRAME ExceptionFrame,
    IN PEXCEPTION_RECORD ExceptionRecord,
    IN PCONTEXT ContextRecord,
    IN KPROCESSOR_MODE PreviousMode,
    IN BOOLEAN SecondChanceException
);

BOOLEAN
NTAPI
KdEnterDebugger(
    IN PKTRAP_FRAME TrapFrame,
    IN PKEXCEPTION_FRAME ExceptionFrame
);

VOID
NTAPI
KdExitDebugger(
    IN BOOLEAN Enable
);

USHORT
NTAPI
KdpPrompt(
    IN LPSTR PromptString,
    IN USHORT PromptLength,
    OUT LPSTR ResponseString,
    IN USHORT MaximumResponseLength,
    IN KPROCESSOR_MODE PreviousMode,
    IN PKTRAP_FRAME TrapFrame,
    IN PKEXCEPTION_FRAME ExceptionFrame
);

VOID
NTAPI
KdpSymbol(
    IN PSTRING DllPath,
    IN PKD_SYMBOLS_INFO SymbolInfo,
    IN BOOLEAN Unload,
    IN KPROCESSOR_MODE PreviousMode,
    IN PCONTEXT ContextRecord,
    IN PKTRAP_FRAME TrapFrame,
    IN PKEXCEPTION_FRAME ExceptionFrame
);

VOID
NTAPI
KdpReportLoadSymbolsStateChange(
    IN PSTRING PathName,
    IN PKD_SYMBOLS_INFO SymbolInfo,
    IN BOOLEAN Unload,
    IN OUT PCONTEXT Context
);

VOID
NTAPI
KdpReportCommandStringStateChange(
    IN PSTRING NameString,
    IN PSTRING CommandString,
    IN OUT PCONTEXT Context
);

BOOLEAN
NTAPI
KdpReportExceptionStateChange(
    IN PEXCEPTION_RECORD ExceptionRecord,
    IN OUT PCONTEXT Context,
    IN BOOLEAN SecondChanceException
);

ULONG
NTAPI
KdpAddBreakpoint(
    IN PVOID Address
);

BOOLEAN
NTAPI
KdpDeleteBreakpoint(
    IN ULONG BpEntry
);

BOOLEAN
NTAPI
KdpDeleteBreakpointRange(
    IN PVOID Base,
    IN PVOID Limit
);

VOID
NTAPI
KdpRestoreAllBreakpoints(
    VOID
);

NTSTATUS
NTAPI
KdpCopyMemoryChunks(
    IN ULONG Address,
    IN PVOID Buffer,
    IN ULONG TotalSize,
    IN ULONG ChunkSize,
    IN ULONG Flags,
    OUT PULONG ActualSize OPTIONAL
);

VOID
NTAPI
KdpGetStateChange(
    IN PDBGKD_MANIPULATE_STATE State,
    IN PCONTEXT Context
);

NTSTATUS
NTAPI
KdpSysReadMsr(
    IN ULONG Msr,
    OUT PLARGE_INTEGER MsrValue
);

NTSTATUS
NTAPI
KdpSysWriteMsr(
    IN ULONG Msr,
    IN PLARGE_INTEGER MsrValue
);

NTSTATUS
NTAPI
KdpSysReadBusData(
    IN ULONG BusDataType,
    IN ULONG BusNumber,
    IN ULONG SlotNumber,
    IN ULONG Offset,
    IN PVOID Buffer,
    IN ULONG Length,
    OUT PULONG ActualLength
);

NTSTATUS
NTAPI
KdpSysWriteBusData(
    IN ULONG BusDataType,
    IN ULONG BusNumber,
    IN ULONG SlotNumber,
    IN ULONG Offset,
    IN PVOID Buffer,
    IN ULONG Length,
    OUT PULONG ActualLength
);

NTSTATUS
NTAPI
KdpSysWriteControlSpace(
    IN ULONG Processor,
    IN ULONG BaseAddress,
    IN PVOID Buffer,
    IN ULONG Length,
    OUT PULONG ActualLength
);

NTSTATUS
NTAPI
KdpSysCheckLowMemory(
    IN ULONG Flags
);

BOOLEAN
NTAPI
RtlpCheckForActiveDebugger(VOID);
BOOL KdpVirtualProtect(IN ULONG Address, IN ULONG dwSize);


KDP_STATUS
NTAPI
KdpPollByte(OUT PBYTE OutByte);

ULONG
NTAPI
DebugService(
    IN ULONG Service,
    IN PVOID Argument1,
    IN PVOID Argument2,
    IN PVOID Argument3,
    IN PVOID Argument4
);

VOID
NTAPI
DebugService2(
    IN PVOID Argument1,
    IN PVOID Argument2,
    IN ULONG Service
);

VOID
NTAPI
KiDispatchException2(IN PEXCEPTION_RECORD ExceptionRecord,
                     IN PKEXCEPTION_FRAME ExceptionFrame,
                     IN PKTRAP_FRAME TrapFrame,
                     IN KPROCESSOR_MODE PreviousMode,
                     IN BOOLEAN FirstChance);

VOID
NTAPI
HalReturnToFirmware2(IN FIRMWARE_REENTRY Action);