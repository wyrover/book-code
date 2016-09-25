#include <ntoskrnl.h>
#define NDEBUG
#include <debug.h>
#include "kdapi.h"

#undef  STATUS_INTEGER_DIVIDE_BY_ZERO
#define STATUS_INTEGER_DIVIDE_BY_ZERO 0xC0000094

#undef  STATUS_SINGLE_STEP
#define STATUS_SINGLE_STEP 0x80000004

#undef  STATUS_BREAKPOINT
#define STATUS_BREAKPOINT 0x80000003

#undef  STATUS_ACCESS_VIOLATION
#define STATUS_ACCESS_VIOLATION 0xC0000005

#undef  STATUS_IN_PAGE_ERROR
#define STATUS_IN_PAGE_ERROR 0xC0000006

#undef  UNEXPECTED_KERNEL_MODE_TRAP
#define UNEXPECTED_KERNEL_MODE_TRAP 0x0000007f

_declspec(naked) _KiExceptionExit2()
{
    _asm{

        mov gs, word ptr[ebp + KTRAP_FRAME.SegGs]
        mov es, word ptr[ebp + KTRAP_FRAME.SegEs]
        mov ds, word ptr[ebp + KTRAP_FRAME.SegDs]

        mov edx, dword ptr[ebp + KTRAP_FRAME.Edx]
        mov ecx, dword ptr[ebp + KTRAP_FRAME.Ecx]
        mov eax, dword ptr[ebp + KTRAP_FRAME.Eax]

        mov fs, word ptr[ebp + KTRAP_FRAME.SegFs]

        mov edi, dword ptr[ebp + KTRAP_FRAME.Edi]
        mov esi, dword ptr[ebp + KTRAP_FRAME.Esi]
        mov ebx, dword ptr[ebp + KTRAP_FRAME.Ebx]

        lea esp, dword ptr[ebp + KTRAP_FRAME.Eip]
        mov ebp, dword ptr[ebp + KTRAP_FRAME.Ebp]

        iretd
    }
}

_declspec(naked) _KiExceptionExit()
{
    _asm{
        lea     esp, [ebp + KTRAP_FRAME.SegGs]

        pop     gs
        pop     es
        pop     ds

        pop     edx
        pop     ecx
        pop     eax

        lea     esp, [ebp + KTRAP_FRAME.SegFs]
        pop     fs
        lea     esp, [ebp + KTRAP_FRAME.Edi]

        pop     edi
        pop     esi
        pop     ebx
        pop     ebp

        add     esp, 4

        iretd
    }
}

_declspec(naked) CommonDispatchException2()
{
    _asm{

        /* 从栈中分异常记录空间 */
        sub     esp, size EXCEPTION_RECORD
        mov     [esp + EXCEPTION_RECORD.ExceptionCode], eax   // 0x80000003
        xor     eax, eax
        mov     [esp + EXCEPTION_RECORD.ExceptionFlags], eax
        mov     [esp + EXCEPTION_RECORD.ExceptionRecord], eax
        mov     [esp + EXCEPTION_RECORD.ExceptionAddress], ebx
        mov     [esp + EXCEPTION_RECORD.NumberParameters], ecx // 参数个数

        /* 保存参数 */
        mov     [esp + EXCEPTION_RECORD.ExceptionInformation], edx  // 参数1
        mov     [esp + EXCEPTION_RECORD.ExceptionInformation + 4], esi // 参数2
        mov     [esp + EXCEPTION_RECORD.ExceptionInformation + 8], edi // 参数3
#ifndef SIMULATE_DEBUG
        mov     eax, dr0
        mov     [ebp + KTRAP_FRAME.Dr0], eax
        mov     eax, dr1
        mov     [ebp + KTRAP_FRAME.Dr1], eax
        mov     eax, dr2
        mov     [ebp + KTRAP_FRAME.Dr2], eax
        mov     eax, dr3
        mov     [ebp + KTRAP_FRAME.Dr3], eax
        mov     eax, dr6
        mov     [ebp + KTRAP_FRAME.Dr6], eax
        mov     eax, dr7
        mov     [ebp + KTRAP_FRAME.Dr7], eax
#endif
        mov     ax, ss
        mov     [ebp + KTRAP_FRAME.TempSegCs], eax
        mov     [ebp + KTRAP_FRAME.TempEsp], ebp
        add     dword ptr [ebp + KTRAP_FRAME.TempEsp], offset KTRAP_FRAME.HardwareEsp

        mov     ecx, esp // 指向异常记录EXCEPTION_RECORD

        mov     eax, [ebp + KTRAP_FRAME.SegCs]
        and     eax, MODE_MASK

        push    1        // IN BOOLEAN FirstChance
        push    eax      // IN KPROCESSOR_MODE PreviousMode
        push    ebp      // IN PKTRAP_FRAME TrapFrame
        push    0        // IN PKEXCEPTION_FRAME ExceptionFrame
        push    ecx      // IN PEXCEPTION_RECORD ExceptionRecord
        call    KiDispatchException2
        mov     esp, ebp // 指向进入例程时建立的KTRAP_FRAME
        jmp     _KiExceptionExit2
    }
}

