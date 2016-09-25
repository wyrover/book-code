/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS System Libraries
 * FILE:            lib/rtl/rtlp.h
 * PURPOSE:         Run-Time Libary Internal Header
 * PROGRAMMER:      Alex Ionescu
 */

/* INCLUDES ******************************************************************/

/* PAGED_CODE equivalent for user-mode RTL */
#if DBG
    extern VOID FASTCALL CHECK_PAGED_CODE_RTL(char *file, int line);
    #define PAGED_CODE_RTL() CHECK_PAGED_CODE_RTL(__FILE__, __LINE__)
#else
    #define PAGED_CODE_RTL()
#endif

#ifdef _PPC_
    #define SWAPD(x) ((((x)&0xff)<<24)|(((x)&0xff00)<<8)|(((x)>>8)&0xff00)|(((x)>>24)&0xff))
    #define SWAPW(x) ((((x)&0xff)<<8)|(((x)>>8)&0xff))
    #define SWAPQ(x) ((SWAPD((x)&0xffffffff) << 32) | (SWAPD((x)>>32)))
#else
    #define SWAPD(x) (x)
    #define SWAPW(x) (x)
    #define SWAPQ(x) (x)
#endif
/*
#define ROUND_DOWN(n, align) \
    (((ULONG)(n)) & ~((align) - 1l))

#define ROUND_UP(n, align) \
    ROUND_DOWN(((ULONG)(n)) + (align) - 1, (align))
*/
#define RVA(m, b) ((PVOID)((ULONG_PTR)(b) + (ULONG_PTR)(m)))

NTSTATUS
NTAPI
RtlpSafeCopyMemory(
    _Out_writes_bytes_all_(Length) VOID UNALIGNED *Destination,
    _In_reads_bytes_(Length) CONST VOID UNALIGNED *Source,
    _In_ SIZE_T Length);

VOID
NTAPI
RtlpGetStackLimits(PULONG_PTR LowLimit,
                   PULONG_PTR HighLimit);

PEXCEPTION_REGISTRATION_RECORD
NTAPI
RtlpGetExceptionList(VOID);

VOID
NTAPI
RtlpSetExceptionList(PEXCEPTION_REGISTRATION_RECORD NewExceptionList);

BOOLEAN
NTAPI
RtlCallVectoredExceptionHandlers(
    IN PEXCEPTION_RECORD ExceptionRecord,
    IN PCONTEXT Context
);

typedef struct _DISPATCHER_CONTEXT {
    PEXCEPTION_REGISTRATION_RECORD RegistrationPointer;
} DISPATCHER_CONTEXT, *PDISPATCHER_CONTEXT;

/* These provide support for sharing code between User and Kernel RTL */
PVOID
NTAPI
RtlpAllocateMemory(
    SIZE_T Bytes,
    ULONG Tag);

VOID
NTAPI
RtlpFreeMemory(
    PVOID Mem,
    ULONG Tag);

KPROCESSOR_MODE
NTAPI
RtlpGetMode(VOID);

BOOLEAN
NTAPI
RtlpCaptureStackLimits(
    IN ULONG_PTR Ebp,
    IN ULONG_PTR *StackBegin,
    IN ULONG_PTR *StackEnd
);

NTSTATUS
NTAPI
RtlDeleteHeapLock(IN OUT PHEAP_LOCK Lock);

NTSTATUS
NTAPI
RtlEnterHeapLock(IN OUT PHEAP_LOCK Lock, IN BOOLEAN Exclusive);

NTSTATUS
NTAPI
RtlInitializeHeapLock(IN OUT PHEAP_LOCK *Lock);

NTSTATUS
NTAPI
RtlLeaveHeapLock(IN OUT PHEAP_LOCK Lock);

BOOLEAN
NTAPI
RtlpCheckForActiveDebugger(VOID);

BOOLEAN
NTAPI
RtlpHandleDpcStackException(IN PEXCEPTION_REGISTRATION_RECORD RegistrationFrame,
                            IN ULONG_PTR RegistrationFrameEnd,
                            IN OUT PULONG_PTR StackLow,
                            IN OUT PULONG_PTR StackHigh);

#define RtlpAllocateStringMemory RtlpAllocateMemory
#define RtlpFreeStringMemory     RtlpFreeMemory

BOOLEAN
NTAPI
RtlpSetInDbgPrint(
    VOID
);

VOID
NTAPI
RtlpClearInDbgPrint(
    VOID
);

/* i386/except.S */

EXCEPTION_DISPOSITION
NTAPI
RtlpExecuteHandlerForException(PEXCEPTION_RECORD ExceptionRecord,
                               PEXCEPTION_REGISTRATION_RECORD RegistrationFrame,
                               PCONTEXT Context,
                               PVOID DispatcherContext,
                               PEXCEPTION_ROUTINE ExceptionHandler);

EXCEPTION_DISPOSITION
NTAPI
RtlpExecuteHandlerForUnwind(PEXCEPTION_RECORD ExceptionRecord,
                            PEXCEPTION_REGISTRATION_RECORD RegistrationFrame,
                            PCONTEXT Context,
                            PVOID DispatcherContext,
                            PEXCEPTION_ROUTINE ExceptionHandler);

