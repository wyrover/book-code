// IPStats.cpp : implementation file
//

#include "stdafx.h"
#include "TCPInfo.h"
#include "IPStats.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIPStats dialog


CIPStats::CIPStats(CWnd* pParent /*=NULL*/)
	: CDialog(CIPStats::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIPStats)
	m_strForward = _T("");
	m_strDefaultTTL = _T("");
	m_strDataRec = _T("");
	m_strDataWithHdrErr = _T("");
	m_strDataWithAddrErr = _T("");
	m_strDataForw = _T("");
	m_strDataWithProtErr = _T("");
	m_strDataInDisc = _T("");
	m_strDataDeliv = _T("");
	m_strDataSnd = _T("");
	m_strDataRoutDisc = _T("");
	m_strDataNoRoutes = _T("");
	m_strReassmTimeout = _T("");
	m_strDataReqReassm = _T("");
	m_strDataReassOK = _T("");
	m_strDataReassmNoOK = _T("");
	m_strDataFragOK = _T("");
	m_strDataFragNOOK = _T("");
	m_strFragsCrt = _T("");
	m_strNoInterfaces = _T("");
	m_strLocalIPAddrs = _T("");
	m_strNoRoutes = _T("");
	m_strTotalDisc = _T("");
	//}}AFX_DATA_INIT
}


