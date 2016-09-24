#include "stdafx.h"

//
//将kbdclass.sys文件内容读取到内存中，以后将调用
//GetReadKbdClassDriverEntryAddrFromPE等函数获取原始地址
//
PVOID ReadKbdClassDriverFileData()
{
    NTSTATUS status;
    HANDLE hFile;
    OBJECT_ATTRIBUTES oa;
    UNICODE_STRING usKbdClass;
    IO_STATUS_BLOCK ioStatus;
    PFILE_STANDARD_INFORMATION pFileInfo;
    PVOID pFileData;
    RtlInitUnicodeString(&usKbdClass, L"\\SystemRoot\\System32\\Drivers\\kbdclass.sys");
    InitializeObjectAttributes(&oa, &usKbdClass, OBJ_CASE_INSENSITIVE, NULL, NULL);
    status = ZwCreateFile(
                 &hFile,
                 GENERIC_READ,
                 &oa,
                 &ioStatus,
                 NULL,
                 FILE_ATTRIBUTE_NORMAL,
                 FILE_SHARE_READ,
                 FILE_OPEN,
                 FILE_SYNCHRONOUS_IO_NONALERT,
                 NULL,
                 0
             );

    if (!NT_SUCCESS(status)) {
        KdPrint(("[ReadKbdClassDriverFileData] ZwCreateFile Failure!err:0x%08x\n",
                 status));
        return NULL;
    }

    pFileInfo = (PFILE_STANDARD_INFORMATION)ExAllocatePoolWithTag(
                    NonPagedPool,
                    sizeof(FILE_STANDARD_INFORMATION),
                    'wjl'
                );

    if (NULL == pFileInfo) {
        ZwClose(hFile);
        return NULL;
    }

    //获取文件的大小
    status = ZwQueryInformationFile(
                 hFile,
                 &ioStatus,
                 pFileInfo,
                 sizeof(FILE_STANDARD_INFORMATION),
                 FileStandardInformation
             );

    if (!NT_SUCCESS(status)) {
        KdPrint(("[ReadKbdClassDriverFileData] ZwQueryInformationFile Failure!err:0x%08x\n",
                 status));
        ZwClose(hFile);
        ExFreePoolWithTag(pFileInfo, 'wjl');
        return NULL;
    }

    pFileData = ExAllocatePoolWithTag(
                    PagedPool,
                    pFileInfo->AllocationSize.u.LowPart,
                    'wjl'
                );

    if (NULL == pFileData) {
        KdPrint(("[ReadKbdClassDriverFileData] ExAllocatePoolWithTag Failure!\n"));
        ZwClose(hFile);
        ExFreePoolWithTag(pFileInfo, 'wjl');
        return NULL;
    }

    //读取内容
    status = ZwReadFile(hFile, NULL, NULL, NULL, &ioStatus, pFileData,
                        pFileInfo->AllocationSize.u.LowPart, NULL, NULL);

    if (!NT_SUCCESS(status)) {
        KdPrint(("[ReadKbdClassDriverFileData] ZwReadFile Failure!err:0x%08x\n",
                 status));
        ZwClose(hFile);
        ExFreePoolWithTag(pFileInfo, 'wjl');
        ExFreePoolWithTag(pFileData, 'wjl');
        return NULL ;
    }

    ZwClose(hFile);
    ExFreePoolWithTag(pFileInfo, 'wjl');
    return pFileData;
}