VOID
NTAPI
RtlpCheckLogException(IN PEXCEPTION_RECORD ExceptionRecord,
                      IN PCONTEXT ContextRecord,
                      IN PVOID ContextData,
                      IN ULONG Size);

VOID
NTAPI
RtlpCaptureContext(OUT PCONTEXT ContextRecord);

//
// Debug Service calls
//
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

/* Tags for the String Allocators */
#define TAG_USTR        'RTSU'
#define TAG_ASTR        'RTSA'
#define TAG_OSTR        'RTSO'

/* Timer Queue */

extern HANDLE TimerThreadHandle;

NTSTATUS
RtlpInitializeTimerThread(VOID);

/* EOF */
//++ree,add
#define ACE_OBJECT_TYPE_PRESENT           0x1
#define ACE_INHERITED_OBJECT_TYPE_PRESENT 0x2

#define SEM_FAILCRITICALERRORS      0x0001
#define SEM_NOGPFAULTERRORBOX       0x0002
#define SEM_NOALIGNMENTFAULTEXCEPT  0x0004
#define SEM_NOOPENFILEERRORBOX      0x8000

#define RTL_IMAGE_NT_HEADER_EX_FLAG_NO_RANGE_CHECK          0x00000001
#define IMAGE_REL_BASED_ABSOLUTE 0
#define IMAGE_REL_BASED_HIGH 1
#define IMAGE_REL_BASED_LOW 2
#define IMAGE_REL_BASED_HIGHLOW 3
#define IMAGE_REL_BASED_HIGHADJ 4
#define IMAGE_REL_BASED_MIPS_JMPADDR 5
#define IMAGE_REL_BASED_DIR64 10

/* Widely used macros */
#define LOBYTE(w)               ((BYTE)(w))
#define HIBYTE(w)               ((BYTE)(((WORD)(w)>>8)&0xFF))
#define LOWORD(l)               ((WORD)((DWORD_PTR)(l)))
#define HIWORD(l)               ((WORD)(((DWORD_PTR)(l)>>16)&0xFFFF))
#define MAKEWORD(a,b)           ((WORD)(((BYTE)(a))|(((WORD)((BYTE)(b)))<<8)))
#define MAKELONG(a,b)           ((LONG)(((WORD)(a))|(((DWORD)((WORD)(b)))<<16)))

#define ITU_IMPLEMENTED_TESTS (IS_TEXT_UNICODE_ODD_LENGTH|IS_TEXT_UNICODE_SIGNATURE)

#define IS_TEXT_UNICODE_ASCII16               0x0001
#define IS_TEXT_UNICODE_REVERSE_ASCII16       0x0010

#define IS_TEXT_UNICODE_STATISTICS            0x0002
#define IS_TEXT_UNICODE_REVERSE_STATISTICS    0x0020

#define IS_TEXT_UNICODE_CONTROLS              0x0004
#define IS_TEXT_UNICODE_REVERSE_CONTROLS      0x0040

#define IS_TEXT_UNICODE_SIGNATURE             0x0008
#define IS_TEXT_UNICODE_REVERSE_SIGNATURE     0x0080

#define IS_TEXT_UNICODE_ILLEGAL_CHARS         0x0100
#define IS_TEXT_UNICODE_ODD_LENGTH            0x0200
#define IS_TEXT_UNICODE_DBCS_LEADBYTE         0x0400
#define IS_TEXT_UNICODE_NULL_BYTES            0x1000

#define IS_TEXT_UNICODE_UNICODE_MASK          0x000F
#define IS_TEXT_UNICODE_REVERSE_MASK          0x00F0
#define IS_TEXT_UNICODE_NOT_UNICODE_MASK      0x0F00
#define IS_TEXT_UNICODE_NOT_ASCII_MASK        0xF000

#define WT_EXECUTEDEFAULT 0x00000000
#define WT_EXECUTEINIOTHREAD 0x00000001
#define WT_EXECUTEINUITHREAD 0x00000002
#define WT_EXECUTEINWAITTHREAD 0x00000004
#define WT_EXECUTEONLYONCE 0x00000008
#define WT_EXECUTELONGFUNCTION 0x00000010
#define WT_EXECUTEINTIMERTHREAD 0x00000020
#define WT_EXECUTEINPERSISTENTIOTHREAD 0x00000040
#define WT_EXECUTEINPERSISTENTTHREAD 0x00000080
#define WT_TRANSFER_IMPERSONATION 0x00000100

#define INFINITE -1

//
// Number of custom-defined bits that can be attached to a handle
//
#define OBJ_HANDLE_TAGBITS                      0x3

//
// RtlFindCharInUnicodeString Flags
//
#define RTL_FIND_CHAR_IN_UNICODE_STRING_START_AT_END        1
#define RTL_FIND_CHAR_IN_UNICODE_STRING_COMPLEMENT_CHAR_SET 2
#define RTL_FIND_CHAR_IN_UNICODE_STRING_CASE_INSENSITIVE    4
#define MAX_COMPUTERNAME_LENGTH 15
#define RtlUnicodeStringToCountedOemSize(STRING) (                      \
        (ULONG)(RtlUnicodeStringToOemSize(STRING) - sizeof(ANSI_NULL))      \
                                                 )

