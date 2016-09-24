#include <windows.h>
#include <tchar.h>
#include <Psapi.h>

#include "ollydbg2_plugin.h"
#include "xvmdebugframe.h"
#include "xvmiatcallframe.h"
#include "xvmblockframe.h"
#include "resource.h"
#include "xdynamic_ollydbg.h"
#include "xvmdebug.h"

#include "../../xHooklib/xhook_inline.h"
#include "../../xHooklib/xhook_peloader.h"
#include "../xVMRuntime/xvmruntime.h"
#include "../../../nNetLib/npipeclient.h"
#include "../xVMRuntime/TinyCScript/tinyc.h"
#include "../../xHooklib/xhook_pefuncs.h"
#include "../xVMRuntime/TinyCScript/crt/internal/internal.h"
#include "../../xHooklib/xhook_typedef.h"
#include "NativeSysCallID.h"

#include "xApiWatch.h"

extern HANDLE ghDbgProcess;


struct MemblockState{
	DWORD oldPG;
	MEMORY_BASIC_INFORMATION	mbi;
};
typedef index_map<ULONG_PTR,MemblockState> mbs_state;
mbs_state	gMemBks;

BOOL isHookedExecutionAddr(ULONG_PTR exceptaddr)
{
	mbs_state::iterator itr = gMemBks.upper_bound(exceptaddr);
	if (itr == gMemBks.end())
		return FALSE;
	if (itr != gMemBks.begin())
		itr--;
	if (exceptaddr >= (ULONG_PTR)itr->second.mbi.BaseAddress && exceptaddr < ((ULONG_PTR)itr->second.mbi.BaseAddress + (ULONG_PTR)itr->second.mbi.RegionSize))
		return TRUE;
	return FALSE;
}
void restoreAllModuleNoneExec()
{
	for (mbs_state::iterator itr = gMemBks.begin(); itr != gMemBks.end();itr++)
	{
		MemblockState* ms = &itr->second;
		DWORD oldpg;
		VirtualProtectEx(ghDbgProcess,ms->mbi.BaseAddress,ms->mbi.RegionSize,ms->oldPG,&oldpg);
	}
	gMemBks.clear();
}

void setAllModuleNoneExec(ULONG_PTR except)
{
	gMemBks.clear();

	t_module* tm = gLPFindmodule(except);

	MEMORY_BASIC_INFORMATION mbi;
	LPCVOID	 bif = 0;
	int szmbi = 0;
	while ((szmbi = VirtualQueryEx(ghDbgProcess,bif,&mbi,sizeof(mbi))) == sizeof(mbi))
	{
		bif = (LPCVOID)((ULONG_PTR)mbi.BaseAddress + mbi.RegionSize);

		if (!(mbi.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)))
			continue;
		if (mbi.BaseAddress == mbi.AllocationBase)
			continue;
		if (mbi.State & (MEM_RESERVE | MEM_FREE))
			continue;
		if (mbi.Protect & PAGE_GUARD)
			continue;
		if ((except >= (ULONG_PTR)mbi.BaseAddress && except < (ULONG_PTR)bif))
			continue;
		if (tm)
		{ 
			if ((ULONG_PTR)mbi.BaseAddress >= tm->base && (ULONG_PTR)mbi.BaseAddress < (tm->base+tm->size))
				continue;
			if ((ULONG_PTR)bif >= tm->base && (ULONG_PTR)bif < (tm->base + tm->size))
				continue;
		}

		char buf[16];
		int szlen = gLPReadmemory(buf,(ULONG_PTR)mbi.AllocationBase,2,MM_SILENT);
		if (szlen < 2)
			continue;
		if (*(WORD*)buf != IMAGE_DOS_SIGNATURE)
			continue;
		DWORD newpg = 0;
		if (mbi.Protect & PAGE_EXECUTE_WRITECOPY)
			newpg = PAGE_WRITECOPY;
		else if (mbi.Protect & PAGE_EXECUTE_READWRITE)
			newpg = PAGE_READWRITE;
		else if (mbi.Protect & PAGE_EXECUTE_READ)
			newpg = PAGE_READONLY;
		else
			newpg = mbi.Protect & ~PAGE_EXECUTE;
		DWORD oldpg = 0;
		if (!VirtualProtectEx(ghDbgProcess,mbi.BaseAddress,mbi.RegionSize,newpg,&oldpg))
			continue;
		MemblockState mbs;
		mbs.mbi = mbi;
		mbs.oldPG = oldpg;
		gMemBks.insert(std::make_pair((ULONG_PTR)mbi.BaseAddress,mbs));
	}
//#ifdef _DEBUG
//	gLPListmemory();
//#endif
}

BOOL xApi_StepOver()
{
	if (gRuninfo->status != STAT_PAUSED)
		return FALSE;
	t_thread* th = gLPFindthread(gLPGetcputhreadid());
	if (!th) return FALSE;
	setAllModuleNoneExec(th->reg.ip);
	gLPRun(STAT_RUNTHR,0);
	return TRUE;
}

void resetApiWatch()
{
	gMemBks.clear();
}

BOOL xApi_RunToReturn()
{
	if (gRuninfo->status != STAT_PAUSED)
		return FALSE;
	t_thread* th = gLPFindthread(gLPGetcputhreadid());
	if (!th) return FALSE;
	ULONG_PTR retptr;
	ULONG_PTR retaddr = 0;

	retptr = th->reg.r[REG_ESP];
	if (gLPReadmemory(&retaddr,retptr,sizeof(ULONG_PTR),MM_SILENT) > 0)
	{
		t_memory* tm = gLPFindmemory(retaddr);
		if (tm)
		{
			if (tm->access & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))
			{
				gLPSetint3breakpoint(retaddr,BP_ONESHOT | BP_EXEC | BP_BREAK,0,0,0,0,L"",L"",L"");
				gLPRun(STAT_RUNTHR,0);
				return TRUE;
			}
		}
	}
	//first try ebp frame
	retptr = th->reg.r[REG_EBP]+sizeof(ULONG_PTR);
	if (gLPReadmemory(&retaddr,retptr,sizeof(ULONG_PTR),MM_SILENT) > 0)
	{
		t_memory* tm = gLPFindmemory(retaddr);
		if (tm)
		{
			if (tm->access & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))
			{
				gLPSetint3breakpoint(retaddr,BP_ONESHOT | BP_EXEC | BP_BREAK,0,0,0,0,L"",L"",L"");
				gLPRun(STAT_RUNTHR,0);
				return TRUE;
			}
		}
	}

	return TRUE;
}
