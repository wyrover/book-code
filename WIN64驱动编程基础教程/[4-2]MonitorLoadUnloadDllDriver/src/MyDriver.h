#include <ntddk.h>
#include <ntimage.h>

#define dprintf             DbgPrint
#define DEVICE_NAME         L"\\Device\\LoadImageNotifyX64"
#define LINK_NAME           L"\\DosDevices\\LoadImageNotifyX64"
#define LINK_GLOBAL_NAME    L"\\DosDevices\\Global\\LoadImageNotifyX64"

BOOLEAN VxkCopyMemory(PVOID pDestination, PVOID pSourceAddress, SIZE_T SizeOfCopy)
{
    PMDL pMdl = NULL;
    PVOID pSafeAddress = NULL;
    pMdl = IoAllocateMdl(pSourceAddress, (ULONG)SizeOfCopy, FALSE, FALSE, NULL);

    if (!pMdl) return FALSE;

    __try {
        MmProbeAndLockPages(pMdl, KernelMode, IoReadAccess);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        IoFreeMdl(pMdl);
        return FALSE;
    }

    pSafeAddress = MmGetSystemAddressForMdlSafe(pMdl, NormalPagePriority);

    if (!pSafeAddress) return FALSE;

    RtlCopyMemory(pDestination, pSafeAddress, SizeOfCopy);
    MmUnlockPages(pMdl);
    IoFreeMdl(pMdl);
    return TRUE;
}

VOID UnicodeToChar(PUNICODE_STRING dst, char *src)
{
    ANSI_STRING string;
    RtlUnicodeStringToAnsiString(&string, dst, TRUE);
    strcpy(src, string.Buffer);
    RtlFreeAnsiString(&string);
}

void DenyLoadDriver(PVOID DriverEntry)
{
    UCHAR fuck[] = "\xB8\x22\x00\x00\xC0\xC3";
    VxkCopyMemory(DriverEntry, fuck, sizeof(fuck));
}

PVOID GetDriverEntryByImageBase(PVOID ImageBase)
{
    PIMAGE_DOS_HEADER pDOSHeader;
    PIMAGE_NT_HEADERS64 pNTHeader;
    PVOID pEntryPoint;
    pDOSHeader = (PIMAGE_DOS_HEADER)ImageBase;
    pNTHeader = (PIMAGE_NT_HEADERS64)((ULONG64)ImageBase + pDOSHeader->e_lfanew);
    pEntryPoint = (PVOID)((ULONG64)ImageBase + pNTHeader->OptionalHeader.AddressOfEntryPoint);
    return pEntryPoint;
}

VOID LoadImageNotifyRoutine
(
    __in_opt PUNICODE_STRING  FullImageName,
    __in HANDLE  ProcessId,
    __in PIMAGE_INFO  ImageInfo
)
{
    PVOID pDrvEntry;
    char szFullImageName[260] = {0};

    if (FullImageName != NULL && MmIsAddressValid(FullImageName)) {
        if (ProcessId == 0) {
            DbgPrint("[LoadImageNotifyX64]%wZ\n", FullImageName);
            pDrvEntry = GetDriverEntryByImageBase(ImageInfo->ImageBase);
            DbgPrint("[LoadImageNotifyX64]DriverEntry: %p\n", pDrvEntry);
            UnicodeToChar(FullImageName, szFullImageName);

            if (strstr(_strlwr(szFullImageName), "win64ast.sys")) {
                DbgPrint("Deny load [WIN64AST.SYS]");
                //Ω˚÷πº”‘ÿwin64ast.sys
                DenyLoadDriver(pDrvEntry);
            }
        }
    }
}