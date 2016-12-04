// hooker.cpp : 定义 DLL 应用程序的导出函数。
//
#include <stdio.h>
#include <windows.h>
#include "../include/detours.h"
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

#pragma warning(disable:4100)   // Trampolines don't use formal parameters.


HANDLE gMockConsoleInputHandle;

HANDLE gMockConsoleOutputHandle;

HANDLE gStdInputHandle;

HANDLE gStdOutputHandle;


void Log(char * msg)
{
	///*
	FILE * f;
	f = fopen("c:\\log.txt", "a");
	fwrite(msg, strlen(msg), 1, f);
	fclose(f);
	//*/
}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_WriteConsoleA( HANDLE hConsoleOutput,	// handle to a console screen buffer 
				  CONST VOID *lpBuffer,	// pointer to buffer to write from 
				  DWORD nNumberOfCharsToWrite,	// number of characters to write 
				  LPDWORD lpNumberOfCharsWritten,	// pointer to number of characters written 
				  LPVOID lpReserved),
				  WriteConsoleA);
static BOOL WINAPI Detour_WriteConsoleA( HANDLE hConsoleOutput,	// handle to a console screen buffer 
										CONST VOID *lpBuffer,	// pointer to buffer to write from 
										DWORD nNumberOfCharsToWrite,	// number of characters to write 
										LPDWORD lpNumberOfCharsWritten,	// pointer to number of characters written 
										LPVOID lpReserved)
{
	Log("WriteConsoleA\n");
	BOOL ret = WriteFile(gStdOutputHandle, lpBuffer, nNumberOfCharsToWrite, lpNumberOfCharsWritten, NULL);
	char * tem = (char *)malloc(nNumberOfCharsToWrite + 1);
	memmove(tem, lpBuffer, nNumberOfCharsToWrite);
	tem[nNumberOfCharsToWrite] = 0;
	Log(tem);
	free(tem);
	if (ret == FALSE)
	{
		Log("fail: WriteConsoleA\n");
	}
	return ret;
}


DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_WriteConsoleW( HANDLE hConsoleOutput,	// handle to a console screen buffer 
				  CONST VOID *lpBuffer,	// pointer to buffer to write from 
				  DWORD nNumberOfCharsToWrite,	// number of characters to write 
				  LPDWORD lpNumberOfCharsWritten,	// pointer to number of characters written 
				  LPVOID lpReserved),
				  WriteConsoleW);
