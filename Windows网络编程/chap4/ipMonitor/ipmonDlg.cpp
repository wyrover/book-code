// ipmonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ipmon.h"
#include "ipmonDlg.h"
#include "mstcpip.h"
#include "iphlpapi.h"
#include "winsock2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


PROTN2T aOfProto [ PROTO_NUM + 1] = 
{  
	{ IPPROTO_IP   , "IP" },
	{ IPPROTO_ICMP , "ICMP" },  
	{ IPPROTO_IGMP , "IGMP" }, 
	{ IPPROTO_GGP  , "GGP" },  
	{ IPPROTO_TCP  , "TCP" },  
	{ IPPROTO_PUP  , "PUP" },  
	{ IPPROTO_UDP  , "UDP" },  
	{ IPPROTO_IDP  , "IDP" },  
	{ IPPROTO_ND   , "NP"  },  
	{ IPPROTO_RAW  , "RAW" },  
	{ IPPROTO_MAX  , "MAX" },
	{ NULL , "" } 
} ;  

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIpmonDlg dialog

CIpmonDlg::CIpmonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIpmonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIpmonDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_threadID = 0  ;
	m_Multihomed = FALSE ;
	m_Local    = TRUE ;
}

void CIpmonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIpmonDlg)
	DDX_Control(pDX, IDC_IPADDRESS2, m_ipctrlhost);
	DDX_Control(pDX, IDC_CHECK1, m_check);
	DDX_Control(pDX, IDC_BUTTON1, m_start);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ipctrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIpmonDlg, CDialog)
	//{{AFX_MSG_MAP(CIpmonDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIpmonDlg message handlers

BOOL CIpmonDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

    // Here read all IPs of this host
	DWORD dwSize = 0 ;
    GetIpAddrTable( NULL , &dwSize, FALSE ) ;
    PMIB_IPADDRTABLE pIpAddrTable = (PMIB_IPADDRTABLE )new BYTE [ dwSize ] ; 
	if( pIpAddrTable )
	{
		if( GetIpAddrTable( (PMIB_IPADDRTABLE)pIpAddrTable,   // // buffer for IP table
							&dwSize,                // size of buffer
							FALSE                  // sort by IP address 
							) == NO_ERROR )
		{
 			if(  pIpAddrTable->dwNumEntries > 2 ) // Second is MS TCP loopback IP ( 127.0.0.1 )
			{
				m_Multihomed = TRUE ;
				char szIP[16];
				for( int i = 0 ; i < (int)pIpAddrTable->dwNumEntries ; i++ )
				{
					in_addr ina ;
					ina.S_un.S_addr = pIpAddrTable->table[i].dwAddr ;
 					char *pIP = inet_ntoa( ina ) ;
					strcpy( szIP , pIP ) ;
					if( stricmp( szIP , "127.0.0.1" ) )
						m_IPArr.Add(pIpAddrTable->table[i].dwAddr) ;
				}
			}
		}
		delete [] pIpAddrTable ;
	}
	m_check.SetCheck(TRUE) ;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIpmonDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIpmonDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIpmonDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CIpmonDlg::IsIpofHost( IPHEADER *pIpHeader )
{
	BOOL bRet = FALSE ;
	if( m_ipcheckedhost ) // check some host like "Windump host host-name"
	{
 		if( ( pIpHeader->sourceIP == m_ipcheckedhost ) || 
			( pIpHeader->destIP == m_ipcheckedhost ) )
			bRet = TRUE ;
	}
	else 
		if( TRUE == m_Local )
		{
			if( FALSE == m_Multihomed )
			{
 				if( ( pIpHeader->sourceIP == m_iphostsource ) || 
					( pIpHeader->destIP == m_iphostsource ) )
					bRet = TRUE ;
			}
			else
			{
				int iCount = m_IPArr.GetSize() ;
				for( int i = 0 ; i < iCount ; i++ )
				{
 					if( ( pIpHeader->sourceIP == m_IPArr.GetAt(i) ) || ( pIpHeader->destIP == m_IPArr.GetAt(i) ) )
						bRet = TRUE ;
				}			
			}
		}
		else
			return TRUE ; // check all network
	return bRet ;
}

char *get_proto_name( unsigned char proto )
{
	BOOL bFound = FALSE ;
	for( int i = 0 ; i < PROTO_NUM ; i++ )
	{
		if( aOfProto[i].proto == proto )
		{
			bFound = TRUE ;
			break ;
		}	
	}
	if( bFound )
		return aOfProto[i].pprototext ;
	return aOfProto[PROTO_NUM].pprototext ;
}

