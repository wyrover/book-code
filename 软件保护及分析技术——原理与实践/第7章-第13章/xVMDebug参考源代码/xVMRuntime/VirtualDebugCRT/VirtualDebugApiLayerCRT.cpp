#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <map>
#include <winternl.h>

#include "../../../../3rdparty/libudis86/extern.h"
#include "../../../../3rdparty/nedmalloc/nedmalloc.h"
#include "../../../xHooklib/xhook_typedef.h"
#include "../../../xHooklib/xhook_inline.h"
#include "../../../../ncvm/ncasm/ncasm86.h"
#include "SafeLocker.h"
#include "mmCom.h"
#include "../xapihookruntime.h"


#include "mmGhostR3.h"
#include "../../../../nCom/PipeBase.h"
#include "threadidcache.h"
#include "breakpoints.h"

#include "VirtualDebugApiLayerCRT.h"

CPipeBase*	gFuncPipe = NULL;
CPipeBase*	gDBGCPipe = NULL;
OSafeLocker gExceptLocker;

typedef std::map<DWORD,HANDLE> map_thread;
map_thread	gmapthread;

LPZwQueryInformationProcess	gEntryZwQueryInformationProcess = NULL;
LPZwSetInformationProcess		gEntryZwSetInformationProcess = NULL;
LPZwQueryInformationThread		gEntryZwQueryInformationThread = NULL;
LPZwSetInformationThread		gEntryZwSetInformationThread = NULL;
LPZwProtectVirtualMemory		gEntryZwProtectVirtualMemory = NULL;


void*	glpThreadEntry = NULL;
void*   glpProcessExit = NULL;

xvmvdb_cfg gvdbcfg;

DWORD	gdwWorkTID = 0;
HANDLE	ghEvent = NULL;

DWORD	gExceptionTID = 0;
CONTEXT gExceptionContext;

threadidcache	gCacheTIDs;
StepBreakPoints		gCacheBPs;

BOOL MyIsBadReadPtr(void* pPtr,long pszBuf)
{
    return IsBadReadPtr(pPtr,pszBuf);
}

typedef struct _THREAD_BASIC_INFORMATION {
    unsigned long ExitStatus;
    void *TebBaseAddress;
    ULONG UniqueProcessId;
    ULONG UniqueThreadId;
    unsigned long AffinityMask;
    unsigned long BasePriority;
    unsigned long DiffProcessPriority;

} THREAD_BASIC_INFORMATION;

DWORD GetThreadID_Query(HANDLE phandle)
{
    THREAD_BASIC_INFORMATION ptbi;
    ULONG pused = 0;
    LONG plRet = gEntryZwQueryInformationThread(phandle,0,&ptbi,sizeof(ptbi),&pused);
    if (plRet == 0)
        return ptbi.UniqueThreadId;
    return 0;
}

DWORD GetProcessID_Query(HANDLE phandle)
{
    PROCESS_BASIC_INFORMATION ppbi;
    ULONG pused = 0;
    LONG plRet = gEntryZwQueryInformationProcess(phandle,ProcessBasicInformation,&ppbi,sizeof(ppbi),&pused);
    if (plRet == 0)
        return ppbi.UniqueProcessId;
    return 0;
}

HANDLE GetOpenedThread(DWORD ptid)
{
    map_thread::iterator pitr = gmapthread.find(ptid);
    if (pitr == gmapthread.end())
    {
        HANDLE phThread = OpenThread(MAXIMUM_ALLOWED,FALSE,ptid);
        if (phThread != 0)
        {
            gmapthread[ptid] = phThread;
            return phThread;
        }else
            return NULL;
    }
    return pitr->second;
}

BOOL CloseOpenedThread(DWORD ptid,BOOL pcHandle = TRUE)
{
    BOOL pbRet = FALSE;
    map_thread::iterator pitr = gmapthread.find(ptid);
    if (pitr != gmapthread.end())
    {
        if (pcHandle)
            pbRet = CloseHandle(pitr->second);
        else
            pbRet = TRUE;
        gmapthread.erase(pitr);
    }
    return pbRet;
}

void MySetThreadContext(PCONTEXT pSetTo,PCONTEXT pSetFrom)
{
    if (pSetFrom->ContextFlags & CONTEXT_CONTROL)
    {
        pSetTo->Ebp = pSetFrom->Ebp;
        pSetTo->Esp = pSetFrom->Esp;
        pSetTo->Eip = pSetFrom->Eip;
        pSetTo->SegCs = pSetFrom->SegCs;
        pSetTo->EFlags= pSetFrom->EFlags;
        pSetTo->SegSs = pSetFrom->SegSs;
    }
    if (pSetFrom->ContextFlags & CONTEXT_INTEGER)
    {
        pSetTo->Eax = pSetFrom->Eax;
        pSetTo->Ecx = pSetFrom->Ecx;
        pSetTo->Edx = pSetFrom->Edx;
        pSetTo->Ebx = pSetFrom->Ebx;
        pSetTo->Esi = pSetFrom->Esi;
        pSetTo->Edi = pSetFrom->Edi;
    }
    if (pSetFrom->ContextFlags & CONTEXT_SEGMENTS)
    {
        pSetTo->SegGs = pSetFrom->SegGs;
        pSetTo->SegFs = pSetFrom->SegFs;
        pSetTo->SegEs = pSetFrom->SegEs;
        pSetTo->SegDs = pSetFrom->SegDs;
    }
    if (pSetFrom->ContextFlags & CONTEXT_DEBUG_REGISTERS)
    {
        pSetTo->Dr0 = pSetFrom->Dr0;
        pSetTo->Dr1 = pSetFrom->Dr1;
        pSetTo->Dr2 = pSetFrom->Dr2;
        pSetTo->Dr3 = pSetFrom->Dr3;
        pSetTo->Dr6 = pSetFrom->Dr6;
        pSetTo->Dr7 = pSetFrom->Dr7;
    }
    if (pSetFrom->ContextFlags & CONTEXT_FLOATING_POINT)
    {
        pSetTo->FloatSave = pSetFrom->FloatSave;
    }
    if (pSetFrom->ContextFlags & CONTEXT_EXTENDED_REGISTERS)
    {
        memcpy(pSetTo->ExtendedRegisters,pSetFrom->ExtendedRegisters,sizeof(pSetTo->ExtendedRegisters));
    }
}

void GetEventBaseInfo(s_mm_dbg_event* pDbgEvent)
{
    pDbgEvent->m_Invoked = true;
    pDbgEvent->m_cmdid = XVM_VD_DBG_EXCEPTION;
    pDbgEvent->m_dbgEvent.dwProcessId = TEBGetProcessId();
    pDbgEvent->m_dbgEvent.dwThreadId = TEBGetThreadId();
}

