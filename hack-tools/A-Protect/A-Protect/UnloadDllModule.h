#include <Windows.h>
#include <tlhelp32.h>

//-----------------------------------------------------------------------------------------------------------  
// 函数: InjectDll  
// 功能: 向目标进程中注入一个指定 Dll 模块文件.  
// 参数: [in] const TCHAR* ptszDllFile - Dll 文件名及路径  
//       [in] DWORD dwProcessId - 目标进程 ID  
// 返回: bool - 注入成功返回 true, 注入失败则返回 false.  
// 说明: 采用远程线程注入技术实现  
//-----------------------------------------------------------------------------------------------------------  
bool InjectDll(const TCHAR* ptszDllFile, DWORD dwProcessId)  
{  
    // 参数无效  
    if (NULL == ptszDllFile || 0 == _tcslen(ptszDllFile))  
    {  
        return false;  
    }  
    // 指定 Dll 文件不存在  
    if (-1 == _taccess(ptszDllFile, 0))  
    {  
        return false;  
    }  
    HANDLE hProcess = NULL;  
    HANDLE hThread = NULL;  
    DWORD dwSize = 0;  
    TCHAR* ptszRemoteBuf = NULL;  
    LPTHREAD_START_ROUTINE lpThreadFun = NULL;  
    // 获取目标进程句柄  
    hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, dwProcessId);  
    if (NULL == hProcess)  
    {  
        return false;  
    }  
    // 在目标进程中分配内存空间  
    dwSize = (DWORD)_tcslen(ptszDllFile) + 1;  
    ptszRemoteBuf = (TCHAR*)VirtualAllocEx(hProcess, NULL, dwSize * sizeof(TCHAR), MEM_COMMIT, PAGE_READWRITE);  
    if (NULL == ptszRemoteBuf)  
    {  
        CloseHandle(hProcess);  
        return false;  
    }  
    // 在目标进程的内存空间中写入所需参数(模块名)  
    if (FALSE == WriteProcessMemory(hProcess, ptszRemoteBuf, (LPVOID)ptszDllFile, dwSize * sizeof(TCHAR), NULL))  
    {  
        VirtualFreeEx(hProcess, ptszRemoteBuf, dwSize, MEM_DECOMMIT);  
        CloseHandle(hProcess);  
        return false;  
    }  
    // 从 Kernel32.dll 中获取 LoadLibrary 函数地址  
#ifdef _UNICODE  
    lpThreadFun = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(_T("Kernel32")), "LoadLibraryW");  
#else  
    lpThreadFun = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(_T("Kernel32")), "LoadLibraryA");  
#endif  
    if (NULL == lpThreadFun)  
    {  
        VirtualFreeEx(hProcess, ptszRemoteBuf, dwSize, MEM_DECOMMIT);  
        CloseHandle(hProcess);  
        return false;  
    }  
    // 创建远程线程调用 LoadLibrary  
    hThread = CreateRemoteThread(hProcess, NULL, 0, lpThreadFun, ptszRemoteBuf, 0, NULL);  
    if (NULL == hThread)  
    {  
        VirtualFreeEx(hProcess, ptszRemoteBuf, dwSize, MEM_DECOMMIT);  
        CloseHandle(hProcess);  
        return false;  
    }  
    // 等待远程线程结束  
    WaitForSingleObject(hThread, INFINITE);  
    // 清理  
    VirtualFreeEx(hProcess, ptszRemoteBuf, dwSize, MEM_DECOMMIT);  
    CloseHandle(hThread);  
    CloseHandle(hProcess);  
    return true;  
}  
//-----------------------------------------------------------------------------------------------------------  
// 函数: UnInjectDll  
// 功能: 从目标进程中卸载一个指定 Dll 模块文件.  
// 参数: [in] const TCHAR* ptszDllFile - Dll 文件名及路径  
//       [in] DWORD dwProcessId - 目标进程 ID  
// 返回: bool - 卸载成功返回 true, 卸载失败则返回 false.  
// 说明: 采用远程线程注入技术实现  
//-----------------------------------------------------------------------------------------------------------  
bool UnInjectDll(const TCHAR* ptszDllFile, DWORD dwProcessId)  
{  
    // 参数无效  
    if (NULL == ptszDllFile || 0 == _tcslen(ptszDllFile))  
    {  
        return false;  
    }  
    HANDLE hModuleSnap = INVALID_HANDLE_VALUE;  
    HANDLE hProcess = NULL;  
    HANDLE hThread = NULL;  
    // 获取模块快照  
    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);  
    if (INVALID_HANDLE_VALUE == hModuleSnap)  
    {  
        return false;  
    }  
    MODULEENTRY32 me32;  
    memset(&me32, 0, sizeof(MODULEENTRY32));  
    me32.dwSize = sizeof(MODULEENTRY32);  
    // 开始遍历  
    if(FALSE == Module32First(hModuleSnap, &me32))  
    {  
        CloseHandle(hModuleSnap);  
        return false;  
    }  
    // 遍历查找指定模块  
    bool isFound = false;  
    do  
    {  
        isFound = (0 == _tcsicmp(me32.szModule, ptszDllFile) || 0 == _tcsicmp(me32.szExePath, ptszDllFile));  
        if (isFound) // 找到指定模块  
        {  
            break;  
        }  
    } while (TRUE == Module32Next(hModuleSnap, &me32));  
    CloseHandle(hModuleSnap);  
    if (false == isFound)  
    {  
        return false;  
    }  
    // 获取目标进程句柄  
    hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION, FALSE, dwProcessId);  
    if (NULL == hProcess)  
    {  
        return false;  
    }  
    // 从 Kernel32.dll 中获取 FreeLibrary 函数地址  
    LPTHREAD_START_ROUTINE lpThreadFun = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(_T("Kernel32")), "FreeLibrary");  
    if (NULL == lpThreadFun)  
    {  
        CloseHandle(hProcess);  
        return false;  
    }  
    // 创建远程线程调用 FreeLibrary  
    hThread = CreateRemoteThread(hProcess, NULL, 0, lpThreadFun, me32.modBaseAddr /* 模块地址 */, 0, NULL);  
    if (NULL == hThread)  
    {  
        CloseHandle(hProcess);  
        return false;  
    }  
    // 等待远程线程结束  
    WaitForSingleObject(hThread, INFINITE);  
    // 清理  
    CloseHandle(hThread);  
    CloseHandle(hProcess);  
    return true;  
}