static BOOL WINAPI Detour_WriteConsoleW( HANDLE hConsoleOutput,	// handle to a console screen buffer 
										CONST VOID *lpBuffer,	// pointer to buffer to write from 
										DWORD nNumberOfCharsToWrite,	// number of characters to write 
										LPDWORD lpNumberOfCharsWritten,	// pointer to number of characters written 
										LPVOID lpReserved)
{
	//这时的nNumberOfCharsToWrite是字符数，不是字节数，字节数因此要*2
	//Log("WriteConsoleW begin\n");
/*	int * pNum = (int *)lpBuffer;
	pNum--;
	int num = *pNum;
	char tem[20];
	itoa(num, tem, 10);
	int len = strlen(tem);
	tem[len] = '\n';
	tem[len + 1] = 0;
	Log(tem);
	itoa(nNumberOfCharsToWrite, tem, 10);
	len = strlen(tem);
	tem[len] = '\n';
	tem[len + 1] = 0;
	Log(tem);
*/
	int bLen = nNumberOfCharsToWrite * 2; 
	int len = bLen + 2;
	char * wstr = (char *)malloc(len);
	memset(wstr, 0 , len);
	memmove(wstr, lpBuffer, bLen);
	int iTextLen;	
	iTextLen = WideCharToMultiByte( CP_ACP,
		0,
		LPCWSTR(wstr),
		-1,
		NULL,
		0,
		NULL,
		NULL );
	//上面函数返回值是写入的字节长度，因此包含了最后的空字节0，所以分配长度是不用+1
	char * pElementText = (char *)malloc(iTextLen);
	memset( pElementText, 0, sizeof( char ) * ( iTextLen) );
	WideCharToMultiByte( CP_ACP,
		0,
		LPCWSTR(wstr),
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL );
	Log(pElementText);
	//Log("\n");	
	int toSend = iTextLen - 1;
	BOOL ret;
	unsigned long wn;
	//其实这里可以用循环每次发两个就不会有一个2字节字符发了一半出去的问题。也不对，发两个还是可能发生只发了一个的
	//问题，只有接收方自己处理
	while (toSend > 0)
	{
	if (!WriteFile(gStdOutputHandle, pElementText, toSend, &wn, NULL))
	{
	
	Log("fail WriteConsoleW\n");
	return FALSE;
	}
	toSend -= wn;
	}
	* lpNumberOfCharsWritten = nNumberOfCharsToWrite;
	free(wstr);
	free(pElementText);
	//Log("WriteConsoleW end\n")	;
	//必须保证全部写完，而且lpNumberOfCharsWritten要以字符计算，非字节。
	/*int toSend = bLen;
	BOOL ret;
	unsigned long wn;
	//其实这里可以用循环每次发两个就不会有一个2字节字符发了一半出去的问题。也不对，发两个还是可能发生只发了一个的
	//问题，只有接收方自己处理
	while (toSend > 0)
	{
		if (!WriteFile(gStdOutputHandle, lpBuffer, toSend, &wn, NULL))
		{
			//要计算发成功的等价于多少unicode字符数，然后在填写lpNumberOfCharsWritten，麻烦，就直接反错误了
			Log("fail WriteConsoleW\n");
			return FALSE;
		}
		toSend -= wn;
	}
	* lpNumberOfCharsWritten = nNumberOfCharsToWrite;
	*/
	return TRUE;
}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_ReadConsoleW(HANDLE hConsoleInput,// handle of a console input buffer 
				  LPVOID lpBuffer,	// address of buffer to receive data 
				  DWORD nNumberOfCharsToRead,	// number of characters to read 
				  LPDWORD lpNumberOfCharsRead,	// address of number of characters read  
				  LPVOID lpReserved	),
				  ReadConsoleW);
static BOOL WINAPI Detour_ReadConsoleW(HANDLE hConsoleInput,	// handle of a console input buffer 
									   LPVOID lpBuffer,	// address of buffer to receive data 
									   DWORD nNumberOfCharsToRead,	// number of characters to read 
									   LPDWORD lpNumberOfCharsRead,	// address of number of characters read  
									   LPVOID lpReserved)
{
	Log("ReadConsoleW begin\n");
	//要求输入的必须是unicode，所以在控制端要转换成unicode
	int toRead = nNumberOfCharsToRead * 2;//
	unsigned long rLen, rLen1;
	if (!ReadFile(gStdInputHandle, lpBuffer, toRead, &rLen1, NULL))
		return FALSE;

	if ((rLen1 % 2) > 0)
	{
		if (!ReadFile(gStdInputHandle, lpBuffer, 1, &rLen, NULL))
		{
			Log("fail ReadConsoleW\n");
			return FALSE;
		}
		rLen = rLen1 + 1;
	}
	else
		rLen = rLen1;
	* lpNumberOfCharsRead = rLen / 2;
	return TRUE;
/*
	char * p = (char *)malloc(*lpNumberOfCharsRead + 1);
	memmove(p, lpBuffer, *lpNumberOfCharsRead);
	p[*lpNumberOfCharsRead] = 0;
	Log(p);
	int  unicodeLen = MultiByteToWideChar( CP_ACP,
		0,
		p,
		-1,
		NULL,
		0 ); 
	LPWSTR  pUnicode; 
	pUnicode = (LPWSTR)lpBuffer; 
	
	memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t)); 
	
	MultiByteToWideChar( CP_ACP,
		0,
		p,
		-1,
		(LPWSTR)pUnicode,
		unicodeLen ); 
	
	free(p);
*/	
	//Log("ReadConsoleW end\n");

}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_FillConsoleOutputAttribute(HANDLE hConsoleOutput,	// handle to screen buffer 
				  WORD wAttribute,	// color attribute to write 
				  DWORD nLength,	// number of character cells to write to 
				  COORD dwWriteCoord,	// x- and y-coordinates of first cell 
				  LPDWORD lpNumberOfAttrsWritten),
				  FillConsoleOutputAttribute);