void ExceptionCreateProcess()   //模拟进程创建
{
    s_mm_dbg_event ptDbgEvent;
    GetEventBaseInfo(&ptDbgEvent);
    LPDEBUG_EVENT	pDbgEvent = &ptDbgEvent.m_dbgEvent;
    pDbgEvent->dwDebugEventCode = CREATE_PROCESS_DEBUG_EVENT;
    pDbgEvent->u.CreateProcessInfo.dwDebugInfoFileOffset = 0;
    pDbgEvent->u.CreateProcessInfo.fUnicode = 0;
    pDbgEvent->u.CreateProcessInfo.nDebugInfoSize = 0;
    HMODULE phImage = GetModuleHandle(NULL);
    pDbgEvent->u.CreateProcessInfo.lpBaseOfImage = (LPVOID)phImage;
    pDbgEvent->u.CreateProcessInfo.lpStartAddress = (LPTHREAD_START_ROUTINE)pDbgEvent->u.CreateProcessInfo.lpBaseOfImage;
    pDbgEvent->u.CreateProcessInfo.lpThreadLocalBase = NULL;
    pDbgEvent->u.CreateProcessInfo.hProcess = (HANDLE)(GetCurrentProcessId()|HANDLE_PROCESS_MASK);
    pDbgEvent->u.CreateProcessInfo.hThread = (HANDLE)(GetCurrentThreadId()|HANDLE_THREAD_MASK);
    char pBufPath[MAX_PATH];
    GetModuleFileNameA(phImage,pBufPath,sizeof(pBufPath));
    pDbgEvent->u.CreateProcessInfo.lpImageName = (LPVOID)pBufPath;
    gDBGCPipe->fpTransact(&ptDbgEvent,sizeof(ptDbgEvent),&ptDbgEvent,sizeof(ptDbgEvent));
}


void ExceptionProcessExit(DWORD pTID,DWORD pExitCode)
{
    s_mm_dbg_event ptDbgEvent;
    GetEventBaseInfo(&ptDbgEvent);
    ptDbgEvent.m_dbgEvent.dwThreadId = pTID;
    LPDEBUG_EVENT	pDbgEvent = &ptDbgEvent.m_dbgEvent;
    pDbgEvent->dwDebugEventCode = EXIT_PROCESS_DEBUG_EVENT;
    pDbgEvent->u.ExitProcess.dwExitCode = pExitCode;
    gDBGCPipe->fpTransact(&ptDbgEvent,sizeof(ptDbgEvent),&ptDbgEvent,sizeof(ptDbgEvent));

}
// void __stdcall SetCatchProc(void*    pCProc)
// {
// 	__asm
// 	{
// 		mov		eax,fs:[0]
// 		push	pCProc
// 		pop		dword ptr ds:[eax+4]
// 	}
// }
// 
// int __stdcall TrapCatchProc(EXCEPTION_RECORD* pExcept,void* pErr,PCONTEXT pContext,void*)
// {
// 	MMDEBUG_DBGEVENT ptDbgEvent = {0};
// 	GetEventBaseInfo(&ptDbgEvent);
// 	LPDEBUG_EVENT	pDbgEvent = &ptDbgEvent.m_dbgEvent;
// 	pDbgEvent->u.Exception.ExceptionRecord = *pExcept;
// 	gExceptionTID = GetCurrentThreadId();
// 	gExceptionContext = pContext;
// 	pDbgEvent->u.Exception.ExceptionRecord.ExceptionAddress=(void*)((ULONG_PTR)pDbgEvent->u.Exception.ExceptionRecord.ExceptionAddress+2);
// 	pDbgEvent->dwDebugEventCode = EXCEPTION_DEBUG_EVENT;
// 	pDbgEvent->u.Exception.dwFirstChance = 1;
// 	gPipeDebug->TransactNamedPipe(&ptDbgEvent,sizeof(ptDbgEvent),&ptDbgEvent,sizeof(ptDbgEvent));
// 	gExceptionTID = 0;
// 	gExceptionContext = NULL;
// 	return 0;
// }



void ExceptionThreadCreate(DWORD pTID,ULONG_PTR pLocalBase,ULONG_PTR pEntry)
{
    if (pTID == gExceptionTID)
        return;
    s_mm_dbg_event ptDbgEvent;
    GetEventBaseInfo(&ptDbgEvent);
    ptDbgEvent.m_dbgEvent.dwThreadId = pTID;
    LPDEBUG_EVENT	pDbgEvent = &ptDbgEvent.m_dbgEvent;
    pDbgEvent->dwDebugEventCode = CREATE_THREAD_DEBUG_EVENT;
    pDbgEvent->u.CreateThread.hThread = (HANDLE)(pTID | HANDLE_THREAD_MASK);
    pDbgEvent->u.CreateThread.lpThreadLocalBase = NULL;
    pDbgEvent->u.CreateThread.lpStartAddress = (LPTHREAD_START_ROUTINE)pEntry;
    gDBGCPipe->fpTransact(&ptDbgEvent,sizeof(ptDbgEvent),&ptDbgEvent,sizeof(ptDbgEvent));
}

void ExceptionThreadExit(DWORD pTID,DWORD pExitCode)
{
    s_mm_dbg_event ptDbgEvent;
    GetEventBaseInfo(&ptDbgEvent);
    ptDbgEvent.m_dbgEvent.dwThreadId = pTID;
    CloseOpenedThread(pTID);
    LPDEBUG_EVENT	pDbgEvent = &ptDbgEvent.m_dbgEvent;
    pDbgEvent->dwDebugEventCode = EXIT_THREAD_DEBUG_EVENT;
    pDbgEvent->u.ExitThread.dwExitCode = pExitCode;
    gDBGCPipe->fpTransact(&ptDbgEvent,sizeof(ptDbgEvent),&ptDbgEvent,sizeof(ptDbgEvent));
}

void ExceptionLoadDll(HMODULE phModule)
{
    s_mm_dbg_event ptDbgEvent;
    GetEventBaseInfo(&ptDbgEvent);
    LPDEBUG_EVENT	pDbgEvent = &ptDbgEvent.m_dbgEvent;
    pDbgEvent->dwDebugEventCode = LOAD_DLL_DEBUG_EVENT;
    pDbgEvent->u.LoadDll.hFile = NULL;
    pDbgEvent->u.LoadDll.dwDebugInfoFileOffset = 0;
    pDbgEvent->u.LoadDll.fUnicode = 0;
    pDbgEvent->u.LoadDll.nDebugInfoSize = 0;
    char pBufPath[MAX_PATH];
    GetModuleFileNameA(phModule,pBufPath,sizeof(pBufPath));
    pDbgEvent->u.LoadDll.lpImageName = (LPVOID)pBufPath;
    pDbgEvent->u.LoadDll.lpBaseOfDll = (LPVOID)phModule;
    gDBGCPipe->fpTransact(&ptDbgEvent,sizeof(ptDbgEvent),&ptDbgEvent,sizeof(ptDbgEvent));
}

void ExceptionExitProcess()
{
    s_mm_dbg_event ptDbgEvent;
    GetEventBaseInfo(&ptDbgEvent);
    LPDEBUG_EVENT	pDbgEvent = &ptDbgEvent.m_dbgEvent;
    pDbgEvent->dwDebugEventCode = EXIT_PROCESS_DEBUG_EVENT;
    pDbgEvent->u.ExitProcess.dwExitCode = 0;
    gDBGCPipe->fpTransact(&ptDbgEvent,sizeof(ptDbgEvent),&ptDbgEvent,sizeof(ptDbgEvent));
}

void ExceptionSystemBreak()
{
    s_mm_dbg_event ptDbgEvent;
    GetEventBaseInfo(&ptDbgEvent);
    LPDEBUG_EVENT	pDbgEvent = &ptDbgEvent.m_dbgEvent;
    pDbgEvent->dwDebugEventCode = EXCEPTION_DEBUG_EVENT;
    pDbgEvent->u.Exception.dwFirstChance = 1;
    pDbgEvent->u.Exception.ExceptionRecord.ExceptionAddress = sysInt3BreakPoint;
    gDBGCPipe->fpTransact(&ptDbgEvent,sizeof(ptDbgEvent),&ptDbgEvent,sizeof(ptDbgEvent));
}

