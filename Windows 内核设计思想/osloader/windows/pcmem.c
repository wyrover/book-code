/*
 *  FreeLoader
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Note: Most of this code comes from the old file "i386mem.c", which
 *       was Copyright (C) 1998-2003 Brian Palmer <brianp@sginet.com>
 */

#include <freeldr.h>
#include <arch/pc/x86common.h>

#define NDEBUG
#include <debug.h>

#include "..\osloader.h"

DBG_DEFAULT_CHANNEL(MEMORY);

#define MAX_BIOS_DESCRIPTORS 32
#define FREELDR_BASE_PAGE  (FREELDR_BASE / PAGE_SIZE)
#define DISKBUF_BASE_PAGE  (DISKREADBUFFER / PAGE_SIZE)
#define STACK_BASE_PAGE    (STACKLOWLIMIT / PAGE_SIZE)
#define STACK_END_PAGE     (STACK32ADDR / PAGE_SIZE)
#define BIOSBUF_BASE_PAGE  (BIOSCALLBUFFER / PAGE_SIZE)

#define FREELDR_PAGE_COUNT (DISKBUF_BASE_PAGE - FREELDR_BASE_PAGE)
#define DISKBUF_PAGE_COUNT (STACK_BASE_PAGE - DISKBUF_BASE_PAGE)
#define STACK_PAGE_COUNT   (STACK_END_PAGE - STACK_BASE_PAGE)
#define BIOSBUF_PAGE_COUNT (1)

BIOS_MEMORY_MAP PcBiosMemoryMap[MAX_BIOS_DESCRIPTORS];
ULONG PcBiosMapCount;

/*FREELDR_MEMORY_DESCRIPTOR PcMemoryMap[MAX_BIOS_DESCRIPTORS + 1] =
{
 { LoaderFirmwarePermanent, 0x00,               1 }, // realmode int vectors
 { LoaderFirmwareTemporary, 0x01,               FREELDR_BASE_PAGE - 1 }, // freeldr stack + cmdline
 { LoaderLoadedProgram,     FREELDR_BASE_PAGE,  FREELDR_PAGE_COUNT }, // freeldr image
 { LoaderFirmwareTemporary, DISKBUF_BASE_PAGE,  DISKBUF_PAGE_COUNT }, // Disk read buffer for int 13h. DISKREADBUFFER
 { LoaderOsloaderStack,     STACK_BASE_PAGE,    STACK_PAGE_COUNT }, // prot mode stack.
 { LoaderFirmwareTemporary, BIOSBUF_BASE_PAGE,  BIOSBUF_PAGE_COUNT }, // BIOSCALLBUFFER
 { LoaderFirmwarePermanent, 0xA0,               0x50 }, // ROM / Video
 { LoaderSpecialMemory,     0xF0,               0x10 }, // ROM / Video
 { LoaderSpecialMemory,     0xFFF,              1 }, // unusable memory
 { 0, 0, 0 }, // end of map
};*/

FREELDR_MEMORY_DESCRIPTOR PcMemoryMap[MAX_BIOS_DESCRIPTORS + 1] = {
    { LoaderFirmwarePermanent, 0x00,               0x01 }, // realmode int vectors
    { LoaderFree,              0x01,               0x1f },
    { LoaderFirmwareTemporary, 0x20,               0x19 },
    { LoaderMemoryData,        0x39,               0x17 },
    { LoaderFirmwareTemporary, 0x50,               0x10 },
    { LoaderFirmwareTemporary, 0x60,               0x02 },
    { LoaderOsloaderHeap,      0x62,               0x3e },
    { LoaderFirmwarePermanent, 0xa0,               0x50 }, // ROM / Video
    { LoaderSpecialMemory,     0xf0,               0x10 }, // ROM / Video
    { LoaderFirmwareTemporary, 0x3ff,              0x01 },
    { LoaderLoadedProgram,     0x400,              0x80 },
    { LoaderFirmwareTemporary, 0x480,              0x60 },
    { LoaderSpecialMemory,     0xfff,              0x01 }, // unusable memory
    { 0, 0, 0 }, // end of map
};

ULONG
AddMemoryDescriptor(
    IN OUT PFREELDR_MEMORY_DESCRIPTOR List,
    IN ULONG MaxCount,
    IN PFN_NUMBER BasePage,
    IN PFN_NUMBER PageCount,
    IN TYPE_OF_MEMORY MemoryType);

