// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "mmGhostR3.h"
#include "WorkCore.h"

//#define _ONLYDLLSTUB
void* gMyOEPJmpBack = NULL;
void* glpBufSelf = NULL;

void __stdcall MyOEPJmpBack()
{
	UnHookCode(gMyOEPJmpBack);
	OEPEntry(glpBufSelf);
}
__declspec(naked) void MyOEPJmp()
{
	__asm
	{
		pushfd
		pushad
		call MyOEPJmpBack
		popad
		popfd
		jmp gMyOEPJmpBack
	}
}

DWORD CALLBACK ThreadSetupRun(void*)
{
	OEPEntry(0);
	return 0;
}
#ifndef _DEBUG
#define _USEVIRTUALSELF
#endif
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			//DisableThreadLibraryCalls(hModule);
#ifdef _USEVIRTUALSELF
			BOOL pVirSelf = TRUE;
			if (lpReserved)
			{
				s_mm_config* pmmcfg = (s_mm_config*)lpReserved;
				pVirSelf = pmmcfg->m_virtualdll;
			}
			ULONG_PTR pulret = FALSE;
			if (pVirSelf)
				pulret = VirtualSelf(HOOK_NONE,lpReserved,FALSE);
			if (pulret)
				return FALSE;
			else
			{
#endif		
				if (lpReserved)
				{
					glpBufSelf = lpReserved;
					s_mm_config* pmmcfg = (s_mm_config*)glpBufSelf;
					if (pmmcfg->m_nowait)
					{
						CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadSetupRun,0,0,0);
					}else
					{
						char* plpPEBase = (char*)GetModuleHandle(NULL);
						PIMAGE_NT_HEADERS	pimNH = EnterImageNtHeader((char*)plpPEBase);
						gMyOEPJmpBack = (void*)(plpPEBase+pimNH->OptionalHeader.AddressOfEntryPoint);
						HookCode(gMyOEPJmpBack,MyOEPJmp);
					}
				}else
				{
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadSetupRun,0,0,0);
				}
#ifdef _USEVIRTUALSELF
			}
#endif
		}break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