void fn_dbg_printlog(int paddr,int pcolor,char* pformat,...)
{
    if (!gDBGCPipe)
        return;
    if (!pformat)
        return;
    int pszalo = 1024;
    s_mm_dbg_printlog* ps_dbg_printlog = (s_mm_dbg_printlog*)malloc(pszalo);
    if (!ps_dbg_printlog)
        return;
    int polen1 = 0;
    int polen2= 0;
    va_list parg;
    while (ps_dbg_printlog)
    {
        memset(ps_dbg_printlog,0,pszalo);
        va_start(parg,pformat);
        polen1 = pszalo-sizeof(s_mm_dbg_printlog);
        polen2 = vsnprintf(ps_dbg_printlog->m_strbuf,polen1,pformat,parg);
        va_end(parg);
        if (polen2 == polen1)
        {
            pszalo *= 10;
            ps_dbg_printlog = (s_mm_dbg_printlog*)realloc(ps_dbg_printlog,pszalo);
            continue;
        }
        ps_dbg_printlog->m_cmdid  = XVM_VD_DBG_PRINTLOG;
        ps_dbg_printlog->m_strlen = polen2;
        ps_dbg_printlog->m_addr	  = paddr;
        ps_dbg_printlog->m_color  = pcolor;
        break;
    }
    if (ps_dbg_printlog)
    {
        gDBGCPipe->fpTransact(ps_dbg_printlog,polen2+sizeof(s_mm_dbg_printlog),ps_dbg_printlog,sizeof(s_mm_dbg_printlog));
        free(ps_dbg_printlog);
    }
    return;
}



void SendModuleInfo()   //模拟发送模块事件
{
    HANDLE	phShort	=	CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,GetCurrentProcessId());
    MODULEENTRY32	pModuleInfo;
    pModuleInfo.dwSize	=	sizeof(MODULEENTRY32);
    Module32First(phShort,&pModuleInfo);
    do
    {
        if (pModuleInfo.th32ProcessID	==	GetCurrentProcessId())
        {
            ExceptionLoadDll(pModuleInfo.hModule);
        }
    } while(Module32Next(phShort,&pModuleInfo));
}


void SendThreadInfo()
{
    HANDLE	phShort	=	CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,GetCurrentProcessId());
    THREADENTRY32	pThreadInfo;
    pThreadInfo.dwSize	=	sizeof(THREADENTRY32);
    Thread32First(phShort,&pThreadInfo);
    do
    {
        if (pThreadInfo.th32OwnerProcessID	==	GetCurrentProcessId() && pThreadInfo.th32ThreadID != GetCurrentThreadId())
        {
            if (pThreadInfo.th32ThreadID != GetCurrentThreadId() && !gCacheTIDs.checkid(pThreadInfo.th32ThreadID))
                ExceptionThreadCreate(pThreadInfo.th32ThreadID,pThreadInfo.tpBasePri,pThreadInfo.tpDeltaPri);
        }
    } while(Thread32Next(phShort,&pThreadInfo));
}


void SuspendAnotherThreads(BOOL pSuspend)
{
    static HANDLE gHandSusped[256];
    static int	  gHandSuped = 0;
    if (pSuspend)
    {
        gHandSuped = 0;
        HANDLE	phShort	=	CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,GetCurrentProcessId());
        THREADENTRY32	pThreadInfo;
        pThreadInfo.dwSize	=	sizeof(THREADENTRY32);
        Thread32First(phShort,&pThreadInfo);
        do
        {
            if (gHandSuped>= (sizeof(gHandSusped)/sizeof(HANDLE)))
                break;
            if (pThreadInfo.th32OwnerProcessID	==	GetCurrentProcessId() && pThreadInfo.th32ThreadID != GetCurrentThreadId())
            {
                HANDLE phThread = OpenThread(THREAD_ALL_ACCESS,FALSE,pThreadInfo.th32ThreadID);
                if (phThread)
                {
                    unsigned int pSCount = (unsigned int)SuspendThread(phThread);
                    if (pSCount == 0)
                    {
                        gHandSusped[gHandSuped++] = phThread;
                    }else if (pSCount > 1)
                    {
                        ResumeThread(phThread);
                        CloseHandle(phThread);
                    }else
                        CloseHandle(phThread);
                }
            }
        } while(Thread32Next(phShort,&pThreadInfo));
    }else
    {
        for (int i=0;i<gHandSuped;i++)
            ResumeThread(gHandSusped[i]);
        gHandSuped = 0;
    }

}
HANDLE ghAttachThread = NULL;
DWORD __stdcall WorkThreadAttach(void* pArg)
{
    WaitForSingleObject(ghEvent,INFINITE);
    DebuggerStart();
    //ExceptionSystemBreak();
    sysInt3BreakPoint();
    return 0;
}



int mmDbgConnect(s_mm_dbg_connect* ps_dbg_conn)
{
    gDBGCPipe->fpClose();
    if (gDBGCPipe->fnOpenInst(ps_dbg_conn->m_dbgPipe))
    {
        ps_dbg_conn->m_RetCode	= true;
        gvdbcfg.m_int3cmd		= ps_dbg_conn->m_int3cmd;
        gvdbcfg.m_int3ecode	= ps_dbg_conn->m_int3ecode;
        gvdbcfg.m_stepcmd		= ps_dbg_conn->m_stepcmd;
        gvdbcfg.m_stepecode	= ps_dbg_conn->m_stepecode;
        ;		gFuncPipe->fpWrite(ps_dbg_conn,sizeof(s_mm_dbg_connect));
#ifdef _DEBUG
        printf("A Debugger Connected...\n");
#endif
        SetEvent(ghEvent);
    }else
    {
        ps_dbg_conn->m_RetCode = false;
        gFuncPipe->fpWrite(ps_dbg_conn,sizeof(s_mm_dbg_connect));
    }
    return 0;
}


int mmDbgAttach(s_mm_dbg_connect* st_dbg_conn)
{
    gDBGCPipe->fpClose();
    if (gDBGCPipe->fnOpenInst(st_dbg_conn->m_dbgPipe))
    {
        gvdbcfg.m_int3cmd		= st_dbg_conn->m_int3cmd;
        gvdbcfg.m_int3ecode	= st_dbg_conn->m_int3ecode;
        gvdbcfg.m_stepcmd		= st_dbg_conn->m_stepcmd;
        gvdbcfg.m_stepecode	= st_dbg_conn->m_stepecode;
        ghAttachThread = CreateThread(NULL,0,WorkThreadAttach,NULL,NULL,NULL);
        st_dbg_conn->m_RetCode = true;
        gFuncPipe->fpWrite(st_dbg_conn,sizeof(s_mm_dbg_connect));
#ifdef _DEBUG
        printf("A Debugger Connected...\n");
#endif
        SetEvent(ghEvent);
    }else
    {
        st_dbg_conn->m_RetCode = false;
        gFuncPipe->fpWrite(st_dbg_conn,sizeof(s_mm_dbg_connect));
    }
    return 0;
}
//模拟实现Proxy_ZwReadVirtualMemory
int mmCmdReadMemory(s_mm_dbg_readmemory* st_redmem)
{   //我们先查看目标地址是否可读
    if (!MyIsBadReadPtr(st_redmem->m_lpAddr,st_redmem->m_szRead))
    {
        st_redmem->m_bRet = TRUE;
        gFuncPipe->fpWrite(st_redmem,sizeof(s_mm_dbg_readmemory));
        BOOL beReaded = FALSE;
        if (gvdbcfg.m_stepcmd)
        {   //这里我们过滤一下区域内是否包含我们设定的断点信息
            if (gCacheBPs.CheckVirReadMem((ULONG_PTR)st_redmem->m_lpAddr,st_redmem->m_szRead))
            {
                void* prbuf = (void*)n_malloc(st_redmem->m_szRead);
                if (prbuf)
                {
                    memcpy(prbuf,st_redmem->m_lpAddr,st_redmem->m_szRead);
                    gCacheBPs.VirReadMem(prbuf,(ULONG_PTR)st_redmem->m_lpAddr,st_redmem->m_szRead);
                    gFuncPipe->fpWrite(prbuf,st_redmem->m_szRead);
                    n_free(prbuf);
                    beReaded = TRUE;
                }
            }
        }
        if (!beReaded)  //如果成功我们发送数据
            gFuncPipe->fpWrite(st_redmem->m_lpAddr,st_redmem->m_szRead);
    }
    else
    {
        st_redmem->m_bRet = FALSE;
        gFuncPipe->fpWrite(st_redmem,sizeof(s_mm_dbg_readmemory));
    }
    return 0;
}