_declspec(naked) CommonDispatchException()
{
    _asm{

        /* 从栈中分异常记录空间 */
        sub     esp, size EXCEPTION_RECORD
        mov     [esp + EXCEPTION_RECORD.ExceptionCode], eax   // 0x80000003
        xor     eax, eax
        mov     [esp + EXCEPTION_RECORD.ExceptionFlags], eax
        mov     [esp + EXCEPTION_RECORD.ExceptionRecord], eax
        mov     [esp + EXCEPTION_RECORD.ExceptionAddress], ebx
        mov     [esp + EXCEPTION_RECORD.NumberParameters], ecx // 参数个数

        /* 保存参数 */
        mov     [esp + EXCEPTION_RECORD.ExceptionInformation], edx  // 参数1
        mov     [esp + EXCEPTION_RECORD.ExceptionInformation + 4], esi // 参数2
        mov     [esp + EXCEPTION_RECORD.ExceptionInformation + 8], edi // 参数3
#ifndef SIMULATE_DEBUG
        mov     eax, dr0
        mov     [ebp + KTRAP_FRAME.Dr0], eax
        mov     eax, dr1
        mov     [ebp + KTRAP_FRAME.Dr1], eax
        mov     eax, dr2
        mov     [ebp + KTRAP_FRAME.Dr2], eax
        mov     eax, dr3
        mov     [ebp + KTRAP_FRAME.Dr3], eax
        mov     eax, dr6
        mov     [ebp + KTRAP_FRAME.Dr6], eax
        mov     eax, dr7
        mov     [ebp + KTRAP_FRAME.Dr7], eax
#endif
        mov     ax, ss
        mov     [ebp + KTRAP_FRAME.TempSegCs], eax
        mov     [ebp + KTRAP_FRAME.TempEsp], ebp
        add     dword ptr [ebp + KTRAP_FRAME.TempEsp], offset KTRAP_FRAME.HardwareEsp

        mov     ecx, esp // 指向异常记录EXCEPTION_RECORD

        mov     eax, [ebp + KTRAP_FRAME.SegCs]
        and     eax, MODE_MASK

        push    1        // IN BOOLEAN FirstChance
        push    eax      // IN KPROCESSOR_MODE PreviousMode
        push    ebp      // IN PKTRAP_FRAME TrapFrame
        push    0        // IN PKEXCEPTION_FRAME ExceptionFrame
        push    ecx      // IN PEXCEPTION_RECORD ExceptionRecord
        call    KiDispatchException
        mov     esp, ebp // 指向进入例程时建立的KTRAP_FRAME
        jmp     _KiExceptionExit
    }
}

#define ENTER_TRAP \
    { \
        _asm mov     word ptr [esp+2], 0x0   /* ErrorCode高位置0 */ \
        _asm push    ebp \
        _asm push    ebx \
        _asm push    esi \
        _asm push    edi \
        _asm push    fs  \
        _asm push    0xffffffff /* KTRAP_FRAME.ExceptionList */ \
        _asm push    -1         /* KTRAP_FRAME.PreviousPreviousMode */ \
        _asm push    eax \
        _asm push    ecx \
        _asm push    edx \
        _asm push    ds  \
        _asm push    es  \
        _asm push    gs  \
        _asm sub     esp, 0x30 \
        _asm mov     ax, 0x23  \
        _asm _emit   0x66   \
        _asm mov     ds,ax \
        _asm _emit   0x66   \
        _asm mov     es,ax \
        _asm mov     ebp, esp  \
        _asm mov     ecx, [ebp+KTRAP_FRAME.Eip]  \
    }

