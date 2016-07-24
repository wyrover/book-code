// 
// EnumProt.c - Display WSAPROTOCOL_INFO information
//			    for all available protocols

// This define causes windows.h
// to include winsock2.h instead
// of winsock.h
#define _WIN32_WINNT 0x0400

#include <windows.h>
#include <winsock2.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "resource.h"
#include "EnumProt.h"

////////////////////////////////////////////////////////////

// Application name
char gszAppName[]  = {"Enumerate Protocols"};

// Window class name
char gszAppClass[] = {"EnumProtClass"};

// Instance handle
HINSTANCE ghInst;

////////////////////////////////////////////////////////////

//
// WinMain()
//
int APIENTRY WinMain(
					  HINSTANCE hInstance,
					  HINSTANCE hPrevInstance,
					  LPSTR lpCmdLine,
					  int nCmdShow
	                 )
{

	// WINSOCK_VERSION is currently defined
	// as MAKEWORD(2,2) in winsock2.h
	WORD wVersionRequested = WINSOCK_VERSION;
	WSADATA wsaData;
	MSG msg;         
	int nRet;

	//
	// Look for WinSock2              
	//
	nRet = WSAStartup(wVersionRequested, &wsaData);
	if (nRet || (wsaData.wVersion != wVersionRequested))
	{
		MessageBox(NULL, "WinSock2 not available", 
						 gszAppName,
						 MB_OK|MB_ICONERROR);
		WSACleanup();
		return 1;
	}

	//
	// Load common control DLL
	//
	InitCommonControls();

	//
	// Register Window class
	//
	if (!InitApplication(hInstance))
		return (FALSE);     

	//
	// Create the ListView window
	//
	if (!InitInstance(hInstance, nCmdShow))
		return (FALSE);

	//
	// Dispatch messages
	//
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg); 
	}

	//
	// Release WinSock
	//
	WSACleanup();
	return (msg.wParam);  
}

////////////////////////////////////////////////////////////

//
// InitApplication()
// Register the window class
//
BOOL InitApplication(HANDLE hInstance)
{
	WNDCLASS  wc;
	
	wc.style = 0;                     
	wc.lpfnWndProc = (WNDPROC)MainWndProc; 
	wc.cbClsExtra = 0;              
	wc.cbWndExtra = 0;              
	wc.hInstance = hInstance;       
	wc.hIcon = LoadIcon(hInstance, 
						MAKEINTRESOURCE(IDI_ICON));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = GetStockObject(WHITE_BRUSH); 
	wc.lpszMenuName =  MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName = gszAppClass;
	return (RegisterClass(&wc));
}

////////////////////////////////////////////////////////////

//
// InitInstance()
// Create main window
//
BOOL InitInstance(HANDLE hInstance, int nCmdShow) 
{
	HWND hWnd;

	ghInst = hInstance;

	hWnd = CreateWindow(
						gszAppClass,           
						gszAppName,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT, 
						CW_USEDEFAULT, 
						CW_USEDEFAULT, 
						CW_USEDEFAULT,
						NULL,               
						NULL,               
						hInstance,          
						NULL);

	if (!hWnd)
		return (FALSE);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd); 
	return (TRUE);      

}

////////////////////////////////////////////////////////////

//
// MainWndProc()
// Process Windows messages
//
LONG APIENTRY MainWndProc(HWND hWnd, 
						  UINT message, 
						  UINT wParam,
						  LONG lParam
						  )
{
	// Handle to the tree control
	static HWND hWndTree;		

	switch (message) 
	{
		case WM_CREATE:
			//
			// Create the tree view control
			//
			hWndTree = CreateTreeControl(hWnd);
			if (hWndTree == NULL)
				MessageBox(hWnd, 
						   "Error creating TreeView",
						   gszAppName,
						   MB_OK|MB_ICONERROR);
			//
			// Add all protocols to TreeView
			//
			AddAllProtocols(hWndTree);
			break;          

        case WM_SIZE:
			//
			// Resize the TreeView along with the window
			//
            MoveWindow(hWndTree, 
					   0, 
					   0, 
					   LOWORD(lParam), 
					   HIWORD(lParam), 
					   TRUE);
            break;

		case WM_COMMAND:
			switch( LOWORD( wParam ))
			{
				case IDM_EXIT:
					PostQuitMessage(0);
					break;
				case IDM_ABOUT:
					DialogBox(ghInst, 
							  MAKEINTRESOURCE(IDD_ABOUT), 
							  hWnd, 
							  (DLGPROC)About);
					break;
				default:
					return (DefWindowProc(hWnd, 
										  message, 
										  wParam, 
										  lParam));
			}
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return (DefWindowProc(hWnd, message, wParam, lParam));
	}
	return (0);
}

