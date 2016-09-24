#ifndef _ReadPE_H_
#define _ReadPE_H_

#include "stdafx.h"

typedef unsigned char boolean;

ULONG RVA2RAW(PVOID Base, ULONG RVAaddr);
ULONG RAW2RVA(PVOID Base, ULONG RAWaddr);

PVOID ReadKbdClassDriverFileData();

boolean GetReadKbdClassDriverEntryAddrFromPE(PContextData Data);

boolean GetRealKbdClassReadAddrFromPE(PContextData Data);

boolean GetKbdServiceCallBackAddrFromPE(PContextData Data);

ULONG GetFunctionLen(ULONG CodeStartAddr, BYTE EndOpcode);

boolean RelocCode(PVOID Base, PVOID CodeRVA, BYTE* Code, ULONG CodeLen, ULONG Offset);

VOID ShowDASM(ULONG CodeAddr);

#endif