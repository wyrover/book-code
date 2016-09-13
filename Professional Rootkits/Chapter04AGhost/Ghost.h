// Copyright Ric Vieler, 2004
// Support header for Ghost.c

#ifndef _GHOST_H_
#define _GHOST_H_

typedef BOOLEAN BOOL;
typedef unsigned long DWORD;
typedef DWORD * PDWORD;
typedef unsigned long ULONG;
typedef unsigned short WORD;
typedef unsigned char BYTE;

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

#endif

