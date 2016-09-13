// injectManager
// Copyright Ric Vieler, 2006
// Hook Dynamic Link Libraries

#include "ntddk.h"
#include "Ghost.h"
#include "hookManager.h"
#include "injectManager.h"
#include "parse86.h"
#include <stdarg.h>
#include <stdio.h>

#pragma code_seg("PAGE")
#pragma optimize( "", off )

extern PVOID kernel32Base;

static void HookTable( void );
static void DetourFunction( void );
static void EndOfInjectedCode( void );
static DWORD beforeEncode( PDWORD stack, DWORD* callbackReturn, IN_PROCESS_DATA* pCallData );
static DWORD BeforeOriginalFunction( DWORD hookIndex, PDWORD originalStack, DWORD* returnParameter, IN_PROCESS_DATA* callData );
static void AfterOriginalFunction( DWORD hookIndex, PDWORD originalStack, DWORD* returnParameter, IN_PROCESS_DATA* callData );

#define JUMP_TO_DETOUR_LOCATION			-5
#define CALLDATA_INDEX_LOCATION			0
#define CALLDATA_PARAMETERS_LOCATION	4
#define CALLDATA_CALLTYPE_LOCATION		8
#define CALLDATA_STACK_OFFSET_LOCATION	12
#define TRAMPOLINE_LOCATION				16
#define START_OF_TRAMPOLINE_PATTERN		-1

void __declspec(naked) HookTable( void )
{
	__asm
	{
		push eax
		xor eax, eax
		call phoney_call
phoney_call:
		lea eax, phoney_call
		lea edx, phoney_jump
		sub edx, eax
		pop eax
		add eax, edx
		mov edx, eax
		pop eax
		jmp DetourFunction
phoney_jump:
		EMIT_FOUR( 0xff )
		EMIT_FOUR( 0x0 )
		EMIT_FOUR( 0x0 )
		EMIT_FOUR( 0x0 )
		EMIT_FOUR( 0x90 )
		EMIT_FOUR( 0x90 )
		EMIT_FOUR( 0x90 )
		EMIT_FOUR( 0x90 )
		EMIT_FOUR( 0x90 )
		EMIT_FOUR( 0x90 )
		EMIT_FOUR( 0x90 )
		EMIT_FOUR( 0x90 )
		EMIT_FOUR( 0x90 )
		jmp EndOfInjectedCode
	}
}

////////////////////////////////
// Injected functions
////////////////////////////////

