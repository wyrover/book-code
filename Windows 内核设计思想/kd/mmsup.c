/*
 * PROJECT:         ReactOS Kernel
 * LICENSE:         BSD - See COPYING.ARM in the top level directory
 * FILE:            ntoskrnl/mm/ARM3/mmsup.c
 * PURPOSE:         ARM Memory Manager Support Routines
 * PROGRAMMERS:     ReactOS Portable Systems Group
 */

/* INCLUDES *******************************************************************/

#include <ntoskrnl.h>
#define NDEBUG
#include <debug.h>


/* PUBLIC FUNCTIONS ***********************************************************/


/*
 * @implemented
 */
BOOLEAN
NTAPI
MmIsAddressValid(IN PVOID VirtualAddress)
{
#if _MI_PAGING_LEVELS >= 4

    /* Check if the PXE is valid */
    if (MiAddressToPxe(VirtualAddress)->u.Hard.Valid == 0) return FALSE;

#endif
#if _MI_PAGING_LEVELS >= 3

    /* Check if the PPE is valid */
    if (MiAddressToPpe(VirtualAddress)->u.Hard.Valid == 0) return FALSE;

#endif
#if _MI_PAGING_LEVELS >= 2

    /* Check if the PDE is valid */
    if (MiAddressToPde(VirtualAddress)->u.Hard.Valid == 0) return FALSE;

#endif

    /* Check if the PTE is valid */
    if (MiAddressToPte(VirtualAddress)->u.Hard.Valid == 0) return FALSE;

    /* This address is valid now, but it will only stay so if the caller holds
     * the PFN lock */
    return TRUE;
}

/* EOF */
