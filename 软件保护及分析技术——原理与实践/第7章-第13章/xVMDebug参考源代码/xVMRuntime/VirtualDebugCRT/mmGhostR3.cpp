// mmGhostR3.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

#include "../../Hooklib/hooklib.h"
#include "../../Hooklib/VirtualSelf.h"
#include "../../Hooklib/AntiCopyRun.h"
#include "./tcc/libtcc.h"
#include "mmGhostR3.h"
#include "WorkCore.h"

#ifdef _DEBUG
CConsole gConsole;
#endif
s_mm_config gmmconfig;
TCCState*	gscpInst = 0;
FILE*		glogfile = 0;

typedef BOOL (WINAPI* PENTRY_CreateProcessInternalW)(HANDLE hToken,
												 LPCWSTR lpApplicationName,
												 LPWSTR lpCommandLine,
												 LPSECURITY_ATTRIBUTES lpProcessAttributes,
												 LPSECURITY_ATTRIBUTES lpThreadAttributes,
												 BOOL bInheritHandles,
												 DWORD dwCreationFlags,
												 LPVOID lpEnvironment,
												 LPCWSTR lpCurrentDirectory,
												 LPSTARTUPINFOW lpStartupInfo,
												 LPPROCESS_INFORMATION lpProcessInformation,
												 PHANDLE hNewToken);

PENTRY_CreateProcessInternalW gEntryCreateProcessInternalW = NULL;

void InjectToProcess(HANDLE phProcess)
{
	int pszbuf = 0;
	char* pcfgbuf = bufmmconfig(&gmmconfig,pszbuf);
	if (!pcfgbuf)
		return;
	void* plpVDBase = VirtualAllocEx(phProcess,NULL,pszbuf,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	if (plpVDBase)
	{
		BOOL pRetC = WriteProcessMemory(phProcess,(char*)plpVDBase,pcfgbuf,pszbuf,NULL);
		if (pRetC)
		{
			RunInProcess(phProcess,(char*)gmmconfig.m_ptselfcp,gmmconfig.m_szselfcp,HOOK_NONE,TRUE,FALSE,FALSE,NULL,plpVDBase,RTYPE_BUFFERDLL);
		}else
		{
#ifdef _DEBUG
			printf("Write data to target faild!\n");
#endif
		}
	}else
	{
#ifdef _DEBUG
		printf("Allocate memory in target process faild!\n");
#endif
	}
	free(pcfgbuf);
}


BOOL WINAPI Proxy_CreateProcessInternalW(HANDLE hToken,
										 LPCWSTR lpApplicationName,
										 LPWSTR lpCommandLine,
										 LPSECURITY_ATTRIBUTES lpProcessAttributes,
										 LPSECURITY_ATTRIBUTES lpThreadAttributes,
										 BOOL bInheritHandles,
										 DWORD dwCreationFlags,
										 LPVOID lpEnvironment,
										 LPCWSTR lpCurrentDirectory,
										 LPSTARTUPINFOW lpStartupInfo,
										 LPPROCESS_INFORMATION lpProcessInformation,
										 PHANDLE hNewToken)
{
	BOOL pnResume = FALSE;
	if (!(dwCreationFlags & CREATE_SUSPENDED))
	{
		dwCreationFlags |= CREATE_SUSPENDED;
		pnResume = TRUE;
	}
	BOOL pRetCode = gEntryCreateProcessInternalW(hToken,lpApplicationName,lpCommandLine,lpProcessAttributes,lpThreadAttributes,bInheritHandles,dwCreationFlags,lpEnvironment,lpCurrentDirectory,lpStartupInfo,lpProcessInformation,hNewToken);
	if (pRetCode)
		InjectToProcess(lpProcessInformation->hProcess);
	if (pnResume)
		ResumeThread(lpProcessInformation->hThread);
	return pRetCode;
}


void injectclient()
{
	gEntryCreateProcessInternalW=	(PENTRY_CreateProcessInternalW)HookOneApi("kernel32.dll","CreateProcessInternalW",Proxy_CreateProcessInternalW);
}

BOOL checkmutx()
{
	long psID = GetCurrentProcessId();
	char psName[64];
	sprintf(psName,"mmGhostInst:%04X",psID);
	HANDLE phMuti = CreateMutex(NULL,FALSE,psName);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(phMuti);
		return FALSE;
	}
	return TRUE;
}