void __declspec(naked) DetourFunction( void )
{
	PUSH_STACKFRAME();
	{
		DWORD		hookIndex;
		DWORD		parameters;
		DWORD		callType;
		DWORD		stackOffset;
		PCHAR		trampolineFunction;
		IN_PROCESS_DATA*	callData;
		PCHAR		codeStart;
		PDWORD		originalStack;
		DWORD		tempStack;
		int			loop;
		int			parameters4return;
		DWORD		parameter2return = 0;
		DWORD		continueFlag;
		DWORD		register_esp;
		DWORD		register_edi;
		DWORD		register_esi;
		DWORD		register_eax;
		DWORD		register_ebx;
		DWORD		register_ecx;
		DWORD		add2stack;

		// setup to call injected functions
		__asm
		{
			mov register_esp, esp
			mov register_edi, edi
			mov register_esi, esi
			mov register_eax, eax
			mov register_ebx, ebx
			mov register_ecx, ecx

			// get parameters
			push edx
			mov edx, [edx+CALLDATA_INDEX_LOCATION]
			mov hookIndex, edx
			pop edx
			push edx
			mov edx, [edx+CALLDATA_PARAMETERS_LOCATION]
			mov parameters, edx
			pop edx
			push edx
			mov edx, [edx+CALLDATA_CALLTYPE_LOCATION]
			mov callType, edx
			pop edx
			push edx
			mov edx, [edx+CALLDATA_STACK_OFFSET_LOCATION]
			mov stackOffset, edx
			pop edx
			push edx
			add edx, TRAMPOLINE_LOCATION
			mov trampolineFunction, edx
			pop edx
			// caculate the start address
			xor eax, eax
			call called_without_return
called_without_return:
			pop eax
			lea ebx, DetourFunction
			lea ecx, called_without_return
			sub ecx, ebx
			sub eax, ecx
			mov codeStart, eax
			// data area
			lea ecx, EndOfInjectedCode
			sub ecx, ebx
			add ecx, eax
			mov callData, ecx
			// caculate the last ret address
			mov eax, ebp
			add eax, 4	// pushed ebp
			add eax, stackOffset
			mov originalStack, eax
		}

		// setup return call type
		if( callType == CDECL_TYPE )
			add2stack = parameters * sizeof( DWORD );
		else
			add2stack = 0;
		// call pre-injected code
		continueFlag = BeforeOriginalFunction( hookIndex, originalStack, &parameter2return, callData );
		if( continueFlag == (DWORD)TRUE )
		{
			for( loop = parameters; loop > 0; loop-- )
			{
				tempStack = originalStack[loop];
				__asm push tempStack
			}
			// Call trampoline (jumps to original function)
			//
			// Since trampoline is a jump, the return in
			// the original function will come back here.
			__asm
			{
				lea ebx, DetourFunction
				lea eax, return_from_trampoline
				sub eax, ebx
				add eax, codeStart
				// construct call
				push eax
				// adjust stack
				sub esp, stackOffset
				// restore registers and call
				mov edi, register_edi
				mov esi, register_esi
				mov eax, register_eax
				mov ebx, register_ebx
				mov ecx, register_ecx
				jmp trampolineFunction
return_from_trampoline:
				add esp, add2stack
				mov parameter2return, eax
			}
			// call post-injected code
			AfterOriginalFunction( hookIndex, originalStack, &parameter2return, callData );
		}
		// prepare to return
		tempStack = *originalStack;
		if( callType == CDECL_TYPE )
			parameters4return = 0;
		else
			parameters4return = parameters;
		__asm
		{
			mov eax, parameter2return
			mov ecx, tempStack
			mov edx, parameters4return
			shl edx, 2
			add edx, stackOffset
			POP_STACKFRAME();
			add esp, 4
			add esp, edx
			jmp ecx
		}
		__asm mov edx, trampolineFunction
	}
	POP_STACKFRAME();
	__asm jmp edx
}

///////////////////////////////////////////////////////////////
// this function is located in the PGP SDK
// dynamic link library (old=PGP_SDK.DLL, new=PGPsdk.dll)
// This function accepts the callers input and output,
// which may be memory or file based, and converts the input
// into encrypted output
//
// return TRUE to allow encryption
// return FALSE to block encryption
///////////////////////////////////////////////////////////////
DWORD beforeEncode( PDWORD stack, DWORD* callbackReturn, IN_PROCESS_DATA* pCallData )
{
	void*					contextPtr = (void*)stack[1];
	PGPOptionList*			optionListPtr = (PGPOptionList*)stack[2];
	DWORD					dwRet = (DWORD)TRUE;

	int index;
	int inputType = 0;
	void* lpBuffer;
	DWORD dwInBufferLen = 0;
	PGPOption* currentOption = optionListPtr->options;
	PFLFileSpec* fileSpec;

	// Look at the options in the option list
	for( index = 0; index < optionListPtr->numOptions; index++)
	{
		if( currentOption->type == 1 )
		{
			// File Input
			inputType = 1;
			fileSpec = (PFLFileSpec*)currentOption->value;
			lpBuffer = fileSpec->data;
			dwInBufferLen = (DWORD)pCallData->plstrlenA((LPCSTR)(lpBuffer));
			break;
		}
		else if( currentOption->type == 2 )
		{
			// Buffer Input
			inputType = 2;
			lpBuffer = (void*)currentOption->value;
			dwInBufferLen = (DWORD)currentOption->valueSize;
			break;
		}
		currentOption++;
	}

	// Process buffer or file before encryption
	if(( inputType == 1 || inputType == 2 ) && ( dwInBufferLen > 0 ))
	{			
		// just blocking this API to show functionality
		dwRet = (DWORD)FALSE;
		*callbackReturn = PGP_BAD_API;
	}
	return dwRet;
}

