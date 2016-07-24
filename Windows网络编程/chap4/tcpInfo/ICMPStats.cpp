// ICMPStats.cpp : implementation file
//

#include "stdafx.h"
#include "TCPInfo.h"
#include "ICMPStats.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CICMPStats dialog


CICMPStats::CICMPStats(CWnd* pParent /*=NULL*/)
	: CDialog(CICMPStats::IDD, pParent)
{
	//{{AFX_DATA_INIT(CICMPStats)
	m_strMsgsI = _T("");
	m_strErrorsI = _T("");
	m_strDstUnrI = _T("");
	m_TTLExcI = _T("");
	m_strParamProbsI = _T("");
	m_strSrcQI = _T("");
	m_strRedirI = _T("");
	m_strEchoI = _T("");
	m_strEchoReplI = _T("");
	m_strTimeI = _T("");
	m_strTimeReplI = _T("");
	m_strAddrI = _T("");
	m_strAddrReplI = _T("");
	m_strMsgsO = _T("");
	m_strErrorsO = _T("");
	m_strDstUnrO = _T("");
	m_strTTLExcO = _T("");
	m_strParamProbsO = _T("");
	m_strSrcQO = _T("");
	m_strRedirO = _T("");
	m_strEchoO = _T("");
	m_strEchoReplO = _T("");
	m_strTimeO = _T("");
	m_strTimeReplO = _T("");
	m_strAddrO = _T("");
	m_strAddrReplO = _T("");
	//}}AFX_DATA_INIT
}