////////////////////////////////////////////////////////////

//
// CreateTreeControl()
// Setup TreeView common control
//
HWND CreateTreeControl(HWND hWndParent)
{
	HWND hwndTree;
	RECT rect;

	// Get the size and position of the parent window.
	GetClientRect(hWndParent, &rect);

	// Create the tree view control
	hwndTree = CreateWindowEx(0L,
							  WC_TREEVIEW,
							  "",
							  WS_VISIBLE | WS_CHILD |	 
							  WS_BORDER | TVS_HASLINES | 
							  TVS_HASBUTTONS |			 
							  TVS_LINESATROOT,
							  0, 
							  0,
							  rect.right - rect.left, 
							  rect.bottom - rect.top - 15,
							  hWndParent,
							  (HMENU)ID_TREEVIEW,
							  ghInst,
							  NULL);
	return (hwndTree);
}

////////////////////////////////////////////////////////////

//
// AddAllProtocols()
// Call WSAEnumProtocols()
// and add info to TreeControl
//
void AddAllProtocols(HWND hWndTree)
{
	LPBYTE pBuf;
	DWORD dwLen;
	int nRet;
	int nCount;
	LPWSAPROTOCOL_INFO pInfo;
	HTREEITEM hParent;
	HTREEITEM hParent2;
	HTREEITEM hParent3;

	//
	// Determine needed buffer size by
	// intentionally generating an error.
	//
	dwLen = 0;
	nRet = WSAEnumProtocols(NULL,
						   NULL,
						   &dwLen);
	if (nRet == SOCKET_ERROR)
	{
		// Look for the expected error
		if (WSAGetLastError() != WSAENOBUFS)
		{
			ShowWinsockError(WSAGetLastError());
			return;
		}
	}

	//
	// dwLen should now contain the needed buffer size
	// Check to see that it's at least the
	// size of 1 WSAPROTOCOL_INFO structure
	//
	if (dwLen < sizeof(WSAPROTOCOL_INFO))
	{
		MessageBox(NULL, "Internal error",
						 gszAppName,
						 MB_OK|MB_ICONERROR);
		return;
	}

	// Add 1 byte just to be paranoid
	dwLen++;
	pBuf = malloc(dwLen);
	if (pBuf == NULL)
	{
		MessageBox(NULL, 
				   "Couldn't allocate protocol buffer",
				   gszAppName,
				   MB_OK|MB_ICONERROR);
		return;
	}

	//
	// Make the "real" call
	//
	nRet = WSAEnumProtocols(NULL, 
						   (LPWSAPROTOCOL_INFO)pBuf, 
						   &dwLen);
	if (nRet == SOCKET_ERROR)
	{
		free(pBuf);
		ShowWinsockError(WSAGetLastError());
		return;
	}

	//
	// Loop through the protocols
	// nRet contains the number of 
	// protocols returned
	//
	pInfo = (LPWSAPROTOCOL_INFO)pBuf;	
	for(nCount = 0; nCount < nRet; nCount++)
	{
		// Each protocol begins at the root
		// of the tree view control
		hParent = AddTreeItem(hWndTree, 
							  TVI_ROOT, 
							  pInfo->szProtocol);

		// Service flags are added one step down
		hParent2 = AddTreeItem(hWndTree, 
							   hParent, 
							   "Service Flags");

		//
		// Helper macro for adding service flags
		//
		#define ADDSF(f, s1, s2)				\
			AddTreeItem(hWndTree,				\
				hParent2,						\
				(pInfo->dwServiceFlags1 & f) ?	\
				s1 : s2)

		ADDSF(XP1_CONNECTIONLESS, 
			"Connectionless",
			"Connection-oriented");

        ADDSF(XP1_GUARANTEED_DELIVERY,
			"Delivery guaranteed",
			"Delivery NOT guaranteed");

        ADDSF(XP1_GUARANTEED_ORDER,
			"Order guaranteed",
			"Order NOT guaranteed");

        ADDSF(XP1_MESSAGE_ORIENTED,
			"Message boundaries preserved",
			"Message boundaries NOT preserved");

        if (pInfo->dwServiceFlags1 & XP1_PSEUDO_STREAM)
			AddTreeItem(hWndTree, hParent2, 
				"Message oriented with boundaries ignored");

        ADDSF(XP1_GRACEFUL_CLOSE,
			"Can perform graceful close",
			"Abortive close only");

        ADDSF(XP1_EXPEDITED_DATA,
			"Supports expedited data",
			"Doesn't support expedited data");

        ADDSF(XP1_CONNECT_DATA,
			"Supplies connect data",
			"Doesn't supply connect data");

        ADDSF(XP1_DISCONNECT_DATA,
			"Supplies disconnect data",
			"Doesn't supply disconnect data");

        ADDSF(XP1_SUPPORT_BROADCAST,
			"Supports broadcasts",
			"Doesn't support broadcasts");

		// Multipoint/multicast
        if (pInfo->dwServiceFlags1 & XP1_SUPPORT_MULTIPOINT)
		{
			hParent3 = AddTreeItem(hWndTree, 
								   hParent2, 
								   "Supports multicast");
			AddTreeItem(hWndTree,
						hParent3,
						(pInfo->dwServiceFlags1 & 
						XP1_MULTIPOINT_CONTROL_PLANE) ?
						"Control plane rooted" :
						"Control plane non-rooted");

			AddTreeItem(hWndTree,
						hParent3,
						(pInfo->dwServiceFlags1 & 
						XP1_MULTIPOINT_DATA_PLANE) ?
						"Data plane rooted" :
						"Data plane non-rooted");
		}
		else
			AddTreeItem(hWndTree, 
						hParent2, 
						"Doesn't support multicast");

        ADDSF(XP1_QOS_SUPPORTED,
			"Supports quality of service",
			"Doesn't support quality of service");

        if (pInfo->dwServiceFlags1 & XP1_UNI_SEND)
			AddTreeItem(hWndTree, 
					hParent2, 
					"Unidirectional in the send direction");

        if (pInfo->dwServiceFlags1 & XP1_UNI_RECV)
			AddTreeItem(hWndTree, 
					hParent2, 
					"Unidirectional in the recv direction");
 
        if (!(pInfo->dwServiceFlags1 & XP1_UNI_SEND) &&
			!(pInfo->dwServiceFlags1 & XP1_UNI_RECV))
			 AddTreeItem(hWndTree, 
					hParent2, 
					"Bidirectional sending and receiving");

        if (pInfo->dwServiceFlags1 & XP1_IFS_HANDLES)
			AddTreeItem(hWndTree, 
					hParent2, 
					"Socket descriptors are IFS handles");

        ADDSF(XP1_PARTIAL_MESSAGE,
			"Supports MSG_PARTIAL",
			"Doesn't support MSG_PARTIAL");

		// Provider flags
		hParent2 = AddTreeItem(hWndTree, 
							   hParent, 
							   "Provider Flags");

		if (pInfo->dwProviderFlags & 
				PFL_MULTIPLE_PROTO_ENTRIES)
			AddTreeItem(hWndTree, 
						hParent2, 
						"This is one behavior of two or"
						" more for this protocol");

		if (pInfo->dwProviderFlags & 
				PFL_RECOMMENDED_PROTO_ENTRY)
			AddTreeItem(hWndTree, 
						hParent2, 
						"This is the recommended entry"
						" for this protocol.");

		if (pInfo->dwProviderFlags & PFL_HIDDEN)
			AddTreeItem(hWndTree, 
						hParent2, 
						"WS2 SPI ERROR."
						" Hidden catalog entry shown");

		if (pInfo->dwProviderFlags & 
				PFL_MATCHES_PROTOCOL_ZERO)
			AddTreeItem(hWndTree, 
						hParent2, 
						"Use zero as protocol parameter");

		// Provider ID
		AddTreeItem(hWndTree, 
					hParent, 
					"GUID: %s", 
					GUIDtoString((GUID *)&pInfo->ProviderId));

		// Catalog entry
		AddTreeItem(hWndTree, 
					hParent, 
					"Catalog entry: %ld", 
					pInfo->dwCatalogEntryId);

		// Protocol chain
		switch(pInfo->ProtocolChain.ChainLen)
		{
			case 0:
				AddTreeItem(hWndTree, 
							hParent, 
							"Layered protocol");
				break;
			case 1:
				AddTreeItem(hWndTree, 
							hParent, 
							"Base protocol");
				break;
			default:
				if (pInfo->ProtocolChain.ChainLen > 1)
					AddTreeItem(hWndTree, 
								hParent, 
								"Protocol chain");
				else
					AddTreeItem(hWndTree, 
								hParent, 
								"SPI ERROR:"
								" Invalid ChainLen");
		}

		// Version
		AddTreeItem(hWndTree, 
					hParent, 
					"Version: %d", 
					pInfo->iVersion);

		// Address family
		AddTreeItem(hWndTree, 
					hParent, 
					"Address family: %s",
					AFtoSTR(pInfo->iAddressFamily));

		// Min/Max SOCKADDR size
		AddTreeItem(hWndTree, 
					hParent, 
					"Min/Max Address length: %d/%d",
					pInfo->iMinSockAddr,
					pInfo->iMaxSockAddr);

		// Socket type
		switch(pInfo->iSocketType)
		{
			case SOCK_STREAM:
				AddTreeItem(hWndTree, 
							hParent, 
							"SOCK_STREAM");
				break;
			case SOCK_DGRAM:
				AddTreeItem(hWndTree, 
							hParent, 
							"SOCK_DGRAM");
				break;
			case SOCK_RAW:
				AddTreeItem(hWndTree, 
							hParent, 
							"SOCK_RAW");
				break;
			case SOCK_RDM:
				AddTreeItem(hWndTree, 
							hParent, 
							"SOCK_RDM");
				break;
			case SOCK_SEQPACKET:
				AddTreeItem(hWndTree, 
							hParent, 
							"SOCK_SEQPACKET");
				break;
			default:
				AddTreeItem(hWndTree, 
							hParent, 
							"Unknown");
		}

		// Protocol
		AddTreeItem(hWndTree, 
					hParent, 
					"Protocol: %s",
					PROTOtoSTR(pInfo->iProtocol));

		// Protocol max offset
		AddTreeItem(hWndTree, 
					hParent, 
					"Protocol Max Offset: %d", 
					pInfo->iProtocolMaxOffset);

		// Network byte order
		switch(pInfo->iNetworkByteOrder)
		{
			case BIGENDIAN:
				AddTreeItem(hWndTree, 
							hParent, 
							"Big Endian");
				break;
			case LITTLEENDIAN:
				AddTreeItem(hWndTree, 
							hParent, 
							"Little Endian");
				break;
			default:
				AddTreeItem(hWndTree, 
							hParent, 
							"Unknown");
		}

		// Security
		AddTreeItem(hWndTree, 
					hParent, 
					"Security scheme: %d",
					pInfo->iSecurityScheme);

		// Message size
		switch(pInfo->dwMessageSize)
		{
			case 0:
				AddTreeItem(hWndTree, 
							hParent, 
							"Max message size"
							" not applicable");
				break;
			case 1:
				AddTreeItem(hWndTree, 
							hParent, 
							"Max message size based on MTU");
				break;
			case 0xFFFFFFFF:
				AddTreeItem(hWndTree, 
							hParent, 
							"No limit on max message size");
				break;
			default:
				AddTreeItem(hWndTree, 
							hParent, 
							"Unknown max message size: %ld", 
							pInfo->dwMessageSize);
		}
		// Move pointer to next protocol
		pInfo++;
	}

	free(pBuf);
	return;
}

