// Copyright Ric Vieler, 2006
// Support header for injectManager.c

#ifndef _USER_HOOK_INJECTION_H_
#define _USER_HOOK_INJECTION_H_

#define USERHOOK_beforeEncode		0
#define TOTAL_HOOKS					1

#define MAX_INSTRUCTION				36

#define STDCALL_TYPE				0
#define CDECL_TYPE					1

#define EMIT_FOUR( x ) __asm{ __asm _emit x __asm _emit x __asm _emit x __asm _emit x }
#define PUSH_STACKFRAME( ) __asm{ __asm push ebp __asm mov ebp, esp __asm sub esp, __LOCAL_SIZE __asm push edi __asm push esi __asm push ebx __asm pushfd }
#define POP_STACKFRAME( ) __asm{ __asm popfd __asm pop ebx __asm pop esi __asm pop edi __asm mov esp, ebp __asm pop ebp }
#define INJECT_JUMP( from, to ) { ((PCHAR)from)[0] = (CHAR)0xe9; *((DWORD *)&(((PCHAR)(from))[1])) = (PCHAR)(to) - (PCHAR)(from) - 5; }
#define GET_JUMP( from ) (((PCHAR)from)[0]==(CHAR)0xe9)? (*((DWORD *)&(((PCHAR)(from))[1])) + 5 + (DWORD)(from)) : 0

#pragma pack(1)

// Prototypes for functions in kernel32.dll that are expected to be used in hook functions
typedef int (__stdcall * PROTOTYPE_lstrlenA)( LPCSTR lpString );
typedef int (__stdcall * PROTOTYPE_lstrlenW)( LPCWSTR lpString );
typedef LPSTR (__stdcall * PROTOTYPE_lstrcpynA)( LPSTR lpString1, LPCSTR lpString2, int iMaxLength );
typedef LPWSTR (__stdcall * PROTOTYPE_lstrcpynW)( LPWSTR lpString1, LPCWSTR lpString2, int iMaxLength );
typedef LPSTR (__stdcall * PROTOTYPE_lstrcpyA)( LPSTR lpString1, LPCSTR lpString2 );
typedef LPWSTR (__stdcall * PROTOTYPE_lstrcpyW)( LPWSTR lpString1, LPCWSTR lpString2 );
typedef int (__stdcall * PROTOTYPE_lstrcmpiA)( LPCSTR lpString1, LPCSTR lpString2 );
typedef int (__stdcall * PROTOTYPE_lstrcmpiW)( LPCWSTR lpString1, LPCWSTR lpString2 );
typedef int (__stdcall * PROTOTYPE_lstrcmpA)( LPCSTR lpString1, LPCSTR lpString2 );
typedef int (__stdcall * PROTOTYPE_lstrcmpW)( LPCWSTR lpString1, LPCWSTR lpString2 );
typedef LPSTR (__stdcall * PROTOTYPE_lstrcatA)( LPSTR lpString1, LPCSTR lpString2 );
typedef LPWSTR (__stdcall * PROTOTYPE_lstrcatW)( LPWSTR lpString1, LPCWSTR lpString2 );
typedef VOID (__stdcall * PROTOTYPE_OutputDebugStringA)( LPCSTR lpOutputString );
typedef VOID (__stdcall * PROTOTYPE_OutputDebugStringW)( LPCWSTR lpOutputString );
typedef HANDLE (__stdcall * PROTOTYPE_CreateFileA)( LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile );
typedef BOOL (__stdcall * PROTOTYPE_DeviceIoControl)( HANDLE hDevice, DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped );
typedef VOID (__stdcall * PROTOTYPE_Sleep)( DWORD dwMilliseconds );
typedef BOOL (__stdcall * PROTOTYPE_CloseHandle)( HANDLE hObject );
typedef DWORD (__stdcall * PROTOTYPE_GetCurrentProcessId)( VOID );
typedef DWORD (__stdcall * PROTOTYPE_GetCurrentThreadId)( VOID );

typedef struct _CALL_DATA_STRUCT
{
	UINT			index;
	UINT			parameters;
	PCHAR			hookFunction;
	UINT			callType;
	UINT			stackOffset;
} CALL_DATA_STRUCT;

typedef struct _IN_PROCESS_DATA
{
	// function addresses						
	PROTOTYPE_lstrlenA						plstrlenA;
	PROTOTYPE_lstrlenW						plstrlenW;
	PROTOTYPE_lstrcpynA						plstrcpynA;
	PROTOTYPE_lstrcpynW						plstrcpynW;
	PROTOTYPE_lstrcpyA						plstrcpyA;
	PROTOTYPE_lstrcpyW						plstrcpyW;
	PROTOTYPE_lstrcmpiA						plstrcmpiA;
	PROTOTYPE_lstrcmpiW						plstrcmpiW;
	PROTOTYPE_lstrcmpA						plstrcmpA;
	PROTOTYPE_lstrcmpW						plstrcmpW;
	PROTOTYPE_lstrcatA						plstrcatA;
	PROTOTYPE_lstrcatW						plstrcatW;
	PROTOTYPE_OutputDebugStringA			pOutputDebugStringA;
	PROTOTYPE_OutputDebugStringW			pOutputDebugStringW;
	PROTOTYPE_CreateFileA					pCreateFileA;
	PROTOTYPE_DeviceIoControl				pDeviceIoControl;
	PROTOTYPE_CloseHandle					pCloseHandle;
	PROTOTYPE_Sleep							pSleep;
	PROTOTYPE_GetCurrentProcessId			pGetCurrentProcessId;
	PROTOTYPE_GetCurrentThreadId			pGetCurrentThreadId;
	char									deviceString[64];
	char									allowString[64];
	char									denyString[64];
} IN_PROCESS_DATA;

BOOL processInject( CALL_DATA_STRUCT* pCallData, int hooks2find, PCHAR pUserMem );
PCHAR allocateUserMemory( void );
BOOL createTrampoline( PCHAR originalAddress, PCHAR newStartAddress, PCHAR newEndAddress );
ULONG getx86Instruction( PCHAR originalCode, PCHAR instructionBuffer, ULONG bufferLength );
DWORD BeforeOriginalFunction( DWORD hookIndex, PDWORD originalStack, DWORD* returnParameter, IN_PROCESS_DATA* callData );
void AfterOriginalFunction( DWORD hookIndex, PDWORD originalStack, DWORD* returnParameter, IN_PROCESS_DATA* callData );
BOOL makeWritable( PVOID address, ULONG size );

// structures required to inject into PGP
typedef struct _PGPOption
{
	unsigned int	type;
	unsigned int	flags;
	unsigned int	value;
	unsigned int	valueSize;
	void*			subOptions;
	void*			handlerProc;
} PGPOption;

typedef struct _PGPVersion
{
	unsigned short	majorVersion;
	unsigned short	minorVersion;
	
} PGPVersion;

typedef struct _PGPOptionList
{
	unsigned int	magic;
	PGPVersion		version;
	void*			context;
	int				err;
	unsigned int	flags;
	unsigned short	maxOptions;
	unsigned short	numOptions;
	
	PGPOption*		options;
} PGPOptionList;

typedef struct _PFLFileSpec
{
	unsigned int	magic;
	void*			memoryMgr;
	unsigned int	type;
	unsigned int	dataSize;
	void*			vtbl;
	void*			data;
} PFLFileSpec;

typedef struct _FILELIST
{
	char*				name;
	int					IsDirectory;
	struct _FILELIST*	next;
} FILELIST;

#define PGP_OK			0
#define PGP_BAD_API		-11460
#define	PGP_FILE_FAIL	-11991

#endif

