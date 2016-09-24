#pragma once

class NativeSysCallID
{
public:
	NativeSysCallID();
	~NativeSysCallID();
	void reset();
	void*	setTargetFile(TCHAR* modfile);
	int		getNativeSysCallID(char* funcName);
	int		getNativeSysCallIDAtCode(const char* lpcode);
	int		getFindowWindowSysCallID();
private:
	int		getFindowWindowSysCallIDInternal();
private:
	HANDLE			m_hFile;
	HANDLE			m_hMap;
	const char*		m_lpData;
};