int mmCmdGetThreadContext(s_mm_dbg_getcontext* ps_dbg_getcontext)
{
    //printf("【GetThreadContext】->TID:%04X\n",ps_dbg_getcontext.m_ThreadID);
    if (gExceptionTID == ps_dbg_getcontext->m_ThreadID)
    {
        ps_dbg_getcontext->m_bRet = TRUE;
        gFuncPipe->fpWrite(ps_dbg_getcontext,sizeof(s_mm_dbg_getcontext));
        gFuncPipe->fpWrite(&gExceptionContext,sizeof(CONTEXT));
    }
    else
    {
        ps_dbg_getcontext->m_bRet = FALSE;
        HANDLE phThread = GetOpenedThread(ps_dbg_getcontext->m_ThreadID);
        if (phThread != 0)
        {
            CONTEXT pContext = {0};
            if (GetThreadContext(phThread,&pContext))
            {
                ps_dbg_getcontext->m_bRet = TRUE;
                gFuncPipe->fpWrite(ps_dbg_getcontext,sizeof(s_mm_dbg_getcontext));
                gFuncPipe->fpWrite(&pContext,sizeof(CONTEXT));
            }
        }
        if (!ps_dbg_getcontext->m_bRet)
            gFuncPipe->fpWrite(ps_dbg_getcontext,sizeof(s_mm_dbg_getcontext));
    }
    return 0;
}

int mmCmdVirtualProtect(s_mm_dbg_virprotect* ps_dbg_virprotect)
{
    //printf("【VirtualProtectEx】->Addr:%08X size:%d\n",pComHdr.u.VirtualProtect.lpAddress,pComHdr.u.VirtualProtect.dwSize);
    ps_dbg_virprotect->RetCode = gEntryZwProtectVirtualMemory(GetCurrentProcess(),
                                                              &ps_dbg_virprotect->lpAddress,
                                                              &ps_dbg_virprotect->dwSize,
                                                              ps_dbg_virprotect->flNewProtect,
                                                              &ps_dbg_virprotect->lpflOldProtect);
    gFuncPipe->fpWrite(ps_dbg_virprotect,sizeof(s_mm_dbg_virprotect));
    return 0;
}

int mmCmdVirtualQuery(s_mm_dbg_virquery* ps_dbg_virquery)
{
    //printf("【VirtualQueryEx】->Addr:%08X \n",pComHdr.u.VirtualQuery.lpAddress);
    ps_dbg_virquery->RetCode = VirtualQuery(ps_dbg_virquery->lpAddress,&ps_dbg_virquery->lpBuffer,ps_dbg_virquery->dwLength);
    gFuncPipe->fpWrite(ps_dbg_virquery,sizeof(s_mm_dbg_virquery));
    return 0;
}

int mmCmdQueryInfoProcess(s_mm_dbg_queryinfops* ps_dbg_qinfops)
{
    //printf("【ZwQueryInformationProcess】->Class:%08X \n",(ULONG_PTR)ps_dbg_qinfops.ProcessInfoClass);
    ps_dbg_qinfops->RetCode = ERROR_ACCESS_DENIED;
    char* plpBuf = (char*)n_malloc(ps_dbg_qinfops->ProcessInfoBufferLength);
    if (plpBuf)
    {
        ULONG pRetd = 0;
        ps_dbg_qinfops->RetCode = gEntryZwQueryInformationProcess(GetCurrentProcess(),
                                                                  ps_dbg_qinfops->ProcessInfoClass,
                                                                  plpBuf,
                                                                  (ULONG_PTR)ps_dbg_qinfops->ProcessInfoBufferLength,
                                                                  &pRetd);
        if (ps_dbg_qinfops->RetCode == ERROR_SUCCESS)
        {
            ps_dbg_qinfops->ProcessInfoBufferLength = pRetd;
            gFuncPipe->fpWrite(ps_dbg_qinfops,sizeof(s_mm_dbg_queryinfops));
            gFuncPipe->fpWrite(plpBuf,(ULONG_PTR)ps_dbg_qinfops->ProcessInfoBufferLength);
            n_free(plpBuf);
            return 0;
        }
        n_free(plpBuf);
    }
    gFuncPipe->fpWrite(ps_dbg_qinfops,sizeof(s_mm_dbg_queryinfops));
    return 0;
}

int mmCmdQueryInfoThread(s_mm_dbg_queryinfotd* ps_dbg_qinfotd)
{
    //printf("【ZwQueryInformationThread】->ThreadID:%08X \n",(ULONG_PTR)ps_dbg_qinfotd.ThreadInfoClass);
    ps_dbg_qinfotd->RetCode = ERROR_ACCESS_DENIED;
    char* plpBuf = (char*)n_malloc(ps_dbg_qinfotd->ThreadInfoBufferLength);
    if (plpBuf)
    {
        HANDLE phThread = GetOpenedThread((DWORD)ps_dbg_qinfotd->hThread);
        if (phThread)
        {
            ps_dbg_qinfotd->RetCode = gEntryZwQueryInformationThread(phThread,ps_dbg_qinfotd->ThreadInfoClass,plpBuf,ps_dbg_qinfotd->ThreadInfoBufferLength,&ps_dbg_qinfotd->BytesReturned);
            if (ps_dbg_qinfotd->RetCode == ERROR_SUCCESS)
            {
                ps_dbg_qinfotd->ThreadInfoBufferLength = ps_dbg_qinfotd->BytesReturned;
                gFuncPipe->fpWrite(ps_dbg_qinfotd,sizeof(s_mm_dbg_queryinfotd));
                gFuncPipe->fpWrite(plpBuf,(DWORD)ps_dbg_qinfotd->ThreadInfoBufferLength);
				n_free(plpBuf);
                return 0;
            }
        }
        n_free(plpBuf);
    }
    gFuncPipe->fpWrite(ps_dbg_qinfotd,sizeof(s_mm_dbg_queryinfotd));
    return 0;
}