static BOOL WINAPI Detour_FillConsoleOutputAttribute(HANDLE hConsoleOutput,	// handle to screen buffer 
													 WORD wAttribute,	// color attribute to write 
													 DWORD nLength,	// number of character cells to write to 
													 COORD dwWriteCoord,	// x- and y-coordinates of first cell 
													 LPDWORD lpNumberOfAttrsWritten)
{
	Log("FillConsoleOutputAttribute\n");
	BOOL ret = Trampoline_FillConsoleOutputAttribute(gMockConsoleOutputHandle, wAttribute, nLength, dwWriteCoord, lpNumberOfAttrsWritten);
	if (ret == FALSE)
	{
		Log("fail: FillConsoleOutputAttribute\n");
	}
	return ret;
}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_FillConsoleOutputCharacterW(HANDLE hConsoleOutput,	// handle to screen buffer 
				  TCHAR cCharacter,	// character to write 
				  DWORD nLength,	// number of character cells to write to 
				  COORD dwWriteCoord,	// x- and y-coordinates of first cell 
				  LPDWORD lpNumberOfCharsWritten),
				  FillConsoleOutputCharacterW);
static BOOL WINAPI Detour_FillConsoleOutputCharacterW(HANDLE hConsoleOutput,	// handle to screen buffer 
													  TCHAR cCharacter,	// character to write 
													  DWORD nLength,	// number of character cells to write to 
													  COORD dwWriteCoord,	// x- and y-coordinates of first cell 
													  LPDWORD lpNumberOfCharsWritten)
{
	Log("FillConsoleOutputCharacterW\n");
	BOOL ret = Trampoline_FillConsoleOutputCharacterW(gMockConsoleOutputHandle, cCharacter, nLength, dwWriteCoord, lpNumberOfCharsWritten);
	if (ret == FALSE)
	{
		Log("fail: FillConsoleOutputCharacterW\n");
	}
	return ret;
}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_GetConsoleCursorInfo( HANDLE hConsoleOutput,	// handle of console screen buffer  
				  PCONSOLE_CURSOR_INFO lpConsoleCursorInfo),
				  GetConsoleCursorInfo);
static BOOL WINAPI Detour_GetConsoleCursorInfo( HANDLE hConsoleOutput,	// handle of console screen buffer  
											   PCONSOLE_CURSOR_INFO lpConsoleCursorInfo)
{
	Log("GetConsoleCursorInfo\n");
	BOOL ret = Trampoline_GetConsoleCursorInfo(gMockConsoleOutputHandle, lpConsoleCursorInfo);
	if (ret == FALSE)
	{
		Log("fail: GetConsoleCursorInfo\n");
	}
	return ret;
}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_GetConsoleScreenBufferInfo(HANDLE hConsoleOutput,	// handle of console screen buffer 
				  PCONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo),
				  GetConsoleScreenBufferInfo);
static BOOL WINAPI Detour_GetConsoleScreenBufferInfo(HANDLE hConsoleOutput,	// handle of console screen buffer 
													 PCONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo)
{
	Log("GetConsoleScreenBufferInfo\n");
	BOOL ret = Trampoline_GetConsoleScreenBufferInfo(gMockConsoleOutputHandle, lpConsoleScreenBufferInfo);
	if (ret == FALSE)
	{
		Log("fail: GetConsoleScreenBufferInfo\n");
	}
	return ret;
}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_GetLargestConsoleWindowSize(HANDLE hConsoleOutput),
				  GetLargestConsoleWindowSize);