DWORD BeforeOriginalFunction( DWORD hookIndex, PDWORD originalStack, DWORD* returnParameter, IN_PROCESS_DATA* callData )
{
	if( hookIndex == USERHOOK_beforeEncode )
	{
		return beforeEncode( originalStack, returnParameter, callData );
	}
	return (DWORD)TRUE;
}

void AfterOriginalFunction( DWORD hookIndex, PDWORD originalStack, DWORD* returnParameter, IN_PROCESS_DATA* callData )
{
}

// EndOfInjectedCode - DetourFunction = size of injected code
// Content doesn't matter, so just trap a debug exception
void __declspec(naked) EndOfInjectedCode( void )
{
	__asm int 3
}

////////////////////////////////
// End injected functions
////////////////////////////////

PCHAR allocateUserMemory()
{
	LONG		memorySize;
	LONG		tableSize;
	LONG		codeSize;
	LONG		dataSize;
	ULONG		buffer[2];
	NTSTATUS	status;
	PCHAR		pMemory;
	IN_PROCESS_DATA* pData;

	// Calculate sizes
	// table = (DetourFunction - HookTable) * TOTAL_HOOKS
	// code = EndOfInjectedCode - DetourFunction
	// data = sizof( IN_PROCESS_DATA )
	__asm
	{
		lea eax, HookTable
		lea ebx, DetourFunction
		lea ecx, EndOfInjectedCode
		mov edx, ebx
		sub edx, eax
		mov tableSize, edx
		mov edx, ecx
		sub edx, ebx
		mov codeSize, edx
	}
	tableSize = tableSize * TOTAL_HOOKS;
	dataSize = sizeof( IN_PROCESS_DATA );
	memorySize = tableSize + codeSize + dataSize;

	// Allocate memory
	buffer[0] = 0;
	buffer[1] = memorySize;
	status = ZwAllocateVirtualMemory( (HANDLE)-1, (PVOID*)buffer, 0, &buffer[1], MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE );
	pMemory = (PCHAR)(buffer[0]);

	if( !NT_SUCCESS( status ) || !pMemory )
		return NULL;

	// initialize memory
	memset( pMemory, 0x90, tableSize + codeSize );
	pData = (IN_PROCESS_DATA*)(pMemory + tableSize + codeSize );
	memset( (PVOID)pData, 0, dataSize );
	
	return pMemory;
}

ULONG getx86Instruction( PCHAR originalCode, PCHAR instructionBuffer, ULONG bufferLength )
{
	PBYTE source = NULL;
	PBYTE destination = NULL;
	ULONG ulCopied = 0;
	PBYTE jumpAddress = NULL;
	LONG  extra = 0;

	memset( instructionBuffer, 0, bufferLength );
	source = (PBYTE)originalCode;
	destination = (PBYTE)instructionBuffer;
	jumpAddress = NULL;
	extra = 0;
	// start with 5 bytes
	for( ulCopied = 0; ulCopied < 5; )
	{
		source = transferInstruction( destination, source, &jumpAddress, &extra );
		if( !source )
		{
			memset( instructionBuffer, 0, bufferLength );
			ulCopied = 0;
			break;
		}
		ulCopied = (DWORD)source - (DWORD)originalCode;
		if( ulCopied >= bufferLength )
		{
			ASSERT( FALSE );
			break;
		}
		destination = (PBYTE)instructionBuffer + ulCopied;
	}
	return ulCopied;
}

BOOL makeWritable( PVOID address, ULONG size )
{
    NTSTATUS	status;
	ULONG		pageAccess;
	ULONG		ZwProtectArray[3] = { 0 };

	pageAccess = PAGE_EXECUTE_READWRITE;
	ZwProtectArray[0] = (ULONG)address;
	ZwProtectArray[1] = size;
	ZwProtectArray[2] = 0;

	status = OldZwProtectVirtualMemory( (HANDLE)-1,
										(PVOID *)(&(ZwProtectArray[0])),
										&(ZwProtectArray[1]),
										pageAccess,
										&(ZwProtectArray[2]) );

	if( !NT_SUCCESS( status ) )
		return FALSE;

	return TRUE;
}