int mmCmdWriteMemory(s_mm_dbg_writemem* ps_dbg_writemem)
{
    if (!IsBadWritePtr((void*)ps_dbg_writemem->BaseAddress,ps_dbg_writemem->BytesToWrite))
    {
        ps_dbg_writemem->BytesWritten = gFuncPipe->fpRead((void*)ps_dbg_writemem->BaseAddress,ps_dbg_writemem->BytesToWrite);
        if (gvdbcfg.m_stepcmd)
            gCacheBPs.VirWriteMem((ULONG_PTR)ps_dbg_writemem->BaseAddress,ps_dbg_writemem->BytesToWrite);
        ps_dbg_writemem->m_RetCode = TRUE;
        gFuncPipe->fpWrite(ps_dbg_writemem,sizeof(s_mm_dbg_writemem));
    }
    else
    {
        ps_dbg_writemem->BytesWritten = 0;
        ps_dbg_writemem->m_RetCode = FALSE;
        void* pvbuf = n_malloc(ps_dbg_writemem->BytesToWrite);
        gFuncPipe->fpRead(pvbuf,ps_dbg_writemem->BytesToWrite);
        n_free(pvbuf);
        gFuncPipe->fpWrite(ps_dbg_writemem,sizeof(s_mm_dbg_writemem));
    }
    return 0;
}

int mmCmdSetThreadContext(s_mm_dbg_setcontext* ps_dbg_setcon)
{
    //printf("【SetThreadContext】->ThreadID:%08X \n",ps_dbg_setcon->m_ThreadID);
    BOOL prret = FALSE;
    if (gExceptionTID == ps_dbg_setcon->m_ThreadID)
    {
        MySetThreadContext(&gExceptionContext,&ps_dbg_setcon->m_context);
        prret = TRUE;
    }
    else
    {
        HANDLE hThread = GetOpenedThread(ps_dbg_setcon->m_ThreadID);
        if (hThread != 0)
        {
            prret = SetThreadContext(hThread,&ps_dbg_setcon->m_context);
        }
    }
    gFuncPipe->fpWrite(&prret,sizeof(prret));
    return 0;
}

int mmCmdSuspendThread(s_mm_dbg_suspendtd* st_dbg_suspendtd)
{
    if (gCacheTIDs.checkid(st_dbg_suspendtd->m_ThreadID))
    {
        st_dbg_suspendtd->m_RetCode = -1;
    }else
    {
        HANDLE hThread = GetOpenedThread(st_dbg_suspendtd->m_ThreadID);
        if (hThread != 0)
        {
            st_dbg_suspendtd->m_RetCode = SuspendThread(hThread);
        }else
            st_dbg_suspendtd->m_RetCode = -1;
    }

    gFuncPipe->fpWrite(st_dbg_suspendtd,sizeof(s_mm_dbg_suspendtd));
    return 0;
}


int mmCmdResumThread(s_mm_dbg_resumtd* ps_dbg_resumtd)
{
    if (gCacheTIDs.checkid(ps_dbg_resumtd->m_ThreadID))
    {
        ps_dbg_resumtd->m_RetCode = -1;
    }else
    {
        HANDLE phThread = GetOpenedThread(ps_dbg_resumtd->m_ThreadID);
        if (phThread != 0)
        {
            ps_dbg_resumtd->m_RetCode = ResumeThread(phThread);
        }else
            ps_dbg_resumtd->m_RetCode = -1;
    }

    gFuncPipe->fpWrite(ps_dbg_resumtd,sizeof(s_mm_dbg_resumtd));
    return 0;
}

int mmCmdContinueG(s_mm_dbg_continueg* ps_dbg_cong)
{
    SetEvent(ghEvent);
    gFuncPipe->fpWrite(ps_dbg_cong,sizeof(s_mm_dbg_continueg));
    return 0;
}

int mmCmdSetInfoThread(s_mm_dbg_setinfotd* st_dbg_setinfotd)
{
#ifdef _DEBUG
    printf("set thread info\n");
#endif
    st_dbg_setinfotd->ThreadInfoBuffer = 0;
    if (st_dbg_setinfotd->ThreadInfoBufferLength > 0)
    {
        st_dbg_setinfotd->ThreadInfoBuffer = n_malloc(st_dbg_setinfotd->ThreadInfoBufferLength);
        if (st_dbg_setinfotd->ThreadInfoBuffer)
            gFuncPipe->fpWrite(st_dbg_setinfotd->ThreadInfoBuffer,st_dbg_setinfotd->ThreadInfoBufferLength);
        else
            st_dbg_setinfotd->ThreadInfoBufferLength = 0;
    }
    HANDLE phThread = GetOpenedThread((DWORD)st_dbg_setinfotd->hThread);
    st_dbg_setinfotd->m_RetCode = gEntryZwSetInformationThread(phThread,st_dbg_setinfotd->ThreadInfoClass,st_dbg_setinfotd->ThreadInfoBuffer,st_dbg_setinfotd->ThreadInfoBufferLength);
    if (st_dbg_setinfotd->ThreadInfoBuffer)
        n_free(st_dbg_setinfotd->ThreadInfoBuffer);
    gFuncPipe->fpWrite(st_dbg_setinfotd,sizeof(s_mm_dbg_setinfotd));
    return 0;
}


int mmCmdTerminateThread(s_mm_dbg_termthread* ps_dbg_termtd)
{
    ps_dbg_termtd->m_RetCode = ERROR_ACCESS_DENIED;
    HANDLE phThread = GetOpenedThread(ps_dbg_termtd->m_ThreadID);
    if (phThread)
    {
        ps_dbg_termtd->m_RetCode = ERROR_SUCCESS;
#ifdef _DEBUG
        printf("not imm\n");
#endif
        TerminateThread(phThread,ps_dbg_termtd->m_dwExitCode);
    }
    gFuncPipe->fpWrite(ps_dbg_termtd,sizeof(ps_dbg_termtd));
    return 0;
}

DWORD fn_mm_getregval(int pudreg,PCONTEXT pcontext)
{
    DWORD pval = 0;
    switch(pudreg)
    {
    case R_EAX:pval = pcontext->Eax;break;
    case R_ECX:pval = pcontext->Ecx;break;
    case R_EDX:pval = pcontext->Edx;break;
    case R_EBX:pval = pcontext->Ebx;break;
    case R_ESP:pval = pcontext->Esp;break;
    case R_EBP:pval = pcontext->Ebp;break;
    case R_ESI:pval = pcontext->Esi;break;
    case R_EDI:pval = pcontext->Edi;break;
    }
    return pval;
}