//
//从PE文件中获取DrierEntry的原始地址（即真实地址）
//方法是从GsDriverEntry中搜索JMP DriverEntry
//注意，本函数的前提是驱动的入口点GsDriverEntry地址真实有效
//
boolean GetReadKbdClassDriverEntryAddrFromPE(PContextData Data)
{
    PIMAGE_DOS_HEADER pDosHdr;
    PIMAGE_NT_HEADERS pNtHdr;
    ULONG uGsDriverEntry = 0, uGsDriverEntryRawOffset = 0;
    ULONG uDelta = 0, uCodeLen = 0, uFnLen = 0, uOffset = 0;
    char *pCode;
    ULONG uDriverEntryAddr = 0;
    INSTRUCTION inst;
    int i = 0;
    pDosHdr = (PIMAGE_DOS_HEADER)Data->KbdClassDriverFileData;

    if (IMAGE_DOS_SIGNATURE != pDosHdr->e_magic) {
        return false;
    }

    pNtHdr = (PIMAGE_NT_HEADERS)((ULONG)pDosHdr + pDosHdr->e_lfanew);
    //获取GsDriverEntry的地址，并作简单判断
    uGsDriverEntry = (ULONG)Data->KbdClassDrvObj->DriverInit;

    if (uGsDriverEntry < (ULONG)Data->KbdClassDrvObj->DriverStart
        || uGsDriverEntry > (ULONG)Data->KbdClassDrvObj->DriverStart + Data->KbdClassDrvObj->DriverSize) {
        KdPrint(("[GetReadKbdClassDriverEntryAddrFromPE] GsDriverEntry Hooked!\n"));
        return false;
    }

    //获取GsDriverEntry的文件偏移
    uGsDriverEntryRawOffset = uGsDriverEntry - (ULONG)Data->KbdClassDrvObj->DriverStart;
    uGsDriverEntryRawOffset = RVA2RAW(Data->KbdClassDriverFileData, uGsDriverEntryRawOffset);
    uGsDriverEntry = uGsDriverEntryRawOffset + (ULONG)Data->KbdClassDriverFileData;
    uCodeLen = GetFunctionLen(uGsDriverEntry, 0xe9);
    pCode = (char*)ExAllocatePoolWithTag(PagedPool, uCodeLen, 'wjl');

    if (NULL == pCode) {
        return false;
    }

    RtlCopyMemory(pCode, (PVOID)uGsDriverEntry, uCodeLen);
    //偏移：读入PE文件的内存首地址与PE首选加载地址之差
    uOffset = (ULONG)Data->KbdClassDriverFileData - pNtHdr->OptionalHeader.ImageBase;
    //重定位
    RelocCode(Data->KbdClassDriverFileData,
              (PVOID)((ULONG)Data->KbdClassDrvObj->DriverInit - (ULONG)Data->KbdClassDrvObj->DriverStart),
              (BYTE*)pCode, uCodeLen, uOffset);

    do {
        uCodeLen = get_instruction(&inst, (BYTE*)((ULONG)pCode + uFnLen), MODE_32);

        if (0 == uCodeLen) {
            break;
        }

        //GsDriverEntry函数中第一个0xe9就是JMP DriverEntry
        if (inst.opcode == 0xe9) {
            //获取DriverEntry的文件偏移
            uDriverEntryAddr = (ULONG)Data->KbdClassDrvObj->DriverInit - (ULONG)Data->KbdClassDrvObj->DriverStart + uFnLen
                               + *(ULONG*)((ULONG)pCode + uFnLen + 1) + 5;
            uDriverEntryAddr = RVA2RAW(Data->KbdClassDriverFileData, uDriverEntryAddr);
            Data->KbdClassDriverEntryAddr = uDriverEntryAddr;
            break;
        }

        uFnLen += uCodeLen;
    } while (i++ < 1000);

    ExFreePoolWithTag(pCode, 'wjl');

    if (uDriverEntryAddr == 0) {
        return false;
    }

    return true;
}

//从PE文件中获取KeyboardClassRead的地址
//方法是搜索DriverEntry中的特征：mov dword [esi+0x44]
boolean GetRealKbdClassReadAddrFromPE(PContextData Data)
{
    INSTRUCTION inst;
    PVOID pCode;
    char cInstCode[50] = {0};
    ULONG uFnLen = 0, uCodeLen = 0, uOffset = 0;
    PIMAGE_DOS_HEADER pDosHdr;
    PIMAGE_NT_HEADERS pNtHdr;
    int i = 0;
    pDosHdr = (PIMAGE_DOS_HEADER)Data->KbdClassDriverFileData;

    if (pDosHdr->e_magic != IMAGE_DOS_SIGNATURE) {
        return false;
    }

    pNtHdr = (PIMAGE_NT_HEADERS)((ULONG)pDosHdr + pDosHdr->e_lfanew);

    if (NULL == Data->KbdClassDriverEntryAddr) {
        return false;
    }

    uFnLen = GetFunctionLen((ULONG)Data->KbdClassDriverFileData + Data->KbdClassDriverEntryAddr, 0xc2);
    pCode = ExAllocatePoolWithTag(PagedPool, uFnLen, 'wjl');

    if (NULL == pCode) {
        return false;
    }

    RtlCopyMemory(pCode, (PVOID)((ULONG)Data->KbdClassDriverFileData + Data->KbdClassDriverEntryAddr), uFnLen);
    //偏移：内存中kbdclass驱动的实际加载地址和PE文件的首选加载地址之差
    uOffset = (ULONG)Data->KbdClassDrvObj->DriverStart - pNtHdr->OptionalHeader.ImageBase;
    //重定位
    RelocCode(Data->KbdClassDriverFileData,
              (PVOID)RAW2RVA(Data->KbdClassDriverFileData, Data->KbdClassDriverEntryAddr),
              (BYTE*)pCode, uFnLen, uOffset);
    uFnLen = 0;

    do {
        uCodeLen = get_instruction(&inst, (BYTE*)((ULONG)pCode + uFnLen), MODE_32);

        if (0 == uCodeLen) {
            break;
        }

        get_instruction_string(&inst, FORMAT_INTEL, 0, cInstCode, 50);

        //DriverEntry中出现的mov dword [esi+0x44]后面所跟的即是读请求函数地址
        if (20 == RtlCompareMemory(cInstCode, "mov dword [esi+0x44]", 20)) {
            Data->KbdClassReadAddr = inst.op2.immediate;
            break;
        }

        uFnLen += uCodeLen;
    } while (i++ < 1000);

    ExFreePoolWithTag(pCode, 'wjl');
    return Data->KbdClassReadAddr ? true : false;
}

