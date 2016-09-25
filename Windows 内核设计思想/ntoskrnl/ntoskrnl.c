// ntoskrnl.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <ntoskrnl.h>
#define NDEBUG
#include <debug.h>

VOID
FASTCALL
KiUnexpectedInterruptTailHandler(IN PKTRAP_FRAME TrapFrame);

VOID
FASTCALL
KiInterruptTemplateHandler(IN PKTRAP_FRAME TrapFrame,
                           IN PKINTERRUPT Interrupt);

KIDTENTRY KiIdt[MAXIMUM_IDTVECTOR + 1];
VOID _cdecl KiTrap08(VOID)
{
    UNIMPLEMENTED
    _asm int 3
}
VOID _cdecl KiTrap13(VOID)
{
    UNIMPLEMENTED
    _asm int 3
}
VOID _cdecl KiFastCallEntry(VOID)
{
    UNIMPLEMENTED
    _asm int 3
}

DECLSPEC_NORETURN VOID FASTCALL KiSystemCallReturn(IN PKTRAP_FRAME TrapFrame)
{
    UNIMPLEMENTED
    _asm int 3
}
DECLSPEC_NORETURN VOID FASTCALL KiSystemCallSysExitReturn(IN PKTRAP_FRAME TrapFrame)
{
    UNIMPLEMENTED
    _asm int 3
}
DECLSPEC_NORETURN VOID FASTCALL KiSystemCallTrapReturn(IN PKTRAP_FRAME TrapFrame)
{
    UNIMPLEMENTED
    _asm int 3
}

DECLSPEC_NORETURN VOID FASTCALL KiTrapReturn(IN PKTRAP_FRAME TrapFrame)
{
    UNIMPLEMENTED
    _asm int 3 // 本书暂时讲解到此
}

DECLSPEC_NORETURN VOID _declspec(naked) FASTCALL KiEditedTrapReturn(IN PKTRAP_FRAME TrapFrame)
{
    _asm{
        /*
            mov     esp, [ecx+14h]
            push    dword ptr [ecx+68h]
            push    dword ptr [ecx+70h]
            mov     ebx, [ecx+5Ch]
            mov     esi, [ecx+58h]
            mov     edi, [ecx+54h]
            mov     ebp, [ecx+60h]
            mov     eax, [ecx+44h]
            mov     edx, [ecx+3Ch]
            mov     ecx, [ecx+40h]
            popf
            retn
        */
        mov     esp, [ecx + KTRAP_FRAME.TempEsp]
        push    dword ptr [ecx + KTRAP_FRAME.Eip]
        push    dword ptr [ecx + KTRAP_FRAME.EFlags]

        /* Restore non volatiles */
        mov ebx, [ecx + KTRAP_FRAME.Ebx]
        mov esi, [ecx + KTRAP_FRAME.Esi]
        mov edi, [ecx + KTRAP_FRAME.Edi]
        mov ebp, [ecx + KTRAP_FRAME.Ebp]

        mov eax, [ecx + KTRAP_FRAME.Eax]
        mov edx, [ecx + KTRAP_FRAME.Edx]
        mov ecx, [ecx + KTRAP_FRAME.Ecx]
        popfd
        retn
    }
}

DECLSPEC_NORETURN VOID _declspec(naked) FASTCALL KiTrapReturnNoSegments(IN PKTRAP_FRAME TrapFrame)
{
    _asm {
        /*
            lea     esp, [ecx+68h]
            mov     ebx, [ecx+5Ch]
            mov     esi, [ecx+58h]
            mov     edi, [ecx+54h]
            mov     ebp, [ecx+60h]
            mov     eax, [ecx+44h]
            mov     edx, [ecx+3Ch]
            mov     ecx, [ecx+40h]
            iretd
        */
        lea     esp, [ecx + KTRAP_FRAME.Eip]

        /* Restore non volatiles */
        mov ebx, [ecx + KTRAP_FRAME.Ebx]
        mov esi, [ecx + KTRAP_FRAME.Esi]
        mov edi, [ecx + KTRAP_FRAME.Edi]
        mov ebp, [ecx + KTRAP_FRAME.Ebp]

        mov     eax, [ecx + KTRAP_FRAME.Eax]
        mov     edx, [ecx + KTRAP_FRAME.Edx]
        mov     ecx, [ecx + KTRAP_FRAME.Ecx]
        iretd
    }
}

unsigned long _declspec(naked) ___readcr4(void)
{
    _asm{
        //mov eax, cr4
        _emit 0x0F
        _emit 0x20
        _emit 0xE0
        ret
    }
}

VOID _declspec(naked) NTAPI ExpInterlockedPopEntrySListFault(VOID)
{
    _asm{
        mov     ebx, [eax]
        lock cmpxchg8b qword ptr [ebp + 0]
        jz      short Epop20
        mov     edx, [ebp + 4]
        mov     eax, [ebp + 0]
        or      eax, eax
        jz      short Epop20
        lea     ecx, [edx - 1]
        Epop20:
        pop     ebp
        pop     ebx
        retn
    }
}

VOID _declspec(naked) NTAPI ExpInterlockedPopEntrySListResume(VOID)
{
    _asm{
        ExpResume:
        mov     edx, [ebp + 4]
        mov     eax, [ebp + 0]
        or      eax, eax
        jz      short end
        lea     ecx, [edx - 1]
        mov     ebx, [eax]
        lock cmpxchg8b qword ptr [ebp + 0]
        jnz     short ExpResume

        end:
        pop     ebp
        pop     ebx
        retn
    }
}

