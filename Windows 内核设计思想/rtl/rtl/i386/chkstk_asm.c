/* $Id: chkstk_asm.s 49826 2010-11-27 22:12:15Z tkreuzer $
 *
 * COPYRIGHT:         See COPYING in the top level directory
 * PROJECT:           ReactOS kernel
 * PURPOSE:           Stack checker
 * FILE:              lib/ntdll/rtl/i386/chkstk.s
 * PROGRAMER:         KJK::Hyperion <noog@libero.it>
 */
#include <ntoskrnl.h>
//#include <asm.inc>
//#include <ks386.inc>

//++ ree
_alloca_probe();
//--

//#define PAGE_SIZE 4096

//PUBLIC __chkstk
//PUBLIC __alloca_probe
//PUBLIC __alloca_probe_16
//.code
_declspec(naked)_alloca_probe_16()
{
    _asm{
        /* 16 byte aligned alloca probe
         * EAX = size to be allocated */
//__alloca_probe_16:
        /* save the ECX register */
        push ecx

        /* ecx = top of the previous stack frame */
        lea ecx, [esp + 8]

        /* Calculate end of allocation */
        sub ecx, eax

        /* Get the misalignment */
        and ecx, 15

        /* Add the misalignment to the original alloc size */
        add eax, ecx

        /* Check for overflow */
        jnc l1

        /* Set maximum value */
        mov eax, HEX(0ffffffff)
        l1:
        /* Restore ecx */
        pop ecx
        /* Fall through to __chkstk */
        jmp _alloca_probe //ree
    }
}

_declspec(naked)_alloca_probe()
{
    _asm{
        /*
         _chkstk() is called by all stack allocations of more than 4 KB. It grows the
         stack in areas of 4 KB each, trying to access each area. This ensures that the
         guard page for the stack is hit, and the stack growing triggered
         */
//__chkstk:
//__alloca_probe:

        /* EAX = size to be allocated */
        /* save the ECX register */
        push ecx

        /* ECX = top of the previous stack frame */
        lea ecx, [esp + 8]

        /* probe the desired memory, page by page */
        cmp eax, PAGE_SIZE
        jl _l_LessThanAPage

        _l_MoreThanAPage:

        /* raise the top of the stack by a page and probe */
        sub ecx, PAGE_SIZE
        test [ecx], eax

        /* loop if still more than a page must be probed */
        sub eax, PAGE_SIZE
        cmp eax, PAGE_SIZE
        jge _l_MoreThanAPage

        _l_LessThanAPage:

        /* raise the top of the stack by EAX bytes (size % 4096) and probe */
        sub ecx, eax
        test [ecx], eax

        /* EAX = top of the stack */
        mov eax, esp

        /* allocate the memory */
        mov esp, ecx

        /* restore ECX */
        mov ecx, [eax]

        /* restore the return address */
        mov eax, [eax + 4]
        push eax

        /* return */
        ret

        /* EOF */
    }
}//END