char* memsearch(HINSTANCE phinst,char* plpText,int pszText)
{
	BYTE*	phExecute = (BYTE*)phinst;
	if (!phExecute)
		return 0;
	__try
	{
		PIMAGE_NT_HEADERS	pimNH	=	EnterImageNtHeader((char*)phExecute);
		PIMAGE_SECTION_HEADER pimSH	=	EnterImageFirstSection((char*)phExecute);
		for (int i=0;i<pimNH->FileHeader.NumberOfSections;i++)
		{
			// 			if (pimSH->Characteristics & IMAGE_SCN_CNT_CODE)
			// 			{
			BYTE* plpCheck = phExecute+pimSH->VirtualAddress;
			ULONG_PTR pupaddr = (ULONG_PTR)plpCheck + pimSH->Misc.VirtualSize;
			for (;(ULONG_PTR)plpCheck<pupaddr;plpCheck++)
			{
				BOOL pisame = TRUE;
				BYTE* poloop = plpCheck;
				for (int j=0;j<pszText;j++,poloop++)
				{
					BYTE plbmem = plpText[j];
					if (plbmem == 0xE6)
					{
						j++;
						plbmem = plpText[j];
					}else if (plbmem == 0xE7)
						continue;
					if (*poloop != plbmem)
					{
						pisame = FALSE;
						break;
					}
				}
				if (pisame)
					return (char*)plpCheck;
			}
			//}
			pimSH++;
		}
	}__except(1)
	{

	}
	return NULL;
}


char* memsearchall(char* pAddress,char* plpText,int pszText,int pType)  
{  
	MEMORY_BASIC_INFORMATION pmemInfo;
	BYTE* plpCheck = NULL;
	BYTE* pfindp = (BYTE*)plpText;
	if (pszText <= 0)
		return 0;
	__try
	{
		DWORD pdwResult=VirtualQuery(pAddress,&pmemInfo,sizeof(MEMORY_BASIC_INFORMATION));  
		while(pdwResult)  
		{  
			//printf("searching [0x%08X]->[0x%08X]\n",pmemInfo.BaseAddress,pmemInfo.RegionSize);
			if (pmemInfo.State == MEM_COMMIT)
			{
				if (!(pmemInfo.Protect & PAGE_NOACCESS) && !(pmemInfo.Protect & PAGE_GUARD))
				{
					//printf("searching %08X [%08X].\n",pAddress,pmemInfo.RegionSize);
					if (pType == pmemInfo.Type || pType == 0)
					{
						plpCheck = (BYTE*)pAddress;
						ULONG_PTR pupaddr = (ULONG_PTR)pmemInfo.BaseAddress+pmemInfo.RegionSize-pszText;
						for (;(ULONG_PTR)plpCheck<pupaddr;plpCheck++)
						{
							BOOL pisame = TRUE;
							BYTE* poloop = plpCheck;
							for (int j=0;j<pszText;j++,poloop++)
							{
								BYTE plbmem = plpText[j];
								if (plbmem == 0xE6)
								{
									j++;
									plbmem = plpText[j];
								}else if (plbmem == 0xE7)
									continue;
								if (*poloop != plbmem)
								{
									pisame = FALSE;
									break;
								}
							}
							if (pisame)
								return (char*)plpCheck;
						}
					}
				}
			}
			pAddress=(char*)pmemInfo.BaseAddress+pmemInfo.RegionSize;  
			pdwResult=VirtualQuery(pAddress,&pmemInfo,sizeof(MEMORY_BASIC_INFORMATION));  
		}  
	}
	__except(1)
	{
#ifdef _DEBUG
		printf("except at: %08X\n",pAddress);
#endif
	}
	return NULL;
}  



char* vcall(char* plpFunc,int pnArg, ...) 
{ 
	void** pTemp =(void**)&pnArg; 
	pTemp+=pnArg;
	char* pRet = NULL;
	__asm
	{
		push pnArg
		xchg dword ptr [esp],eax
		push pTemp
		xchg dword ptr [esp],ebx
		push plpFunc
		push ecx
		mov  ecx,eax
	lab_loop:
		cmp eax,0
		jz lab_over
		push dword ptr [ebx]
		sub ebx,4
		dec eax
		jmp lab_loop
lab_over:
		mov ebx,dword ptr [esp+ecx*4+8]
		mov eax,dword ptr [esp+ecx*4+0xC]
		push lab_ret
		push dword ptr [esp+ecx*4+8]
		mov ecx,dword ptr [esp+ecx*4+0x8]
		ret
lab_ret:
		add esp,0x10
		mov pRet,eax
	}
	return pRet;
}
char* vclasscall(char* pclass,char* plpFunc,int pnArg, ...) 
{ 
	void** pTemp =(void**)&pnArg; 
	pTemp+=pnArg;
	char* pRet = NULL;
	__asm
	{
		push pnArg
		xchg dword ptr [esp],eax
		push pTemp
		xchg dword ptr [esp],ebx
		push plpFunc
		push pclass
		mov  ecx,eax
	lab_loop:
		cmp eax,0
		jz lab_over
		push dword ptr [ebx]
		sub ebx,4
		dec eax
		jmp lab_loop
lab_over:
		mov ebx,dword ptr [esp+ecx*4+8]
		mov eax,dword ptr [esp+ecx*4+0xC]
		push lab_ret
		push dword ptr [esp+ecx*4+8]
		mov ecx,dword ptr [esp+ecx*4+0x8]
		ret
lab_ret:
		add esp,0x10
		mov pRet,eax
	}
	return pRet;
}