VOID _declspec(naked) _cdecl KiInterruptTemplate(VOID)
{
    _asm{
        sub     esp, 68h
        mov     [esp + 96], ebp
        mov     [esp + 92], ebx
        mov     [esp + 88], esi
        mov     [esp + 84], edi
        mov     [esp + 68], eax
        mov     [esp + 64], ecx
        mov     [esp + 60], edx
        test    byte ptr [esp + 114], 2
        jz      short loc_12AD

        mov     eax, [esp + 128]
        mov     ecx, [esp + 124]
        mov     [esp + 56], eax
        mov     [esp + 52], ecx
        mov     eax, [esp + 132]
        mov     ecx, [esp + 136]
        mov     [esp + 80], eax
        mov     [esp + 48], ecx
        jmp     short loc_12C5

        loc_12AD:
        //mov     eax, ds
        _emit 0x8C
        _emit 0xD8

        //mov     ecx, es
        _emit 0x8C
        _emit 0xC1

        mov     [esp + 56], eax
        mov     [esp + 52], ecx

        //mov     eax, fs
        _emit 0x8C
        _emit 0xE0

        //mov     ecx, gs
        _emit 0x8C
        _emit 0xE9

        mov     [esp + 80], eax
        mov     [esp + 48], ecx

        loc_12C5:
        mov     ax, 23h ; '#'
        mov     ds, ax
        mov     es, ax
        mov     ax, 30h ; '0'
        mov     fs, ax
        mov     eax, [esp + 68h]
        mov     [esp + 4], eax
        mov     [esp], ebp
        mov     ebp, esp
        mov     ecx, esp
        cld

        mov     edx, 0
    }
}

_declspec(naked) KiInterruptTemplateObject(VOID)
{
    _asm {
        mov     eax, offset KiInterruptTemplateHandler
        jmp     eax
    }
}

PVOID
__declspec(naked)
FASTCALL
__InterlockedCompareExchange(
    PVOID   * Destination,
    PVOID   Exchange,
    PVOID   Comperand
)
{
    _asm{
        mov     eax, [esp + 4]
        lock cmpxchg [ecx], edx
        retn    4
    }
}

LONG
__declspec(naked)
FASTCALL
__InterlockedExchangeAdd(
    PLONG   Addend,
    LONG    Value
)
{
    _asm{
        mov     eax, edx
        lock xadd [ecx], eax
        retn
    }
}

LONG
__declspec(naked)
FASTCALL
__InterlockedIncrement(PLONG Addend)
{
    _asm{
        mov     eax, 1
        lock xadd [ecx], eax
        inc     eax
        retn
    }
}

LONG
__declspec(naked)
FASTCALL
__InterlockedDecrement(PLONG Addend)
{
    _asm{
        or      eax, 0FFFFFFFFh
        lock xadd [ecx], eax
        dec     eax
        retn
    }
}

long _ExInterlockedAddLargeStatistic(volatile long long * const Addend, const long Value)
{
    *Addend += Value;
    return Value;
}

NTSTATUS
NTAPI
ZwClose(IN HANDLE Handle)
{
    /* Call the internal API */
    return NtClose(Handle);
}

NTSTATUS
NTAPI
ZwCreateFile(
    _Out_ PHANDLE FileHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_opt_ PLARGE_INTEGER AllocationSize,
    _In_ ULONG FileAttributes,
    _In_ ULONG ShareAccess,
    _In_ ULONG CreateDisposition,
    _In_ ULONG CreateOptions,
    _In_reads_bytes_opt_(EaLength) PVOID EaBuffer,
    _In_ ULONG EaLength)
{
    return NtCreateFile(FileHandle, DesiredAccess, ObjectAttributes,
                        IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength);
}

NTSTATUS
NTAPI
ZwQueryValueKey(
    _In_ HANDLE KeyHandle,
    _In_ PUNICODE_STRING ValueName,
    _In_ KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
    _Out_writes_bytes_opt_(Length) PVOID KeyValueInformation,
    _In_ ULONG Length,
    _Out_ PULONG ResultLength
)
{
    return NtQueryValueKey(
               KeyHandle,
               ValueName,
               KeyValueInformationClass,
               KeyValueInformation,
               Length,
               ResultLength);
}

NTSTATUS
NTAPI
ZwSetValueKey(
    _In_ HANDLE KeyHandle,
    _In_ PUNICODE_STRING ValueName,
    _In_opt_ ULONG TitleIndex,
    _In_ ULONG Type,
    _In_ PVOID Data,
    _In_ ULONG DataSize
)
{
    return NtSetValueKey(
               KeyHandle,
               ValueName,
               TitleIndex,
               Type,
               Data,
               DataSize);
}

NTSTATUS
NTAPI
ZwDeleteKey(IN HANDLE KeyHandle)
{
    return NtDeleteKey(KeyHandle);
}

NTSTATUS
NTAPI
ZwCreateKey(
    _Out_ PHANDLE KeyHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Reserved_ ULONG TitleIndex,
    _In_opt_ PUNICODE_STRING Class,
    _In_ ULONG CreateOptions,
    _Out_opt_ PULONG Disposition
)
{
    return NtCreateKey(
               KeyHandle,
               DesiredAccess,
               ObjectAttributes,
               TitleIndex,
               Class,
               CreateOptions,
               Disposition
           );
}

NTSTATUS
NTAPI
ZwOpenKey(
    _Out_ PHANDLE KeyHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes
)
{
    return NtOpenKey(
               KeyHandle,
               DesiredAccess,
               ObjectAttributes
           );
}

NTSTATUS
NTAPI
ZwDeleteValueKey(IN HANDLE KeyHandle,
                 IN PUNICODE_STRING ValueName)
{
    return NtDeleteValueKey(KeyHandle,
                            ValueName);
}

NTSTATUS
NTAPI
ZwEnumerateKey(
    _In_ HANDLE KeyHandle,
    _In_ ULONG Index,
    _In_ KEY_INFORMATION_CLASS KeyInformationClass,
    _Out_bytecap_(Length) PVOID KeyInformation,
    _In_ ULONG Length,
    _Out_ PULONG ResultLength
)
{
    return NtEnumerateKey(
               KeyHandle,
               Index,
               KeyInformationClass,
               KeyInformation,
               Length,
               ResultLength
           );
}

