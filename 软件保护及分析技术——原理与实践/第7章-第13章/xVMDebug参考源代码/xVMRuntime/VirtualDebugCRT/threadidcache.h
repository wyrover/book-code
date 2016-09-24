#pragma once

class threadidcache
{
public:
	threadidcache();
	~threadidcache();
	BOOL pushid(DWORD ptid);
	void popid(DWORD ptid);
	BOOL checkid(DWORD ptid);
private:
	DWORD*	m_threadids;
	int		m_nids;
};