#define ENTER_TRAP2 \
    { \
        _asm sub esp, offset KTRAP_FRAME.ErrCode \
        \
        /* Save nonvolatile registers */ \
        _asm mov [esp + KTRAP_FRAME.Ebp], ebp \
        _asm mov [esp + KTRAP_FRAME.Ebx], ebx \
        _asm mov [esp + KTRAP_FRAME.Esi], esi \
        _asm mov [esp + KTRAP_FRAME.Edi], edi \
        \
        _asm mov word ptr[esp + KTRAP_FRAME.SegFs], fs \
        _asm mov [esp + KTRAP_FRAME.ExceptionList], 0xffffffff \
        _asm mov [esp + KTRAP_FRAME.PreviousPreviousMode], -1 \
        \
        /* Save eax for system calls, for use by the C handler */ \
        _asm mov [esp + KTRAP_FRAME.Eax], eax \
        \
        /* Does the caller want nonvolatiles only? */ \
        _asm mov [esp + KTRAP_FRAME.Ecx], ecx \
        _asm mov [esp + KTRAP_FRAME.Edx], edx \
        \
        /* Save segment selectors */ \
        _asm mov word ptr[esp + KTRAP_FRAME.SegDs], ds \
        _asm mov word ptr[esp + KTRAP_FRAME.SegEs], es \
        _asm mov word ptr[esp + KTRAP_FRAME.SegGs], gs \
        \
        /* Load correct data segments */ \
        _asm mov ax, KGDT_R3_DATA OR RPL_MASK \
        _asm mov ds, ax \
        _asm mov es, ax \
        _asm mov ebp, esp   \
        /*Inspect old EIP in case this is a single stepped sysenter instruction.*/  \
        _asm mov ecx, [ebp+KTRAP_FRAME.Eip]  \
    }

_declspec(naked) _KiTrap00()
{
    _asm{
        push    0    // 压入伪错误代码
        ENTER_TRAP2

        mov     ebx, [ebp + KTRAP_FRAME.Eip]    // 指向出错位置
        mov     eax, STATUS_INTEGER_DIVIDE_BY_ZERO  // 0xC0000094
        xor     ecx, ecx
        jmp    CommonDispatchException2
    }
}

_declspec(naked) _KiTrap01()
{
    _asm{
        push    0   // 压入伪错误代码
        ENTER_TRAP

        and     dword ptr [ebp + KTRAP_FRAME.EFlags], ~EFLAGS_TF // 0x100L
        mov     ebx, [ebp + KTRAP_FRAME.Eip]    // 指向单步位置
        mov     eax, STATUS_SINGLE_STEP // 0x80000004
        xor     ecx, ecx
        jmp    CommonDispatchException
    }
}


_declspec(naked) _KiTrap03()
{
    _asm{
        push    0  // 压入伪错误代码
        ENTER_TRAP

        mov     edx, eax                // 参数1
        mov     esi, ecx                // 参数2
        mov     edi, edx                // 参数3
        xor     edx, edx

        mov     ebx, [ebp + KTRAP_FRAME.Eip] // 指向断点位置
#ifdef SIMULATE_DEBUG
        sub     ebx, 5
#else
        dec     ebx
#endif
        mov     ecx, 3
        mov     eax, STATUS_BREAKPOINT  // 0x80000003
        jmp    CommonDispatchException
    }
}

_declspec(naked) _KiTrap0D()
{
    _asm{
        //push    0     // 不需要压入伪错误代码
        ENTER_TRAP2

        test    dword ptr [ebp + KTRAP_FRAME.SegCs], MODE_MASK

        mov     ebx, [ebp + KTRAP_FRAME.Eip] // 指向出错位置
        mov     edx, [ebp + KTRAP_FRAME.ErrCode]
        and     edx, 0xffff
        mov     ecx, 1
        mov     eax, STATUS_ACCESS_VIOLATION // 0xC0000005
        jmp    CommonDispatchException2
    }
}