NTSTATUS
NTAPI
ZwEnumerateValueKey(
    _In_ HANDLE KeyHandle,
    _In_ ULONG Index,
    _In_ KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
    _Out_writes_bytes_opt_(Length) PVOID KeyValueInformation,
    _In_ ULONG Length,
    _Out_ PULONG ResultLength
)
{
    return NtEnumerateValueKey(
               KeyHandle,
               Index,
               KeyValueInformationClass,
               KeyValueInformation,
               Length,
               ResultLength
           );
}

NTSTATUS
NTAPI
ZwQueryKey(
    _In_ HANDLE KeyHandle,
    _In_ KEY_INFORMATION_CLASS KeyInformationClass,
    _Out_bytecap_(Length) PVOID KeyInformation,
    _In_ ULONG Length,
    _Out_ PULONG ResultLength)
{
    return NtQueryKey(
               KeyHandle,
               KeyInformationClass,
               KeyInformation,
               Length,
               ResultLength);
}

NTSTATUS
NTAPI
ZwFlushKey(IN HANDLE KeyHandle)
{
    return NtFlushKey(KeyHandle);
}

NTSTATUS
NTAPI
ZwNotifyChangeKey(
    _In_ HANDLE KeyHandle,
    _In_opt_ HANDLE EventHandle,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG NotifyFilter,
    _In_ BOOLEAN WatchSubtree,
    _Out_writes_bytes_opt_(BufferLength) PVOID Buffer,
    _In_ ULONG BufferLength,
    _In_ BOOLEAN Asynchronous)
{
    return NtNotifyChangeKey(
               KeyHandle,
               EventHandle,
               ApcRoutine,
               ApcContext,
               IoStatusBlock,
               NotifyFilter,
               WatchSubtree,
               Buffer,
               BufferLength,
               Asynchronous);
}

NTSTATUS
NTAPI
ZwInitiatePowerAction(
    _In_ POWER_ACTION SystemAction,
    _In_ SYSTEM_POWER_STATE MinSystemState,
    _In_ ULONG Flags,
    _In_ BOOLEAN Asynchronous
)
{
    return NtInitiatePowerAction(
               SystemAction,
               MinSystemState,
               Flags,
               Asynchronous);
}

NTSTATUS
NTAPI
ZwSetSystemPowerState(
    _In_ POWER_ACTION SystemAction,
    _In_ SYSTEM_POWER_STATE MinSystemState,
    _In_ ULONG Flags)
{
    return NtSetSystemPowerState(
               SystemAction,
               MinSystemState,
               Flags);
}

NTSTATUS
NTAPI
ZwFsControlFile(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG FsControlCode,
    _In_reads_bytes_opt_(InputBufferLength) PVOID InputBuffer,
    _In_ ULONG InputBufferLength,
    _Out_writes_bytes_opt_(OutputBufferLength) PVOID OutputBuffer,
    _In_ ULONG OutputBufferLength)
{
    return NtFsControlFile(
               FileHandle,
               Event,
               ApcRoutine,
               ApcContext,
               IoStatusBlock,
               FsControlCode,
               InputBuffer,
               InputBufferLength,
               OutputBuffer,
               OutputBufferLength);
}

NTSTATUS
NTAPI
ZwQueryInformationFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID FileInformation,
    _In_ ULONG Length,
    _In_ FILE_INFORMATION_CLASS FileInformationClass)
{
    return NtQueryInformationFile(
               FileHandle,
               IoStatusBlock,
               FileInformation,
               Length,
               FileInformationClass);
}

NTSTATUS
NTAPI
ZwOpenFile(
    _Out_ PHANDLE FileHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG ShareAccess,
    _In_ ULONG OpenOptions)
{
    return NtOpenFile(
               FileHandle,
               DesiredAccess,
               ObjectAttributes,
               IoStatusBlock,
               ShareAccess,
               OpenOptions);
}

NTSTATUS
NTAPI
ZwWriteFile(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_reads_bytes_(Length) PVOID Buffer,
    _In_ ULONG Length,
    _In_opt_ PLARGE_INTEGER ByteOffset,
    _In_opt_ PULONG Key)
{
    return NtWriteFile(
               FileHandle,
               Event,
               ApcRoutine,
               ApcContext,
               IoStatusBlock,
               Buffer,
               Length,
               ByteOffset,
               Key);
}

NTSTATUS
NTAPI
ZwQuerySecurityObject(
    _In_ HANDLE Handle,
    _In_ SECURITY_INFORMATION SecurityInformation,
    _Out_writes_bytes_opt_(Length) PSECURITY_DESCRIPTOR SecurityDescriptor,
    _In_ ULONG Length,
    _Out_ PULONG LengthNeeded)
{
    return NtQuerySecurityObject(
               Handle,
               SecurityInformation,
               SecurityDescriptor,
               Length,
               LengthNeeded);
}

NTSTATUS
NTAPI
ZwQuerySystemInformation(
    _In_ SYSTEM_INFORMATION_CLASS SystemInformationClass,
    _Out_ PVOID SystemInformation,
    _In_ ULONG Length,
    _Out_opt_ PULONG ResultLength)
{
    return NtQuerySystemInformation(
               SystemInformationClass,
               SystemInformation,
               Length,
               ResultLength);
}

NTSTATUS
NTAPI
ZwDelayExecution(
    _In_ BOOLEAN Alertable,
    _In_ LARGE_INTEGER *Interval
)
{
    return NtDelayExecution(Alertable,
                            Interval);
}

NTSTATUS
NTAPI
ZwUnmapViewOfSection(
    _In_ HANDLE ProcessHandle,
    _In_ PVOID BaseAddress)
{
    return NtUnmapViewOfSection(
               ProcessHandle,
               BaseAddress);
}

