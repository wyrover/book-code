#ifndef VIRTUALDEBUGAPILAYERCRT_H_
#define VIRTUALDEBUGAPILAYERCRT_H_

#if __cplusplus
extern "C"{
#endif
long	TEBGetThreadId();
long	TEBGetProcessId();
extern void*	glpThreadEntry;
extern void*    glpProcessExit;
extern void*    glpOrgEntry;
extern void __stdcall HookedThreadCreate(ULONG_PTR pEntry);
extern void __stdcall HookedProcessExit(HANDLE phProcess,DWORD pdwExitCode);
extern void HookedThreadEntry();
extern void HookedProcessExitEntry();
extern void sysInt3BreakPoint();
extern void* __stdcall Hook_KiUserExceptionDispatcher(PEXCEPTION_RECORD ExceptionRecord, PCONTEXT Context);
void Hooked_KiUserExceptionDispatcherEntry();
extern void sysBreakBeforeEntry();
#if __cplusplus
}
#endif

typedef struct xvmvdb_cfg{
    unsigned char	m_int3cmd;			//int3替代指令	0xF4	0xC0000096
    unsigned long	m_int3ecode;		//int3 exception code
    unsigned char	m_stepcmd;			//单步替代指令
    unsigned long	m_stepecode;
}xvmvdb_cfg;


BOOL	MyIsBadReadPtr(void* pPtr,long pszBuf);

BOOL    NativeApiInit();
BOOL    StartDebugListing();

BOOL	SetBreakPoint(void* pbpAddr);
BOOL	DeleteBreakPoint(void* pbpAddr);
BOOL	SetApiBreakPointCp(char* pDllName,char* pfnName);

void	ExceptionCreateProcess();
void	SendSystemBreak();
BOOL	DebuggerStart();
BOOL	CreateRecvPipe();


void	fn_dbg_printlog(int paddr,int pcolor,char* pformat,...);


#endif //VIRTUALDEBUGAPILAYERCRT_H_