// Parse first instruction of original function.
// Replace first instruction with jump to hook.
// Save first instruction to trampoline function.
// Only call original function through trampoline.
BOOL createTrampoline( PCHAR originalAddress, PCHAR tableAddress, PCHAR trampolineAddress )
{
	ULONG		newOriginalAddress = 0;
	char		instruction[MAX_INSTRUCTION] = { 0 };
	ULONG		instructionLength;

	instructionLength = getx86Instruction( originalAddress, instruction, sizeof(instruction) );
	newOriginalAddress = (ULONG)(originalAddress + instructionLength);
	// see if it's a jump
	if( isJump( instruction, instructionLength ) )
	{
		PVOID pOldDstAddr = (PVOID)(GET_JUMP( instruction ));
		if( pOldDstAddr )
		{
			// If first instruction of original function
			// is a jump, trampoline instruction is NO-OP
			// and jump target is original jump target
			memset( instruction, 0x90, sizeof(instruction) );
			instructionLength = 0;
			newOriginalAddress = (ULONG)pOldDstAddr;
		}
		else
		{
			return FALSE;
		}
	}
	if( makeWritable( (PVOID)trampolineAddress, MAX_INSTRUCTION + 5 ) )
	{
		// write trampoline function
		memset( trampolineAddress, 0x90, MAX_INSTRUCTION + 5 );
		memcpy( trampolineAddress, instruction, instructionLength );
		INJECT_JUMP( trampolineAddress + instructionLength, newOriginalAddress );
		// set original function to jump to trampoline function
		if( makeWritable( originalAddress, instructionLength + 5 ) )
		{
			INJECT_JUMP( originalAddress, tableAddress );
			return TRUE;
		}
	}
	return FALSE;
}

BOOL getHookPointers( PCHAR pMemory, PCHAR* pTable, PCHAR* pCode, PCHAR* pData )
{
	LONG	tableSize = 0;
	LONG	codeSize = 0;
	LONG	dataSize = 0;

	__asm
	{
		lea eax, HookTable
		lea ebx, DetourFunction
		lea ecx, EndOfInjectedCode
		mov edx, ebx
		sub edx, eax
		mov tableSize, edx
		mov edx, ecx
		sub edx, ebx
		mov codeSize, edx
	}
	
	tableSize = tableSize * TOTAL_HOOKS;
	dataSize = sizeof(IN_PROCESS_DATA);
	*pTable = pMemory;
	*pCode = *pTable + tableSize;
	*pData = *pCode + codeSize;
	return TRUE;
}

