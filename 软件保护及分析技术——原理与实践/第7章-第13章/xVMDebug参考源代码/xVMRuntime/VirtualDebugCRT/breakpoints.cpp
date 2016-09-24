#include <windows.h>
#include "breakpoints.h"
#include "VirtualDebugApiLayerCRT.h"

StepBreakPoints::StepBreakPoints(void)
	:m_nbp(2)
	,m_bps(0)
{
}


StepBreakPoints::~StepBreakPoints(void)
{
	if (m_bps)
	{
		free(m_bps);
		m_bps = 0;
		m_nbp = 0;
	}
}

BOOL StepBreakPoints::AddBreakPoint( ULONG_PTR paddr,DWORD pTID,unsigned char pstepcmd,ULONG pstepecode )
{
	if (MyIsBadReadPtr((void*)paddr,1))
		return FALSE;
	if (!m_bps)
	{
		if (m_nbp == 0) m_nbp = 2;
		m_bps = (s_step_breakpoint*)malloc(m_nbp*sizeof(s_step_breakpoint));
		memset(m_bps,0,sizeof(s_step_breakpoint)*m_nbp);
	}
	if (!m_bps)
		return FALSE;
	s_step_breakpoint* pbpone = 0;
	for (int i=0;i<m_nbp;i++)
	{
		if (m_bps[i].m_addr == paddr)
		{
			pbpone = &m_bps[i];
			break;
		}
	}
	if (!pbpone)
	{
		for (int i=0;i<m_nbp;i++)
		{
			if (m_bps[i].m_addr == 0)
			{
				pbpone = &m_bps[i];
				break;
			}
		}
	}
	if (!pbpone)
	{
		int pnid = m_nbp;
		if (m_nbp == 0)
			m_nbp = 2;
		else
			m_nbp *=2;
		m_bps = (s_step_breakpoint*)realloc(m_bps,sizeof(s_step_breakpoint)*m_nbp);
		if (!m_bps)
			return FALSE;
		for (int i=pnid;i<m_nbp;i++)
			m_bps[i].m_addr = 0;
		pbpone = &m_bps[pnid];
	}
	if (!pbpone)
		return FALSE;

	pbpone->m_TID = pTID;
	pbpone->m_stepcmd = pstepcmd;
	pbpone->m_stepcode= pstepecode;
	if (!pbpone->m_addr)
		pbpone->m_orgchr = *(unsigned char*)paddr;
	pbpone->m_addr = paddr;
	DWORD poldpg = 0;
	if (!VirtualProtect((LPVOID)paddr,1,PAGE_EXECUTE_READWRITE,&poldpg))
	{
		pbpone->m_addr = 0;
		return FALSE;
	}
	*(unsigned char*)paddr = pstepcmd;
	VirtualProtect((LPVOID)paddr,1,poldpg,&poldpg);
#ifdef _DEBUG
	//printf("added steping breakpoint:%08X tid:%08X\n",paddr,pTID);
#endif
	return TRUE;
}

BOOL StepBreakPoints::CheckVirReadMem( ULONG_PTR ppbuf,DWORD pszBuf )
{
	if (!m_bps)	return FALSE;
	for (int i=0;i<m_nbp;i++)
	{
		if ((m_bps[i].m_addr >= ppbuf) && (m_bps[i].m_addr <= (ppbuf+pszBuf)))
			return TRUE;
	}
	return FALSE;
}
BOOL StepBreakPoints::VirReadMem( void* pmembuf,ULONG_PTR ppbuf,DWORD pszBuf )
{
	if (!m_bps)	return FALSE;
	for (int i=0;i<m_nbp;i++)
	{
		s_step_breakpoint* pbpone = &m_bps[i];
		if ((pbpone->m_addr >= ppbuf) && (pbpone->m_addr <= (ppbuf+pszBuf)))
		{
			int poft = pbpone->m_addr - ppbuf;
			*(unsigned char*)((char*)pmembuf+poft) = pbpone->m_orgchr;
		}
	}
	return TRUE;

}


BOOL StepBreakPoints::VirWriteMem( ULONG_PTR ppbuf,DWORD pszBuf )
{
	if (!m_bps)	return FALSE;
	for (int i=0;i<m_nbp;i++)
	{
		s_step_breakpoint* pbpone = &m_bps[i];
		if ((pbpone->m_addr >= ppbuf) && (pbpone->m_addr <= (ppbuf+pszBuf)))
		{
			int poft = pbpone->m_addr - ppbuf;
			*(unsigned char*)(ppbuf+poft) = pbpone->m_stepcmd;
		}
	}
	return TRUE;
}


BOOL StepBreakPoints::CheckDelBreakPoint( ULONG_PTR paddr,DWORD pTID )
{
	if (!m_bps)	return FALSE;
#ifdef _DEBUG
	//printf("check breakpoint:%08X tid:%08X %d\n",paddr,pTID,m_nbp);
#endif
	for (int i=0;i<m_nbp;i++)
	{
		s_step_breakpoint* pbpone = &m_bps[i];
		if (pbpone->m_addr == paddr)
		{
			if (!MyIsBadReadPtr((void*)paddr,1))
			{
				if (*(unsigned char*)paddr == pbpone->m_stepcmd)
				{
					DWORD poldpg = 0;
					if (VirtualProtect((void*)pbpone->m_addr,1,PAGE_EXECUTE_READWRITE,&poldpg))
					{
						*(unsigned char*)paddr = pbpone->m_orgchr;
						VirtualProtect((void*)pbpone->m_addr,1,poldpg,&poldpg);
					}
				}
			}
			DWORD pbptid = pbpone->m_TID;
			pbpone->m_addr = 0;
			return (pTID == pbptid);
		}
	}
	return FALSE;
}

