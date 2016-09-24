#include <windows.h>
#include <tchar.h>
#include <iostream>


ULONG_PTR MiniGetFunctionAddress(ULONG_PTR phModule,char* pProcName )
{
    PIMAGE_DOS_HEADER	pimDH;
    PIMAGE_NT_HEADERS	pimNH;
    PIMAGE_EXPORT_DIRECTORY	pimED;
    ULONG_PTR	pResult	=	0;
    DWORD*	pAddressOfNames;
    WORD*	pAddressOfNameOrdinals;
    DWORD i;
    if (!phModule)
        return	0;
    pimDH	=	(PIMAGE_DOS_HEADER)phModule;
    pimNH	=	(PIMAGE_NT_HEADERS)((char*)phModule+pimDH->e_lfanew);
    pimED	=	(PIMAGE_EXPORT_DIRECTORY)(phModule+pimNH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    if(pimNH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size==0 || (ULONG_PTR)pimED <= phModule)
        return 0;
    if ((ULONG_PTR)pProcName < 0x10000)
    {
        if ((ULONG_PTR)pProcName	>=	pimED->NumberOfFunctions+pimED->Base || (ULONG_PTR)pProcName < pimED->Base)
            return	0;
        pResult	=	phModule+((DWORD*)(phModule+pimED->AddressOfFunctions))[(ULONG_PTR)pProcName-pimED->Base];
    }else
    {
        pAddressOfNames	=	(DWORD*)(phModule+pimED->AddressOfNames);
        for (i=0;i<pimED->NumberOfNames;i++)
        {
            char*	pExportName	=	(char*)(phModule+pAddressOfNames[i]);
            if (strcmp(pProcName,pExportName) == 0)
            {
                pAddressOfNameOrdinals	=	(WORD*)(phModule+pimED->AddressOfNameOrdinals);
                pResult		=	phModule+((DWORD*)(phModule+pimED->AddressOfFunctions))[pAddressOfNameOrdinals[i]];
                break;
            }
        }
    }
    return pResult;
}


void injectedCode()
{

}

void __declspec(naked) executeProc()
{
    __asm{
        pushfd
        pushad
        call injectedCode
        popad
        popfd
        ret
    }
}

typedef struct  reloc_line
{
    WORD	m_addr:12;
    WORD	m_type:4;
}reloc_line;

void LoadVReloc(ULONG_PTR hBase,bool bForce,ULONG_PTR delta)
{
    IMAGE_NT_HEADERS* imNH = (PIMAGE_NT_HEADERS)(hBase+((PIMAGE_DOS_HEADER)hBase)->e_lfanew);
    if (imNH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress == 0)
        return;//没有重定位不处理
    if (hBase == imNH->OptionalHeader.ImageBase && bForce == FALSE)
        return; //如果装入了默认的地址，那么不用处理
    if (delta == 0)
        delta = hBase - imNH->OptionalHeader.ImageBase;
    ULONG_PTR lpreloc = hBase + imNH->OptionalHeader.
            DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;

    IMAGE_BASE_RELOCATION* pimBR = (PIMAGE_BASE_RELOCATION)lpreloc;
    while (pimBR->VirtualAddress != 0)
    {
        reloc_line* reline = (reloc_line*)((char*)pimBR+sizeof(IMAGE_BASE_RELOCATION));
        int preNum = (pimBR->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / 2;
        for (int i=0;i<preNum;i++)
        {
            switch (reline->m_type)
            {
            case IMAGE_REL_BASED_HIGHLOW:
                {
                    *(DWORD*)(hBase+pimBR->VirtualAddress+reline->m_addr) +=delta;
                }break;
            case IMAGE_REL_BASED_DIR64:
                {
                    *(ULONG_PTR*)(hBase+pimBR->VirtualAddress+reline->m_addr) +=delta;
                }break;
            }
            reline++;
        }
        pimBR	=	(PIMAGE_BASE_RELOCATION)reline;
    }
}


PVOID buildRemoteData(HANDLE hProcess)
{
    char* lpImage = (char*)GetModuleHandleA(0);
    PIMAGE_NT_HEADERS imnh = (PIMAGE_NT_HEADERS)(lpImage+((PIMAGE_DOS_HEADER)lpImage)->e_lfanew);
    DWORD szImage = imnh->OptionalHeader.SizeOfImage;
    char* lpData = (char*)VirtualAlloc(0,szImage,MEM_COMMIT,PAGE_READWRITE);
    if (lpData == NULL)
        return NULL;

    PIMAGE_SECTION_HEADER	imsh = (PIMAGE_SECTION_HEADER)((ULONG_PTR)imnh+sizeof(IMAGE_NT_HEADERS));
    DWORD	szHeader = imsh->VirtualAddress;
    for (DWORD i=0;i<imnh->FileHeader.NumberOfSections;i++)
    {
        if (imsh->PointerToRawData != 0)
            szHeader = min(imsh->PointerToRawData,szHeader);
        memcpy(lpData+imsh->VirtualAddress,lpImage+imsh->PointerToRawData,imsh->SizeOfRawData);
        imsh++;
    }
    //将PE头复制回去
    memcpy(lpData,lpImage,szHeader);
    PVOID newBase = VirtualAllocEx(hProcess,0,szImage,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
    ULONG_PTR	delta	= (ULONG_PTR)newBase - (ULONG_PTR)lpImage;
    if (delta != 0)//需要重新定位
        LoadVReloc((ULONG_PTR)lpData,TRUE,delta);
    return (PVOID)((ULONG_PTR)executeProc - (ULONG_PTR)lpImage + (ULONG_PTR)newBase);
}

//PVOID buildRemoteData(DWORD& szData)
//{
//    HMODULE hSelf = GetModuleHandleA(0);
//    PIMAGE_NT_HEADERS imnh = (PIMAGE_NT_HEADERS)((ULONG_PTR)hSelf + ((PIMAGE_DOS_HEADER)hSelf)->e_lfanew);
//    DWORD szImage = imnh->OptionalHeader.SizeOfImage;
//    PVOID lpData = VirtualAlloc(0,szImage,MEM_COMMIT,PAGE_READWRITE);
//    if (!lpData)
//        return 0;


//    return lpData;
//}

int main()
{

    STARTUPINFO	pStartInfo = {0};
    PROCESS_INFORMATION	pProcInfo = {0};
    pStartInfo.cb = sizeof(STARTUPINFO);

    if (!CreateProcess(_T("c:\\windows\\system32\\notepad.exe")
                       ,_T("")
                       ,NULL,NULL,FALSE,CREATE_SUSPENDED,NULL,
                       _T("c:\\windows\\system32\\"),&pStartInfo,&pProcInfo))
    {
        return -1;
    }

    PVOID lpData = buildRemoteData(pProcInfo.hProcess);
    if (!lpData)
        return -2;

    CONTEXT ct;
    BOOL result;
    memset(&ct,0,sizeof(ct));
    ct.ContextFlags = CONTEXT_INTEGER | CONTEXT_CONTROL;
    result = GetThreadContext(pProcInfo.hThread,&ct);
    if (result)
    {
        ct.ContextFlags = CONTEXT_INTEGER | CONTEXT_CONTROL;
        ct.Eax = (ULONG_PTR)lpData;
        result = SetThreadContext(pProcInfo.hThread,&ct);
    }

    ResumeThread(pProcInfo.hThread);

    return 0;
}