//
//获取KeyboardClassServiceCallback的真实地址
//方法是先从DriverEntry中搜索KeyboardAddDeviceEx地址，
//再从KeyboardAddDeviceEx中搜索KeyboardClassServiceCallback地址
//
boolean GetKbdServiceCallBackAddrFromPE(PContextData Data)
{
    ULONG uDriverEntryAddr = Data->KbdClassDriverEntryAddr;
    ULONG uKeyboardAddDeviceExAddr = 0, uKeyboardClassServiceCallback = 0;
    PVOID pCode = NULL;
    ULONG uFnLen = 0, uCodeLen = 0, uOffset = 0, uPreviousLen = 0;
    PIMAGE_DOS_HEADER pDosHdr = NULL;
    PIMAGE_NT_HEADERS pNtHdr = NULL;
    INSTRUCTION inst;
    char cInstCode[50] = {0};
    int i = 0;
    pDosHdr = (PIMAGE_DOS_HEADER)Data->KbdClassDriverFileData;

    if (IMAGE_DOS_SIGNATURE != pDosHdr->e_magic) {
        return false;
    }

    pNtHdr = (PIMAGE_NT_HEADERS)((ULONG)pDosHdr + pDosHdr->e_lfanew);
    //
    //1.找到KeyboardAddDeviceEx地址
    //
    uFnLen = GetFunctionLen((ULONG)Data->KbdClassDriverFileData + Data->KbdClassDriverEntryAddr, 0xc2);
    pCode = ExAllocatePoolWithTag(PagedPool, uFnLen, 'wjl');

    if (NULL == pCode) {
        return false;
    }

    RtlCopyMemory(pCode,
                  (PVOID)((ULONG)Data->KbdClassDriverFileData + Data->KbdClassDriverEntryAddr), uFnLen);
    //偏移：分配用来读入KbdClass驱动的内存地址和PE首选加载地址之差
    uOffset = (ULONG)Data->KbdClassDriverFileData - pNtHdr->OptionalHeader.ImageBase;
    RelocCode(Data->KbdClassDriverFileData,
              (PVOID)RAW2RVA(Data->KbdClassDriverFileData, Data->KbdClassDriverEntryAddr),
              (BYTE*)pCode, uFnLen, uOffset);
    uFnLen = 0;

    do {
        uCodeLen = get_instruction(&inst, (BYTE*)((ULONG)pCode + uFnLen), MODE_32);

        if (0 == uCodeLen) {
            break;
        }

        get_instruction_string(&inst, FORMAT_INTEL, 0, cInstCode, 50);

        //如果搜索到"mov [eax+0x28]"，并且下一条指令的opcode是0xe8，
        //那么，就找到了KeyboardAddDeviceEx
        if (14 == RtlCompareMemory(cInstCode, "mov [eax+0x28]", 14)) {
            get_instruction(&inst, (BYTE*)((ULONG)pCode + uFnLen + uCodeLen), MODE_32);

            if (inst.opcode == 0xe8) {
                uKeyboardAddDeviceExAddr = RVA2RAW(
                                               Data->KbdClassDriverFileData,
                                               RAW2RVA(Data->KbdClassDriverFileData, Data->KbdClassDriverEntryAddr) +
                                               uFnLen + uCodeLen + 5 + inst.op1.immediate);
                //ShowDASM((ULONG)Data->KbdClassDriverFileData+uKeyboardAddDeviceExAddr);   输出反汇编代码
                break;
            }
        }

        uFnLen += uCodeLen;
    } while (i++ < 1000);

    ExFreePoolWithTag(pCode, 'wjl');

    if (0 == uKeyboardAddDeviceExAddr) {
        return false;
    }

    //
    //2.找到KeyboardClassServiceCallback地址
    //
    uFnLen = GetFunctionLen((ULONG)Data->KbdClassDriverFileData + uKeyboardAddDeviceExAddr, 0xc2);
    pCode = ExAllocatePoolWithTag(PagedPool, uFnLen, 'wjl');

    if (NULL == pCode) {
        return false;
    }

    RtlCopyMemory(pCode, (PVOID)((ULONG)Data->KbdClassDriverFileData + uKeyboardAddDeviceExAddr), uFnLen);
    //偏移：kbdclass驱动的实际加载首地址与PE首选加载基地址之差
    uOffset = (ULONG)Data->KbdClassDrvObj->DriverStart - pNtHdr->OptionalHeader.ImageBase;
    RelocCode(Data->KbdClassDriverFileData, (PVOID)RAW2RVA(Data->KbdClassDriverFileData, uKeyboardAddDeviceExAddr),
              (BYTE*)pCode, uFnLen, uOffset);
    uFnLen = 0;
    i = 0;

    do {
        uCodeLen = get_instruction(&inst, (BYTE*)((ULONG)pCode + uFnLen), MODE_32);

        if (0 == uCodeLen) {
            break;
        }

        get_instruction_string(&inst, FORMAT_INTEL, 0, cInstCode, 50);

        //如果当前指令是push esi，并且下条指令的opcode是0xe8，那么
        //上条指令中保存有KeyboardClassServiceCallback地址
        if (8 == RtlCompareMemory(cInstCode, "push esi", 8)) {
            get_instruction(&inst, (BYTE*)((ULONG)pCode + uFnLen + uCodeLen), MODE_32);

            if (inst.opcode == 0xe8) {
                get_instruction(&inst, (BYTE*)((ULONG)pCode + uFnLen - uPreviousLen), MODE_32);
                Data->KbdServiceCallbackAddr = inst.op1.immediate;
                break;
            }
        }

        uPreviousLen = uCodeLen;
        uFnLen += uCodeLen;
    } while (i++ < 1000);

    ExFreePoolWithTag(pCode, 'wjl');

    if (0 == Data->KbdServiceCallbackAddr) {
        return false;
    }

    return true;
}