char* hookcodemem( void* paddr,void* proc )
{
	return (char*)HookCode(paddr,proc,HOOKTYPE_PUSH,0);
}

BOOL unhookcodemem(void* pproc)
{
	return UnHookCode(pproc);
}

void fn_error_func(void *opaque, const char *msg)
{
	FILE* pflog = (FILE*)opaque;
	if (pflog)
		fprintf(pflog,"%s\n",msg);
#ifdef _DEBUG
	printf("%s\n",msg);
#endif
}
typedef void (__cdecl* PENTRY_unload)();

void fn_tcc_add_private(TCCState*	ptcInst);
int fn_run_script(const char* pscpfiles,const char* pcrtpath,const char* psyspath,const char* pincpath,const char* plibpath,const char* plibfiles,const char* plogfile,TCCState** ppInst)
{
	TCCState*	ptcInst = 0;
	int			pret = 0;
	char*		ptstr;
	int			i,j,k,pslen;
	s_mm_scpenv pscpenv;
	if (!pscpfiles)
	{
		if (plogfile)
		{
			glogfile = glogfile?glogfile:fopen(plogfile,"w+");
			if (glogfile)
			{
				fn_error_func(glogfile,"empty source files!");
				fclose(glogfile);
				glogfile = 0;
			}
		}
		return -1;
	}

	if (ppInst && *ppInst)
	{
		ptcInst = *ppInst;
		PENTRY_unload pentryunload = (PENTRY_unload)tcc_get_symbol(ptcInst,"unload");
		if (pentryunload)
		{
			pentryunload();
		}
		tcc_delete(ptcInst);
		if (glogfile)
		{
			fclose(glogfile);
			glogfile = 0;
		}
		*ppInst = 0;
		ptcInst = 0;
	}

	(pscpenv.m_envs[0]  = plogfile?(char*)malloc(strlen(plogfile)+1):0)?strcpy(pscpenv.m_envs[0],plogfile):0;
	(pscpenv.m_envs[1]  = pcrtpath?(char*)malloc(strlen(pcrtpath)+1):0)?strcpy(pscpenv.m_envs[1],pcrtpath):0;
	(pscpenv.m_envs[2]  = psyspath?(char*)malloc(strlen(psyspath)+1):0)?strcpy(pscpenv.m_envs[2],psyspath):0;
	(pscpenv.m_envs[3]  = pincpath?(char*)malloc(strlen(pincpath)+1):0)?strcpy(pscpenv.m_envs[3],pincpath):0;
	(pscpenv.m_envs[4]  = plibpath?(char*)malloc(strlen(plibpath)+1):0)?strcpy(pscpenv.m_envs[4],plibpath):0;
	(pscpenv.m_envs[5]  = plibfiles?(char*)malloc(strlen(plibfiles)+1):0)?strcpy(pscpenv.m_envs[5],plibfiles):0;
	(pscpenv.m_envs[6]  = pscpfiles?(char*)malloc(strlen(pscpfiles)+1):0)?strcpy(pscpenv.m_envs[6],pscpfiles):0;


	ptcInst = tcc_new();
#ifdef _DEBUG
	tcc_define_symbol(ptcInst,"_DEBUG","1");
#endif
	tcc_set_output_type(ptcInst,TCC_OUTPUT_MEMORY);
	fn_tcc_add_private(ptcInst);

	for (k=0;k<sizeof(pscpenv.m_envs)/sizeof(char*);k++)
	{
		ptstr = pscpenv.m_envs[k];
		pslen = strlen(ptstr)+1;
		for (i=0,j=0;i<pslen;i++)
		{
			if (ptstr[i] >= 0x80)
			{
				i++;
				continue;
			}
			if (ptstr[i] == ';' || ptstr[i] == 0)
			{
				ptstr[i] = 0;
				switch(k)
				{
				case 0:
					{
						if (glogfile)
						{
							fclose(glogfile);
							glogfile = 0;
						}
						glogfile = fopen(pscpenv.m_logfile,"w+");
						tcc_set_error_func(ptcInst,glogfile,fn_error_func);
					}break;
				case 1:tcc_set_lib_path(ptcInst,ptstr+j);break;
				case 2:tcc_add_sysinclude_path(ptcInst,ptstr+j);break;
				case 3:tcc_add_include_path(ptcInst,ptstr+j);break;
				case 4:tcc_add_library_path(ptcInst,ptstr+j);break;
				case 5:tcc_add_library(ptcInst,ptstr+j);break;
				case 6:pret |= tcc_add_file(ptcInst,ptstr+j);break;
				}
				ptstr[i] = (ptstr[i]==0)?0:';';
				j=i+1;
			}
		}
	}
	if (pret >= 0)
	{
		pret = tcc_run(ptcInst,1,(char**)ppInst);
	}
	if (pret != 1)
	{
		tcc_delete(ptcInst);
		if (glogfile)
		{
			fclose(glogfile);
			glogfile = 0;
		}
		ptcInst = 0;
	}
	if (ppInst) *ppInst = ptcInst;
	for (k=0;k<sizeof(pscpenv.m_envs)/sizeof(char*);k++)
		if (pscpenv.m_envs[k]) free(pscpenv.m_envs[k]);
	return pret;
}

