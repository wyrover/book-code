#include <windows.h>
#include <map>
#include "xpedumper.h"
#include "../../xHooklib/xhook_pefuncs.h"
#include "xapihookruntime.h"
xpedumper::xpedumper()
{
}

xpedumper::~xpedumper()
{

}

int xpedumper::InitDumpFromMemory(HMODULE hBase, PVOID toAddress)
{
    m_nla.setempty(true);
    if (!hBase)
        return -1;
    MEMORY_BASIC_INFORMATION mbi = {0};
    BYTE* lpMem = (BYTE*)hBase;
    if (!IsValidPE((const char*)lpMem))
        return -2;
    //我们根据内存块来dump
    SIZE_T szt = VirtualQuery(lpMem,&mbi,sizeof(mbi));
    while(szt)
    {
        if (lpMem >= (BYTE*)toAddress)
            break;
        int szbk = (int)mbi.RegionSize;
        int szraw = szbk;
        if (!IsBadReadPtr(lpMem,1))
        {
            //尝试是否能够优化区段大小
            for (signed int pos = szbk;pos > 0;pos--)
            {
                if (lpMem[pos-1] != 0)
                {
                    szraw = pos;
                    break;
                }
            }
            //如果是第一个内存块，尝试识别PE头
            if (lpMem == (BYTE*)hBase)//PE Header
            {
                m_nla.append((const char*)lpMem,mbi.RegionSize);
                PIMAGE_NT_HEADERS imnh = EnterImageNtHeader((const char*)m_nla.data());
                imnh->FileHeader.NumberOfSections = 0;
                imnh->OptionalHeader.SizeOfImage = mbi.RegionSize;
                imnh->OptionalHeader.ImageBase = (ULONG_PTR)hBase;
                imnh->OptionalHeader.AddressOfEntryPoint = (ULONG_PTR)snapshotEntry-(ULONG_PTR)hBase;
            }else
            {
                //其它都以区段形式dump
                AppendSection(lpMem,szraw,(ULONG_PTR)mbi.BaseAddress-(ULONG_PTR)hBase,mbi.RegionSize,FALSE);
            }
        }
        lpMem=(BYTE*)mbi.BaseAddress+mbi.RegionSize;
        szt=VirtualQuery(lpMem,&mbi,sizeof(mbi));
    }
    return 0;
}

int xpedumper::SaveToFile(const TCHAR *filename)
{
    _tprintf(_T("Save to file:%s\n"),filename);
    FILE* fp = _tfopen(filename,_T("wb"));
    if (!fp)
        return -1;
    fwrite(m_nla.data(),m_nla.size(),1,fp);
    fclose(fp);
    return 0;
}

int xpedumper::AppendSection(BYTE *lpMem, int rawSize, DWORD VirtualAddr, DWORD VirtualSize, BOOL placeHold)
{
    PIMAGE_SECTION_HEADER sechdr;
    PIMAGE_NT_HEADERS imnh = EnterImageNtHeader(m_nla.data());
    m_nla.alignTo(imnh->OptionalHeader.FileAlignment);
    imnh = EnterImageNtHeader(m_nla.data());
    if (imnh->FileHeader.NumberOfSections > 0)
    {
        sechdr = EnterImageFirstSection(m_nla.data())+imnh->FileHeader.NumberOfSections - 1;
        sechdr->Misc.VirtualSize = VirtualAddr - sechdr->VirtualAddress;
        if (rawSize == 0)
        {
            sechdr->Misc.VirtualSize += VirtualSize;
            return 1;
        }
    }

    sechdr = EnterImageFirstSection(m_nla.data()) + imnh->FileHeader.NumberOfSections;
    sechdr->VirtualAddress = VirtualAddr;
    sechdr->Misc.VirtualSize = VirtualSize;
    sechdr->SizeOfRawData = rawSize;
    sechdr->PointerToRawData = m_nla.size();
    sechdr->Characteristics = 0xE0000040;
    if (imnh->FileHeader.NumberOfSections > 5)
        memset(sechdr->Name,0,sizeof(sechdr->Name));
    imnh->FileHeader.NumberOfSections++;
    imnh->OptionalHeader.SizeOfImage = sechdr->VirtualAddress + AdjustRva(sechdr->Misc.VirtualSize,imnh->OptionalHeader.SectionAlignment);


    m_nla.append((const char*)lpMem,rawSize);



    return 0;
}




//LPNtOpenSection gNtOpenSection = 0;
//LPNtCreateSection gNtCreateSection = 0;
//LPNtMapViewOfSection gNtMapViewOfSection = 0;
//LPRtlEqualUnicodeString gRtlEqualUnicodeString = 0;

//NTSTATUS NTAPI Proxy_NtMapViewOfSection(
//        IN HANDLE hSection,
//        IN HANDLE hProcess,
//        IN OUT PVOID *BaseAddress,
//        IN ULONG ZeroBits,
//        IN ULONG CommitSize,
//        IN OUT PLARGE_INTEGER SectionOffset OPTIONAL,
//        IN OUT PULONG ViewSize,
//        IN SECTION_INHERIT InheritDisposition,
//        IN ULONG AllocationType,
//        IN ULONG Protect
//        )
//{

