//////////////////////////////////////////////////////
// comm.cpp文件


#include <winsock2.h>
#include <windows.h>

#include "comm.h"


///////////////////////////////////////////////////////////////////////
 
CPeerList::CPeerList()
{
	m_nCurrentSize = 0;
	m_nTatolSize = 100;
	m_pPeer = new PEER_INFO[m_nTatolSize];
}

CPeerList::~CPeerList()
{
	delete[] m_pPeer;
}

BOOL CPeerList::AddAPeer(PEER_INFO *pPeer)
{
	if(GetAPeer(pPeer->szUserName) != NULL)
		return FALSE;
	// 申请空间
	if(m_nCurrentSize >= m_nTatolSize) // 已经用完？
	{
		PEER_INFO *pTmp = m_pPeer;
		m_nTatolSize = m_nTatolSize * 2;
		m_pPeer = new PEER_INFO[m_nTatolSize];
		memcpy(m_pPeer, pTmp, m_nCurrentSize);
		delete pTmp;
	}
	// 添加到表中
	memcpy(&m_pPeer[m_nCurrentSize ++], pPeer, sizeof(PEER_INFO));
	return TRUE;
}

PEER_INFO *CPeerList::GetAPeer(char *pszUserName)
{
	for(int i=0; i<m_nCurrentSize; i++)
	{
		if(stricmp(m_pPeer[i].szUserName, pszUserName) == 0)
		{
			return &m_pPeer[i];
		}
	}
	return NULL;
}

void CPeerList::DeleteAPeer(char *pszUserName)
{
	for(int i=0; i<m_nCurrentSize; i++)
	{
		if(stricmp(m_pPeer[i].szUserName, pszUserName) == 0)
		{
			memcpy(&m_pPeer[i], &m_pPeer[i+1], (m_nCurrentSize - i - 1)*sizeof(PEER_INFO));
			m_nCurrentSize --;
			break;
		}
	}
}

void CPeerList::DeleteAllPeers()
{
	m_nCurrentSize = 0;
}