_declspec(naked) _KiTrap0E()
{
    _asm{
        //push    0     // 不需要压入伪错误代码
        ENTER_TRAP2

        mov     edi, cr2

        push    ebp                     // TrapInformation
        mov     eax, [ebp + KTRAP_FRAME.SegCs]
        and     eax, MODE_MASK

        push    eax                     // Mode
        push    edi                     // 设置页错误虚拟地址
        mov     eax, [ebp + KTRAP_FRAME.ErrCode]
        and     eax, KeErrorMask
        push    eax                     // TrapInformation
        call    MmAccessFault
        test    eax, eax                // 检查错误是否解决
        jl      Error
        jmp     _KiExceptionExit2
        Error:
        mov     edx, ecx
        mov     ebx, esi
        mov     esi, edi
        mov     ecx, 3
        mov     edi, eax
        mov     eax, STATUS_IN_PAGE_ERROR // 0xC0000006
        jmp    CommonDispatchException2
    }
}


_declspec(naked) _KiDebugService()
{
    _asm{
        push    0   // 压入伪错误代码
        ENTER_TRAP2

        inc     [ebp + KTRAP_FRAME.Eip]     // 指向出错位置
        mov     ebx, [ebp + KTRAP_FRAME.Eip]
        mov     edx, [ebp + KTRAP_FRAME.Eax] // 服务类型
        mov     esi, [ebp + KTRAP_FRAME.Ecx] // 参数1
        mov     edi, [ebp + KTRAP_FRAME.Edx] // 参数2
        mov     ecx, 3
        mov     eax, STATUS_BREAKPOINT
        jmp    CommonDispatchException2
    }
}

_declspec(naked) _KiTrap02() // INT 02: NMI Interrupt
{
    UNIMPLEMENTED
    _asm int 3
}
_declspec(naked) _KiTrap04() // Overflow Exception (#OF)
{
    UNIMPLEMENTED
    _asm int 3
}
_declspec(naked) _KiTrap05() // BOUND Range Exceeded (#BR)
{
    UNIMPLEMENTED
    _asm int 3
}
_declspec(naked) _KiTrap06() // INT 06: Invalid Opcode Code (#UD)
{
    //不支持SSE指令, 使用VS2008编译
    UNIMPLEMENTED
    _asm int 3
}
_declspec(naked) _KiTrap07() // Device Not Available (#NM)
{
    UNIMPLEMENTED
    _asm int 3
}
_declspec(naked) _KiTrap08() // Double Fault Exception (#DF)
{
    UNIMPLEMENTED
    _asm int 3
}
_declspec(naked) _KiTrap09() // RESERVED
{
    UNIMPLEMENTED
    _asm int 3
}
_declspec(naked) _KiTrap0A() // Invalid TSS Exception (#TS)
{
    UNIMPLEMENTED
    _asm int 3
}
_declspec(naked) _KiTrap0B() // Segment Not Present (#NP
{
    UNIMPLEMENTED
    _asm int 3
}
_declspec(naked) _KiTrap0C() // Stack Fault Exception (#SS)
{
    UNIMPLEMENTED
    _asm int 3
}
_declspec(naked) _KiTrap0F() // RESERVED
{
    UNIMPLEMENTED
    _asm int 3
}

_declspec(naked) _KiTrap10() // x87 FPU Error (#MF)
{
    UNIMPLEMENTED
    _asm int 3
}
_declspec(naked) _KiTrap11() // Align Check Exception (#AC)
{
    UNIMPLEMENTED
    _asm int 3
}
// _KiTrap2A
_declspec(naked) _KiGetTickCount() // Get Tick Count Handler
{
    UNIMPLEMENTED
    _asm int 3
}
// _KiTrap2B
_declspec(naked) _KiCallbackReturn() // User-Mode Callback Return
{
    UNIMPLEMENTED
    _asm int 3
}
// _KiTrap2C
_declspec(naked) _KiRaiseAssertion() // Debug Assertion Handler
{
    UNIMPLEMENTED
    _asm int 3
}
// _KiTrap2E
_declspec(naked) _KiSystemService() // System Call Service Handler
{
    UNIMPLEMENTED
    _asm int 3
}