ULONG_PTR fn_mm_calcoperval(ncasm::x86::inst* ins,int pnop,PCONTEXT pcontext,int pclen)
{
    ULONG_PTR pval = 0;
	ncasm::x86::operand* op = &ins->ops[pnop];
    LONG_PTR plval = ins->imm.sqword;
    switch(op->type)
    {
    case O_REG:
    {
        pval = fn_mm_getregval(op->index,pcontext);
    }break;
    case O_MEM:
    {
        ULONG_PTR pmemptr = (ins->disp?plval:0) + fn_mm_getregval(op->index,pcontext) + (fn_mm_getregval(op->index,pcontext)*(ins->scale>0?ins->scale:1));
        //printf("size:%d oft:%08X val:%08X base:%08X idx:%08X scale:%d mem:%08X\n",poper->size,poper->offset,poper->lval.udword,poper->base,poper->index,poper->scale,pmemptr);
        if (pmemptr > 0)
        {
            if (!MyIsBadReadPtr((void*)pmemptr,sizeof(ULONG_PTR)))
                pval = *(ULONG_PTR*)pmemptr;
        }
    }break;
    case O_PTR:
    case O_IMM:
	case O_IMM1:
	case O_IMM2:
    {
        pval = plval;
    }break;
    case O_PC:
    {
        pval = pcontext->Eip+(signed long)plval+pclen;
    }break;
    }
#ifdef _DEBUG
    //printf("type:%d result:%08X\n",poper->type,pval);
#endif
    return pval;
}
void fn_mm_steping(PCONTEXT pcontext)
{
    char* pcmdbuf = (char*)pcontext->Eip;
    if (MyIsBadReadPtr((void*)pcmdbuf,16))
        return;
    s_e_flags* pseflags = (s_e_flags*)&pcontext->EFlags;
	
	ncasm::x86::code ci;
	ci.code = (const uint8_t*)pcmdbuf;
	ci.codeOffset = (_OffsetType)pcmdbuf;
	ci.codeLen = 16;
	ci.dt = Decode32Bits;
	ncasm::x86::inst ins;
	ins.disp = 0;
	int nded = ncasm::x86::disasm(&ci,&ins,1);
	int pLen = ci.nextOffset - ci.codeOffset;
    ULONG_PTR pNextEip = 0;
    switch(ins.opcode)
    {
    case	I_RET:
    case	I_RETF:
    {
        if (!MyIsBadReadPtr((void*)pcontext->Esp,sizeof(pcontext->Esp)))
            pNextEip = *(ULONG_PTR*)pcontext->Esp;
    }break;
    case	I_JMP:
    case	I_CALL:
    {
        pNextEip = fn_mm_calcoperval(&ins,0,pcontext,pLen);
    }break;
    case I_JB:
        pNextEip = pseflags->m_f_CF?fn_mm_calcoperval(&ins,0,pcontext,pLen):((ULONG_PTR)pcmdbuf + pLen);
        break;
    case I_JAE:
        pNextEip = (!pseflags->m_f_CF)?fn_mm_calcoperval(&ins,0,pcontext,pLen):((ULONG_PTR)pcmdbuf + pLen);
        break;
    case I_JZ:
        pNextEip = pseflags->m_f_ZF?fn_mm_calcoperval(&ins,0,pcontext,pLen):((ULONG_PTR)pcmdbuf + pLen);
        break;
    case I_JNZ:
        pNextEip = (!pseflags->m_f_ZF)?fn_mm_calcoperval(&ins,0,pcontext,pLen):((ULONG_PTR)pcmdbuf + pLen);
        break;
    case I_JS:
        pNextEip = pseflags->m_f_SF?fn_mm_calcoperval(&ins,0,pcontext,pLen):((ULONG_PTR)pcmdbuf + pLen);
        break;
    case I_JNS:
        pNextEip = (!pseflags->m_f_SF)?fn_mm_calcoperval(&ins,0,pcontext,pLen):((ULONG_PTR)pcmdbuf + pLen);
        break;
    case I_JO:
        pNextEip = pseflags->m_f_OF?fn_mm_calcoperval(&ins,0,pcontext,pLen):((ULONG_PTR)pcmdbuf + pLen);
        break;
    case I_JNO:
        pNextEip = (!pseflags->m_f_OF)?fn_mm_calcoperval(&ins,0,pcontext,pLen):((ULONG_PTR)pcmdbuf + pLen);
        break;
    case I_JP:
        pNextEip = (pseflags->m_f_PF)?fn_mm_calcoperval(&ins,0,pcontext,pLen):((ULONG_PTR)pcmdbuf + pLen);
        break;
    case I_JNP:
        pNextEip = (!pseflags->m_f_PF)?fn_mm_calcoperval(&ins,0,pcontext,pLen):((ULONG_PTR)pcmdbuf + pLen);
        break;
    case I_JA:
        pNextEip = (!pseflags->m_f_ZF && !pseflags->m_f_CF)?fn_mm_calcoperval(&ins,0,pcontext,pLen):((ULONG_PTR)pcmdbuf + pLen);
        break;
    case I_JBE:
        pNextEip = (pseflags->m_f_ZF || pseflags->m_f_CF)?fn_mm_calcoperval(&ins,0,pcontext,pLen):((ULONG_PTR)pcmdbuf + pLen);
        break;
    case I_JL:
        pNextEip = (pseflags->m_f_SF != pseflags->m_f_OF)?fn_mm_calcoperval(&ins,0,pcontext,pLen):((ULONG_PTR)pcmdbuf + pLen);
        break;
    case I_JGE:
        pNextEip = (pseflags->m_f_SF == pseflags->m_f_OF)?fn_mm_calcoperval(&ins,0,pcontext,pLen):((ULONG_PTR)pcmdbuf + pLen);
        break;
    case I_JLE:
        pNextEip = ((pseflags->m_f_SF != pseflags->m_f_OF) || pseflags->m_f_OF)?fn_mm_calcoperval(&ins,0,pcontext,pLen):((ULONG_PTR)pcmdbuf + pLen);
        break;
    case I_JG:
        pNextEip = ((pseflags->m_f_SF == pseflags->m_f_OF) || !pseflags->m_f_OF)?fn_mm_calcoperval(&ins,0,pcontext,pLen):((ULONG_PTR)pcmdbuf + pLen);
        break;
    case I_JCXZ:
        pNextEip = (LOWORD(pcontext->Ecx) == 0)?fn_mm_calcoperval(&ins,0,pcontext,pLen):((ULONG_PTR)pcmdbuf + pLen);
        break;
    case I_JRCXZ:
    case I_JECXZ:
        pNextEip = (pcontext->Ecx == 0)?fn_mm_calcoperval(&ins,0,pcontext,pLen):((ULONG_PTR)pcmdbuf + pLen);
        break;
    case I_LOOP:
        pNextEip = (pcontext->Ecx != 1)?fn_mm_calcoperval(&ins,0,pcontext,pLen):((ULONG_PTR)pcmdbuf + pLen);
        break;
    case I_LOOPNZ:
        pNextEip = (!pseflags->m_f_ZF && pcontext->Ecx != 1)?fn_mm_calcoperval(&ins,0,pcontext,pLen):((ULONG_PTR)pcmdbuf + pLen);
        break;
    case I_LOOPZ:
        pNextEip = (pseflags->m_f_ZF && pcontext->Ecx != 1)?fn_mm_calcoperval(&ins,0,pcontext,pLen):((ULONG_PTR)pcmdbuf + pLen);
        break;
    default:
        pNextEip = (ULONG_PTR)pcmdbuf + pLen;
        break;
    }
    if (pNextEip != 0)
    {
        if (gCacheBPs.AddBreakPoint(pNextEip,TEBGetThreadId(),gvdbcfg.m_stepcmd,gvdbcfg.m_stepecode))
        {
            pseflags->m_f_TF = 0;
        }
    }
#ifdef _DEBUG
    //printf("step len:%d t:%d result:%08X\n",pLen,pud.mnemonic,pNextEip);
#endif

}


LPZwContinue gEntryZwContinue = NULL;
LPKiUserExceptionDispatcher gEntryKiUserExceptionDispatcher = NULL;

void __stdcall HookedThreadCreate(ULONG_PTR pEntry)
{
    ExceptionThreadCreate(GetCurrentThreadId(),(ULONG_PTR)getThreadTEB(),pEntry);
}