NTSTATUS
NTAPI
ZwMapViewOfSection(
    _In_ HANDLE SectionHandle,
    _In_ HANDLE ProcessHandle,
    _Inout_ PVOID *BaseAddress,
    _In_ ULONG_PTR ZeroBits,
    _In_ SIZE_T CommitSize,
    _Inout_opt_ PLARGE_INTEGER SectionOffset,
    _Inout_ PSIZE_T ViewSize,
    _In_ SECTION_INHERIT InheritDisposition,
    _In_ ULONG AllocationType,
    _In_ ULONG AccessProtection)
{
    return NtMapViewOfSection(
               SectionHandle,
               ProcessHandle,
               BaseAddress,
               ZeroBits,
               CommitSize,
               SectionOffset,
               ViewSize,
               InheritDisposition,
               AllocationType,
               AccessProtection);
}

NTSTATUS
NTAPI
ZwCreateSection(
    _Out_ PHANDLE SectionHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
    _In_opt_ PLARGE_INTEGER MaximumSize,
    _In_ ULONG SectionPageProtection,
    _In_ ULONG AllocationAttributes,
    _In_opt_ HANDLE FileHandle)
{
    return NtCreateSection(
               SectionHandle,
               DesiredAccess,
               ObjectAttributes,
               MaximumSize,
               SectionPageProtection,
               AllocationAttributes,
               FileHandle);
}

NTSTATUS
NTAPI
ZwSetEvent(
    _In_ HANDLE EventHandle,
    _Out_opt_ PLONG PreviousState)
{
    return NtSetEvent(
               EventHandle,
               PreviousState);
}

NTSTATUS
NTAPI
ZwOpenEvent(
    _Out_ PHANDLE EventHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes)
{
    return NtOpenEvent(
               EventHandle,
               DesiredAccess,
               ObjectAttributes);
}

NTSTATUS
NTAPI
ZwReadFile(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID Buffer,
    _In_ ULONG Length,
    _In_opt_ PLARGE_INTEGER ByteOffset,
    _In_opt_ PULONG Key)
{
    return NtReadFile(
               FileHandle,
               Event,
               ApcRoutine,
               ApcContext,
               IoStatusBlock,
               Buffer,
               Length,
               ByteOffset,
               Key);
}

NTSTATUS
NTAPI
ZwFlushBuffersFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock)
{
    return NtFlushBuffersFile(
               FileHandle,
               IoStatusBlock);
}

NTSYSCALLAPI
NTSTATUS
NTAPI
ZwQueryVirtualMemory(
    _In_ HANDLE ProcessHandle,
    _In_ PVOID Address,
    _In_ MEMORY_INFORMATION_CLASS VirtualMemoryInformationClass,
    _Out_ PVOID VirtualMemoryInformation,
    _In_ SIZE_T Length,
    _Out_opt_ PSIZE_T ResultLength)
{
    return NtQueryVirtualMemory(
               ProcessHandle,
               Address,
               VirtualMemoryInformationClass,
               VirtualMemoryInformation,
               Length,
               ResultLength);
}

NTSTATUS
NTAPI
ZwAllocateVirtualMemory(IN HANDLE ProcessHandle,
                        IN OUT PVOID* UBaseAddress,
                        IN ULONG_PTR ZeroBits,
                        IN OUT PSIZE_T URegionSize,
                        IN ULONG AllocationType,
                        IN ULONG Protect)
{
    return NtAllocateVirtualMemory(ProcessHandle,
                                   UBaseAddress,
                                   ZeroBits,
                                   URegionSize,
                                   AllocationType,
                                   Protect);
}

NTSTATUS
NTAPI
ZwFreeVirtualMemory(IN HANDLE ProcessHandle,
                    IN PVOID* UBaseAddress,
                    IN PSIZE_T URegionSize,
                    IN ULONG FreeType)
{
    return NtFreeVirtualMemory(ProcessHandle,
                               UBaseAddress,
                               URegionSize,
                               FreeType);
}

NTSTATUS
NTAPI
ZwQueryDirectoryFile(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID FileInformation,
    _In_ ULONG Length,
    _In_ FILE_INFORMATION_CLASS FileInformationClass,
    _In_ BOOLEAN ReturnSingleEntry,
    _In_opt_ PUNICODE_STRING FileName,
    _In_ BOOLEAN RestartScan)
{
    return NtQueryDirectoryFile(
               FileHandle,
               Event,
               ApcRoutine,
               ApcContext,
               IoStatusBlock,
               FileInformation,
               Length,
               FileInformationClass,
               ReturnSingleEntry,
               FileName,
               RestartScan);
}

NTSTATUS
NTAPI
ZwSetSecurityObject(IN HANDLE Handle,
                    IN SECURITY_INFORMATION SecurityInformation,
                    IN PSECURITY_DESCRIPTOR SecurityDescriptor)
{
    return NtSetSecurityObject(Handle,
                               SecurityInformation,
                               SecurityDescriptor);
}

NTSTATUS
NTAPI
ZwQueryVolumeInformationFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID FsInformation,
    _In_ ULONG Length,
    _In_ FS_INFORMATION_CLASS FsInformationClass)
{
    return NtQueryVolumeInformationFile(
               FileHandle,
               IoStatusBlock,
               FsInformation,
               Length,
               FsInformationClass);
}

NTSTATUS
NTAPI
ZwOpenDirectoryObject(
    _Out_ PHANDLE FileHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes)
{
    return NtOpenDirectoryObject(
               FileHandle,
               DesiredAccess,
               ObjectAttributes);
}

NTSTATUS
NTAPI
ZwPowerInformation(IN POWER_INFORMATION_LEVEL PowerInformationLevel,
                   IN PVOID InputBuffer  OPTIONAL,
                   IN ULONG InputBufferLength,
                   OUT PVOID OutputBuffer  OPTIONAL,
                   IN ULONG OutputBufferLength)
{
    return NtPowerInformation(PowerInformationLevel,
                              InputBuffer,
                              InputBufferLength,
                              OutputBuffer,
                              OutputBufferLength);
}