// Arguments for idt
#define INT_32_DPL0                 HEX(08E00)
#define INT_32_DPL3                 HEX(0EE00)

#define TRAP_32_DPL0                HEX(08F00)
#define TRAP_32_DPL3                HEX(0EF00)

#define idt(vector, handler, access) \
    { \
        ((PKIDTENTRY)vector)->Offset            = (USHORT)((ULONG)handler & 0xffff); \
        ((PKIDTENTRY)vector)->ExtendedOffset    = (USHORT)((ULONG)handler >> 0x10); \
        ((PKIDTENTRY)vector)->Selector          = KGDT_R0_CODE; \
        ((PKIDTENTRY)vector)->Access            = access; \
    }

ULONG KiUnexpectedEntrySize = 7; //_KiUnexpectedInterrupt49 - _KiUnexpectedInterrupt48

VOID _declspec(naked) NTAPI KiStartUnexpectedRange(VOID)
{
    _asm{
        push    0xFFFFFFB0
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFB1
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFB2
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFB3
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFB4
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFB5
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFB6
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFB7
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFB8
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFB9
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFBA
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFBB
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFBC
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFBD
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFBE
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFBF
        jmp     KiEndUnexpectedRange

        push    0xFFFFFFC0
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFC1
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFC2
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFC3
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFC4
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFC5
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFC6
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFC7
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFC8
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFC9
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFCA
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFCB
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFCC
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFCD
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFCE
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFCF
        jmp     KiEndUnexpectedRange

        push    0xFFFFFFD0
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFD1
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFD2
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFD3
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFD4
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFD5
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFD6
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFD7
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFD8
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFD9
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFDA
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFDB
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFDC
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFDD
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFDE
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFDF
        jmp     KiEndUnexpectedRange

        push    0xFFFFFFE0
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFE1
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFE2
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFE3
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFE4
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFE5
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFE6
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFE7
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFE8
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFE9
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFEA
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFEB
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFEC
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFED
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFEE
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFEF
        jmp     KiEndUnexpectedRange

        push    0xFFFFFFF0
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFF1
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFF2
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFF3
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFF4
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFF5
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFF6
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFF7
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFF8
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFF9
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFFA
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFFB
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFFC
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFFD
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFFE
        jmp     KiEndUnexpectedRange
        push    0xFFFFFFFF
        jmp     KiEndUnexpectedRange

        push    0x0
        jmp     KiEndUnexpectedRange
        push    0x1
        jmp     KiEndUnexpectedRange
        push    0x2
        jmp     KiEndUnexpectedRange
        push    0x3
        jmp     KiEndUnexpectedRange
        push    0x4
        jmp     KiEndUnexpectedRange
        push    0x5
        jmp     KiEndUnexpectedRange
        push    0x6
        jmp     KiEndUnexpectedRange
        push    0x7
        jmp     KiEndUnexpectedRange
        push    0x8
        jmp     KiEndUnexpectedRange
        push    0x9
        jmp     KiEndUnexpectedRange
        push    0xA
        jmp     KiEndUnexpectedRange
        push    0xB
        jmp     KiEndUnexpectedRange
        push    0xC
        jmp     KiEndUnexpectedRange
        push    0xD
        jmp     KiEndUnexpectedRange
        push    0xE
        jmp     KiEndUnexpectedRange
        push    0xF
        jmp     KiEndUnexpectedRange

        push    0x10
        jmp     KiEndUnexpectedRange
        push    0x11
        jmp     KiEndUnexpectedRange
        push    0x12
        jmp     KiEndUnexpectedRange
        push    0x13
        jmp     KiEndUnexpectedRange
        push    0x14
        jmp     KiEndUnexpectedRange
        push    0x15
        jmp     KiEndUnexpectedRange
        push    0x16
        jmp     KiEndUnexpectedRange
        push    0x17
        jmp     KiEndUnexpectedRange
        push    0x18
        jmp     KiEndUnexpectedRange
        push    0x19
        jmp     KiEndUnexpectedRange
        push    0x1A
        jmp     KiEndUnexpectedRange
        push    0x1B
        jmp     KiEndUnexpectedRange
        push    0x1C
        jmp     KiEndUnexpectedRange
        push    0x1D
        jmp     KiEndUnexpectedRange
        push    0x1E
        jmp     KiEndUnexpectedRange
        push    0x1F
        jmp     KiEndUnexpectedRange

        push    0x20
        jmp     KiEndUnexpectedRange
        push    0x21
        jmp     KiEndUnexpectedRange
        push    0x22
        jmp     KiEndUnexpectedRange
        push    0x23
        jmp     KiEndUnexpectedRange
        push    0x24
        jmp     KiEndUnexpectedRange
        push    0x25
        jmp     KiEndUnexpectedRange
        push    0x26
        jmp     KiEndUnexpectedRange
        push    0x27
        jmp     KiEndUnexpectedRange
        push    0x28
        jmp     KiEndUnexpectedRange
        push    0x29
        jmp     KiEndUnexpectedRange
        push    0x2A
        jmp     KiEndUnexpectedRange
        push    0x2B
        jmp     KiEndUnexpectedRange
        push    0x2C
        jmp     KiEndUnexpectedRange
        push    0xD
        jmp     KiEndUnexpectedRange
        push    0x2E
        jmp     KiEndUnexpectedRange
        push    0x2F
        jmp     KiEndUnexpectedRange

        push    0x30
        jmp     KiEndUnexpectedRange
        push    0x31
        jmp     KiEndUnexpectedRange
        push    0x32
        jmp     KiEndUnexpectedRange
        push    0x33
        jmp     KiEndUnexpectedRange
        push    0x34
        jmp     KiEndUnexpectedRange
        push    0x35
        jmp     KiEndUnexpectedRange
        push    0x36
        jmp     KiEndUnexpectedRange
        push    0x37
        jmp     KiEndUnexpectedRange
        push    0x38
        jmp     KiEndUnexpectedRange
        push    0x39
        jmp     KiEndUnexpectedRange
        push    0x3A
        jmp     KiEndUnexpectedRange
        push    0x3B
        jmp     KiEndUnexpectedRange
        push    0x3C
        jmp     KiEndUnexpectedRange
        push    0x3D
        jmp     KiEndUnexpectedRange
        push    0x3E
        jmp     KiEndUnexpectedRange
        push    0x3F
        jmp     KiEndUnexpectedRange

        push    0x40
        jmp     KiEndUnexpectedRange
        push    0x41
        jmp     KiEndUnexpectedRange
        push    0x42
        jmp     KiEndUnexpectedRange
        push    0x43
        jmp     KiEndUnexpectedRange
        push    0x44
        jmp     KiEndUnexpectedRange
        push    0x45
        jmp     KiEndUnexpectedRange
        push    0x46
        jmp     KiEndUnexpectedRange
        push    0x47
        jmp     KiEndUnexpectedRange
        push    0x48
        jmp     KiEndUnexpectedRange
        push    0x49
        jmp     KiEndUnexpectedRange
        push    0x4A
        jmp     KiEndUnexpectedRange
        push    0x4B
        jmp     KiEndUnexpectedRange
        push    0x4C
        jmp     KiEndUnexpectedRange
        push    0x4D
        jmp     KiEndUnexpectedRange
        push    0x4E
        jmp     KiEndUnexpectedRange
        push    0x4F
        jmp     KiEndUnexpectedRange

        push    0x50
        jmp     KiEndUnexpectedRange
        push    0x51
        jmp     KiEndUnexpectedRange
        push    0x52
        jmp     KiEndUnexpectedRange
        push    0x53
        jmp     KiEndUnexpectedRange
        push    0x4
        jmp     KiEndUnexpectedRange
        push    0x55
        jmp     KiEndUnexpectedRange
        push    0x56
        jmp     KiEndUnexpectedRange
        push    0x57
        jmp     KiEndUnexpectedRange
        push    0x58
        jmp     KiEndUnexpectedRange
        push    0x59
        jmp     KiEndUnexpectedRange
        push    0x5A
        jmp     KiEndUnexpectedRange
        push    0x5B
        jmp     KiEndUnexpectedRange
        push    0x5C
        jmp     KiEndUnexpectedRange
        push    0x5D
        jmp     KiEndUnexpectedRange
        push    0x5E
        jmp     KiEndUnexpectedRange
        push    0x5F
        jmp     KiEndUnexpectedRange

        push    0x60
        jmp     KiEndUnexpectedRange
        push    0x61
        jmp     KiEndUnexpectedRange
        push    0x62
        jmp     KiEndUnexpectedRange
        push    0x63
        jmp     KiEndUnexpectedRange
        push    0x64
        jmp     KiEndUnexpectedRange
        push    0x65
        jmp     KiEndUnexpectedRange
        push    0x66
        jmp     KiEndUnexpectedRange
        push    0x67
        jmp     KiEndUnexpectedRange
        push    0x68
        jmp     KiEndUnexpectedRange
        push    0x69
        jmp     KiEndUnexpectedRange
        push    0x6A
        jmp     KiEndUnexpectedRange
        push    0x6B
        jmp     KiEndUnexpectedRange
        push    0x6C
        jmp     KiEndUnexpectedRange
        push    0x6D
        jmp     KiEndUnexpectedRange
        push    0x6E
        jmp     KiEndUnexpectedRange
        push    0x6F
        jmp     KiEndUnexpectedRange

        push    0x70
        jmp     KiEndUnexpectedRange
        push    0x71
        jmp     KiEndUnexpectedRange
        push    0x72
        jmp     KiEndUnexpectedRange
        push    0x73
        jmp     KiEndUnexpectedRange
        push    0x74
        jmp     KiEndUnexpectedRange
        push    0x75
        jmp     KiEndUnexpectedRange
        push    0x76
        jmp     KiEndUnexpectedRange
        push    0x77
        jmp     KiEndUnexpectedRange
        push    0x78
        jmp     KiEndUnexpectedRange
        push    0x79
        jmp     KiEndUnexpectedRange
        push    0x7A
        jmp     KiEndUnexpectedRange
        push    0x7B
        jmp     KiEndUnexpectedRange
        push    0x7C
        jmp     KiEndUnexpectedRange
        push    0x7D
        jmp     KiEndUnexpectedRange
        push    0x7E
        jmp     KiEndUnexpectedRange
        push    0x7F
        jmp     KiEndUnexpectedRange
    }
}