LPZwTerminateThread gEntryTerminateThread = 0;
LONG WINAPI Proxy_ZwTerminateThread(__in HANDLE hThread,__in DWORD dwExitCode)
{
    DWORD pTID = GetCurrentThreadId();
    if (hThread && hThread != GetCurrentThread())
        pTID = GetThreadID_Query(hThread);
    if (gCacheTIDs.checkid(pTID))
        return ERROR_SUCCESS;
    if (pTID)
        ExceptionThreadExit(pTID,dwExitCode);
    return gEntryTerminateThread(hThread,dwExitCode);
}


void __stdcall HookedProcessExit(HANDLE phProcess,DWORD pdwExitCode)
{
    if (phProcess)
    {
        if (GetProcessID_Query(phProcess) != GetCurrentProcessId())
            return;
    }
    ExceptionProcessExit(GetCurrentThreadId(),pdwExitCode);
}

//转发进程内异常
void* __stdcall Hook_KiUserExceptionDispatcher(PEXCEPTION_RECORD ExceptionRecord, PCONTEXT ctx)
{
    if (gCacheTIDs.checkid(TEBGetThreadId()))	//排除自身线程异常，因为有些异常是我们管道线程产生的我们有能力自己处理
        return gEntryKiUserExceptionDispatcher;
    //这里我们将锁定保证异常事件是一个个被发送并处理的
    gExceptLocker.Lock();
    gExceptionTID = TEBGetThreadId();   //开始构造异常结构数据
    gExceptionContext = *ctx;
    s_mm_dbg_event pstdbg;
    GetEventBaseInfo(&pstdbg);
    LPDEBUG_EVENT	DbgEvent = &pstdbg.m_dbgEvent;
    DbgEvent->dwDebugEventCode = EXCEPTION_DEBUG_EVENT;
    DbgEvent->u.Exception.dwFirstChance = 1;
    DbgEvent->u.Exception.ExceptionRecord = *ExceptionRecord;
    if (gvdbcfg.m_stepcmd && (gvdbcfg.m_stepecode == ExceptionRecord->ExceptionCode))
    {   //针对我们没有使用常用指令来下断点的处理，我们让调试器看起来仍然是正常断点事件
        if (gCacheBPs.CheckDelBreakPoint((ULONG_PTR)ExceptionRecord->ExceptionAddress,gExceptionTID))
            DbgEvent->u.Exception.ExceptionRecord.ExceptionCode = 0x80000004;
    }
    long pszRet = 0;
    //发送异常信息到调试器端并等待调试器端处理
    if (gExceptionContext.Eip == (ULONG_PTR)sysInt3BreakPoint)
    {   //如果是系统断点地址特殊处理
        ULONG_PTR pcpAddr = (ULONG_PTR)(gExceptionContext.Eip);
        //pDbgEvent->u.Exception.ExceptionRecord.ExceptionCode = 0x80000003;
        DbgEvent->u.Exception.ExceptionRecord.ExceptionAddress = (void*)((ULONG_PTR)sysInt3BreakPoint+1);
        gExceptionContext.Eip = (ULONG_PTR)sysInt3BreakPoint+1;
        pszRet = gDBGCPipe->fpTransact(&pstdbg,sizeof(pstdbg),&pstdbg,sizeof(pstdbg));
        gExceptionContext.Eip = pcpAddr;
        gExceptionContext.Eip++;
    }else
        pszRet = gDBGCPipe->fpTransact(&pstdbg,sizeof(pstdbg),&pstdbg,sizeof(pstdbg));
    //如果通信成功
    if (pszRet != 0)
    {
        gExceptionTID = 0;  //如果处理结果是继续程式执行，那么我们恢复现场并做相关工作并继续
        if (pstdbg.m_result == XVM_VD_DBG_CONTINUE)
        {
            *ctx = gExceptionContext;
            s_e_flags peflags;
            peflags.m_eflags = ctx->EFlags;
            if (peflags.m_f_TF && gvdbcfg.m_stepcmd != 0)
            {
                DWORD pTID = TEBGetThreadId();
                gCacheTIDs.pushid(pTID);
                fn_mm_steping(ctx);
                gCacheTIDs.popid(pTID);
            }

            gExceptLocker.Unlock();
            gEntryZwContinue(ctx,FALSE);
        }
    }
    gExceptLocker.Unlock(); //如果调试器不管我们也不管直接让系统的函数去处理好了
    return (void*)gEntryKiUserExceptionDispatcher;
}


BOOL DebuggerStart()    //模拟调试初始化事件
{
    HMODULE hNtdll = GetModuleHandle(_T("NTDLL"));
    //首先我们通过接管进程内的异常处理程式入口，所有异常如果在没有外部调试器或者内核处理之前，都会到这里在分发下去
    if (!gEntryKiUserExceptionDispatcher)
		gEntryKiUserExceptionDispatcher = (LPKiUserExceptionDispatcher)HookOneApi(_T("NTDLL"), "KiUserExceptionDispatcher", Hooked_KiUserExceptionDispatcherEntry, HOOKTYPE_PUSH);
    //接管线程入口函数，所有线程创建都会经过此入口
    if (!glpThreadEntry)
    {
        HMODULE phkr32 = GetModuleHandleA("kernel32.dll");
        void* pbasetnk = (void*)GetProcAddress(phkr32,"BaseThreadInitThunk");
        if (pbasetnk)
            glpThreadEntry = HookCode(pbasetnk,HookedThreadEntry,HOOKTYPE_PUSH,0);
    }
    //接管线程终止进程终止函数，也许并不是所有的线程终止都会到达这里
    //但是既然都到达结束的份上了，也不在乎这点遗漏
    if (!gEntryTerminateThread)
    {
        gEntryTerminateThread = (LPZwTerminateThread)HookOneApi(_T("NTDLL"),"ZwTerminateThread",Proxy_ZwTerminateThread,HOOKTYPE_PUSH);
        void* ptermprocess= (void*)GetProcAddress(hNtdll,"ZwTerminateProcess");
        if (ptermprocess)
            glpProcessExit = (void*)HookCode(ptermprocess,HookedProcessExitEntry,HOOKTYPE_PUSH,0);
    }
    //下面这些模拟都是一次性模拟，也就是在程式初始化时我们要向调试器发送当前程式已经有的信息
    //之后的信息由于我们接管了相关的事件入口，因此会在那些入口处模拟相应事件
    //模拟一个进程创建调试事件
    ExceptionCreateProcess();
    SendModuleInfo();//模拟模块加载事件
    SendThreadInfo();//模拟线程创建事件
    return TRUE;
}

void SetupNativeApis()
{
    HMODULE phNtdll = GetModuleHandle(_T("NTDLL"));
    gEntryZwQueryInformationProcess= (LPZwQueryInformationProcess)GetProcAddress(phNtdll,"ZwQueryInformationProcess");
    gEntryZwSetInformationProcess  = (LPZwSetInformationProcess)GetProcAddress(phNtdll,"ZwSetInformationProcess");
    gEntryZwQueryInformationThread = (LPZwQueryInformationThread)GetProcAddress(phNtdll,"ZwQueryInformationThread");
    gEntryZwSetInformationThread   = (LPZwSetInformationThread)GetProcAddress(phNtdll,"ZwSetInformationThread");
    gEntryZwProtectVirtualMemory   = (LPZwProtectVirtualMemory)GetProcAddress(phNtdll,"ZwProtectVirtualMemory");
    gEntryZwContinue			   = (LPZwContinue)GetProcAddress(phNtdll,"ZwContinue");
}