static BOOL WINAPI Detour_GetLargestConsoleWindowSize(HANDLE hConsoleOutput)
{
	Log("GetLargestConsoleWindowSize\n");
	BOOL ret = Trampoline_GetLargestConsoleWindowSize(gMockConsoleOutputHandle);
	if (ret == FALSE)
	{
		Log("fail: GetLargestConsoleWindowSize\n");
	}
	return ret;
}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_ReadConsoleOutput(HANDLE hConsoleOutput,	// handle of a console screen buffer 
				  PCHAR_INFO lpBuffer,	// address of buffer that receives data 
				  COORD dwBufferSize,	// column-row size of destination buffer 
				  COORD dwBufferCoord,	// upper-left cell to write to 
				  PSMALL_RECT lpReadRegion),
				  ReadConsoleOutput);
static BOOL WINAPI Detour_ReadConsoleOutput(HANDLE hConsoleOutput,	// handle of a console screen buffer 
											PCHAR_INFO lpBuffer,	// address of buffer that receives data 
											COORD dwBufferSize,	// column-row size of destination buffer 
											COORD dwBufferCoord,	// upper-left cell to write to 
											PSMALL_RECT lpReadRegion)
{
	Log("ReadConsoleOutput\n");
	BOOL ret = Trampoline_ReadConsoleOutput(gMockConsoleOutputHandle, lpBuffer, dwBufferSize, dwBufferCoord, lpReadRegion);
	if (ret == FALSE)
	{
		Log("fail: ReadConsoleOutput\n");
	}
	return ret;
}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_ReadConsoleOutputAttribute(HANDLE hConsoleOutput,	// handle of a console screen buffer 
				  LPWORD lpAttribute,	// address of buffer to receive attributes 
				  DWORD nLength,	// number of character cells to read from  
				  COORD dwReadCoord,	// coordinates of first cell to read from 
				  LPDWORD lpNumberOfAttrsRead),
				  ReadConsoleOutputAttribute);
static BOOL WINAPI Detour_ReadConsoleOutputAttribute(HANDLE hConsoleOutput,	// handle of a console screen buffer 
													 LPWORD lpAttribute,	// address of buffer to receive attributes 
													 DWORD nLength,	// number of character cells to read from  
													 COORD dwReadCoord,	// coordinates of first cell to read from 
													 LPDWORD lpNumberOfAttrsRead)
{
	Log("ReadConsoleOutputAttribute\n");
	BOOL ret = Trampoline_ReadConsoleOutputAttribute(gMockConsoleOutputHandle, lpAttribute, nLength, dwReadCoord, lpNumberOfAttrsRead);
	if (ret == FALSE)
	{
		Log("fail: ReadConsoleOutputAttribute\n");
	}
	return ret;
}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_ScrollConsoleScreenBuffer(HANDLE hConsoleOutput,	// handle of a console screen buffer 
				  CONST SMALL_RECT *lpScrollRectangle,	// address of screen buffer rect. to move 
				  CONST SMALL_RECT *lpClipRectangle,	// address of affected screen buffer rect. 
				  COORD dwDestinationOrigin,	// new location of screen buffer rect. 
				  CONST CHAR_INFO *lpFill),
				  ScrollConsoleScreenBuffer);