VOID
FASTCALL
KiUnexpectedInterruptTailHandler(IN PKTRAP_FRAME TrapFrame);

VOID _declspec(naked) NTAPI KiUnexpectedInterruptTail(VOID)
{
    _asm{
        sub     esp, 64h
        mov     [esp + 60h], ebp
        mov     [esp + 5Ch], ebx
        mov     [esp + 58h], esi
        mov     [esp + 54h], edi
        mov     [esp + 44h], eax
        mov     [esp + 40h], ecx
        mov     [esp + 60], edx
        test    byte ptr [esp + 114], 2
        jz      short _1

        mov     eax, [esp + 128]
        mov     ecx, [esp + 124]
        mov     [esp + 56], eax
        mov     [esp + 52], ecx
        mov     eax, [esp + 132]
        mov     ecx, [esp + 136]
        mov     [esp + 80], eax
        mov     [esp + 48], ecx
        jmp     short _2

        _1:
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
        _2:
        mov     ax, 23h
        mov     ds, ax
        mov     es, ax
        mov     ax, 30h
        mov     fs, ax
        mov     eax, [esp + 104]
        mov     [esp + 4], eax
        mov     [esp], ebp
        mov     ebp, esp
        mov     ecx, esp
        cld
        call    KiUnexpectedInterruptTailHandler
    }
}

