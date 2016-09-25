/*
 * PROJECT:         ReactOS Boot Loader
 * LICENSE:         BSD - See COPYING.ARM in the top level directory
 * FILE:            boot/freeldr/freeldr/disk/ramdisk.c
 * PURPOSE:         Implements routines to support booting from a RAM Disk
 * PROGRAMMERS:     ReactOS Portable Systems Group
 *                  Herv?Poussineau
 */

/* INCLUDES *******************************************************************/
#include <ntoskrnl.h>
#include <freeldr.h>
#define NDEBUG
#include <debug.h>

#include "..\osloader.h"

LOAD_BOOT_FILE g_FileList[64] = {0};
ULONG g_DeviceId = 0;

void LdfsReadSector(IN ULONG SectorStart, IN ULONG SectorCount, IN PUCHAR Buffer)
{
    ULONG Count = 0;
    LARGE_INTEGER Position = {0};
    Position.LowPart = (SectorStart - 1) * SECTOR_SIZE;
    ArcSeek(g_DeviceId, &Position, SeekAbsolute);
    ArcRead(g_DeviceId, Buffer, SectorCount * SECTOR_SIZE, &Count);
}

BOOL
LdfsReadDisk(
    IN ULONG DeviceId,
    IN ULONG Start,
    IN ULONG Size,
    IN ULONG Buffer
)
{
    BOOL ret = FALSE;
    PUCHAR SectorBuffer;
    ULONG SectorCount;
    ULONG SectorLoop;
    ULONG SectorLeft;
    ULONG ReadOffset = 0;
    ULONG ReadCount = 0;
    ULONG i = 0;
    ULONG SectorStart       = Start / SECTOR_SIZE;
    ULONG SectorStartOffset = Start % SECTOR_SIZE;
    ULONG SectorEnd         = (Start + Size) / SECTOR_SIZE;
    ULONG SectorEndOffset   = (Start + Size) % SECTOR_SIZE;
    PVOID Block = MmHeapAlloc(READ_SECTOR * SECTOR_SIZE);

    if (!Block) {
        return FALSE;
    }

    memset(Block, 0, READ_SECTOR * SECTOR_SIZE);
    SectorBuffer = Block;

    if (SectorStartOffset) {
        LdfsReadSector(SectorStart, 1, SectorBuffer);
        //ExternalServicesTable->ReadWriteSector(DeviceId, SectorStart, 0, 1, SectorBuffer, 0x42);
        ReadOffset   = (ULONG)SectorBuffer + SectorStartOffset;

        if (SectorEnd == SectorStart) {
            ReadCount    = SectorEndOffset - SectorStartOffset;
            RtlCopyMemory((void*)Buffer, (const void*)ReadOffset, ReadCount);
            MmHeapFree(Block);
            return TRUE;
        }

        ReadCount = SECTOR_SIZE - SectorStartOffset;
        RtlCopyMemory((void*)Buffer, (const void*)ReadOffset, ReadCount);
        Buffer += ReadCount;
        SectorStart += 1;
    }

    SectorCount = SectorEnd - SectorStart;
    SectorLoop  = SectorCount / READ_SECTOR;
    SectorLeft  = SectorCount % READ_SECTOR;

    for (i = 0; i < SectorLoop; i++) {
        LdfsReadSector(SectorStart, READ_SECTOR, SectorBuffer);
        //ExternalServicesTable->ReadWriteSector(DeviceId, SectorStart, 0, READ_SECTOR, SectorBuffer, 0x42);
        ReadOffset   = (ULONG)SectorBuffer;
        ReadCount    = READ_SECTOR * SECTOR_SIZE;
        RtlCopyMemory((void*)Buffer, (const void*)ReadOffset, ReadCount);
        Buffer += ReadCount;
        SectorStart += READ_SECTOR;
    }

    if (SectorLeft) {
        LdfsReadSector(SectorStart, SectorLeft, SectorBuffer);
        //ExternalServicesTable->ReadWriteSector(DeviceId, SectorStart, 0, SectorLeft, SectorBuffer, 0x42);
        ReadOffset   = (ULONG)SectorBuffer;
        ReadCount    = SectorLeft * SECTOR_SIZE;
        RtlCopyMemory((void*)Buffer, (const void*)ReadOffset, ReadCount);
        Buffer += ReadCount;
        SectorStart += SectorLeft;
    }

    if (SectorEndOffset && (SectorEnd == SectorStart)) {
        LdfsReadSector(SectorStart, 1, SectorBuffer);
        //ExternalServicesTable->ReadWriteSector(DeviceId, SectorStart, 0, 1, SectorBuffer, 0x42);
        ReadOffset   = (ULONG)SectorBuffer;
        ReadCount    = SectorEndOffset;
        RtlCopyMemory((void*)Buffer, (const void*)ReadOffset, ReadCount);
        Buffer += ReadCount;
        SectorStart += 1;
    }

    MmHeapFree(Block);
    return TRUE;
}

