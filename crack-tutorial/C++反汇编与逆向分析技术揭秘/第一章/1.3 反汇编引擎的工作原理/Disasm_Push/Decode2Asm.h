// Decode2Asm.h: interface for the CDecode2Asm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DECODE2ASM_H__7AE15245_B351_41F6_B8B2_09D157829988__INCLUDED_)
#define AFX_DECODE2ASM_H__7AE15245_B351_41F6_B8B2_09D157829988__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern "C"
void
__stdcall
Decode2Asm(IN PBYTE pCodeEntry,
           OUT char* strAsmCode,
           OUT UINT* pnCodeSize,
           UINT nAddress);
#endif // !defined(AFX_DECODE2ASM_H__7AE15245_B351_41F6_B8B2_09D157829988__INCLUDED_)