static BOOL WINAPI Detour_ScrollConsoleScreenBuffer(HANDLE hConsoleOutput,	// handle of a console screen buffer 
													CONST SMALL_RECT *lpScrollRectangle,	// address of screen buffer rect. to move 
													CONST SMALL_RECT *lpClipRectangle,	// address of affected screen buffer rect. 
													COORD dwDestinationOrigin,	// new location of screen buffer rect. 
													CONST CHAR_INFO *lpFill)
{
	Log("ScrollConsoleScreenBuffer\n");
	BOOL ret = Trampoline_ScrollConsoleScreenBuffer(gMockConsoleOutputHandle, lpScrollRectangle, lpClipRectangle, dwDestinationOrigin, lpFill);
	if (ret == FALSE)
	{
		Log("fail: ScrollConsoleScreenBuffer\n");
	}
	return ret;
}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_SetConsoleActiveScreenBuffer(HANDLE hConsoleOutput),
				  SetConsoleActiveScreenBuffer);
static BOOL WINAPI Detour_SetConsoleActiveScreenBuffer(HANDLE hConsoleOutput)
{
	Log("SetConsoleActiveScreenBuffer\n");
	if (hConsoleOutput != gMockConsoleOutputHandle)
	{
		Log("set new output buffer ...............\n");
		if (hConsoleOutput != gMockConsoleInputHandle)
			Log("new output buffer handle is not mock input...............\n");
	}
	BOOL ret = Trampoline_SetConsoleActiveScreenBuffer(gMockConsoleOutputHandle);
	if (ret == FALSE)
	{
		Log("fail: SetConsoleActiveScreenBuffer\n");
	}
	return ret;
}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_SetConsoleCursorInfo(HANDLE hConsoleOutput,	// handle of console screen buffer  
				  CONST CONSOLE_CURSOR_INFO *lpConsoleCursorInfo),
				  SetConsoleCursorInfo);
static BOOL WINAPI Detour_SetConsoleCursorInfo(HANDLE hConsoleOutput,	// handle of console screen buffer  
											   CONST CONSOLE_CURSOR_INFO *lpConsoleCursorInfo)
{
	Log("SetConsoleCursorInfo\n");
	BOOL ret = Trampoline_SetConsoleCursorInfo(gMockConsoleOutputHandle, lpConsoleCursorInfo);
	if (ret == FALSE)
	{
		Log("fail: SetConsoleCursorInfo\n");
	}
	return ret;
}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_SetConsoleCursorPosition(HANDLE hConsoleOutput,	// handle of console screen buffer 
				  COORD dwCursorPosition),
				  SetConsoleCursorPosition);
static BOOL WINAPI Detour_SetConsoleCursorPosition(HANDLE hConsoleOutput,	// handle of console screen buffer 
												   COORD dwCursorPosition)
{
	Log("SetConsoleCursorPosition\n");
	BOOL ret = Trampoline_SetConsoleCursorPosition(gMockConsoleOutputHandle, dwCursorPosition);
	if (ret == FALSE)
	{
		Log("fail: SetConsoleCursorPosition\n");
	}
	return ret;
}


DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_SetConsoleScreenBufferSize(HANDLE hConsoleOutput,	// handle of console screen buffer 
				  COORD dwSize),
				  SetConsoleScreenBufferSize);
static BOOL WINAPI Detour_SetConsoleScreenBufferSize(HANDLE hConsoleOutput,	// handle of console screen buffer 
													 COORD dwSize)
{
	Log("SetConsoleScreenBufferSize\n");
	BOOL ret = Trampoline_SetConsoleScreenBufferSize(gMockConsoleOutputHandle, dwSize);
	if (ret == FALSE)
	{
		Log("fail: SetConsoleScreenBufferSize\n");
	}
	return ret;
}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_SetConsoleWindowInfo( HANDLE hConsoleOutput,	// handle of console screen buffer 
				  BOOL bAbsolute,	// coordinate type flag 
				  CONST SMALL_RECT *lpConsoleWindow),
				  SetConsoleWindowInfo);
