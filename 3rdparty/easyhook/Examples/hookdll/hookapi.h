// HookSvr.h  

#pragma once  
#include <Windows.h>  

//#ifndef _M_X64  
//#pragma comment(lib, "EasyHook32.lib")  
//#else  
//#pragma comment(lib, "EasyHook64.lib")  
//#endif  

HANDLE WINAPI MyCreateFileW(  
							__in     LPCWSTR lpFileName,  
							__in     DWORD dwDesiredAccess,  
							__in     DWORD dwShareMode,  
							__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,  
							__in     DWORD dwCreationDisposition,  
							__in     DWORD dwFlagsAndAttributes,  
							__in_opt HANDLE hTemplateFile  
							);  

typedef HANDLE (WINAPI *ptrCreateFileW)(  
										__in     LPCWSTR lpFileName,  
										__in     DWORD dwDesiredAccess,  
										__in     DWORD dwShareMode,  
										__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,  
										__in     DWORD dwCreationDisposition,  
										__in     DWORD dwFlagsAndAttributes,  
										__in_opt HANDLE hTemplateFile  
										);  

extern ptrCreateFileW realCreateFileW;  

HANDLE WINAPI MyCreateFileA(  
							__in     LPCSTR lpFileName,  
							__in     DWORD dwDesiredAccess,  
							__in     DWORD dwShareMode,  
							__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,  
							__in     DWORD dwCreationDisposition,  
							__in     DWORD dwFlagsAndAttributes,  
							__in_opt HANDLE hTemplateFile  
							);  

typedef HANDLE (WINAPI *ptrCreateFileA)(  
										__in     LPCSTR lpFileName,  
										__in     DWORD dwDesiredAccess,  
										__in     DWORD dwShareMode,  
										__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,  
										__in     DWORD dwCreationDisposition,  
										__in     DWORD dwFlagsAndAttributes,  
										__in_opt HANDLE hTemplateFile  
										);  

typedef BOOL (WINAPI *ptrPlaySoundW)( LPCWSTR pszSound,
									  HMODULE hwnd,
									  DWORD fdwSound
									 ); 





extern ptrCreateFileA realCreateFileA;