/* GLOBALS ********************************************************************/

PVOID gLdfsBase;
ULONG gLdfsSize;
ULONG gLdfsOffset;

/* FUNCTIONS ******************************************************************/

static LONG LdfsClose(ULONG FileId)
{
    gLdfsBase = 0;
    gLdfsSize = 0;
    gLdfsOffset = 0;
    return ESUCCESS;
}

static LONG LdfsGetFileInformation(ULONG FileId, FILEINFORMATION* Information)
{
    Information->StartingAddress.LowPart = gLdfsBase;
    Information->EndingAddress.LowPart = gLdfsSize;
    Information->CurrentAddress.LowPart = gLdfsOffset;
    return ESUCCESS;
}

static LONG LdfsOpen(CHAR* Path, OPENMODE OpenMode, ULONG* FileId)
{
    int i;

    for (i = 0; i < 64; i++) {
        char* p = strrchr(Path, '\\');

        if (p)
            p += 1;
        else
            p = Path;

        if (stricmp(g_FileList[i].FileName, p) == 0) {
            gLdfsBase = g_FileList[i].FileStart;
            gLdfsSize = g_FileList[i].FileSize;
            gLdfsOffset = 0;
            //*FileId = i;
            break;
        }
    }

    return ESUCCESS;
}

static LONG LdfsRead(ULONG FileId, VOID* Buffer, ULONG N, ULONG* Count)
{
    PVOID StartAddress;
    //
    // Get actual pointer
    //
    StartAddress = (PVOID)((ULONG_PTR)gLdfsBase + gLdfsOffset);

    //
    // Don't allow reads past our image
    //
    if (gLdfsOffset + N > gLdfsSize) {
        *Count = 0;
        return EIO;
    }

    //
    // Do the read
    //
    *Count = N;
    LdfsReadDisk(0x80, StartAddress, N, Buffer);
    return ESUCCESS;
}

static LONG LdfsSeek(ULONG FileId, LARGE_INTEGER* Position, SEEKMODE SeekMode)
{
    //
    // Only accept absolute mode now
    //
    if (SeekMode != SeekAbsolute)
        return EINVAL;

    //
    // Check if we're in the ramdisk
    //
    if (Position->HighPart != 0)
        return EINVAL;

    if (Position->LowPart >= gLdfsSize)
        return EINVAL;

    //
    // OK, remember seek position
    //
    gLdfsOffset = Position->LowPart;
    return ESUCCESS;
}

const DEVVTBL LdfsFuncTable = {
    LdfsClose,
    LdfsGetFileInformation,
    LdfsOpen,
    LdfsRead,
    LdfsSeek,
    L"ldfs",
};

const DEVVTBL* LdfsMount(ULONG DeviceId)
{
    g_DeviceId = DeviceId;
    LdfsReadSector(2, 4, &g_FileList);
    return &LdfsFuncTable;
}