//    return gNtMapViewOfSection(hSection,hProcess,BaseAddress,ZeroBits,CommitSize,SectionOffset,
//                               ViewSize,InheritDisposition,AllocationType,Protect);
//}

//NTSTATUS NTAPI Proxy_LPNtOpenSection(
//        OUT PHANDLE phSection,
//        IN ACCESS_MASK DesiredAccess,
//        IN POBJECT_ATTRIBUTES ObjectAttributes
//        )
//{

//    if (ObjectAttributes && ObjectAttributes->ObjectName && ObjectAttributes->ObjectName->Buffer)
//    {
//        if (sys_wcsicmp((const short*)ObjectAttributes->ObjectName->Buffer,(const short*)L"user32.dll") == 0
//                )
//        {
//            static BOOL once = FALSE;
//            if (!once)
//            {
//                once = TRUE;
//                __asm jmp $
//                        return 0xC0000034;
//            }

//        }
//    }
//    //gConsole->PrintfA("OpenSection:%ws\n", ObjectAttributes->ObjectName->Buffer);
//    return gNtOpenSection(phSection,DesiredAccess,ObjectAttributes);
//}


//NTSTATUS NTAPI Proxy_NtCreateSection(
//        OUT PHANDLE phSection,
//        IN ACCESS_MASK DesiredAccess,
//        IN POBJECT_ATTRIBUTES ObjectAttributes,
//        IN PLARGE_INTEGER MaximumSize OPTIONAL,
//        IN ULONG SectionPageProtection,
//        IN ULONG AllocationAttributes,
//        IN HANDLE hFile OPTIONAL
//        )
//{
//    //        if (ObjectAttributes && ObjectAttributes->ObjectName && ObjectAttributes->ObjectName->Buffer)
//    //        {
//    //            if (sys_wcsicmp((const short*)ObjectAttributes->ObjectName->Buffer,(const short*)L"user32.dll") == 0)
//    //            {


//    //            }
//    //        }
//    return gNtCreateSection(phSection,DesiredAccess,ObjectAttributes,MaximumSize,
//                            SectionPageProtection,AllocationAttributes,hFile);
//}



//void EnableModuleRebase(HMODULE hNtdll)
//{
//    PVOID baseAddr = 0;
//    LPNtProtectVirtualMemory lpNtProtectVirtualMemory = (LPNtProtectVirtualMemory)GetFunctionAddress((ULONG_PTR)hNtdll,"NtProtectVirtualMemory");
//    if (lpNtProtectVirtualMemory)
//    {
//        void* lpFunc = (void*)GetFunctionAddress((ULONG_PTR)hNtdll,"NtOpenSection");
//        if (lpFunc)
//        {
//            baseAddr = lpFunc;
//            ULONG szrg = 16;
//            ULONG odpg = 0;
//            if (lpNtProtectVirtualMemory((HANDLE)-1,&baseAddr,&szrg,PAGE_EXECUTE_READWRITE,&odpg) == 0)
//            {
//                gNtOpenSection = (LPNtOpenSection)HookCodeDirect(0,lpFunc,Proxy_LPNtOpenSection,HOOKTYPE_PUSH,0);
//                lpNtProtectVirtualMemory((HANDLE)-1,&baseAddr,&szrg,odpg,&odpg);
//            }
//        }
//        lpFunc = (void*)GetFunctionAddress((ULONG_PTR)hNtdll,"NtCreateSection");
//        if (lpFunc)
//        {
//            baseAddr = lpFunc;
//            ULONG szrg = 16;
//            ULONG odpg = 0;
//            if (lpNtProtectVirtualMemory((HANDLE)-1,&baseAddr,&szrg,PAGE_EXECUTE_READWRITE,&odpg) == 0)
//            {
//                gNtCreateSection = (LPNtCreateSection)HookCodeDirect(0,lpFunc,Proxy_NtCreateSection,HOOKTYPE_PUSH,0);
//                lpNtProtectVirtualMemory((HANDLE)-1,&baseAddr,&szrg,odpg,&odpg);
//            }
//        }

//        lpFunc = (void*)GetFunctionAddress((ULONG_PTR)hNtdll,"NtMapViewOfSection");
//        if (lpFunc)
//        {
//            baseAddr = lpFunc;
//            ULONG szrg = 16;
//            ULONG odpg = 0;
//            if (lpNtProtectVirtualMemory((HANDLE)-1,&baseAddr,&szrg,PAGE_EXECUTE_READWRITE,&odpg) == 0)
//            {
//                gNtMapViewOfSection = (LPNtMapViewOfSection)HookCodeDirect(0,lpFunc,Proxy_NtMapViewOfSection,HOOKTYPE_PUSH,0);
//                lpNtProtectVirtualMemory((HANDLE)-1,&baseAddr,&szrg,odpg,&odpg);
//            }
//        }


//    }

//}