PSU_MEMORY_DESCRIPTOR MemoryDescriptorList;
MACHVTBL MachVtbl;

VOID
PcMachInit(const char *CmdLine)
{
    ExternalServicesTable = ((PBOOT_CONTEXT)CmdLine)->ExternalServicesTable;
    MemoryDescriptorList = ((PBOOT_CONTEXT)CmdLine)->MemoryDescriptorList;
    LoaderPagesSpanned = 0x1000;
    /* Setup vtbl */
    MachVtbl.GetMemoryMap = PcMemGetMemoryMap;
    MachVtbl.HwDetect = PcHwDetect;
    MachVtbl.DiskReadLogicalSectors = DiskReadLogicalSectors;
    MachVtbl.DiskGetBootPath = DiskGetBootPath;
}

PFREELDR_MEMORY_DESCRIPTOR
PcMemGetMemoryMap(ULONG *MemoryMapSize)
{
    ULONG MapCount = 0;
    ULONG RealBaseAddress, RealSize;
    TYPE_OF_MEMORY MemoryType;
    ASSERT(PcBiosMapCount == 0);
    TRACE("GetBiosMemoryMap()\n");

    for (;;) {
        PcBiosMemoryMap[PcBiosMapCount].BaseAddress = MemoryDescriptorList->BlockBase;
        PcBiosMemoryMap[PcBiosMapCount].Length = MemoryDescriptorList->BlockSize;
        PcBiosMemoryMap[PcBiosMapCount].Type = BiosMemoryUsable;
        TRACE("BaseAddress: 0x%llx\n", PcBiosMemoryMap[PcBiosMapCount].BaseAddress);
        TRACE("Length: 0x%llx\n", PcBiosMemoryMap[PcBiosMapCount].Length);
        TRACE("Type: 0x%lx\n", PcBiosMemoryMap[PcBiosMapCount].Type);
        TRACE("Reserved: 0x%lx\n", PcBiosMemoryMap[PcBiosMapCount].Reserved);
        TRACE("\n");

        /* Check if this is free memory */
        if (PcBiosMemoryMap[PcBiosMapCount].Type == BiosMemoryUsable) {
            MemoryType = LoaderFree;
            /* Align up base of memory area */
            RealBaseAddress = PcBiosMemoryMap[PcBiosMapCount].BaseAddress & ~(MM_PAGE_SIZE - 1ULL);
            /* Calculate the length after aligning the base */
            RealSize = PcBiosMemoryMap[PcBiosMapCount].BaseAddress +
                       PcBiosMemoryMap[PcBiosMapCount].Length - RealBaseAddress;
            RealSize = (RealSize + MM_PAGE_SIZE - 1) & ~(MM_PAGE_SIZE - 1ULL);
        } else {
            if (PcBiosMemoryMap[PcBiosMapCount].Type == BiosMemoryReserved)
                MemoryType = LoaderFirmwarePermanent;
            else
                MemoryType = LoaderSpecialMemory;

            /* Align down base of memory area */
            RealBaseAddress = PcBiosMemoryMap[PcBiosMapCount].BaseAddress & ~(MM_PAGE_SIZE - 1ULL);
            /* Calculate the length after aligning the base */
            RealSize = PcBiosMemoryMap[PcBiosMapCount].BaseAddress +
                       PcBiosMemoryMap[PcBiosMapCount].Length - RealBaseAddress;
            RealSize = (RealSize + MM_PAGE_SIZE - 1) & ~(MM_PAGE_SIZE - 1ULL);
        }

        /* Check if we can add this descriptor */
        if ((RealSize >= MM_PAGE_SIZE) && (MapCount < MAX_BIOS_DESCRIPTORS)) {
            /* Add the descriptor */
            MapCount = AddMemoryDescriptor(PcMemoryMap,
                                           MAX_BIOS_DESCRIPTORS,
                                           (PFN_NUMBER)(RealBaseAddress / MM_PAGE_SIZE),
                                           (PFN_NUMBER)(RealSize / MM_PAGE_SIZE),
                                           MemoryType);
        }

        if (!MemoryDescriptorList++->BlockSize)
            break;

        PcBiosMapCount++;
    };

    *MemoryMapSize = MapCount;

    return &PcMemoryMap;
}

/* EOF */
