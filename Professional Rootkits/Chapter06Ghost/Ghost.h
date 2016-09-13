// Copyright Ric Vieler, 2004
// Support header for Ghost.c

#ifndef _GHOST_H_
#define _GHOST_H_

// Flag for IoManager.h
#define _GHOST_ROOTKIT_

typedef unsigned long	DWORD;
typedef int				BOOL;
typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef float			FLOAT;
typedef FLOAT*			PFLOAT;
typedef BOOL*			PBOOL;
typedef BYTE*			PBYTE;
typedef int*			PINT;
typedef WORD*			PWORD;
typedef DWORD*			PDWORD;
typedef DWORD*			LPDWORD;
typedef int				INT;
typedef unsigned int	UINT;
typedef unsigned int*	PUINT;
typedef long*			LPLONG;

typedef void*			PVOID;
#define LPVOID			PVOID
typedef PVOID			FARPROC;
typedef const void*		LPCVOID;

typedef struct _SECURITY_ATTRIBUTES
{
    DWORD nLength;
    LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

typedef struct _OVERLAPPED
{
    DWORD   Internal;
    DWORD   InternalHigh;
    DWORD   Offset;
    DWORD   OffsetHigh;
    HANDLE  hEvent;
} OVERLAPPED, *LPOVERLAPPED;

typedef struct _DRIVER_DATA
{
   LIST_ENTRY listEntry;
   DWORD  unknown1;
   DWORD  unknown2;
   DWORD  unknown3;
   DWORD  unknown4;
   DWORD  unknown5;
   DWORD  unknown6;
   DWORD  unknown7;
   UNICODE_STRING path;
   UNICODE_STRING name;
} DRIVER_DATA;

#define CREATE_NEW          1
#define CREATE_ALWAYS       2
#define OPEN_EXISTING       3
#define OPEN_ALWAYS         4
#define TRUNCATE_EXISTING   5

#define INVALID_HANDLE_VALUE  ((HANDLE)((LONG_PTR)-1))

#endif