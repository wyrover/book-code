// Copyright Ric Vieler, 2006
// Support header for parse86.c

#ifndef _USER_HOOK_PARSE_H_
#define _USER_HOOK_PARSE_H_

#include "ghost.h"

#pragma optimize( "", off )

#define IS_BETWEEN(x,mn,mx) ((x)>=(mn)&&(x)<=(mx))
#define IS_EQUAL(x,ix) ((x)==(ix))
#define TARGETLESS_X86INSTRUCTION ((PBYTE)0)
#define DYNAMIC_X86INSTRUCTION ((PBYTE)~0ul)


PBYTE transferInstruction( PBYTE destination, PBYTE source, PBYTE* jumpAddress, LONG* extra );
BOOL isJump( PCHAR instruction, ULONG instructionLength );
ULONG getNextInstruction( PCHAR pCodeSrc, ULONG ulMinBytes, PCHAR pDstBuffer, ULONG ulBufferLen );

#pragma optimize( "", on )

#endif
