// UDPConnections.cpp : implementation file
//

#include "stdafx.h"
#include "TCPInfo.h"
#include "UDPConnections.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUDPConnections dialog


CUDPConnections::CUDPConnections(CWnd* pParent /*=NULL*/)
	: CDialog(CUDPConnections::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUDPConnections)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUDPConnections::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUDPConnections)
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUDPConnections, CDialog)
	//{{AFX_MSG_MAP(CUDPConnections)
	ON_BN_CLICKED(IDOK2, OnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUDPConnections message handlers

BOOL CUDPConnections::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	RECT				rect;
	PMIB_UDPTABLE		udpTable = NULL;
	BYTE				pBuffer[100*20+4];
	DWORD				dwTableSize = 0;
	in_addr	addrLoc;

	// Setup the list box
	m_list.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	ListView_SetExtendedListViewStyle(m_list.m_hWnd, LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT  );

	m_list.DeleteAllItems();
	m_list.InsertColumn(0, "Local Address");
	m_list.InsertColumn(1, "Local Port");

	m_list.GetWindowRect(&rect);
	int nTotalX = rect.right - rect.left;

	m_list.SetColumnWidth(0, nTotalX/2);
	m_list.SetColumnWidth(1, nTotalX/2);

	dwTableSize = 100*8+4;

	// Get the UDP table, it will contain the local address and port 
	// of the connctions involved
	DWORD dwRet = GetUdpTable((PMIB_UDPTABLE)pBuffer, &dwTableSize, TRUE);
	if(dwRet == NO_ERROR)
	{
		udpTable = (PMIB_UDPTABLE)pBuffer;

		for(DWORD i=0; i<udpTable->dwNumEntries; i++)
		{
			char szTemp[100];
			char szLocAddr[100];

			DWORD dwLocIP = htonl(udpTable->table[i].dwLocalAddr);
			unsigned short nLocPort = htons(udpTable->table[i].dwLocalPort);

			addrLoc.S_un.S_addr = ntohl(dwLocIP);
			strcpy(szLocAddr, inet_ntoa(addrLoc));
			
			m_list.InsertItem(i, "");
			m_list.SetItemText(i, 0, szLocAddr);

			sprintf(szTemp, "%u", nLocPort);
			m_list.SetItemText(i, 1, szTemp);
		}
	}
	else
	{
		m_list.SetItemText(0, 0, "Unable to get UDP Connections");
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUDPConnections::OnRefresh() 
{
	PMIB_UDPTABLE		udpTable = NULL;
	BYTE				pBuffer[100*20+4];
	DWORD				dwTableSize = 0;
	in_addr	addrLoc;

	m_list.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	ListView_SetExtendedListViewStyle(m_list.m_hWnd, LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT  );

	m_list.DeleteAllItems();

	dwTableSize = 100*8+4;
	DWORD dwRet = GetUdpTable((PMIB_UDPTABLE)pBuffer, &dwTableSize, TRUE);
	if(dwRet == NO_ERROR)
	{
		udpTable = (PMIB_UDPTABLE)pBuffer;

		for(DWORD i=0; i<udpTable->dwNumEntries; i++)
		{
			char szTemp[100];
			char szLocAddr[100];

			DWORD dwLocIP = htonl(udpTable->table[i].dwLocalAddr);
			unsigned short nLocPort = htons(udpTable->table[i].dwLocalPort);

			addrLoc.S_un.S_addr = ntohl(dwLocIP);
			strcpy(szLocAddr, inet_ntoa(addrLoc));
			
			m_list.InsertItem(i, "");
			m_list.SetItemText(i, 0, szLocAddr);

			sprintf(szTemp, "%u", nLocPort);
			m_list.SetItemText(i, 1, szTemp);
		}
	}
	else
	{
		m_list.SetItemText(0, 0, "Unable to get UDP Connections");
	}

	UpdateData(FALSE);
	
}
