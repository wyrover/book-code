#include <windows.h>

#include "threadidcache.h"

threadidcache::threadidcache()
	:m_threadids(0)
	,m_nids(2)
{

}

threadidcache::~threadidcache()
{
	if (m_threadids)
	{
		free(m_threadids);
		m_threadids = 0;
	}
}

BOOL threadidcache::pushid( DWORD ptid )
{
	if (!m_threadids)
	{
		if (m_nids == 0)
			m_nids = 2;
		m_threadids = (DWORD*)malloc(m_nids);
		memset(m_threadids,0,sizeof(DWORD)*m_nids);
	}
	if (!m_threadids)
		return FALSE;
	for (int i=0;i<m_nids;i++)
	{
		if (m_threadids[i] == ptid)
			return TRUE;
	}
	for (int i=0;i<m_nids;i++)
	{
		if (m_threadids[i] == 0)
		{
			m_threadids[i] = ptid;
			return TRUE;
		}
	}
	int pnid = m_nids;
	if (m_nids == 0)
		m_nids = 2;
	else
		m_nids *=2;
	m_threadids = (DWORD*)realloc(m_threadids,sizeof(DWORD)*m_nids);
	if (!m_threadids)
		return FALSE;
	m_threadids[pnid] = ptid;
	for (int i=pnid+1;i<m_nids;i++)
		m_threadids[i] = 0;
	return TRUE;
}

void threadidcache::popid( DWORD ptid )
{
	if (!m_threadids)
		return;
	for (int i=0;i<m_nids;i++)
	{
		if (m_threadids[i] == ptid)
		{
			m_threadids[i] = 0;
			break;
		}
	}
}

BOOL threadidcache::checkid( DWORD ptid )
{
	if (!m_threadids)
		return FALSE;
	for (int i=0;i<m_nids;i++)
	{
		if (m_threadids[i] == ptid)
			return TRUE;
	}
	return FALSE;
}