#define RtlGetProcessHeap() (NtCurrentPeb()->ProcessHeap)

typedef struct tagPROPVARIANT PROPVARIANT;

typedef enum _ACL_INFORMATION_CLASS {
    AclRevisionInformation = 1,
    AclSizeInformation
} ACL_INFORMATION_CLASS;

typedef struct _ACL_REVISION_INFORMATION {
    DWORD AclRevision;
} ACL_REVISION_INFORMATION;
typedef ACL_REVISION_INFORMATION *PACL_REVISION_INFORMATION;

typedef struct _ACL_SIZE_INFORMATION {
    DWORD AceCount;
    DWORD AclBytesInUse;
    DWORD AclBytesFree;
} ACL_SIZE_INFORMATION;
typedef ACL_SIZE_INFORMATION *PACL_SIZE_INFORMATION;

typedef enum _RTL_BSD_ITEM_TYPE {
    RtlBsdItemVersionNumber,
    RtlBsdItemProductType,
    RtlBsdItemAabEnabled,
    RtlBsdItemAabTimeout,
    RtlBsdItemBootGood,
    RtlBsdItemBootShutdown,
    RtlBsdItemMax
} RTL_BSD_ITEM_TYPE, *PRTL_BSD_ITEM_TYPE;

typedef BOOLEAN
(NTAPI *PRTL_CONFLICT_RANGE_CALLBACK)(
    PVOID Context,
    struct _RTL_RANGE *Range
);


#define EXCEPTION_ACCESS_VIOLATION ((DWORD)0xC0000005)
#define EXCEPTION_PRIV_INSTRUCTION ((DWORD)0xC0000096)

typedef struct _IMAGE_RESOURCE_DIRECTORY_ENTRY {
    union {
        struct {
            DWORD NameOffset: 31;
            DWORD NameIsString: 1;
        };
        DWORD   Name;
        WORD    Id;
    };
    union {
        DWORD   OffsetToData;
        struct {
            DWORD   OffsetToDirectory: 31;
            DWORD   DataIsDirectory: 1;
        };
    };
} IMAGE_RESOURCE_DIRECTORY_ENTRY, *PIMAGE_RESOURCE_DIRECTORY_ENTRY;

typedef struct _IMAGE_RESOURCE_DIR_STRING_U {
    WORD Length;
    WCHAR NameString[1];
} IMAGE_RESOURCE_DIR_STRING_U, *PIMAGE_RESOURCE_DIR_STRING_U;

typedef struct _RTL_CONDITION_VARIABLE {
    PVOID Ptr;
} RTL_CONDITION_VARIABLE, *PRTL_CONDITION_VARIABLE;

typedef struct _RTL_SRWLOCK {
    PVOID Ptr;
} RTL_SRWLOCK, *PRTL_SRWLOCK;

PVOID NTAPI
RtlReAllocateHeap(HANDLE HeapPtr,
                  ULONG Flags,
                  PVOID Ptr,
                  SIZE_T Size);
BOOLEAN
NTAPI
RtlGetUserInfoHeap(IN PVOID HeapHandle,
                   IN ULONG Flags,
                   IN PVOID BaseAddress,
                   OUT PVOID *UserValue,
                   OUT PULONG UserFlags);
BOOLEAN
NTAPI
RtlSetUserValueHeap(IN PVOID HeapHandle,
                    IN ULONG Flags,
                    IN PVOID BaseAddress,
                    IN PVOID UserValue);
BOOLEAN
NTAPI
RtlSetUserFlagsHeap(IN PVOID HeapHandle,
                    IN ULONG Flags,
                    IN PVOID BaseAddress,
                    IN ULONG UserFlagsReset,
                    IN ULONG UserFlagsSet);

VOID
NTAPI
RtlInitializeSListHead(IN PSLIST_HEADER ListHead);

BOOLEAN NTAPI RtlValidateHeap(
    HANDLE HeapPtr,
    ULONG Flags,
    PVOID Block
);

PVOID NTAPI
RtlAllocateHeap(IN PVOID HeapPtr,
                IN ULONG Flags,
                IN SIZE_T Size);

FORCEINLINE
NTSTATUS
NTAPI
RtlpSafeCopyMemory(
    _Out_writes_bytes_all_(Length) VOID UNALIGNED *Destination,
    _In_reads_bytes_(Length) CONST VOID UNALIGNED *Source,
    _In_ SIZE_T Length)
{
    _SEH2_TRY {
        RtlCopyMemory(Destination, Source, Length);
    }
    _SEH2_EXCEPT(EXCEPTION_EXECUTE_HANDLER) {
        _SEH2_YIELD(return _SEH2_GetExceptionCode());
    }
    _SEH2_END;
    return STATUS_SUCCESS;
}
//--