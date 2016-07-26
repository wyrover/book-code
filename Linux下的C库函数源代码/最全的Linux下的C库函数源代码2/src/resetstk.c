/***
*resetstk.c - Recover from Stack overflow.
*
*       Copyright (c) 1989-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines the _resetstkoflw() function.
*
*******************************************************************************/

#include <stdlib.h>
#include <malloc.h>
#include <windows.h>
#include <internal.h>

#define MIN_STACK_REQ_WIN9X 17

#if defined (_M_IA64)

#include <intrin.h>

//
// IA-64 has a large context record and needs an extra page
// to do the unwind of a stack overflow exception.
// The BSP may also need to be reset.
//

#define MIN_STACK_REQ_WINNT 3
#define MIN_BSP_REQ_WINNT 2
#else  /* defined (_M_IA64) */
#define MIN_STACK_REQ_WINNT 2
#endif  /* defined (_M_IA64) */

/***
* void _resetstkoflw(void) - Recovers from Stack Overflow
*
* Purpose:
*       Sets the guard page to its position before the stack overflow.
*
* Exit:
*       Returns nonzero on success, zero on failure
*
*******************************************************************************/

int __cdecl _resetstkoflw(void)
{
    LPBYTE pStack, pStackBase, pMaxGuard, pMinGuard;
#if defined (_M_IA64)
    LPBYTE pBspBase;
    DWORD BspRegionSize;
#endif  /* defined (_M_IA64) */
    MEMORY_BASIC_INFORMATION mbi;
    SYSTEM_INFO si;
    DWORD PageSize;
    DWORD RegionSize;
    DWORD flNewProtect;
    DWORD flOldProtect;
    BOOL
    (*SetThreadStackGuaranteePointer) (
        ULONG *StackSizeInBytes
    );
    unsigned int osplatform = 0;

    // Use _alloca() to get the current stack pointer
#pragma warning(push)
#pragma warning(disable:6255)
    // prefast(6255): This alloca is safe and we do not want a __try here
    pStack = (LPBYTE)_alloca(1);
#pragma warning(pop)

    // Find the base of the stack.

    if (VirtualQuery(pStack, &mbi, sizeof mbi) == 0) {
        return 0;
    }

    pStackBase = (LPBYTE)mbi.AllocationBase;

    GetSystemInfo(&si);
    PageSize = si.dwPageSize;
    RegionSize = 0;

    // Enable the new guard page.
    _ERRCHECK(_get_osplatform(&osplatform));
    if (osplatform == VER_PLATFORM_WIN32_NT) {

        //
        // Note: if the GuaranteedStackBytes TEB field is 0
        // (on older OS versions or if SetThreadStackGuarantee is
        // not being used) we will use the default value of
        // RegionSize (2 pages for ia64, 1 page for other platforms).
        //

        ULONG StackSizeInBytes;
        HMODULE ModuleHandle;

        //
        // Don't call SetThreadStackGuarantee directly as older kernel32.dll
        // versions do not have this export.
        //

        ModuleHandle = GetModuleHandle ("kernel32.dll");

        if (ModuleHandle != NULL) {

            SetThreadStackGuaranteePointer = (PVOID) GetProcAddress (ModuleHandle, "SetThreadStackGuarantee");

            if (SetThreadStackGuaranteePointer != NULL) {
                StackSizeInBytes = 0;       // Indicate just querying
                if (SetThreadStackGuaranteePointer (&StackSizeInBytes) == TRUE &&
                    StackSizeInBytes > 0) {
                    RegionSize = StackSizeInBytes;
                }
            }
        }
    }

    flNewProtect = (osplatform == VER_PLATFORM_WIN32_WINDOWS)
                   ? PAGE_NOACCESS
                   : PAGE_READWRITE | PAGE_GUARD;


    RegionSize = (RegionSize + PageSize - 1) & ~(PageSize - 1);

    //
    // If there is a stack guarantee (RegionSize nonzero), then increase
    // our guard page size by 1 so that even a subsequent fault that occurs
    // midway (instead of at the beginning) through the first guard page
    // will have the extra page to preserve the guarantee.
    //

    if (RegionSize != 0) {
        RegionSize += PageSize;
    }

#if defined (_M_IA64)

    //
    // Reset the backstore stack pages.
    //

    //
    // Calculate the top of the BSP stack, by getting the size of the normal
    // stack and adding it to the StackBase.
    //

    pBspBase = (LPBYTE)(((ULONG_PTR)(((PNT_TIB)NtCurrentTeb())->StackBase) - (ULONG_PTR) mbi.AllocationBase) +
        (ULONG_PTR)(((PNT_TIB)NtCurrentTeb())->StackBase));

    //
    // Get the current BSP and round up since the BSP grows up.
    //

    pMinGuard = (LPBYTE) ((__getReg(__REG_IA64_RsBSP) + PageSize) & ~(ULONG_PTR)(PageSize - 1));

    //
    // The highest BSP address is the top of the BSP stack less one page for
    // the guard.
    //

    pMaxGuard = pBspBase - PageSize;

    BspRegionSize = RegionSize;

    if (BspRegionSize < MIN_BSP_REQ_WINNT * PageSize) {
        BspRegionSize = MIN_BSP_REQ_WINNT * PageSize;
    }

    if (((ULONG_PTR)pMaxGuard < BspRegionSize) ||
        (pMaxGuard - BspRegionSize) < pMinGuard) {

        //
        // The current BSP is already in the highest guard region.
        //

        return 0;
    }

    if (VirtualAlloc (pMinGuard, BspRegionSize, MEM_COMMIT, PAGE_READWRITE) == NULL ||
        VirtualProtect (pMinGuard, BspRegionSize, flNewProtect, &flOldProtect) == 0) {
        return 0;
    }

#endif  /* defined (_M_IA64) */

    if (RegionSize < MIN_STACK_REQ_WINNT * PageSize) {
        RegionSize = MIN_STACK_REQ_WINNT * PageSize;
    }

    //
    // Find the page(s) just below where the stack pointer currently points.
    // This is the highest potential guard page.
    //

    pMaxGuard = (LPBYTE) (((DWORD_PTR)pStack & ~(DWORD_PTR)(PageSize - 1))
                       - RegionSize);

    //
    // If the potential guard page is too close to the start of the stack
    // region, abandon the reset effort for lack of space.  Win9x has a
    // larger reserved stack requirement.
    //

    pMinGuard = pStackBase + (
                              (osplatform == VER_PLATFORM_WIN32_WINDOWS)
                              ? MIN_STACK_REQ_WIN9X
                              : PageSize);

    if (pMaxGuard < pMinGuard) {
        return 0;
    }

    // Set the new guard page just below the current stack page.

    if (VirtualAlloc (pMaxGuard, RegionSize, MEM_COMMIT, PAGE_READWRITE) == NULL ||
        VirtualProtect (pMaxGuard, RegionSize, flNewProtect, &flOldProtect) == 0) {
        return 0;
    }

    return 1;
}