NTSTATUS
NTAPI
ZwQueryDirectoryObject(
    _In_ HANDLE DirectoryHandle,
    _Out_ PVOID Buffer,
    _In_ ULONG BufferLength,
    _In_ BOOLEAN ReturnSingleEntry,
    _In_ BOOLEAN RestartScan,
    _Inout_ PULONG Context,
    _Out_opt_ PULONG ReturnLength)
{
    return NtQueryDirectoryObject(
               DirectoryHandle,
               Buffer,
               BufferLength,
               ReturnSingleEntry,
               RestartScan,
               Context,
               ReturnLength);
}

NTSTATUS
NTAPI
ZwQueryObject(
    _In_opt_ HANDLE Handle,
    _In_ OBJECT_INFORMATION_CLASS ObjectInformationClass,
    _Out_writes_bytes_opt_(ObjectInformationLength) PVOID ObjectInformation,
    _In_ ULONG ObjectInformationLength,
    _Out_opt_ PULONG ReturnLength)
{
    return NtQueryObject(
               Handle,
               ObjectInformationClass,
               ObjectInformation,
               ObjectInformationLength,
               ReturnLength);
}

NTSTATUS
NTAPI
ZwOpenSection(
    _Out_ PHANDLE SectionHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes)
{
    return NtOpenSection(
               SectionHandle,
               DesiredAccess,
               ObjectAttributes);
}

NTSTATUS
NTAPI
ZwWaitForSingleObject(
    _In_ HANDLE Object,
    _In_ BOOLEAN Alertable,
    _In_opt_ PLARGE_INTEGER Time)
{
    return NtWaitForSingleObject(
               Object,
               Alertable,
               Time);
}

NTSTATUS
NTAPI
ZwDeviceIoControlFile(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG IoControlCode,
    _In_reads_bytes_opt_(InputBufferLength) PVOID InputBuffer,
    _In_ ULONG InputBufferLength,
    _Out_writes_bytes_opt_(OutputBufferLength) PVOID OutputBuffer,
    _In_ ULONG OutputBufferLength)
{
    return NtDeviceIoControlFile(
               FileHandle,
               Event,
               ApcRoutine,
               ApcContext,
               IoStatusBlock,
               IoControlCode,
               InputBuffer,
               InputBufferLength,
               OutputBuffer,
               OutputBufferLength);
}


const CCHAR KiFindFirstSetLeft[256] = {
    0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};

#define ClearMember(Member, Set) \
    Set = Set & (~((ULONG_PTR)1 << (Member)))

#define KeFindFirstSetLeftMember(Set, Member) {                        \
        ULONG _Mask;                                                       \
        ULONG _Offset = 16;                                                \
        if ((_Mask = Set >> 16) == 0) {                                    \
            _Offset = 0;                                                   \
            _Mask = Set;                                                   \
        }                                                                  \
        if (_Mask >> 8) {                                                  \
            _Offset += 8;                                                  \
        }                                                                  \
        *(Member) = KiFindFirstSetLeft[Set >> _Offset] + _Offset;          \
    }

#define KeFindFirstSetLeftAffinity(Set, Member) {                      \
        ULONG _Mask_;                                                      \
        ULONG _Offset_ = 32;                                               \
        if ((_Mask_ = (ULONG)(Set >> 32)) == 0) {                          \
            _Offset_ = 0;                                                  \
            _Mask_ = (ULONG)Set;                                           \
        }                                                                  \
        KeFindFirstSetLeftMember(_Mask_, Member);                          \
        *(Member) += _Offset_;                                             \
    }

NTSTATUS
KeI386SetGdtSelector(
    ULONG       Selector,
    PKGDTENTRY  GdtValue
)
{
    KAFFINITY       TargetSet;
    PKPRCB          Prcb;
    PKPCR           Pcr;
    PKGDTENTRY      GdtEntry;
    ULONG           GdtIndex, BitNumber;
    GdtIndex = Selector >> 3;

    if ((Selector & 0x7) != 0  || GdtIndex < KGDT_NUMBER) {
        return STATUS_UNSUCCESSFUL;
    }

    TargetSet = KeActiveProcessors;

    while (TargetSet != 0) {
        KeFindFirstSetLeftAffinity(TargetSet, &BitNumber);
        ClearMember(BitNumber, TargetSet);
        Prcb = KiProcessorBlock[BitNumber];
        Pcr  = CONTAINING_RECORD(Prcb, KIPCR, PrcbData);
        GdtEntry = Pcr->GDT + GdtIndex;
        *GdtEntry = *GdtValue;
    }

    return STATUS_SUCCESS;
}

PVOID
ExAllocateFromPagedLookasideListEx(
    _Inout_ PPAGED_LOOKASIDE_LIST Lookaside)
{
    PVOID Entry;
    Lookaside->L.TotalAllocates++;
#ifdef NONAMELESSUNION
    Entry = InterlockedPopEntrySList(&Lookaside->L.u.ListHead);

    if (Entry == NULL) {
        Lookaside->L.u2.AllocateMisses++;
        Entry = (Lookaside->L.u4.Allocate)(Lookaside->L.Type,
                                           Lookaside->L.Size,
                                           Lookaside->L.Tag);
    }

#else /* NONAMELESSUNION */
    Entry = InterlockedPopEntrySList(&Lookaside->L.ListHead);

    if (Entry == NULL) {
        Lookaside->L.AllocateMisses++;
        Entry = (Lookaside->L.Allocate)(Lookaside->L.Type,
                                        Lookaside->L.Size,
                                        Lookaside->L.Tag);
    }

#endif /* NONAMELESSUNION */
    return Entry;
}

VOID
ExFreeToPagedLookasideListEx(
    _Inout_ PPAGED_LOOKASIDE_LIST Lookaside,
    _In_ PVOID Entry)
{
    Lookaside->L.TotalFrees++;
#ifdef NONAMELESSUNION

    if (ExQueryDepthSList(&Lookaside->L.u.ListHead) >= Lookaside->L.Depth) {
        Lookaside->L.u3.FreeMisses++;
        (Lookaside->L.u5.Free)(Entry);
    } else {
        InterlockedPushEntrySList(&Lookaside->L.u.ListHead, (PSLIST_ENTRY)Entry);
    }

#else /* NONAMELESSUNION */

    if (ExQueryDepthSList(&Lookaside->L.ListHead) >= Lookaside->L.Depth) {
        Lookaside->L.FreeMisses++;
        (Lookaside->L.Free)(Entry);
    } else {
        InterlockedPushEntrySList(&Lookaside->L.ListHead, (PSLIST_ENTRY)Entry);
    }

#endif /* NONAMELESSUNION */
}