DWORD CALLBACK fn_func_routine(void* parg)
{
    gCacheTIDs.pushid(TEBGetThreadId());
    gFuncPipe = new CPipeBase();
    TCHAR pPipeName[64];
    _stprintf(pPipeName,_T("\\\\.\\pipe\\xVMDebug%04X"),GetCurrentProcessId());
    while (1)
    {
        gFuncPipe->fpCreateInst(pPipeName);
        gFuncPipe->fpWaitInst();
        try
        {
            s_mm_hdr	cmdHdr;
            while (TRUE)
            {
                int pszRead = gFuncPipe->fpRead(&cmdHdr,sizeof(cmdHdr));
                if (pszRead == 0)
                {
#ifdef _DEBUG
                    printf("[%d]debugger disconnected!\n",GetLastError());
#endif
                    break;
                }
                switch (cmdHdr.m_cmdid)
                {
                case	XVM_VD_DBG_CONNECT:
                {
                    s_mm_dbg_connect ps_dbg_conn;
                    gFuncPipe->fpRead((char*)&ps_dbg_conn+sizeof(cmdHdr),sizeof(ps_dbg_conn)-sizeof(cmdHdr));
                    mmDbgConnect(&ps_dbg_conn);
                }break;
                case	XVM_VD_DBG_ATTACH:
                {
                    s_mm_dbg_connect ps_dbg_conn;
                    gFuncPipe->fpRead((char*)&ps_dbg_conn+sizeof(cmdHdr),sizeof(ps_dbg_conn)-sizeof(cmdHdr));
                    mmDbgAttach(&ps_dbg_conn);
                }break;
                case	XVM_VD_CMD_READMEMORY:
                {
                    s_mm_dbg_readmemory ps_dbg_redmem;
                    gFuncPipe->fpRead((char*)&ps_dbg_redmem+sizeof(cmdHdr),sizeof(ps_dbg_redmem)-sizeof(cmdHdr));
                    mmCmdReadMemory(&ps_dbg_redmem);
                }break;
                case	XVM_VD_CMD_GETTHREADCONTEXT:
                {
                    s_mm_dbg_getcontext ps_dbg_getcontext;
                    gFuncPipe->fpRead((char*)&ps_dbg_getcontext+sizeof(cmdHdr),sizeof(ps_dbg_getcontext)-sizeof(cmdHdr));
                    mmCmdGetThreadContext(&ps_dbg_getcontext);
                }break;
                case	XVM_VD_CMD_VIRTUALPROTECT:
                {
                    s_mm_dbg_virprotect ps_dbg_virprotect;
                    gFuncPipe->fpRead((char*)&ps_dbg_virprotect+sizeof(cmdHdr),sizeof(ps_dbg_virprotect)-sizeof(cmdHdr));
                    mmCmdVirtualProtect(&ps_dbg_virprotect);
                }break;
                case	XVM_VD_CMD_VIRTUALQUERY:
                {
                    s_mm_dbg_virquery ps_dbg_virquery;
                    gFuncPipe->fpRead((char*)&ps_dbg_virquery+sizeof(cmdHdr),sizeof(ps_dbg_virquery)-sizeof(cmdHdr));
                    mmCmdVirtualQuery(&ps_dbg_virquery);
                }break;
                case	XVM_VD_CMD_QUERYINFOPROCESS:
                {
                    s_mm_dbg_queryinfops ps_dbg_qinfops;
                    gFuncPipe->fpRead((char*)&ps_dbg_qinfops+sizeof(cmdHdr),sizeof(ps_dbg_qinfops)-sizeof(cmdHdr));
                    mmCmdQueryInfoProcess(&ps_dbg_qinfops);
                }break;
                case	XVM_VD_CMD_QUERYINFOTHREAD:
                {
                    s_mm_dbg_queryinfotd ps_dbg_qinfotd;
                    gFuncPipe->fpRead((char*)&ps_dbg_qinfotd+sizeof(cmdHdr),sizeof(ps_dbg_qinfotd)-sizeof(cmdHdr));
                    mmCmdQueryInfoThread(&ps_dbg_qinfotd);
                }break;
                case	XVM_VD_CMD_WRITEMEMORY:
                {
                    s_mm_dbg_writemem ps_dbg_writemem;
                    gFuncPipe->fpRead((char*)&ps_dbg_writemem+sizeof(cmdHdr),sizeof(ps_dbg_writemem)-sizeof(cmdHdr));
                    mmCmdWriteMemory(&ps_dbg_writemem);
                }break;
                case	XVM_VD_CMD_SETTHREADCONTEXT:
                {
                    s_mm_dbg_setcontext ps_dbg_setcon;
                    gFuncPipe->fpRead((char*)&ps_dbg_setcon+sizeof(cmdHdr),sizeof(ps_dbg_setcon)-sizeof(cmdHdr));
                    mmCmdSetThreadContext(&ps_dbg_setcon);
                }break;
                case	XVM_VD_CMD_SUSPENDTHREAD:
                {
                    s_mm_dbg_suspendtd ps_dbg_suspendtd;
                    gFuncPipe->fpRead((char*)&ps_dbg_suspendtd+sizeof(cmdHdr),sizeof(ps_dbg_suspendtd)-sizeof(cmdHdr));
                    mmCmdSuspendThread(&ps_dbg_suspendtd);
                }break;
                case	XVM_VD_CMD_RESUMTHREAD:
                {
                    s_mm_dbg_resumtd ps_dbg_resumtd;
                    gFuncPipe->fpRead((char*)&ps_dbg_resumtd+sizeof(cmdHdr),sizeof(ps_dbg_resumtd)-sizeof(cmdHdr));
                    mmCmdResumThread(&ps_dbg_resumtd);
                }break;
                case	XVM_VD_CMD_CONTINUEG:
                {
                    s_mm_dbg_continueg ps_dbg_cong;
                    mmCmdContinueG(&ps_dbg_cong);
                }break;
                case	XVM_VD_CMD_SETINFOTHREAD:
                {
                    s_mm_dbg_setinfotd ps_dbg_setinfotd;
                    gFuncPipe->fpRead((char*)&ps_dbg_setinfotd+sizeof(cmdHdr),sizeof(ps_dbg_setinfotd)-sizeof(cmdHdr));
                    mmCmdSetInfoThread(&ps_dbg_setinfotd);
                }break;

                }
            }
        }
        catch(...){
#ifdef _DEBUG
            printf("should not be here(a pipe recv exception)...\n");
#endif
        }
        gFuncPipe->fpClose();
    }
    gCacheTIDs.popid(TEBGetThreadId());
    return 0;
}

typedef VOID (WINAPI* PENTRY_DebugBreak)(VOID);

//开始建立通信
BOOL StartDebugListing()
{
    if (!gDBGCPipe) //如果管道没有初始化，初始化之并启动一个线程时刻准备着
    {
        gDBGCPipe = new CPipeBase();
        ghEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
        CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)fn_func_routine,0,0,0);
        WaitForSingleObject(ghEvent,INFINITE);
    }
    BOOL pbRet = TRUE;
    pbRet = DebuggerStart();//真正模拟调试初始化事件
    return pbRet;
}

BOOL NativeApiInit()
{
    if (gEntryZwQueryInformationProcess)
        return TRUE;
    SetupNativeApis();
    return TRUE;
}


