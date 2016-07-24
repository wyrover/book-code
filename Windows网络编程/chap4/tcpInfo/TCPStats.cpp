// TCPStats.cpp : implementation file
//

#include "stdafx.h"
#include "TCPInfo.h"
#include "TCPStats.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTCPStats dialog


CTCPStats::CTCPStats(CWnd* pParent /*=NULL*/)
	: CDialog(CTCPStats::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTCPStats)
	m_strAlgo = _T("");
	m_strMinTimeout = _T("");
	m_strMaxTimeout = _T("");
	m_strMaxConn = _T("");
	m_strActiveConn = _T("");
	m_strPassiveConn = _T("");
	m_strFailedAttempts = _T("");
	m_strConnReset = _T("");
	m_strEsatblishedConn = _T("");
	m_strSegmentsRec = _T("");
	m_strSegmentSnd = _T("");
	m_strSegRetrans = _T("");
	m_strErrRec = _T("");
	m_strSegWithRST = _T("");
	m_strCumConn = _T("");
	//}}AFX_DATA_INIT
}


void CTCPStats::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTCPStats)
	DDX_Text(pDX, IDC_STATIC1, m_strAlgo);
	DDX_Text(pDX, IDC_STATIC2, m_strMinTimeout);
	DDX_Text(pDX, IDC_STATIC3, m_strMaxTimeout);
	DDX_Text(pDX, IDC_STATIC4, m_strMaxConn);
	DDX_Text(pDX, IDC_STATIC5, m_strActiveConn);
	DDX_Text(pDX, IDC_STATIC6, m_strPassiveConn);
	DDX_Text(pDX, IDC_STATIC7, m_strFailedAttempts);
	DDX_Text(pDX, IDC_STATIC8, m_strConnReset);
	DDX_Text(pDX, IDC_STATIC9, m_strEsatblishedConn);
	DDX_Text(pDX, IDC_STATIC10, m_strSegmentsRec);
	DDX_Text(pDX, IDC_STATIC11, m_strSegmentSnd);
	DDX_Text(pDX, IDC_STATIC12, m_strSegRetrans);
	DDX_Text(pDX, IDC_STATIC13, m_strErrRec);
	DDX_Text(pDX, IDC_STATIC14, m_strSegWithRST);
	DDX_Text(pDX, IDC_STATIC15, m_strCumConn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTCPStats, CDialog)
	//{{AFX_MSG_MAP(CTCPStats)
	ON_BN_CLICKED(IDC_BUTTON1, OnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTCPStats message handlers

BOOL CTCPStats::OnInitDialog() 
{
	CDialog::OnInitDialog();

	MIB_TCPSTATS  mibTcpStats;

	// Get the TCP stats
	if(GetTcpStatistics(&mibTcpStats) == NO_ERROR)
	{
		switch(mibTcpStats.dwRtoAlgorithm)
		{
		case MIB_TCP_RTO_CONSTANT:
			m_strAlgo.Format("Constant Time-Out");
			break;
		case MIB_TCP_RTO_RSRE:
			m_strAlgo.Format("MIL-STD-1778");
			break;
		case MIB_TCP_RTO_VANJ:
			m_strAlgo.Format("Van Jacobson's Algorithm");
			break;
		case MIB_TCP_RTO_OTHER:
			m_strAlgo.Format("Unknown algorithm");
			break;
		default:
			m_strAlgo.Format("Unknown algorithm");
		}
		
		m_strMinTimeout.Format("%u", mibTcpStats.dwRtoMin);
		m_strMaxTimeout.Format("%u", mibTcpStats.dwRtoMax);
		m_strMaxConn.Format("%u", mibTcpStats.dwMaxConn);
		m_strActiveConn.Format("%u", mibTcpStats.dwActiveOpens);
		m_strPassiveConn.Format("%u", mibTcpStats.dwPassiveOpens);
		m_strFailedAttempts.Format("%u", mibTcpStats.dwAttemptFails);
		m_strConnReset.Format("%u", mibTcpStats.dwEstabResets);
		m_strEsatblishedConn.Format("%u", mibTcpStats.dwCurrEstab);
		m_strSegmentsRec.Format("%u", mibTcpStats.dwInSegs);
		m_strSegmentSnd.Format("%u", mibTcpStats.dwOutSegs);
		m_strSegRetrans.Format("%u", mibTcpStats.dwRetransSegs);
		m_strErrRec.Format("%u", mibTcpStats.dwInErrs);
		m_strSegWithRST.Format("%u", mibTcpStats.dwOutRsts);
		m_strCumConn.Format("%u", mibTcpStats.dwNumConns);
	}	
	else
	{
		m_strAlgo.Format("Unable to get Tcp stats");
		m_strMinTimeout.Format("Unable to get Tcp stats");
		m_strMaxTimeout.Format("Unable to get Tcp stats");
		m_strMaxConn.Format("Unable to get Tcp stats");
		m_strActiveConn.Format("Unable to get Tcp stats");
		m_strPassiveConn.Format("Unable to get Tcp stats");
		m_strFailedAttempts.Format("Unable to get Tcp stats");
		m_strConnReset.Format("Unable to get Tcp stats");
		m_strEsatblishedConn.Format("Unable to get Tcp stats");
		m_strSegmentsRec.Format("Unable to get Tcp stats");
		m_strSegmentSnd.Format("Unable to get Tcp stats");
		m_strSegRetrans.Format("Unable to get Tcp stats");
		m_strErrRec.Format("Unable to get Tcp stats");
		m_strSegWithRST.Format("Unable to get Tcp stats");
		m_strCumConn.Format("Unable to get Tcp stats");
	}
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTCPStats::OnRefresh() 
{
	MIB_TCPSTATS  mibTcpStats;

	if(GetTcpStatistics(&mibTcpStats) == NO_ERROR)
	{
		switch(mibTcpStats.dwRtoAlgorithm)
		{
		case MIB_TCP_RTO_CONSTANT:
			m_strAlgo.Format("Constant Time-Out");
			break;
		case MIB_TCP_RTO_RSRE:
			m_strAlgo.Format("MIL-STD-1778");
			break;
		case MIB_TCP_RTO_VANJ:
			m_strAlgo.Format("Van Jacobson's Algorithm");
			break;
		case MIB_TCP_RTO_OTHER:
			m_strAlgo.Format("Unknown algorithm");
			break;
		default:
			m_strAlgo.Format("Unknown algorithm");
		}
		
		m_strMinTimeout.Format("%u", mibTcpStats.dwRtoMin);
		m_strMaxTimeout.Format("%u", mibTcpStats.dwRtoMax);
		m_strMaxConn.Format("%u", mibTcpStats.dwMaxConn);
		m_strActiveConn.Format("%u", mibTcpStats.dwActiveOpens);
		m_strPassiveConn.Format("%u", mibTcpStats.dwPassiveOpens);
		m_strFailedAttempts.Format("%u", mibTcpStats.dwAttemptFails);
		m_strConnReset.Format("%u", mibTcpStats.dwEstabResets);
		m_strEsatblishedConn.Format("%u", mibTcpStats.dwCurrEstab);
		m_strSegmentsRec.Format("%u", mibTcpStats.dwInSegs);
		m_strSegmentSnd.Format("%u", mibTcpStats.dwOutSegs);
		m_strSegRetrans.Format("%u", mibTcpStats.dwRetransSegs);
		m_strErrRec.Format("%u", mibTcpStats.dwInErrs);
		m_strSegWithRST.Format("%u", mibTcpStats.dwOutRsts);
		m_strCumConn.Format("%u", mibTcpStats.dwNumConns);
	}	
	else
	{
		m_strAlgo.Format("Unable to get Tcp stats");
		m_strMinTimeout.Format("Unable to get Tcp stats");
		m_strMaxTimeout.Format("Unable to get Tcp stats");
		m_strMaxConn.Format("Unable to get Tcp stats");
		m_strActiveConn.Format("Unable to get Tcp stats");
		m_strPassiveConn.Format("Unable to get Tcp stats");
		m_strFailedAttempts.Format("Unable to get Tcp stats");
		m_strConnReset.Format("Unable to get Tcp stats");
		m_strEsatblishedConn.Format("Unable to get Tcp stats");
		m_strSegmentsRec.Format("Unable to get Tcp stats");
		m_strSegmentSnd.Format("Unable to get Tcp stats");
		m_strSegRetrans.Format("Unable to get Tcp stats");
		m_strErrRec.Format("Unable to get Tcp stats");
		m_strSegWithRST.Format("Unable to get Tcp stats");
		m_strCumConn.Format("Unable to get Tcp stats");
	}
	
	UpdateData(FALSE);
}
