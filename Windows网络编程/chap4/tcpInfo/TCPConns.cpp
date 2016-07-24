// TCPConns.cpp : implementation file
//

#include "stdafx.h"
#include "TCPInfo.h"
#include "TCPConns.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTCPConns dialog


CTCPConns::CTCPConns(CWnd* pParent /*=NULL*/)
	: CDialog(CTCPConns::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTCPConns)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTCPConns::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTCPConns)
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTCPConns, CDialog)
	//{{AFX_MSG_MAP(CTCPConns)
	ON_BN_CLICKED(IDOK2, OnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTCPConns message handlers

BOOL CTCPConns::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	RECT				rect;
	PMIB_TCPTABLE		tcpTable = NULL;
	BYTE				pBuffer[100*20+4];
	DWORD				dwTableSize = 0;
	in_addr	addrLoc, addrRem;

	// Setup the list box
	m_list.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	ListView_SetExtendedListViewStyle(m_list.m_hWnd, LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT  );

	m_list.DeleteAllItems();
	m_list.InsertColumn(0, "Local Address");
	m_list.InsertColumn(1, "Local Port");
	m_list.InsertColumn(2, "Remote Address");
	m_list.InsertColumn(3, "Remote Port");
	m_list.InsertColumn(4, "Status");

	m_list.GetWindowRect(&rect);
	int nTotalX = rect.right - rect.left;

	m_list.SetColumnWidth(0, nTotalX/5);
	m_list.SetColumnWidth(1, nTotalX/5);
	m_list.SetColumnWidth(2, nTotalX/5);
	m_list.SetColumnWidth(3, nTotalX/5);
	m_list.SetColumnWidth(4, nTotalX/5);


	dwTableSize = 100*20+4;

	// Get the TCP Table
	// if the buffer is small this function returns the required size in dwTableSize
	DWORD dwRet = GetTcpTable((PMIB_TCPTABLE)pBuffer, &dwTableSize, TRUE);
	if(dwRet == NO_ERROR)
	{
		tcpTable = (PMIB_TCPTABLE)pBuffer;

		for(DWORD i=0; i<tcpTable->dwNumEntries; i++)
		{
			char szTemp[100];
			char szLocAddr[100], szRemAddr[100];

			DWORD dwRemIP = htonl(tcpTable->table[i].dwRemoteAddr);
			unsigned short nRemPort = htons(tcpTable->table[i].dwRemotePort);

			DWORD dwLocIP = htonl(tcpTable->table[i].dwLocalAddr);
			unsigned short nLocPort = htons(tcpTable->table[i].dwLocalPort);

			addrLoc.S_un.S_addr = ntohl(dwLocIP);
			addrRem.S_un.S_addr = ntohl(dwRemIP);

			strcpy(szLocAddr, inet_ntoa(addrLoc));
			strcpy(szRemAddr, inet_ntoa(addrRem));
			
			m_list.InsertItem(i, "");
			m_list.SetItemText(i, 0, szLocAddr);

			sprintf(szTemp, "%u", nLocPort);
			m_list.SetItemText(i, 1, szTemp);

			m_list.SetItemText(i, 2, szRemAddr);

			sprintf(szTemp, "%u", nRemPort);
			m_list.SetItemText(i, 3, szTemp);

			switch(tcpTable->table[i].dwState)
			{
			case MIB_TCP_STATE_CLOSED:
				m_list.SetItemText(i, 4, "Closed");
				break;
			case MIB_TCP_STATE_LISTEN:
				m_list.SetItemText(i, 4, "Listening");
				break;
			case MIB_TCP_STATE_SYN_SENT:
				m_list.SetItemText(i, 4, "SYN Sent");
				break;
			case MIB_TCP_STATE_SYN_RCVD:
				m_list.SetItemText(i, 4, "SYN Received");
				break;
			case MIB_TCP_STATE_ESTAB:
				m_list.SetItemText(i, 4, "Established");
				break;
			case MIB_TCP_STATE_FIN_WAIT1:
				m_list.SetItemText(i, 4, "Waiting for FIN");
				break;
			case MIB_TCP_STATE_FIN_WAIT2:
				m_list.SetItemText(i, 4, "Waiting for FIN");
				break;
			case MIB_TCP_STATE_CLOSE_WAIT:
				m_list.SetItemText(i, 4, "Waiting for Close");
				break;
			case MIB_TCP_STATE_CLOSING:
				m_list.SetItemText(i, 4, "Closing");
				break;
			case MIB_TCP_STATE_LAST_ACK:
				m_list.SetItemText(i, 4, "Last ACK");
				break;
			case MIB_TCP_STATE_TIME_WAIT:
				m_list.SetItemText(i, 4, "Time Wait");
				break;
			case MIB_TCP_STATE_DELETE_TCB:
				m_list.SetItemText(i, 4, "TCB deleted");
				break;
			default:
				m_list.SetItemText(i, 4, "Unknown status");

			}
		}
	}
	else
	{
		m_list.SetItemText(0, 0, "Unable to get TCP Connections");
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTCPConns::OnRefresh() 
{
	PMIB_TCPTABLE		tcpTable = NULL;
	BYTE				pBuffer[100*20+4];
	DWORD				dwTableSize = 0;
	in_addr	addrLoc, addrRem;

	m_list.DeleteAllItems();

	dwTableSize = 100*20+4;
	DWORD dwRet = GetTcpTable((PMIB_TCPTABLE)pBuffer, &dwTableSize, TRUE);
	if(dwRet == NO_ERROR)
	{
		tcpTable = (PMIB_TCPTABLE)pBuffer;

		for(DWORD i=0; i<tcpTable->dwNumEntries; i++)
		{
			char szTemp[100];
			char szLocAddr[100], szRemAddr[100];

			DWORD dwRemIP = htonl(tcpTable->table[i].dwRemoteAddr);
			unsigned short nRemPort = htons(tcpTable->table[i].dwRemotePort);

			DWORD dwLocIP = htonl(tcpTable->table[i].dwLocalAddr);
			unsigned short nLocPort = htons(tcpTable->table[i].dwLocalPort);

			addrLoc.S_un.S_addr = ntohl(dwLocIP);
			addrRem.S_un.S_addr = ntohl(dwRemIP);

			strcpy(szLocAddr, inet_ntoa(addrLoc));
			strcpy(szRemAddr, inet_ntoa(addrRem));
			
			m_list.InsertItem(i, "");
			m_list.SetItemText(i, 0, szLocAddr);

			sprintf(szTemp, "%u", nLocPort);
			m_list.SetItemText(i, 1, szTemp);

			m_list.SetItemText(i, 2, szRemAddr);

			sprintf(szTemp, "%u", nRemPort);
			m_list.SetItemText(i, 3, szTemp);

			switch(tcpTable->table[i].dwState)
			{
			case MIB_TCP_STATE_CLOSED:
				m_list.SetItemText(i, 4, "Closed");
				break;
			case MIB_TCP_STATE_LISTEN:
				m_list.SetItemText(i, 4, "Listening");
				break;
			case MIB_TCP_STATE_SYN_SENT:
				m_list.SetItemText(i, 4, "SYN Sent");
				break;
			case MIB_TCP_STATE_SYN_RCVD:
				m_list.SetItemText(i, 4, "SYN Received");
				break;
			case MIB_TCP_STATE_ESTAB:
				m_list.SetItemText(i, 4, "Established");
				break;
			case MIB_TCP_STATE_FIN_WAIT1:
				m_list.SetItemText(i, 4, "Waiting for FIN");
				break;
			case MIB_TCP_STATE_FIN_WAIT2:
				m_list.SetItemText(i, 4, "Waiting for FIN");
				break;
			case MIB_TCP_STATE_CLOSE_WAIT:
				m_list.SetItemText(i, 4, "Waiting for Close");
				break;
			case MIB_TCP_STATE_CLOSING:
				m_list.SetItemText(i, 4, "Closing");
				break;
			case MIB_TCP_STATE_LAST_ACK:
				m_list.SetItemText(i, 4, "Last ACK");
				break;
			case MIB_TCP_STATE_TIME_WAIT:
				m_list.SetItemText(i, 4, "Time Wait");
				break;
			case MIB_TCP_STATE_DELETE_TCB:
				m_list.SetItemText(i, 4, "TCB deleted");
				break;
			default:
				m_list.SetItemText(i, 4, "Unknown status");

			}
		}
	}
	else
	{
		m_list.SetItemText(0, 0, "Unable to get TCP Connections");
	}

	UpdateData(FALSE);
}