#define ExInterlockedFlushSList(Head) \
    ExpInterlockedFlushSList(Head)

NTSTATUS
NTAPI
PsWrapApcWow64Thread(
    _Inout_ PVOID *ApcContext,
    _Inout_ PVOID *ApcRoutine)
{
    return STATUS_SUCCESS;
}

VOID
NTAPI
KeInitializeSpinLockEx(_Out_ PKSPIN_LOCK SpinLock)
{
    /* Clear the lock */
    *SpinLock = 0;
}

void KeEnterCriticalRegionEx()
{
    PKTHREAD _Thread = KeGetCurrentThread();
    /* Sanity checks */
    ASSERT(_Thread == KeGetCurrentThread());
    ASSERT((_Thread->KernelApcDisable <= 0) &&
           (_Thread->KernelApcDisable != -32768));
    /* Disable Kernel APCs */
    _Thread->KernelApcDisable--;
}

void KeEnterGuardedRegionEx()
{
    PKTHREAD _Thread = KeGetCurrentThread();
    /* Sanity checks */
    ASSERT(KeGetCurrentIrql() <= APC_LEVEL);
    ASSERT(_Thread == KeGetCurrentThread());
    ASSERT((_Thread->SpecialApcDisable <= 0) &&
           (_Thread->SpecialApcDisable != -32768));
    /* Disable Special APCs */
    _Thread->SpecialApcDisable--;
}

BOOLEAN
KeIsWaitListEmpty(
    IN PVOID Object
)
{
    PKEVENT Event = (PKEVENT)Object;
    PLIST_ENTRY ListHead;
    ListHead = &Event->Header.WaitListHead;
    KeMemoryBarrier();
    return (BOOLEAN)(ListHead == ListHead->Flink);
}

void KeLeaveCriticalRegionEx()
{
    PKTHREAD _Thread = KeGetCurrentThread();
    /* Sanity checks */
    ASSERT(_Thread == KeGetCurrentThread());
    ASSERT(_Thread->KernelApcDisable < 0);
    /* Enable Kernel APCs */
    _Thread->KernelApcDisable++;

    /* Check if Kernel APCs are now enabled */
    if (!(_Thread->KernelApcDisable)) {
        /* Check if we need to request an APC Delivery */
        if (!(IsListEmpty(&_Thread->ApcState.ApcListHead[KernelMode])) &&
            !(_Thread->SpecialApcDisable)) {
            /* Check for the right environment */
            KiCheckForKernelApcDelivery();
        }
    }
}

void KeLeaveGuardedRegionEx()
{
    PKTHREAD _Thread = KeGetCurrentThread();
    /* Sanity checks */
    ASSERT(KeGetCurrentIrql() <= APC_LEVEL);
    ASSERT(_Thread == KeGetCurrentThread());
    ASSERT(_Thread->SpecialApcDisable < 0);

    /* Leave region and check if APCs are OK now */
    if (!(++_Thread->SpecialApcDisable)) {
        /* Check for Kernel APCs on the list */
        if (!IsListEmpty(&_Thread->ApcState.
                         ApcListHead[KernelMode])) {
            /* Check for APC Delivery */
            KiCheckForKernelApcDelivery();
        }
    }
}

KSPIN_LOCK MiTrackIoLock;
LONG MiActiveIoCounter;
SINGLE_LIST_ENTRY MmLockedIoPagesHead;

LOGICAL
NTAPI
MmIsIoSpaceActive(
    IN PHYSICAL_ADDRESS StartAddress,
    IN SIZE_T NumberOfBytes
)
{
    KIRQL OldIrql;
    PFN_NUMBER NumberOfPages;
    PPFN_NUMBER Page;
    PPFN_NUMBER LastPage;
    PVOID StartingVa;
    PMDL MemoryDescriptorList;
    PFN_NUMBER StartPage;
    PFN_NUMBER EndPage;
    PHYSICAL_ADDRESS EndAddress;
    StartPage = (PFN_NUMBER)(StartAddress.QuadPart >> PAGE_SHIFT);
    EndAddress.QuadPart = StartAddress.QuadPart + NumberOfBytes - 1;
    EndPage = (PFN_NUMBER)(EndAddress.QuadPart >> PAGE_SHIFT);
    InterlockedIncrement(&MiActiveIoCounter);
    ExAcquireSpinLock(&MiTrackIoLock, &OldIrql);
    MemoryDescriptorList = (PMDL) MmLockedIoPagesHead.Next;

    while (MemoryDescriptorList != NULL) {
        StartingVa = (PVOID)((PCHAR)MemoryDescriptorList->StartVa +
                             MemoryDescriptorList->ByteOffset);
        NumberOfPages = ADDRESS_AND_SIZE_TO_SPAN_PAGES(StartingVa,
                        MemoryDescriptorList->ByteCount);
        ASSERT(NumberOfPages != 0);
        Page = (PPFN_NUMBER)(MemoryDescriptorList + 1);
        LastPage = Page + NumberOfPages;

        do {
            if (*Page == MM_EMPTY_LIST) {
                break;
            }

            if ((*Page >= StartPage) && (*Page <= EndPage)) {
                ExReleaseSpinLock(&MiTrackIoLock, OldIrql);
                return TRUE;
            }

            Page += 1;
        } while (Page < LastPage);

        MemoryDescriptorList = MemoryDescriptorList->Next;
    }

    ExReleaseSpinLock(&MiTrackIoLock, OldIrql);
    return FALSE;
}