//
//将相对虚拟地址转换为文件偏移
//
ULONG RVA2RAW(PVOID Base, ULONG RVAaddr)
{
    PIMAGE_DOS_HEADER pDosHdr;
    PIMAGE_NT_HEADERS pNtHdr;
    PIMAGE_SECTION_HEADER pSectionHdr;
    int i;
    pDosHdr = (PIMAGE_DOS_HEADER)Base;

    if (IMAGE_DOS_SIGNATURE != pDosHdr->e_magic) {
        KdPrint(("[RVA2RAW] IMAGE_DOS_SIGNATURE X!\n"));
        return false;
    }

    pNtHdr = (PIMAGE_NT_HEADERS)((LONG)pDosHdr + pDosHdr->e_lfanew);
    pSectionHdr = IMAGE_FIRST_SECTION(pNtHdr);

    for (i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++, pSectionHdr++) {
        if (RVAaddr >= pSectionHdr->VirtualAddress
            && RVAaddr < pSectionHdr->VirtualAddress + pSectionHdr->Misc.VirtualSize) {
            return RVAaddr + pSectionHdr->PointerToRawData - pSectionHdr->VirtualAddress;
        }
    }

    return 0;
}

//
//将文件偏移转换为相对虚拟地址
//
ULONG RAW2RVA(PVOID Base, ULONG RAWaddr)
{
    PIMAGE_DOS_HEADER pDosHdr;
    PIMAGE_NT_HEADERS pNtHdr;
    PIMAGE_SECTION_HEADER pSectionHdr;
    int i;
    pDosHdr = (PIMAGE_DOS_HEADER)Base;

    if (IMAGE_DOS_SIGNATURE != pDosHdr->e_magic) {
        KdPrint(("[RAW2RVA] IMAGE_DOS_SIGNATURE X!\n"));
        return false;
    }

    pNtHdr = (PIMAGE_NT_HEADERS)((LONG)pDosHdr + pDosHdr->e_lfanew);
    pSectionHdr = IMAGE_FIRST_SECTION(pNtHdr);

    for (i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++, pSectionHdr++) {
        if (RAWaddr >= pSectionHdr->PointerToRawData
            && RAWaddr < pSectionHdr->PointerToRawData + pSectionHdr->SizeOfRawData) {
            return RAWaddr + pSectionHdr->VirtualAddress - pSectionHdr->PointerToRawData;
        }
    }

    return 0;
}