static BOOL WINAPI Detour_SetConsoleWindowInfo( HANDLE hConsoleOutput,	// handle of console screen buffer 
											   BOOL bAbsolute,	// coordinate type flag 
											   CONST SMALL_RECT *lpConsoleWindow)
{
	Log("SetConsoleWindowInfo\n");
	BOOL ret = Trampoline_SetConsoleWindowInfo(gMockConsoleOutputHandle, bAbsolute, lpConsoleWindow);
	if (ret == FALSE)
	{
		Log("fail: SetConsoleWindowInfo\n");
	}
	return ret;
}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_WriteConsoleOutputW(HANDLE hConsoleOutput,	// handle to a console screen buffer 
				  CONST CHAR_INFO *lpBuffer,	// pointer to buffer with data to write  
				  COORD dwBufferSize,	// column-row size of source buffer 
				  COORD dwBufferCoord,	// upper-left cell to write from 
				  PSMALL_RECT lpWriteRegion),
				  WriteConsoleOutputW);
static BOOL WINAPI Detour_WriteConsoleOutputW( HANDLE hConsoleOutput,	// handle to a console screen buffer 
											  CONST CHAR_INFO *lpBuffer,	// pointer to buffer with data to write  
											  COORD dwBufferSize,	// column-row size of source buffer 
											  COORD dwBufferCoord,	// upper-left cell to write from 
											  PSMALL_RECT lpWriteRegion)
{
	Log("WriteConsoleOutputW\n");
	BOOL ret = Trampoline_WriteConsoleOutputW(gMockConsoleOutputHandle, lpBuffer, dwBufferSize, dwBufferCoord, lpWriteRegion);
	if (ret == FALSE)
	{
		Log("fail: WriteConsoleOutputW\n");
	}
	return ret;
}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_WriteConsoleOutputAttribute(HANDLE hConsoleOutput,	// handle to a console screen buffer 
				  CONST WORD *lpAttribute,	// pointer to buffer to write attributes from  
				  DWORD nLength,	// number of character cells to write to 
				  COORD wWriteCoord,	// coordinates of first cell to write to 
				  LPDWORD lpNumberOfAttrsWritten),
				  WriteConsoleOutputAttribute);
static BOOL WINAPI Detour_WriteConsoleOutputAttribute(HANDLE hConsoleOutput,	// handle to a console screen buffer 
													  CONST WORD *lpAttribute,	// pointer to buffer to write attributes from  
													  DWORD nLength,	// number of character cells to write to 
													  COORD wWriteCoord,	// coordinates of first cell to write to 
													  LPDWORD lpNumberOfAttrsWritten)
{
	Log("WriteConsoleOutputAttribute\n");
	BOOL ret = Trampoline_WriteConsoleOutputAttribute(gMockConsoleOutputHandle, lpAttribute, nLength, wWriteCoord, lpNumberOfAttrsWritten);
	if (ret == FALSE)
	{
		Log("fail: WriteConsoleOutputAttribute\n");
	}
	return ret;
}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_WriteConsoleOutputCharacterA(HANDLE hConsoleOutput,	// handle to a console screen buffer 
				  LPCTSTR lpCharacter,	// pointer to buffer to write characters from 
				  DWORD nLength,	// number of character cells to write to 
				  COORD dwWriteCoord,	// coordinates of first cell to write to 
				  LPDWORD lpNumberOfCharsWritten),
				  WriteConsoleOutputCharacterA);
static BOOL WINAPI Detour_WriteConsoleOutputCharacterA(HANDLE hConsoleOutput,	// handle to a console screen buffer 
													   LPCTSTR lpCharacter,	// pointer to buffer to write characters from 
													   DWORD nLength,	// number of character cells to write to 
													   COORD dwWriteCoord,	// coordinates of first cell to write to 
													   LPDWORD lpNumberOfCharsWritten)
{
	Log("WriteConsoleOutputCharacterA\n");
	BOOL ret = Trampoline_WriteConsoleOutputCharacterA(gMockConsoleOutputHandle, lpCharacter, nLength, dwWriteCoord, lpNumberOfCharsWritten);
	if (ret == FALSE)
	{
		Log("fail: WriteConsoleOutputCharacterA\n");
	}
	return ret;
}