NTSTATUS
NTAPI
MmCreateMirror(
    VOID
)
{
    return STATUS_SUCCESS;
}

NTSTATUS
MmCommitSessionMappedView(
    __in_bcount(ViewSize) PVOID MappedAddress,
    __in SIZE_T ViewSize
)
{
    return STATUS_SUCCESS;
}

VOID
ObDeleteCapturedInsertInfo(
    IN PVOID Object
)
{
    POBJECT_HEADER ObjectHeader;
    ObjectHeader = OBJECT_TO_OBJECT_HEADER(Object);

    if (ObjectHeader->Flags & 0x01) {
        if (ObjectHeader->ObjectCreateInfo != NULL) {
            ObpFreeObjectCreateInformation(ObjectHeader->ObjectCreateInfo);
            ObjectHeader->ObjectCreateInfo = NULL;
        }
    }
}

typedef struct _OBJECT_HANDLE_FLAG_INFORMATION {
    BOOLEAN Inherit;
    BOOLEAN ProtectFromClose;
} OBJECT_HANDLE_FLAG_INFORMATION, *POBJECT_HANDLE_FLAG_INFORMATION;

NTSTATUS
ObSetHandleAttributes(
    __in HANDLE Handle,
    __in POBJECT_HANDLE_FLAG_INFORMATION HandleFlags,
    __in KPROCESSOR_MODE PreviousMode
)
{
    return STATUS_SUCCESS;
}

PETHREAD
NTAPI
PsGetCurrentThreadEx(VOID)
{
    return (PETHREAD)KeGetCurrentThread();
}

#define _PsGetCurrentThread() ((PETHREAD)KeGetCurrentThread())
#define THREAD_TO_PROCESS(Thread) ((Thread)->ThreadsProcess)

PEPROCESS
PsGetCurrentThreadProcess(
    VOID
)
{
    return THREAD_TO_PROCESS(_PsGetCurrentThread());
}

HANDLE
PsGetCurrentThreadProcessId(
    VOID
)
{
    return _PsGetCurrentThread()->Cid.UniqueProcess;
}

PVOID
PsGetCurrentThreadTeb(
    VOID
)
{
    return _PsGetCurrentThread()->Tcb.Teb;
}


FORCEINLINE
VOID
KeEnterCriticalRegionThread(
    PKTHREAD Thread
)
{
    Thread->KernelApcDisable -= 1;
    KeMemoryBarrierWithoutFence();
}


typedef
VOID (*PBBT_NOTIFY_ROUTINE)(
    PKTHREAD Thread
);

PBBT_NOTIFY_ROUTINE PspBBTNotifyRoutine = NULL;

ULONG
PsSetBBTNotifyRoutine(
    __in PBBT_NOTIFY_ROUTINE BBTNotifyRoutine
)
{
    PAGED_CODE();
    PspBBTNotifyRoutine = BBTNotifyRoutine;
    return 0x00184;//FIELD_OFFSET(KTHREAD,BBTData);
}
PVOID
PsGetCurrentThreadWin32ThreadAndEnterCriticalRegion(
    __out PHANDLE ProcessId
)
{
    PETHREAD Thread = _PsGetCurrentThread();
    *ProcessId = Thread->Cid.UniqueProcess;
    KeEnterCriticalRegionThread(&Thread->Tcb);
    return Thread->Tcb.Win32Thread;
}

/*
 * @implemented
 */
BOOLEAN
NTAPI
KdRefreshDebuggerNotPresent(VOID)
{
    UNIMPLEMENTED;
    /* Just return whatever was set previously -- FIXME! */
    return (BOOLEAN)KdDebuggerNotPresent;
}

VOID
__declspec(naked)
NTAPI
KeUpdateRunTime3(IN PKTRAP_FRAME TrapFrame,
                 IN KIRQL Irql)
{
    _asm {
        mov     eax, dword ptr fs:1Ch
        push    ebx
        inc     dword ptr [eax+644h]
        mov     ebx, [eax+124h]
        mov     ecx, [ebx+38h]
        test    dword ptr [ebp+70h], 20000h
        jnz     short _3
        test    byte ptr [ebp+6Ch], 1
        jnz     short _3
        inc     dword ptr [eax+648h]
        cmp     byte ptr [esp+8], 2
        jb      short _2
        ja      short _1
        cmp     byte ptr fs:[0A7Ah], 0
        jz      short _2
        inc     dword ptr [eax+650h]
        jmp     short _4
        _1:
        inc     dword ptr [eax+658h]
        jmp     short _4
        _2:
        inc     dword ptr [ebx+160h]
        jmp     short _4
        _3:
        inc     dword ptr [eax+64Ch]
        inc     dword ptr [ebx+18Ch]
        _4:
        mov     ecx, [eax+0A50h]
        mov     edx, [eax+0A80h]
        mov     [eax+0A80h], ecx
        sub     ecx, edx
        add     ecx, [eax+0A70h]
        shr     ecx, 1
        mov     [eax+0A70h], ecx
        cmp     dword ptr [eax+0A4Ch], 0
        jz      short _5
        cmp     byte ptr [eax+0A7Ah], 0
        jnz     short _5
        cmp     byte ptr [eax+0A78h], 0
        jnz     short _5
        mov     ecx, 2
        call    HalRequestSoftwareInterrupt
        mov     eax, dword ptr fs:[1Ch]
        mov     ecx, [eax+0A70h]
        mov     edx, ds:KiAdjustDpcThreshold
        mov     [eax+65Ch], edx
        cmp     ecx, ds:KiIdealDpcRate
        jge     short _6
        cmp     dword ptr [eax+0A6Ch], 1
        jz      short _6
        dec     dword ptr [eax+0A6Ch]
        jmp     short _6
        _5:
        dec     dword ptr [eax+65Ch]
        jnz     short _6
        mov     ecx, ds:KiAdjustDpcThreshold
        mov     [eax+65Ch], ecx
        mov     ecx, ds:KiMaximumDpcQueueDepth
        cmp     ecx, [eax+0A6Ch]
        jz      short _6
        inc     dword ptr [eax+0A6Ch]
        lea     ecx, [ecx+0]
        _6:
        sub     byte ptr [ebx+15Dh], 3
        jg      short _7
        cmp     ebx, [eax+12Ch]
        jz      short _7
        mov     byte ptr [eax+0AA1h], 1
        mov     ecx, 2
        call    ds:HalRequestSoftwareInterrupt
        _7:
        pop     ebx
        retn    4
    }
}