//
//计算函数的长度
//EndOpcode参数指定了函数的结束指令的opcode(一般都是0xc2,0xc3)
//
ULONG GetFunctionLen(ULONG CodeStartAddr, BYTE EndOpcode)
{
    INSTRUCTION inst;
    ULONG uCodeLen = 0, uFnLen = 0;
    int i = 0;

    while (i++ < 1000) {
        uCodeLen = get_instruction(&inst, (BYTE*)(CodeStartAddr + uFnLen), MODE_32);

        if (uCodeLen == 0) {
            break;
        }

        uFnLen += uCodeLen;

        if (inst.opcode == EndOpcode) {
            break;
        }
    }

    return uFnLen;
}

//
//重定位函数
//利用PE文件的重定位数据信息进行重定位
//
boolean RelocCode(PVOID Base, PVOID CodeRVA, BYTE* Code, ULONG CodeLen, ULONG Offset)
{
    PIMAGE_BASE_RELOCATION pReloc;
    PIMAGE_DOS_HEADER pDosHdr;
    PIMAGE_NT_HEADERS pNTHdr;
    ULONG uNumItem = 0, i = 0, uRelocEntry = 0;
    PUSHORT pItem;
    pDosHdr = (PIMAGE_DOS_HEADER)Base;

    if (IMAGE_DOS_SIGNATURE != pDosHdr->e_magic) {
        return false;
    }

    pNTHdr = (PIMAGE_NT_HEADERS)((ULONG)pDosHdr + pDosHdr->e_lfanew);

    if (IMAGE_NT_SIGNATURE != pNTHdr->Signature) {
        return false;
    }

    //定位重定位表在分配的内存中的地址
    pReloc = (PIMAGE_BASE_RELOCATION)((ULONG)pDosHdr +
                                      RVA2RAW(
                                          Base,
                                          pNTHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress)
                                     );

    if ((ULONG)pReloc == (ULONG)pDosHdr) {
        return false;
    }

    //遍历所有重定位块
    //注意，每个重定位块大小可能不一样，但都负责1k大小虚拟地址范围的重定位工作，
    //并且重定位块按照负责的工作范围基地址从小到大排序
    while (pReloc->SizeOfBlock) {
        if (pReloc->VirtualAddress < (ULONG)CodeRVA) {
            //如果该重定位块负责的重定位范围完全在待重定位地址范围以下，那么判断下一个重定位块
            if (pReloc->VirtualAddress + 0x1000 < (ULONG)CodeRVA) {
                pReloc = (PIMAGE_BASE_RELOCATION)((ULONG)pReloc + pReloc->SizeOfBlock);
                continue;
            }
        } else {
            //如果该重定位块负责的重定位范围完全在待重定位地址范围以上，说明再也不会有重定位块会负责待重定位
            //地址范围内的重定位工作了，结束。
            if (pReloc->VirtualAddress > (ULONG)CodeRVA + CodeLen) {
                break;
            }
        }

        //
        //找到需要重定位的地址，使用Offset重定位
        //每个重定位项是2个字节，低12位即是待重定位的地址（需要加上重定位块的基地址）
        uNumItem = (pReloc->SizeOfBlock - 8) / 2;
        pItem = (PUSHORT)((ULONG)pReloc + 8);

        for (i = 0; i < uNumItem; ++i) {
            if ((pItem[i] & 0xf000) == 0x3000) {
                if ((pItem[i] & 0x0fff) + pReloc->VirtualAddress >= (ULONG)CodeRVA
                    && (pItem[i] & 0x0fff) + pReloc->VirtualAddress <= (ULONG)CodeRVA + CodeLen) {
                    uRelocEntry = (ULONG)Code + (pItem[i] & 0x0fff) + pReloc->VirtualAddress - (ULONG)CodeRVA;
                    *(ULONG*)uRelocEntry += Offset;
                }
            }
        }

        pReloc = (PIMAGE_BASE_RELOCATION)((ULONG)pReloc + pReloc->SizeOfBlock);
    }

    return true;
}

//
//输出反汇编信息
//
VOID ShowDASM(ULONG CodeAddr)
{
    int i = 0;
    INSTRUCTION inst;
    ULONG uFnLen = 0, uCodeLen = 0;
    char Code[50] = {0};

    while (++i <= 1000) {
        uCodeLen = get_instruction(&inst, (BYTE*)(CodeAddr + uFnLen), MODE_32);

        if (0 == uCodeLen)
            break;

        get_instruction_string(&inst, FORMAT_INTEL, 0, Code, 50);
        KdPrint(("%s\n", Code));

        if (inst.opcode == 0xc2 || inst.opcode == 0xc3)
            break;

        uFnLen += uCodeLen;
    }

    return ;
}



