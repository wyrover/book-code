#include "StdAfx.h"
#include "DbgFunction.h"
#include "WorkCore.h"

void FuckmmAntiHook()
{
	MEMORY_BASIC_INFORMATION mbi; 
	PBYTE ptr = NULL; 
	DWORD dwBytesReturn = sizeof(MEMORY_BASIC_INFORMATION); 
	//	__asm int 3
	while( dwBytesReturn == sizeof(MEMORY_BASIC_INFORMATION) ) 
	{ 
		dwBytesReturn = VirtualQuery( ptr,&mbi,sizeof(MEMORY_BASIC_INFORMATION) ); 
		ptr = (PBYTE)mbi.BaseAddress+mbi.RegionSize;
		if (mbi.Type == MEM_PRIVATE)
		{
			// 			char pDbgString[256];
			// 			sprintf(pDbgString,"checking %08X...\n",mbi.BaseAddress);
			// 			OutputDebugStringA(pDbgString);
			ULONG pold = 0;
			if (VirtualProtect((char*)mbi.BaseAddress,mbi.RegionSize,PAGE_EXECUTE_READWRITE,&pold))
			{
				PIMAGE_DOS_HEADER pimDH = (PIMAGE_DOS_HEADER)mbi.BaseAddress;
				if (!MyIsBadReadPtr(pimDH,sizeof(IMAGE_DOS_HEADER)))
				{
					if (pimDH->e_magic == IMAGE_DOS_SIGNATURE)
					{
						PIMAGE_NT_HEADERS pimNH = (PIMAGE_NT_HEADERS)((ULONG_PTR)pimDH+pimDH->e_lfanew);
						if (pimNH->Signature == IMAGE_NT_SIGNATURE)
						{
							BOOL pBadPE = FALSE;
							PIMAGE_SECTION_HEADER pimSH = (PIMAGE_SECTION_HEADER)((ULONG_PTR)pimNH+sizeof(IMAGE_NT_HEADERS));
							if (pimNH->OptionalHeader.FileAlignment != pimNH->OptionalHeader.SectionAlignment)
							{
								int pCompSize = min(pimNH->OptionalHeader.SectionAlignment-pimNH->OptionalHeader.FileAlignment,pimNH->OptionalHeader.FileAlignment);
								for (int i=0;i<pCompSize;i++)
								{
									if (*(BYTE*)((BYTE*)pimDH+pimSH->PointerToRawData+i) != 0)
									{
										pBadPE = TRUE;
										break;
									}
								}
							}
							if (pBadPE)
								continue;
							char pPath[MAX_PATH];
							if (GetModuleFileNameA((HMODULE)pimDH,pPath,sizeof(pPath)))
								continue;

							if (pimNH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress != 0)
							{
								PIMAGE_EXPORT_DIRECTORY pimED = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)pimDH+pimNH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
								if (MyIsBadReadPtr(pimED,sizeof(IMAGE_EXPORT_DIRECTORY)))
									continue;
								char* plpName = (char*)pimDH+pimED->Name;
								if (MyIsBadReadPtr(plpName,1))
									continue;
								HMODULE phDLL = GetModuleHandleA(plpName);
								if (!phDLL)
									continue;
								// 								sprintf(pDbgString,"%08X->%s...\n",(ULONG_PTR)pimDH,plpName);
								// 								OutputDebugStringA(pDbgString);
								//__asm int 3

								// 								for (int i=0;i<pimED->NumberOfFunctions;i++)
								// 								{
								// 
								// 									PULONG puFunaddr = (PULONG)((BYTE*)pimDH+pimED->AddressOfFunctions);
								// 									ULONG pofFunc = puFunaddr[i];
								// 									BYTE* plpFunc = (BYTE*)pimDH + pofFunc;
								// 									ULONG pold = 0;
								// 									if (VirtualProtect((char*)plpFunc,0x10,PAGE_EXECUTE_READWRITE,&pold))
								// 									{
								// 										*plpFunc = 0xE9;
								// 										*(ULONG*)(plpFunc+1) = (ULONG)phDLL + pofFunc - (ULONG)plpFunc - 5;
								// 										VirtualProtect((char*)plpFunc,0x10,pold,&pold);
								// 									}
								// 								}

							}

						}
					}
				}
				VirtualProtect((char*)mbi.BaseAddress,mbi.RegionSize,pold,&pold);
			}

		}

	}
	OutputDebugString(TEXT("Fix DLL Copy Finish!\n"));
}



// void SuspendAnotherThreads(BOOL pSuspend)
// {
// 	HANDLE	phShort	=	CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,GetCurrentProcessId());
// 	THREADENTRY32	pThreadInfo;
// 	pThreadInfo.dwSize	=	sizeof(THREADENTRY32);
// 	static HANDLE gHandSusped[256];
// 	Thread32First(phShort,&pThreadInfo);
// 	do 
// 	{
// 		if (pThreadInfo.th32OwnerProcessID	==	GetCurrentProcessId() && pThreadInfo.th32ThreadID != GetCurrentThreadId())
// 		{
// 		}
// 	} while(Thread32Next(phShort,&pThreadInfo));
// }