VOID
NTAPI
Kei386EoiHelper(VOID);

VOID
_declspec(naked)
NTAPI
KeUpdateSystemTime3(IN KIRQL Irql,
                    IN PKTRAP_FRAME TrapFrame,
                    IN ULONG Increment
                   )
{
    _asm {
        mov     ecx, 0FFDF0000h
        mov     edi, [ecx+8]
        mov     esi, [ecx+0Ch]
        add     edi, eax
        adc     esi, 0
        mov     [ecx+10h], esi
        mov     [ecx+8], edi
        mov     [ecx+0Ch], esi
        lock sub ds:KiTickOffset, eax
        mov     eax, ds:KeTickCount.LowPart
        mov     ebx, eax
        jg      short _2
        mov     ebx, 0FFDF0000h
        mov     ecx, [ebx+14h]
        mov     edx, [ebx+18h]
        add     ecx, ds:KeTimeAdjustment
        adc     edx, 0
        mov     [ebx+1Ch], edx
        mov     [ebx+14h], ecx
        mov     [ebx+18h], edx
        mov     ebx, eax
        mov     ecx, eax
        mov     edx, ds:KeTickCount.High1Time
        add     ecx, 1
        adc     edx, 0
        mov     ds:KeTickCount.High2Time, edx
        mov     ds:KeTickCount.LowPart, ecx
        mov     ds:KeTickCount.High1Time, edx
        mov     ds:0FFDF0328h, edx
        mov     ds:0FFDF0320h, ecx
        mov     ds:0FFDF0324h, edx
        and     eax, 1FFh
        shl     eax, 4
        cmp     esi, ds:[KiTimerTableListHead+4+eax]
        jb      short _1
        ja      short _3
        cmp     edi, ds:[KiTimerTableListHead+eax]
        jnb     short _3
        _1:
        inc     ebx
        mov     eax, ebx
        _2:
        and     eax, 1FFh
        shl     eax, 4
        cmp     esi, ds:[KiTimerTableListHead+4+eax]
        jb      short _4
        ja      short _3
        cmp     edi, ds:[KiTimerTableListHead+eax]
        jb      short _4
        _3:
        mov     ecx, dword ptr fs:[20h]
        cmp     dword ptr [ecx+968h], 0
        jnz     short _4
        mov     [ecx+968h], esp
        mov     [ecx+964h], ebx
        mov     ecx, 2
        call    ds:HalRequestSoftwareInterrupt
        _4:
        cmp     ds:KdDebuggerEnabled, 0
        jnz     short _7
        _5:
        cmp     ds:KiTickOffset, 0
        jg      short _6
        mov     eax, ds:KeMaximumIncrement
        add     ds:KiTickOffset, eax
        push    dword ptr [esp]
        call    KeUpdateRunTime3
        cli
        call    ds:HalEndSystemInterrupt
        jmp     Kei386EoiHelper
        _6:
        inc     dword ptr fs:[644h]
        cli
        call    ds:HalEndSystemInterrupt
        jmp     Kei386EoiHelper
        _7:
        call    KdPollBreakIn
        or      al, al
        jz      short _5
        jmp     short _5
    }
}

int
_memicmp(const void *s1, const void *s2, size_t n)
{
    if (n != 0) {
        const unsigned char *p1 = s1, *p2 = s2;

        do {
            if (toupper(*p1) != toupper(*p2))
                return (*p1 - *p2);

            p1++;
            p2++;
        } while (--n != 0);
    }

    return 0;
}


PPREFIX_TABLE_ENTRY
_declspec(naked)
NTAPI
PfxFindPrefix(
    _In_ PPREFIX_TABLE PrefixTable,
    _In_ PSTRING FullName)
{
    UNIMPLEMENTED
    _asm int 3
}

VOID
_declspec(naked)
NTAPI
PfxInitialize(
    _Out_ PPREFIX_TABLE PrefixTable)
{
    UNIMPLEMENTED
    _asm int 3
}

BOOLEAN
_declspec(naked)
NTAPI
PfxInsertPrefix(
    _In_ PPREFIX_TABLE PrefixTable,
    _In_ __drv_aliasesMem PSTRING Prefix,
    _Out_ PPREFIX_TABLE_ENTRY PrefixTableEntry)
{
    UNIMPLEMENTED
    _asm int 3
}

VOID
_declspec(naked)
NTAPI
PfxRemovePrefix(
    _In_ PPREFIX_TABLE PrefixTable,
    _In_ PPREFIX_TABLE_ENTRY PrefixTableEntry)
{
    UNIMPLEMENTED
    _asm int 3
}

//RtlTraceDatabaseCreate
//RtlTraceDatabaseDestroy
//RtlTraceDatabaseValidate
//RtlTraceDatabaseAdd
//RtlTraceDatabaseFind
//RtlTraceDatabaseEnumerate
//RtlTraceDatabaseLock
//RtlTraceDatabaseUnlock
//RtlLockBootStatusData
//RtlUnlockBootStatusData
//RtlGetSetBootStatusData
//
//SeReportSecurityEvent
//SeSetAuditParameter
//VfFailDriver
//WmiFlushTrace
//WmiGetClock
//WmiQueryTrace
//WmiStartTrace
//WmiStopTrace
//WmiTraceFastEvent
//WmiUpdateTrace

//KeI386Call16BitFunction
//KeI386Call16BitCStyleFunction
//
//KeI386AbiosCall
//KeI386GetLid
//KeI386ReleaseLid