//----------------------------------------------------------------------mock input------------------
DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_GetConsoleMode(HANDLE hConsoleHandle,	// handle of console input or screen buffer  
				  LPDWORD lpMode),
				  GetConsoleMode);
static BOOL WINAPI Detour_GetConsoleMode(HANDLE hConsoleHandle,	// handle of console input or screen buffer  
										 LPDWORD lpMode)
{
	Log("GetConsoleMode\n");
	BOOL ret = Trampoline_GetConsoleMode(gMockConsoleInputHandle, lpMode);
	if (ret == FALSE)
	{
		Log("fail: GetConsoleMode\n");
	}
	return ret;
}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_SetConsoleMode(HANDLE hConsoleHandle,	// handle of console input or screen buffer  
				  DWORD dwMode),
				  SetConsoleMode);
static BOOL WINAPI Detour_SetConsoleMode(HANDLE hConsoleHandle,	// handle of console input or screen buffer  
										 DWORD dwMode)
{
	//return TRUE;
	Log("SetConsoleMode\n");
	BOOL ret = Trampoline_SetConsoleMode(gMockConsoleInputHandle, dwMode);
	if (ret == FALSE)
	{
		Log("fail: SetConsoleMode\n");
	}
	return ret;
}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_ReadConsoleInputA(HANDLE hConsoleInput,	// handle of a console input buffer 
				  PINPUT_RECORD lpBuffer,	// address of the buffer for read data 
				  DWORD nLength,	// number of records to read 
				  LPDWORD lpNumberOfEventsRead),
				  ReadConsoleInputA);
static BOOL WINAPI Detour_ReadConsoleInputA(HANDLE hConsoleInput,	// handle of a console input buffer 
											PINPUT_RECORD lpBuffer,	// address of the buffer for read data 
											DWORD nLength,	// number of records to read 
											LPDWORD lpNumberOfEventsRead)
{
	
	Log("ReadConsoleInputA\n");
	BOOL ret = Trampoline_ReadConsoleInputA(gMockConsoleInputHandle, lpBuffer, nLength, lpNumberOfEventsRead);
	if (ret == FALSE)
	{
		Log("fail: ReadConsoleInputA\n");
	}
	return ret;
}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_ReadConsoleInputW(HANDLE hConsoleInput,	// handle of a console input buffer 
				  PINPUT_RECORD lpBuffer,	// address of the buffer for read data 
				  DWORD nLength,	// number of records to read 
				  LPDWORD lpNumberOfEventsRead),
				  ReadConsoleInputW);
static BOOL WINAPI Detour_ReadConsoleInputW(HANDLE hConsoleInput,	// handle of a console input buffer 
											PINPUT_RECORD lpBuffer,	// address of the buffer for read data 
											DWORD nLength,	// number of records to read 
											LPDWORD lpNumberOfEventsRead)
{
	Log("ReadConsoleInputW\n");
	BOOL ret = Trampoline_ReadConsoleInputW(gMockConsoleInputHandle, lpBuffer, nLength, lpNumberOfEventsRead);
	if (ret == FALSE)
	{
		Log("fail: ReadConsoleInputW\n");
	}
	return ret;
}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_WriteConsoleInputW(HANDLE hConsoleInput,	// handle to a console input buffer 
				  CONST INPUT_RECORD *lpBuffer,	// pointer to the buffer for write data 
				  DWORD nLength,	// number of records to write 
				  LPDWORD lpNumberOfEventsWritten),
				  WriteConsoleInputW);