void fn_tcc_add_private(TCCState*	ptcInst)
{
	tcc_add_symbol(ptcInst,"memsearch",memsearch);
	tcc_add_symbol(ptcInst,"memsearchall",memsearchall);
	tcc_add_symbol(ptcInst,"checkmutx",checkmutx);
	tcc_add_symbol(ptcInst,"hookcodemem",hookcodemem);
	tcc_add_symbol(ptcInst,"unhookcodemem",unhookcodemem);
	tcc_add_symbol(ptcInst,"injectclient",injectclient);
	tcc_add_symbol(ptcInst,"vcall",vcall);
	tcc_add_symbol(ptcInst,"vclasscall",vclasscall);
	tcc_add_symbol(ptcInst,"dbglistening",StartDebugListing);
	tcc_add_symbol(ptcInst,"runscript",fn_run_script);
	tcc_add_symbol(ptcInst,"virusedmodule",VirtualUsedModule);
	tcc_add_symbol(ptcInst,"print",fn_dbg_printlog);
}



void OEPEntry(void* pcfgbuf)
{
#ifdef _DEBUG
	gConsole.CreateConsole("mmGhostR3");
#endif
	//__asm int 3
	BOOL pLoadBuf = TRUE;
	char* ppathexe= _pgmptr;
	char* pptxml = (char*)malloc(strlen(ppathexe)+1);
	if (pptxml)
	{
		strcpy(pptxml,ppathexe);
		strcpy(pptxml+strlen(pptxml)-4,".xmm");
		if (loadmmconfigxml(&gmmconfig,pptxml) == 0)
		{
			pLoadBuf = FALSE;
			if (!gmmconfig.m_ptselfcp || gmmconfig.m_szselfcp == 0)
			{
				if (pcfgbuf)
				{
					s_mm_config* pmmbuf = (s_mm_config*)pcfgbuf;
					int pipos = (int)pmmbuf->m_ptselfcp;
					if (pipos > 0 && pmmbuf->m_szselfcp > 0)
					{
						gmmconfig.m_ptselfcp = (char*)malloc(pmmbuf->m_szselfcp);
						if (gmmconfig.m_ptselfcp)
						{
							gmmconfig.m_szselfcp = pmmbuf->m_szselfcp;
							memcpy(gmmconfig.m_ptselfcp,(char*)pmmbuf+pipos,gmmconfig.m_szselfcp);
						}else
							gmmconfig.m_szselfcp = 0;
					}
				}
			}
		}
		free(pptxml);
	}
	if (pLoadBuf)
		loadmmconfig(&gmmconfig,pcfgbuf);

	WorkCoreInit();	
	//__asm int 3
	int piRet = fn_run_script(gmmconfig.m_scpenv.m_scpfiles
								,gmmconfig.m_scpenv.m_crtpath
								,gmmconfig.m_scpenv.m_syspath
								,gmmconfig.m_scpenv.m_incpath
								,gmmconfig.m_scpenv.m_libpath
								,gmmconfig.m_scpenv.m_libfiles
								,gmmconfig.m_scpenv.m_logfile
								,0);
	if (piRet < 0)
		StartDebugListing();
}