void CIPStats::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIPStats)
	DDX_Text(pDX, IDC_STATIC1, m_strForward);
	DDX_Text(pDX, IDC_STATIC2, m_strDefaultTTL);
	DDX_Text(pDX, IDC_STATIC3, m_strDataRec);
	DDX_Text(pDX, IDC_STATIC4, m_strDataWithHdrErr);
	DDX_Text(pDX, IDC_STATIC5, m_strDataWithAddrErr);
	DDX_Text(pDX, IDC_STATIC6, m_strDataForw);
	DDX_Text(pDX, IDC_STATIC7, m_strDataWithProtErr);
	DDX_Text(pDX, IDC_STATIC8, m_strDataInDisc);
	DDX_Text(pDX, IDC_STATIC9, m_strDataDeliv);
	DDX_Text(pDX, IDC_STATIC10, m_strDataSnd);
	DDX_Text(pDX, IDC_STATIC11, m_strDataRoutDisc);
	DDX_Text(pDX, IDC_STATIC12, m_strDataNoRoutes);
	DDX_Text(pDX, IDC_STATIC13, m_strReassmTimeout);
	DDX_Text(pDX, IDC_STATIC14, m_strDataReqReassm);
	DDX_Text(pDX, IDC_STATIC15, m_strDataReassOK);
	DDX_Text(pDX, IDC_STATIC16, m_strDataReassmNoOK);
	DDX_Text(pDX, IDC_STATIC17, m_strDataFragOK);
	DDX_Text(pDX, IDC_STATIC18, m_strDataFragNOOK);
	DDX_Text(pDX, IDC_STATIC19, m_strFragsCrt);
	DDX_Text(pDX, IDC_STATIC20, m_strNoInterfaces);
	DDX_Text(pDX, IDC_STATIC21, m_strLocalIPAddrs);
	DDX_Text(pDX, IDC_STATIC22, m_strNoRoutes);
	DDX_Text(pDX, IDC_STATIC23, m_strTotalDisc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIPStats, CDialog)
	//{{AFX_MSG_MAP(CIPStats)
	ON_BN_CLICKED(IDOK2, OnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIPStats message handlers

void CIPStats::OnRefresh() 
{
	MIB_IPSTATS mibIPStats;

	// Get the IP statistics
	if(GetIpStatistics(&mibIPStats) == NO_ERROR)
	{
		if(mibIPStats.dwForwarding)
			m_strForward.Format("Enabled");
		else
			m_strForward.Format("Disabled");
		
		m_strDefaultTTL.Format("%u", mibIPStats.dwDefaultTTL);
		m_strDataRec.Format("%u", mibIPStats.dwInReceives);
		m_strDataWithHdrErr.Format("%u", mibIPStats.dwInHdrErrors);
		m_strDataWithAddrErr.Format("%u", mibIPStats.dwInAddrErrors);
		m_strDataForw.Format("%u", mibIPStats.dwForwDatagrams);
		m_strDataWithProtErr.Format("%u", mibIPStats.dwInUnknownProtos);
		m_strDataInDisc.Format("%u", mibIPStats.dwInDiscards);
		m_strDataDeliv.Format("%u", mibIPStats.dwInDelivers);
		m_strDataSnd.Format("%u", mibIPStats.dwOutRequests);
		m_strDataRoutDisc.Format("%u", mibIPStats.dwRoutingDiscards );
		m_strDataNoRoutes.Format("%u", mibIPStats.dwOutNoRoutes );
		m_strReassmTimeout.Format("%u", mibIPStats.dwReasmTimeout );
		m_strDataReqReassm.Format("%u", mibIPStats.dwReasmReqds);
		m_strDataReassOK.Format("%u", mibIPStats.dwReasmOks );
		m_strDataReassmNoOK.Format("%u", mibIPStats.dwReasmFails );
		m_strDataFragOK.Format("%u", mibIPStats.dwFragOks );
		m_strDataFragNOOK.Format("%u", mibIPStats.dwFragFails );
		m_strFragsCrt.Format("%u", mibIPStats.dwFragCreates );
		m_strNoInterfaces.Format("%u", mibIPStats.dwNumIf );
		m_strLocalIPAddrs.Format("%u", mibIPStats.dwNumAddr );
		m_strNoRoutes.Format("%u", mibIPStats.dwNumRoutes );
		m_strTotalDisc.Format("%u", mibIPStats.dwOutDiscards );
	}	
	else
	{
		m_strForward.Format("Unable to get IP Stats");
		m_strDefaultTTL.Format("Unable to get IP Stats");
		m_strDataRec.Format("Unable to get IP Stats");
		m_strDataWithHdrErr.Format("Unable to get IP Stats");
		m_strDataWithAddrErr.Format("Unable to get IP Stats");
		m_strDataForw.Format("Unable to get IP Stats");
		m_strDataWithProtErr.Format("Unable to get IP Stats");
		m_strDataInDisc.Format("Unable to get IP Stats");
		m_strDataDeliv.Format("Unable to get IP Stats");
		m_strDataSnd.Format("Unable to get IP Stats");
		m_strDataRoutDisc.Format("Unable to get IP Stats");
		m_strDataNoRoutes.Format("Unable to get IP Stats");
		m_strReassmTimeout.Format("Unable to get IP Stats");
		m_strDataReqReassm.Format("Unable to get IP Stats");
		m_strDataReassOK.Format("Unable to get IP Stats");
		m_strDataReassmNoOK.Format("Unable to get IP Stats");
		m_strDataFragOK.Format("Unable to get IP Stats");
		m_strDataFragNOOK.Format("Unable to get IP Stats");
		m_strFragsCrt.Format("Unable to get IP Stats");
		m_strNoInterfaces.Format("Unable to get IP Stats");
		m_strLocalIPAddrs.Format("Unable to get IP Stats");
		m_strNoRoutes.Format("Unable to get IP Stats");
		m_strTotalDisc.Format("Unable to get IP Stats");
	}
	
	UpdateData(FALSE);
}

BOOL CIPStats::OnInitDialog() 
{
	CDialog::OnInitDialog();

	MIB_IPSTATS mibIPStats;

	// Get the IP statistics
	if(GetIpStatistics(&mibIPStats) == NO_ERROR)
	{
		if(mibIPStats.dwForwarding)
			m_strForward.Format("Enabled");
		else
			m_strForward.Format("Disabled");
		
		m_strDefaultTTL.Format("%u", mibIPStats.dwDefaultTTL);
		m_strDataRec.Format("%u", mibIPStats.dwInReceives);
		m_strDataWithHdrErr.Format("%u", mibIPStats.dwInHdrErrors);
		m_strDataWithAddrErr.Format("%u", mibIPStats.dwInAddrErrors);
		m_strDataForw.Format("%u", mibIPStats.dwForwDatagrams);
		m_strDataWithProtErr.Format("%u", mibIPStats.dwInUnknownProtos);
		m_strDataInDisc.Format("%u", mibIPStats.dwInDiscards);
		m_strDataDeliv.Format("%u", mibIPStats.dwInDelivers);
		m_strDataSnd.Format("%u", mibIPStats.dwOutRequests);
		m_strDataRoutDisc.Format("%u", mibIPStats.dwRoutingDiscards );
		m_strDataNoRoutes.Format("%u", mibIPStats.dwOutNoRoutes );
		m_strReassmTimeout.Format("%u", mibIPStats.dwReasmTimeout );
		m_strDataReqReassm.Format("%u", mibIPStats.dwReasmReqds);
		m_strDataReassOK.Format("%u", mibIPStats.dwReasmOks );
		m_strDataReassmNoOK.Format("%u", mibIPStats.dwReasmFails );
		m_strDataFragOK.Format("%u", mibIPStats.dwFragOks );
		m_strDataFragNOOK.Format("%u", mibIPStats.dwFragFails );
		m_strFragsCrt.Format("%u", mibIPStats.dwFragCreates );
		m_strNoInterfaces.Format("%u", mibIPStats.dwNumIf );
		m_strLocalIPAddrs.Format("%u", mibIPStats.dwNumAddr );
		m_strNoRoutes.Format("%u", mibIPStats.dwNumRoutes );
		m_strTotalDisc.Format("%u", mibIPStats.dwOutDiscards );
	}	
	else
	{
		m_strForward.Format("Unable to get IP Stats");
		m_strDefaultTTL.Format("Unable to get IP Stats");
		m_strDataRec.Format("Unable to get IP Stats");
		m_strDataWithHdrErr.Format("Unable to get IP Stats");
		m_strDataWithAddrErr.Format("Unable to get IP Stats");
		m_strDataForw.Format("Unable to get IP Stats");
		m_strDataWithProtErr.Format("Unable to get IP Stats");
		m_strDataInDisc.Format("Unable to get IP Stats");
		m_strDataDeliv.Format("Unable to get IP Stats");
		m_strDataSnd.Format("Unable to get IP Stats");
		m_strDataRoutDisc.Format("Unable to get IP Stats");
		m_strDataNoRoutes.Format("Unable to get IP Stats");
		m_strReassmTimeout.Format("Unable to get IP Stats");
		m_strDataReqReassm.Format("Unable to get IP Stats");
		m_strDataReassOK.Format("Unable to get IP Stats");
		m_strDataReassmNoOK.Format("Unable to get IP Stats");
		m_strDataFragOK.Format("Unable to get IP Stats");
		m_strDataFragNOOK.Format("Unable to get IP Stats");
		m_strFragsCrt.Format("Unable to get IP Stats");
		m_strNoInterfaces.Format("Unable to get IP Stats");
		m_strLocalIPAddrs.Format("Unable to get IP Stats");
		m_strNoRoutes.Format("Unable to get IP Stats");
		m_strTotalDisc.Format("Unable to get IP Stats");
	}
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
