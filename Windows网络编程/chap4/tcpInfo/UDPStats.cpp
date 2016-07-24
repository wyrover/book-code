// UDPStats.cpp : implementation file
//

#include "stdafx.h"
#include "TCPInfo.h"
#include "UDPStats.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUDPStats dialog


CUDPStats::CUDPStats(CWnd* pParent /*=NULL*/)
	: CDialog(CUDPStats::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUDPStats)
	m_strDataRec = _T("");
	m_strDataNoPort = _T("");
	m_strErrOnRec = _T("");
	m_strDataSnd = _T("");
	m_strListenConn = _T("");
	//}}AFX_DATA_INIT
}


void CUDPStats::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUDPStats)
	DDX_Text(pDX, IDC_STATIC1, m_strDataRec);
	DDX_Text(pDX, IDC_STATIC2, m_strDataNoPort);
	DDX_Text(pDX, IDC_STATIC3, m_strErrOnRec);
	DDX_Text(pDX, IDC_STATIC4, m_strDataSnd);
	DDX_Text(pDX, IDC_STATIC5, m_strListenConn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUDPStats, CDialog)
	//{{AFX_MSG_MAP(CUDPStats)
	ON_BN_CLICKED(IDC_BUTTON1, OnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUDPStats message handlers

BOOL CUDPStats::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	MIB_UDPSTATS mibUdpStats;

	// Get the UDP connections
	if(GetUdpStatistics(&mibUdpStats) == NO_ERROR)
	{
		m_strDataRec.Format("%u", mibUdpStats.dwInDatagrams);
		m_strDataNoPort.Format("%u", mibUdpStats.dwNoPorts);
		m_strErrOnRec.Format("%u", mibUdpStats.dwInErrors);
		m_strDataSnd.Format("%u", mibUdpStats.dwOutDatagrams);
		m_strListenConn.Format("%u", mibUdpStats.dwNumAddrs);
	}
	else
	{
		m_strDataRec.Format("Unable to get UDP stats");
		m_strDataNoPort.Format("Unable to get UDP stats");
		m_strErrOnRec.Format("Unable to get UDP stats");
		m_strDataSnd.Format("Unable to get UDP stats");
		m_strListenConn.Format("Unable to get UDP stats");
	}

	UpdateData(FALSE);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUDPStats::OnRefresh() 
{
	MIB_UDPSTATS mibUdpStats;

	if(GetUdpStatistics(&mibUdpStats) == NO_ERROR)
	{
		m_strDataRec.Format("%u", mibUdpStats.dwInDatagrams);
		m_strDataNoPort.Format("%u", mibUdpStats.dwNoPorts);
		m_strErrOnRec.Format("%u", mibUdpStats.dwInErrors);
		m_strDataSnd.Format("%u", mibUdpStats.dwOutDatagrams);
		m_strListenConn.Format("%u", mibUdpStats.dwNumAddrs);
	}
	else
	{
		m_strDataRec.Format("Unable to get UDP stats");
		m_strDataNoPort.Format("Unable to get UDP stats");
		m_strErrOnRec.Format("Unable to get UDP stats");
		m_strDataSnd.Format("Unable to get UDP stats");
		m_strListenConn.Format("Unable to get UDP stats");
	}

	UpdateData(FALSE);
	
}
