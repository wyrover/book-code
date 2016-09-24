#include <windows.h>
#include <tchar.h>
#include "NativeSysCallID.h"
#include "../../xHooklib/xhook_pefuncs.h"
#include "../../../ncvm/ncasm/ncasm86.h"

NativeSysCallID::NativeSysCallID()
:m_hFile(INVALID_HANDLE_VALUE)
,m_hMap(0)
,m_lpData(0)
{
}


NativeSysCallID::~NativeSysCallID()
{
	reset();
}



void* NativeSysCallID::setTargetFile(TCHAR* modfile)
{
	reset();
	m_hFile = CreateFile(modfile,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
	if (m_hFile == INVALID_HANDLE_VALUE)
		return 0;
	m_hMap = CreateFileMapping(m_hFile,0,PAGE_READONLY,0,0,0);
	if (m_hMap == 0)
	{
		CloseHandle(m_hFile);
		return 0;
	}
	DWORD szFile = GetFileSize(m_hFile,0);
	m_lpData = (const char*)MapViewOfFile(m_hMap,FILE_MAP_READ,0,0,szFile);
	if (!m_lpData)
	{
		CloseHandle(m_hMap);
		CloseHandle(m_hFile);
		return 0;
	}
	return (void*)m_lpData;
}

void NativeSysCallID::reset()
{
	if (m_lpData)
	{
		UnmapViewOfFile(m_lpData);
		m_lpData = 0;
	}
	if (m_hMap)
	{
		CloseHandle(m_hMap);
		m_hMap = 0;
	}
	if (m_hFile)
	{
		CloseHandle(m_hFile);
		m_hFile = 0;
	}
}
int NativeSysCallID::getNativeSysCallID(char* funcName)
{
	if (!m_lpData) return -1;
	const char* lpcode = GetFunctionAddressInFileBuffer(m_lpData,funcName);
	if (!lpcode) return -2;
	return getNativeSysCallIDAtCode(lpcode);
}

int NativeSysCallID::getFindowWindowSysCallID()
{
	TCHAR path[MAX_PATH];
	GetSystemDirectory(path,sizeof(path) / sizeof(TCHAR)-1);
	_tcscat(path,_T("\\USER32.DLL"));
	NativeSysCallID sc;
	sc.setTargetFile(path);
	return sc.getFindowWindowSysCallIDInternal();
}

int NativeSysCallID::getFindowWindowSysCallIDInternal()
{
	if (!m_lpData) return -1;
	const char* lpcode = GetFunctionAddressInFileBuffer(m_lpData,"FindWindowW");
	if (!lpcode) return -2;
	ncasm::x86::inst	ins;
	ncasm::x86::code	ci;
	ci.code = (const uint8_t*)lpcode;
	ci.codeOffset = (_OffsetType)lpcode;
	ci.codeLen = 16;
	ci.dt = Decode32Bits;
	ci.features = 0;
	int nc = 0;
	int npush = 0;
	int state = 0;
	int ncount = 0;
	while ((nc = ncasm::x86::disasm(&ci,&ins,1)) > 0 && (ncount++ < 100))
	{
		switch (ins.opcode)
		{
		case I_PUSH:npush++;break;
		case I_CALL:{
			if (npush >= 4)
			{
				ULONG_PTR pc = INSTRUCTION_GET_TARGET(&ins);
				if (IsBadReadPtr((const void*)pc,100))
					return -9;
				state++;
				if (state == 2)
				{
					return getNativeSysCallIDAtCode((const char*)pc);
				}
				ci.nextOffset = (_OffsetType)pc;
			}
		}break;
		case I_RET:case I_RETF:
		{
			return -4;
		}break;
		default:{
			npush = 0; 
		}break;
		}
		ci.code = (const uint8_t*)ci.nextOffset;
		ci.codeOffset = (_OffsetType)ci.nextOffset;
		ci.codeLen = 16;
		ci.dt = Decode32Bits;
		ci.features = 0;
	}


	return -3;
}

int NativeSysCallID::getNativeSysCallIDAtCode(const char* lpcode)
{
	ncasm::x86::inst	ins[10];
	ncasm::x86::code	ci;
	ci.code = (const uint8_t*)lpcode;
	ci.codeOffset = (_OffsetType)lpcode;
	ci.codeLen = 160;
	ci.dt = Decode32Bits;
	ci.features = 0;
	int nc = ncasm::x86::disasm(&ci,ins,10);
	if (nc < 4) return -3;
	if (ins[0].opcode != I_MOV) return -4;
	int state = 0;
	for (int i = 1; i < nc; i++)
	{
		switch (ins[i].opcode)
		{
		case I_CALL:case I_CALL_FAR:
		{
						if (state == 0)
							state++;
						else
							return -5;
		}break;
		case I_RET:case I_RETF:
		{
					   if (state != 1)
						   return -6;
					   i = nc;
		}break;
		}
	}

	if (ins[0].imm.sqword >= 0 && ins[0].imm.sqword < 0x10000)
		return ins[0].imm.sqword;
	return -7;
}