void CICMPStats::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CICMPStats)
	DDX_Text(pDX, IDC_STATIC1, m_strMsgsI);
	DDX_Text(pDX, IDC_STATIC2, m_strErrorsI);
	DDX_Text(pDX, IDC_STATIC3, m_strDstUnrI);
	DDX_Text(pDX, IDC_STATIC4, m_TTLExcI);
	DDX_Text(pDX, IDC_STATIC5, m_strParamProbsI);
	DDX_Text(pDX, IDC_STATIC6, m_strSrcQI);
	DDX_Text(pDX, IDC_STATIC7, m_strRedirI);
	DDX_Text(pDX, IDC_STATIC8, m_strEchoI);
	DDX_Text(pDX, IDC_STATIC9, m_strEchoReplI);
	DDX_Text(pDX, IDC_STATIC10, m_strTimeI);
	DDX_Text(pDX, IDC_STATIC11, m_strTimeReplI);
	DDX_Text(pDX, IDC_STATIC12, m_strAddrI);
	DDX_Text(pDX, IDC_STATIC13, m_strAddrReplI);
	DDX_Text(pDX, IDC_STATIC24, m_strMsgsO);
	DDX_Text(pDX, IDC_STATIC25, m_strErrorsO);
	DDX_Text(pDX, IDC_STATIC26, m_strDstUnrO);
	DDX_Text(pDX, IDC_STATIC27, m_strTTLExcO);
	DDX_Text(pDX, IDC_STATIC28, m_strParamProbsO);
	DDX_Text(pDX, IDC_STATIC29, m_strSrcQO);
	DDX_Text(pDX, IDC_STATIC30, m_strRedirO);
	DDX_Text(pDX, IDC_STATIC31, m_strEchoO);
	DDX_Text(pDX, IDC_STATIC32, m_strEchoReplO);
	DDX_Text(pDX, IDC_STATIC33, m_strTimeO);
	DDX_Text(pDX, IDC_STATIC34, m_strTimeReplO);
	DDX_Text(pDX, IDC_STATIC35, m_strAddrO);
	DDX_Text(pDX, IDC_STATIC36, m_strAddrReplO);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CICMPStats, CDialog)
	//{{AFX_MSG_MAP(CICMPStats)
	ON_BN_CLICKED(IDOK2, OnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CICMPStats message handlers

BOOL CICMPStats::OnInitDialog() 
{
	CDialog::OnInitDialog();

	MIB_ICMP  mibICMP;

	// Get the ICMP statistics
	if(GetIcmpStatistics(&mibICMP) == NO_ERROR)
	{
		m_strMsgsI.Format("%u", mibICMP.stats.icmpInStats.dwMsgs);
		m_strErrorsI.Format("%u", mibICMP.stats.icmpInStats.dwErrors);
		m_strDstUnrI.Format("%u", mibICMP.stats.icmpInStats.dwDestUnreachs);
		m_TTLExcI.Format("%u", mibICMP.stats.icmpInStats.dwTimeExcds);
		m_strParamProbsI.Format("%u", mibICMP.stats.icmpInStats.dwParmProbs);
		m_strSrcQI.Format("%u", mibICMP.stats.icmpInStats.dwSrcQuenchs);
		m_strRedirI.Format("%u", mibICMP.stats.icmpInStats.dwRedirects);
		m_strEchoI.Format("%u", mibICMP.stats.icmpInStats.dwEchos);
		m_strEchoReplI.Format("%u", mibICMP.stats.icmpInStats.dwEchoReps);
		m_strTimeI.Format("%u", mibICMP.stats.icmpInStats.dwTimestamps);
		m_strTimeReplI.Format("%u", mibICMP.stats.icmpInStats.dwTimestampReps);
		m_strAddrI.Format("%u", mibICMP.stats.icmpInStats.dwAddrMasks);
		m_strAddrReplI.Format("%u", mibICMP.stats.icmpInStats.dwAddrMaskReps);

		m_strMsgsO.Format("%u", mibICMP.stats.icmpOutStats.dwMsgs);
		m_strErrorsO.Format("%u", mibICMP.stats.icmpOutStats.dwErrors);
		m_strDstUnrO.Format("%u", mibICMP.stats.icmpOutStats.dwDestUnreachs);
		m_strTTLExcO.Format("%u", mibICMP.stats.icmpOutStats.dwTimeExcds);
		m_strParamProbsO.Format("%u", mibICMP.stats.icmpOutStats.dwParmProbs);
		m_strSrcQO.Format("%u", mibICMP.stats.icmpOutStats.dwSrcQuenchs);
		m_strRedirO.Format("%u", mibICMP.stats.icmpOutStats.dwRedirects);
		m_strEchoO.Format("%u", mibICMP.stats.icmpOutStats.dwEchos);
		m_strEchoReplO.Format("%u", mibICMP.stats.icmpOutStats.dwEchoReps);
		m_strTimeO.Format("%u", mibICMP.stats.icmpOutStats.dwTimestamps);
		m_strTimeReplO.Format("%u", mibICMP.stats.icmpOutStats.dwTimestampReps);
		m_strAddrO.Format("%u", mibICMP.stats.icmpOutStats.dwAddrMasks);
		m_strAddrReplO.Format("%u", mibICMP.stats.icmpOutStats.dwAddrMaskReps);
	}
	else
	{
		m_strMsgsI.Format("Unable to get ICMP stats");
		m_strErrorsI.Format("Unable to get ICMP stats");
		m_strDstUnrI.Format("Unable to get ICMP stats");
		m_TTLExcI.Format("Unable to get ICMP stats");
		m_strParamProbsI.Format("Unable to get ICMP stats");
		m_strSrcQI.Format("Unable to get ICMP stats");
		m_strRedirI.Format("Unable to get ICMP stats");
		m_strEchoI.Format("Unable to get ICMP stats");
		m_strEchoReplI.Format("Unable to get ICMP stats");
		m_strTimeI.Format("Unable to get ICMP stats");
		m_strTimeReplI.Format("Unable to get ICMP stats");
		m_strAddrI.Format("Unable to get ICMP stats");
		m_strAddrReplI.Format("Unable to get ICMP stats");

		m_strMsgsO.Format("Unable to get ICMP stats");
		m_strErrorsO.Format("Unable to get ICMP stats");
		m_strDstUnrO.Format("Unable to get ICMP stats");
		m_strTTLExcO.Format("Unable to get ICMP stats");
		m_strParamProbsO.Format("Unable to get ICMP stats");
		m_strSrcQO.Format("Unable to get ICMP stats");
		m_strRedirO.Format("Unable to get ICMP stats");
		m_strEchoO.Format("Unable to get ICMP stats");
		m_strEchoReplO.Format("Unable to get ICMP stats");
		m_strTimeO.Format("Unable to get ICMP stats");
		m_strTimeReplO.Format("Unable to get ICMP stats");
		m_strAddrO.Format("Unable to get ICMP stats");
		m_strAddrReplO.Format("Unable to get ICMP stats");
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CICMPStats::OnRefresh() 
{
	MIB_ICMP  mibICMP;

	// Get the ICMP statistics
	if(GetIcmpStatistics(&mibICMP) == NO_ERROR)
	{
		m_strMsgsI.Format("%u", mibICMP.stats.icmpInStats.dwMsgs);
		m_strErrorsI.Format("%u", mibICMP.stats.icmpInStats.dwErrors);
		m_strDstUnrI.Format("%u", mibICMP.stats.icmpInStats.dwDestUnreachs);
		m_TTLExcI.Format("%u", mibICMP.stats.icmpInStats.dwTimeExcds);
		m_strParamProbsI.Format("%u", mibICMP.stats.icmpInStats.dwParmProbs);
		m_strSrcQI.Format("%u", mibICMP.stats.icmpInStats.dwSrcQuenchs);
		m_strRedirI.Format("%u", mibICMP.stats.icmpInStats.dwRedirects);
		m_strEchoI.Format("%u", mibICMP.stats.icmpInStats.dwEchos);
		m_strEchoReplI.Format("%u", mibICMP.stats.icmpInStats.dwEchoReps);
		m_strTimeI.Format("%u", mibICMP.stats.icmpInStats.dwTimestamps);
		m_strTimeReplI.Format("%u", mibICMP.stats.icmpInStats.dwTimestampReps);
		m_strAddrI.Format("%u", mibICMP.stats.icmpInStats.dwAddrMasks);
		m_strAddrReplI.Format("%u", mibICMP.stats.icmpInStats.dwAddrMaskReps);

		m_strMsgsO.Format("%u", mibICMP.stats.icmpOutStats.dwMsgs);
		m_strErrorsO.Format("%u", mibICMP.stats.icmpOutStats.dwErrors);
		m_strDstUnrO.Format("%u", mibICMP.stats.icmpOutStats.dwDestUnreachs);
		m_strTTLExcO.Format("%u", mibICMP.stats.icmpOutStats.dwTimeExcds);
		m_strParamProbsO.Format("%u", mibICMP.stats.icmpOutStats.dwParmProbs);
		m_strSrcQO.Format("%u", mibICMP.stats.icmpOutStats.dwSrcQuenchs);
		m_strRedirO.Format("%u", mibICMP.stats.icmpOutStats.dwRedirects);
		m_strEchoO.Format("%u", mibICMP.stats.icmpOutStats.dwEchos);
		m_strEchoReplO.Format("%u", mibICMP.stats.icmpOutStats.dwEchoReps);
		m_strTimeO.Format("%u", mibICMP.stats.icmpOutStats.dwTimestamps);
		m_strTimeReplO.Format("%u", mibICMP.stats.icmpOutStats.dwTimestampReps);
		m_strAddrO.Format("%u", mibICMP.stats.icmpOutStats.dwAddrMasks);
		m_strAddrReplO.Format("%u", mibICMP.stats.icmpOutStats.dwAddrMaskReps);
	}
	else
	{
		m_strMsgsI.Format("Unable to get ICMP stats");
		m_strErrorsI.Format("Unable to get ICMP stats");
		m_strDstUnrI.Format("Unable to get ICMP stats");
		m_TTLExcI.Format("Unable to get ICMP stats");
		m_strParamProbsI.Format("Unable to get ICMP stats");
		m_strSrcQI.Format("Unable to get ICMP stats");
		m_strRedirI.Format("Unable to get ICMP stats");
		m_strEchoI.Format("Unable to get ICMP stats");
		m_strEchoReplI.Format("Unable to get ICMP stats");
		m_strTimeI.Format("Unable to get ICMP stats");
		m_strTimeReplI.Format("Unable to get ICMP stats");
		m_strAddrI.Format("Unable to get ICMP stats");
		m_strAddrReplI.Format("Unable to get ICMP stats");

		m_strMsgsO.Format("Unable to get ICMP stats");
		m_strErrorsO.Format("Unable to get ICMP stats");
		m_strDstUnrO.Format("Unable to get ICMP stats");
		m_strTTLExcO.Format("Unable to get ICMP stats");
		m_strParamProbsO.Format("Unable to get ICMP stats");
		m_strSrcQO.Format("Unable to get ICMP stats");
		m_strRedirO.Format("Unable to get ICMP stats");
		m_strEchoO.Format("Unable to get ICMP stats");
		m_strEchoReplO.Format("Unable to get ICMP stats");
		m_strTimeO.Format("Unable to get ICMP stats");
		m_strTimeReplO.Format("Unable to get ICMP stats");
		m_strAddrO.Format("Unable to get ICMP stats");
		m_strAddrReplO.Format("Unable to get ICMP stats");
	}

	UpdateData(FALSE);
	
}