////////////////////////////////////////////////////////////

//
// AddTreeItem()
// Format a string and add it to a TreeView
//
HTREEITEM AddTreeItem(HWND hWndTree, 
					  HTREEITEM hParent, 
					  LPSTR lpszFormat, 
					  ...)
{
	va_list Marker;
	char szBuf[256];
	TV_ITEM tvi;
	TV_INSERTSTRUCT tvis;

	// Format the string
	va_start(Marker, lpszFormat);
	vsprintf(szBuf, lpszFormat, Marker);
	va_end(Marker);

	// Fill in the TV_ITEM structure
	tvi.mask	      = TVIF_TEXT;
	tvi.pszText	      = szBuf; 
	tvi.cchTextMax    = lstrlen(szBuf); 

	// Fill in the TV_INSERTSTRUCT
	tvis.item         = tvi;
	tvis.hInsertAfter = TVI_LAST;
	tvis.hParent       = hParent; 
	return (TreeView_InsertItem(hWndTree, &tvis));
}


// 
// About()
// About dialog box procedure
//
BOOL APIENTRY About(
   HWND hDlg,
	UINT message,
	UINT wParam,
	LONG lParam)
{
	switch (message)
	{
	   case WM_INITDIALOG:
		  return (TRUE);

	   case WM_COMMAND:              
		  if (LOWORD(wParam) == IDOK)
		 {
			  EndDialog(hDlg, TRUE);
			  return (TRUE);
		  }
		  break;
	}
	return (FALSE);   

}


void ShowWinsockError(int iError)
{
	char szMessage[256];

	if (LoadString(ghInst, 
				   iError, 
				   szMessage, 
				   sizeof(szMessage)))
	{
		MessageBox(NULL, szMessage,
						 gszAppName,
						 MB_OK|MB_ICONERROR);

	}
	else
	{
		MessageBox(NULL, "Unknown Winsock Error",
						 gszAppName,
						 MB_OK|MB_ICONERROR);

	}
	return;
}