BOOL processInject( CALL_DATA_STRUCT* pCallData, int hooks, PCHAR pMemory )
{
	int	loop;
	int	offsetToPattern;
	PCHAR pNewTable;
	PCHAR pNewCode;
	IN_PROCESS_DATA* pNewData;
	PCHAR pOldTable;
	PCHAR pOldCode;
	PCHAR pOldData;
	DWORD tableLength;
	DWORD tableOffset;
	PCHAR callDataOffset;

	if( !kernel32Base )
		return FALSE;

	if( !getHookPointers( pMemory, &pNewTable, &pNewCode, (PCHAR*)&pNewData ) )
		return FALSE;

	pNewData->pOutputDebugStringA = (PROTOTYPE_OutputDebugStringA)GetFunctionAddress( kernel32Base, "OutputDebugStringA", NULL, 0 );
	pNewData->pOutputDebugStringW = (PROTOTYPE_OutputDebugStringW)GetFunctionAddress( kernel32Base, "OutputDebugStringW", NULL, 0 );
	pNewData->pCloseHandle = (PROTOTYPE_CloseHandle)GetFunctionAddress( kernel32Base, "CloseHandle", NULL, 0 );
	pNewData->pSleep = (PROTOTYPE_Sleep)GetFunctionAddress( kernel32Base, "Sleep", NULL, 0 );
	pNewData->pCreateFileW = (PROTOTYPE_CreateFileW)GetFunctionAddress( kernel32Base, "CreateFileW", NULL, 0 );
	pNewData->plstrlenA = (PROTOTYPE_lstrlenA)GetFunctionAddress( kernel32Base, "lstrlenA", NULL, 0 );
	pNewData->plstrlenW = (PROTOTYPE_lstrlenW)GetFunctionAddress( kernel32Base, "lstrlenW", NULL, 0 );
	pNewData->plstrcpynA = (PROTOTYPE_lstrcpynA)GetFunctionAddress( kernel32Base, "lstrcpynA", NULL, 0 );
	pNewData->plstrcpynW = (PROTOTYPE_lstrcpynW)GetFunctionAddress( kernel32Base, "lstrcpynW", NULL, 0 );
	pNewData->plstrcpyA = (PROTOTYPE_lstrcpyA)GetFunctionAddress( kernel32Base, "lstrcpyA", NULL, 0 );
	pNewData->plstrcpyW = (PROTOTYPE_lstrcpyW)GetFunctionAddress( kernel32Base, "lstrcpyW", NULL, 0 );
	pNewData->plstrcmpiA = (PROTOTYPE_lstrcmpiA)GetFunctionAddress( kernel32Base, "lstrcmpiA", NULL, 0 );
	pNewData->plstrcmpiW = (PROTOTYPE_lstrcmpiW)GetFunctionAddress( kernel32Base, "lstrcmpiW", NULL, 0 );
	pNewData->plstrcmpA = (PROTOTYPE_lstrcmpA)GetFunctionAddress( kernel32Base, "lstrcmpA", NULL, 0 );
	pNewData->plstrcmpW = (PROTOTYPE_lstrcmpW)GetFunctionAddress( kernel32Base, "lstrcmpW", NULL, 0 );
	pNewData->plstrcatA = (PROTOTYPE_lstrcatA)GetFunctionAddress( kernel32Base, "lstrcatA", NULL, 0 );
	pNewData->plstrcatW = (PROTOTYPE_lstrcatW)GetFunctionAddress( kernel32Base, "lstrcatW", NULL, 0 );
	sprintf( pNewData->debugString, "This is a string contained in injected memory\n" );

	__asm
	{
		lea eax, HookTable
		mov pOldTable, eax
		lea eax, DetourFunction
		mov pOldCode, eax
		lea eax, EndOfInjectedCode
		mov pOldData, eax
	}

	memcpy( pNewCode, pOldCode, pOldData - pOldCode );
	tableLength = pOldCode - pOldTable;
	for( loop = 0; loop < (int)tableLength - 4; loop ++ )
	{
		if( *(PDWORD)(pOldTable+loop) == (DWORD)START_OF_TRAMPOLINE_PATTERN )
		{
			offsetToPattern = loop;
			break;
		}
	}
	for( loop = 0; loop < hooks; loop ++ )
	{
		tableOffset = tableLength * pCallData[loop].index;
		callDataOffset =  pNewTable + tableOffset + offsetToPattern;
		memcpy( pNewTable + tableOffset, pOldTable, tableLength );
		*((PDWORD)(callDataOffset + CALLDATA_INDEX_LOCATION)) = pCallData[loop].index;
		*((PDWORD)(callDataOffset + CALLDATA_PARAMETERS_LOCATION)) = pCallData[loop].parameters;
		*((PDWORD)(callDataOffset + CALLDATA_CALLTYPE_LOCATION)) = pCallData[loop].callType;
		*((PDWORD)(callDataOffset + CALLDATA_STACK_OFFSET_LOCATION)) = pCallData[loop].stackOffset;
		INJECT_JUMP( callDataOffset + JUMP_TO_DETOUR_LOCATION, pNewCode );
		createTrampoline( pCallData[loop].hookFunction,
			pNewTable + tableOffset,
			callDataOffset + TRAMPOLINE_LOCATION);
	}
	return TRUE;
}

#pragma optimize( "", on )