VOID _declspec(naked) NTAPI KiEndUnexpectedRange(VOID)
{
    _asm{
        add     dword ptr [esp + 0], 80h
        jmp     KiUnexpectedInterruptTail
    }
}

VOID
NTAPI
KdSetTrap(VOID)
{
    int i;
    PKIDTENTRY Base;
#pragma pack(2)
    static struct {
        USHORT Limit;
        ULONG Base;
    } GdtDef, IdtDef;
#pragma pack(4)
    // 获取当前 GDT & IDT
    _asm {
        sgdt GdtDef;
        sidt IdtDef;
    }
    Base = (PKIDTENTRY)IdtDef.Base;
    /* This is the Software Interrupt Table that we handle in this file:               */
    idt(&Base[0x00], _KiTrap00, INT_32_DPL0);  /* INT 00: Divide Error (#DE)           */
    idt(&Base[0x01], _KiTrap01, INT_32_DPL0);  /* INT 01: Debug Exception (#DB)        */
    idt(&Base[0x02], _KiTrap02, INT_32_DPL0);  /* INT 02: NMI Interrupt                */
    idt(&Base[0x03], _KiTrap03, INT_32_DPL3);  /* INT 03: Breakpoint Exception (#BP)   */
    idt(&Base[0x04], _KiTrap04, INT_32_DPL3);  /* INT 04: Overflow Exception (#OF)     */
    idt(&Base[0x05], _KiTrap05, INT_32_DPL0);  /* INT 05: BOUND Range Exceeded (#BR)   */
    idt(&Base[0x06], _KiTrap06, INT_32_DPL0);  /* INT 06: Invalid Opcode Code (#UD)    */
    idt(&Base[0x07], _KiTrap07, INT_32_DPL0);  /* INT 07: Device Not Available (#NM)   */
    idt(&Base[0x08], _KiTrap08, INT_32_DPL0);  /* INT 08: Double Fault Exception (#DF) */
    idt(&Base[0x09], _KiTrap09, INT_32_DPL0);  /* INT 09: RESERVED                     */
    idt(&Base[0x0A], _KiTrap0A, INT_32_DPL0);  /* INT 0A: Invalid TSS Exception (#TS)  */
    idt(&Base[0x0B], _KiTrap0B, INT_32_DPL0);  /* INT 0B: Segment Not Present (#NP)    */
    idt(&Base[0x0C], _KiTrap0C, INT_32_DPL0);  /* INT 0C: Stack Fault Exception (#SS)  */
    idt(&Base[0x0D], _KiTrap0D, INT_32_DPL0);  /* INT 0D: General Protection (#GP)     */
    idt(&Base[0x0E], _KiTrap0E, INT_32_DPL0);  /* INT 0E: Page-Fault Exception (#PF)   */
    idt(&Base[0x0F], _KiTrap0F, INT_32_DPL0);  /* INT 0F: RESERVED                     */
    idt(&Base[0x10], _KiTrap10, INT_32_DPL0);  /* INT 10: x87 FPU Error (#MF)          */
    idt(&Base[0x11], _KiTrap11, INT_32_DPL0);  /* INT 11: Align Check Exception (#AC)  */
    idt(&Base[0x12], _KiTrap0F, INT_32_DPL0);  /* INT 12: Machine Check Exception (#MC)*/
    idt(&Base[0x13], _KiTrap0F, INT_32_DPL0);  /* INT 13: SIMD FPU Exception (#XF)     */

    for (i = 0x14; i <= 0x29; i++) {           /* INT 14-29: UNDEFINED INTERRUPTS      */
        idt(&Base[i], _KiTrap0F, INT_32_DPL0);
    }

    idt(&Base[0x2A], _KiGetTickCount,   INT_32_DPL3);/* INT 2A: Get Tick Count Handler      */
    idt(&Base[0x2B], _KiCallbackReturn, INT_32_DPL3);/* INT 2B: User-Mode Callback Return   */
    idt(&Base[0x2C], _KiRaiseAssertion, INT_32_DPL3);/* INT 2C: Debug Assertion Handler     */
    idt(&Base[0x2D], _KiDebugService,   INT_32_DPL3);/* INT 2D: Debug Service Handler       */
    idt(&Base[0x2E], _KiSystemService,  INT_32_DPL3);/* INT 2E: System Call Service Handler */
    idt(&Base[0x2F], _KiTrap0F,         INT_32_DPL0);/* INT 2F: RESERVED                    */

    for (i = 0x30; i <= 0xB0; i++) {                 /* INT 30-FF: UNDEFINED INTERRUPTS     */
        idt(&Base[i], KiStartUnexpectedRange + (i - 0x30)*KiUnexpectedEntrySize, INT_32_DPL0);
    }

    _asm {
        // 保存GDT & IDT，由于没有改变IDT位置，其实这步可以省略
        lgdt GdtDef;
        lidt IdtDef;
    }
}