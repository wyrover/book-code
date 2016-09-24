#pragma once

struct s_step_breakpoint 
{
	ULONG_PTR		m_addr;
	DWORD			m_TID;
	unsigned char	m_orgchr;
	unsigned char	m_stepcmd;
	ULONG			m_stepcode;
};

class StepBreakPoints
{
public:
	StepBreakPoints(void);
	~StepBreakPoints(void);

	BOOL	AddBreakPoint(ULONG_PTR paddr,DWORD pTID,unsigned char pstepcmd,ULONG pstepecode);
	BOOL	CheckVirReadMem(ULONG_PTR ppbuf,DWORD pszBuf);
	BOOL	VirReadMem(void* pmembuf,ULONG_PTR ppbuf,DWORD pszBuf);
	BOOL	VirWriteMem(ULONG_PTR ppbuf,DWORD pszBuf);
	BOOL	CheckDelBreakPoint(ULONG_PTR paddr,DWORD pTID);
private:
	s_step_breakpoint*	m_bps;
	int					m_nbp;
};