static BOOL WINAPI Detour_WriteConsoleInputW(HANDLE hConsoleInput,	// handle to a console input buffer 
											 CONST INPUT_RECORD *lpBuffer,	// pointer to the buffer for write data 
											 DWORD nLength,	// number of records to write 
											 LPDWORD lpNumberOfEventsWritten)
{	
	//return TRUE;
	Log("WriteConsoleInputW\n");
	BOOL ret = Trampoline_WriteConsoleInputW(gMockConsoleInputHandle, lpBuffer, nLength, lpNumberOfEventsWritten);
	if (ret == FALSE)
	{
		Log("fail: WriteConsoleInputW\n");
	}
	return ret;
}

void init()
{
	Log("init begin...\n");
	gMockConsoleInputHandle = CreateFileA("CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS	, FILE_ATTRIBUTE_NORMAL, NULL);
	//gMockConsoleOutputHandle = CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	gMockConsoleOutputHandle = CreateFileA("CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS	, FILE_ATTRIBUTE_NORMAL, NULL);
	SetConsoleActiveScreenBuffer(gMockConsoleOutputHandle);
	gStdInputHandle = GetStdHandle(STD_INPUT_HANDLE);
	gStdOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetStdHandle(STD_INPUT_HANDLE, gMockConsoleInputHandle);
	SetStdHandle(STD_OUTPUT_HANDLE, gMockConsoleOutputHandle);
	Log("init end...\n");
}


bool hooked = false;

void hook()
{
	if (hooked)
	{
		return;
	}
	init();
	DetourFunctionWithTrampoline((PBYTE)Trampoline_WriteConsoleA, (PBYTE)Detour_WriteConsoleA);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_WriteConsoleW, (PBYTE)Detour_WriteConsoleW);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_ReadConsoleW, (PBYTE)Detour_ReadConsoleW);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_FillConsoleOutputAttribute, (PBYTE)Detour_FillConsoleOutputAttribute);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_FillConsoleOutputCharacterW, (PBYTE)Detour_FillConsoleOutputCharacterW);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_GetConsoleCursorInfo, (PBYTE)Detour_GetConsoleCursorInfo);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_GetConsoleScreenBufferInfo, (PBYTE)Detour_GetConsoleScreenBufferInfo);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_GetLargestConsoleWindowSize, (PBYTE)Detour_GetLargestConsoleWindowSize);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_ReadConsoleOutput, (PBYTE)Detour_ReadConsoleOutput);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_ReadConsoleOutputAttribute, (PBYTE)Detour_ReadConsoleOutputAttribute);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_ScrollConsoleScreenBuffer, (PBYTE)Detour_ScrollConsoleScreenBuffer);
	//DetourFunctionWithTrampoline((PBYTE)Trampoline_SetConsoleActiveScreenBuffer, (PBYTE)Detour_SetConsoleActiveScreenBuffer);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_SetConsoleCursorInfo, (PBYTE)Detour_SetConsoleCursorInfo);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_SetConsoleCursorPosition, (PBYTE)Detour_SetConsoleCursorPosition);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_SetConsoleScreenBufferSize, (PBYTE)Detour_SetConsoleScreenBufferSize);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_SetConsoleWindowInfo, (PBYTE)Detour_SetConsoleWindowInfo);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_WriteConsoleOutputW, (PBYTE)Detour_WriteConsoleOutputW);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_WriteConsoleOutputAttribute, (PBYTE)Detour_WriteConsoleOutputAttribute);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_WriteConsoleOutputCharacterA, (PBYTE)Detour_WriteConsoleOutputCharacterA);

	DetourFunctionWithTrampoline((PBYTE)Trampoline_GetConsoleMode, (PBYTE)Detour_GetConsoleMode);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_SetConsoleMode, (PBYTE)Detour_SetConsoleMode);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_ReadConsoleInputA, (PBYTE)Detour_ReadConsoleInputA);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_ReadConsoleInputW, (PBYTE)Detour_ReadConsoleInputW);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_WriteConsoleInputW, (PBYTE)Detour_WriteConsoleInputW);
	Log("hook end..\n");
}