UINT threadFunc ( LPVOID p )
{
 	CIpmonDlg *pDlg = static_cast<CIpmonDlg *>(p) ;
	char  buf [1000] , *bufwork ;
	MSG   msg ;
	int   iRet ;
	DWORD dwErr ;
    char  *pSource , *pDest ;
	IPHEADER *pIpHeader ;
	in_addr ina ;
	char   szSource [16] , szDest[16] , szErr [ 50 ];
	char *pLastBuf = NULL ;

    PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE)	; // Force to make the queue
 	pDlg->m_threadID = GetCurrentThreadId() ;
	pDlg->m_list.ResetContent() ;
	while( TRUE )
	{
        if( PeekMessage( &msg , 0 , WM_CLOSE,WM_CLOSE,PM_NOREMOVE ) )
		{
 	        closesocket( pDlg->m_s ) ;
 	        pDlg->m_threadID = 0 ;
			// Only after you see Next message you can press on Start button 
			pDlg->m_start.EnableWindow(TRUE) ; 
			break ;
		}
		memset( buf , 0 , sizeof(buf) ) ;
		iRet = 	recv( pDlg->m_s , buf , sizeof( buf ) , 0 ) ;
		if( iRet == SOCKET_ERROR )
		{
			dwErr = WSAGetLastError() ;
			sprintf( szErr , "Error recv() = %ld " , dwErr ) ;
			continue ;
		}
		else
			if( *buf )
			{ // check IP here
//              sprintf( szErr , "recv() = %ld " , iRet ) ;
//				AfxMessageBox( szErr ) ;
				bufwork   = buf ;
        		pIpHeader = (IPHEADER *)bufwork ;
	            WORD iLen = ntohs(pIpHeader->total_len) ;
				while( TRUE )
				{
					if( iLen <= iRet )
					{
 					 	if( pDlg->IsIpofHost( pIpHeader ) )
						{ // Form the line for m_list
 							ina.S_un.S_addr = pIpHeader->sourceIP ;
							pSource = inet_ntoa( ina ) ;
							strcpy( szSource , pSource ) ;
							ina.S_un.S_addr = pIpHeader->destIP ;
							pDest = inet_ntoa( ina ) ;
							strcpy( szDest , pDest ) ;
							CString str ;
							if( pIpHeader->sourceIP == ( pDlg->m_ipcheckedhost ? 
								           pDlg->m_ipcheckedhost : pDlg->m_iphostsource ) )
								str.Format("%s > %s len = %d ttl=%d proto=%s" , szSource , szDest ,
						 					iLen , pIpHeader->ttl , get_proto_name( pIpHeader->proto ) ) ;
							else
								str.Format("%s < %s len = %d ttl=%d proto=%s" , szDest , szSource ,
						 					iLen , pIpHeader->ttl , get_proto_name( pIpHeader->proto ) ) ;
				  			pDlg->m_list.AddString( str ) ;
							// Sleep stabilize work of list , otherwise  sometimes pressing on scroll
							// cased close of program
							Sleep(50) ; 
						}
						if( iLen < iRet )
						{
							iRet -= iLen ;
							bufwork  += iLen ;
        					pIpHeader = (IPHEADER *)bufwork ;
						}
						else
							break ; // pIpHeader->total_len == iRet and go out
					}
					else
					{ // read last part of buf. I wrote it , but always recv() read exactly 
                      // the lenght of the packet
						int iLast = iLen - iRet ;
						pLastBuf = new char [ iLen ] ;
						int iReaden = iRet ;
						memcpy( pLastBuf , bufwork , iReaden ) ;
						iRet = 	recv( pDlg->m_s , pLastBuf + iReaden , iLast , 0 ) ;
						if( iRet == SOCKET_ERROR )
						{
							dwErr = WSAGetLastError() ;
							sprintf( szErr , "Error recv() = %ld " , dwErr ) ;
							break ;
						}
						else
						{
							bufwork = pLastBuf ;	
       						pIpHeader = (IPHEADER *)bufwork ;
							if( iRet == iLast )
								iRet = iLen ;
							else
							{ // read all last data
								iReaden += iRet ;
								iLast -= iRet ;
								while( TRUE )
								{
									iRet = recv( pDlg->m_s , pLastBuf +iReaden , iLast , 0 ) ;
									if( iRet == SOCKET_ERROR )
									{
										dwErr = WSAGetLastError() ;
										sprintf( szErr , "Error recv() = %ld " , dwErr ) ;
										break ;
									}
									else
									{
								        iReaden += iRet ;
								        iLast -= iRet ;
									    if( iLast <= 0 ) 
										    break ;
									}	
								} // while
							}
						}
					}	
				}   // while
				if( pLastBuf )
					delete [ ] pLastBuf ;
			}
			else
			{
				AfxMessageBox( "No data on network" ) ;
				continue ;
			}
		Sleep( 100 ) ; //  Polling each 100 millisecond
	}

	return TRUE ;
}

void CIpmonDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	char        szErr [ 50 ] , szHostName[MAX_PATH];
	DWORD       dwErr ;
    SOCKADDR_IN sa;
	int         iRet ;

	m_Local = m_check.GetCheck() ;
  	iRet = m_ipctrl.GetAddress( m_ipsource ) ;
	gethostname(szHostName, sizeof(szHostName)) ;
 	m_iphostsource = ntohl(m_ipsource) ;
  	iRet = m_ipctrlhost.GetAddress( m_iphost ) ;
 	m_ipcheckedhost = ntohl(m_iphost) ;
 	if( IsThisHostIP() ) // Check only specific IP of this host
	{
		CString str ;
		if( 0 == m_threadID )
		{
			str.LoadString( IDS_STOP ) ;
			m_start.SetWindowText( str ) ;
		}
		else
		{
			if( m_threadID )
			{
 	            PostThreadMessage(m_threadID,WM_CLOSE,0,0) ;
			    str.LoadString( IDS_START ) ;
		        m_start.SetWindowText( str ) ;
				m_start.EnableWindow(FALSE) ;
			}
			return ;
		}
    	DWORD dwBufferLen[10] ;
		DWORD dwBufferInLen= 1 ;
		DWORD dwBytesReturned = 0 ;
		m_s = socket( AF_INET , SOCK_RAW , IPPROTO_IP ) ;
		if( INVALID_SOCKET == m_s )
		{
			dwErr = WSAGetLastError() ;
			sprintf( szErr , "Error socket() = %ld " , dwErr ) ;
			AfxMessageBox( szErr ) ;
			closesocket( m_s ) ;
			return ;

		}
		int rcvtimeo = 5000 ; // 5 sec insteadof 45 as default
        if( setsockopt( m_s , SOL_SOCKET , SO_RCVTIMEO , (const char *)&rcvtimeo , sizeof(rcvtimeo) ) == SOCKET_ERROR)
		{
			dwErr = WSAGetLastError() ;
			sprintf( szErr , "Error WSAIoctl = %ld " , dwErr ) ;
			AfxMessageBox( szErr ) ;
			closesocket( m_s ) ;
			return ;
		}
		sa.sin_family = AF_INET;
 		sa.sin_port = htons(7000);
		sa.sin_addr.s_addr= m_iphostsource;
        if (bind(m_s,(PSOCKADDR)&sa, sizeof(sa)) == SOCKET_ERROR)
		{
			dwErr = WSAGetLastError() ;
			sprintf( szErr , "Error bind() = %ld " , dwErr ) ;
			AfxMessageBox( szErr ) ;
			closesocket( m_s ) ;
			return ;
		} 
        if( SOCKET_ERROR != WSAIoctl( m_s, SIO_RCVALL , &dwBufferInLen, sizeof(dwBufferInLen),             
                                      &dwBufferLen, sizeof(dwBufferLen),
									  &dwBytesReturned , NULL , NULL ) )
            AfxBeginThread( threadFunc , (LPVOID)this );
		else
		{
			dwErr = WSAGetLastError() ;
			sprintf( szErr , "Error WSAIoctl = %ld " , dwErr ) ;
			AfxMessageBox( szErr ) ;
			closesocket( m_s ) ;
			return ;
		}
	}
	else
	{
		AfxMessageBox( "IP not of this host" ) ;
	}
}

// If you want check other computers simply return TRUE always
BOOL CIpmonDlg::IsThisHostIP() 
{
	char  szHostName[MAX_PATH];
	gethostname(szHostName, sizeof(szHostName)) ;
	HOSTENT *pHE  = gethostbyaddr ( (const char *)&m_iphostsource , 4, PF_INET ) ;
	if( pHE && m_iphostsource )
	{
	    char *pDelim =  strchr( pHE->h_name , '.' ) ;
//		if( !stricmp( pDelim ? pDelim + 1 : pHE->h_name , szHostName ) )
        if(  pDelim ) *pDelim = 0 ;  
        if( !stricmp( pDelim ? pDelim + 1 : pHE->h_name , szHostName ) || 
			!stricmp( pHE->h_name , szHostName ) )
		    return TRUE ;	
		else
		    return FALSE ;
	}	
	else
		return FALSE ;	
}	


void CIpmonDlg::close_thread()
{
	if( NULL != m_threadID )
 	    PostThreadMessage(m_threadID,WM_CLOSE,0,0) ;
	if( m_IPArr.GetSize() )
	    m_IPArr.RemoveAll() ;
}

void CIpmonDlg::OnOK() 
{
	// TODO: Add extra validation here
	close_thread() ;
	CDialog::OnOK();
}

void CIpmonDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	close_thread() ;
	CDialog::OnCancel();
}
