// Window Explorer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "WndExp.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "WndExpDoc.h"
#include "WndExpView.h"
#include "About.h"
#include "regclass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CWndExpApp::UWM_KILL = 0;

/////////////////////////////////////////////////////////////////////////////
// CWndExpApp

BEGIN_MESSAGE_MAP(CWndExpApp, CWinApp)
	//{{AFX_MSG_MAP(CWndExpApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWndExpApp construction

CWndExpApp::CWndExpApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWndExpApp object

CWndExpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWndExpApp initialization

/****************************************************************************
*                               subclass_About
* Inputs:
*       HWND hwnd:
* Result: void
*       
* Effect: 
*       Pops up an imitation "About" box
****************************************************************************/

static void subclass_About(HWND hwnd)
    {
     CString body;
     CString caption;
     body.LoadString(IDS_ABOUT_BODY);
     caption.LoadString(IDS_ABOUT_CAPTION);

     ::MessageBox(hwnd, body, caption, MB_ICONINFORMATION | MB_OK);
    }

/****************************************************************************
*                               handleTrackMenu
* Inputs:
*       HWND hwnd:
*	WPARAM wParam: LOWORD: Menu ID
*		       HIWORD: notification code from control,
*			       0 for menu item
*			       1 for accelerator
*	LPARAM lParam: handle of child control, 0 for menu
* Result: void
*       
* Effect: 
*       If the item is from a menu, it must be from either our dummy
*	menu or from the popup menu.  Since we have control of the
*	menu, we ensure that the codes are disjoint
****************************************************************************/

void subclass_OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam)
    {
     CWnd * wnd;
     if(HIWORD(wParam) == 0)
        { /* menu item */
	 switch(LOWORD(wParam))
	    { /* menu id */
	     case IDM_ABOUT_THIS:
		     subclass_About(hwnd);
		     break;
	     case IDM_DESTROY: // from popup or from 'Exit' menu
		     wnd = (CWnd *)::GetWindowLong(hwnd, GWL_USERDATA);
		     ::PostMessage(hwnd, WM_CLOSE, 0, 0);

		     // Note: the order is very important; we want the
		     // close message processed before the kill message.
		     // Since these all go thru the same message loop,
		     // we post this one second

		     if(wnd != NULL)
			wnd->PostMessage(CWndExpApp::UWM_KILL);

		     break;
	     case IDM_PROPERTIES:
		     // TODO: write this
		     break;
	    } /* menu id */
	} /* menu item */
     else
     if(HIWORD(wParam) == 1)
        { /* accelerator */
	} /* accelerator */
     else
        { /* notification code */
	 // TODO: log this
	} /* notification code */
    }

/****************************************************************************
*                                  trackMenu
* Inputs:
*       HWND hwnd: Window handle of current window
* Result: void
*       
* Effect: 
*       Pops up a menu and tracks it.  Messages are sent to the window
*	which activated the menu
****************************************************************************/

void trackMenu(HWND hwnd, CPoint pt)
    {
     CWnd * wnd = CWnd::FromHandle(hwnd);

     CMenu menu;
     menu.LoadMenu(IDR_CONTEXTMENU);
     CMenu * popup = menu.GetSubMenu(0);

     wnd->ClientToScreen(&pt);

     popup->TrackPopupMenu(TPM_CENTERALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,
     			wnd, NULL);

     menu.DestroyMenu();     		
    }

/****************************************************************************
*                                subclassproc
* Inputs:
*       HWND hwnd: Window handle
*	UINT msg: Message
*	WPARAM wParam: 
*	LPARAM lParam:
* Result: LRESULT
*       As per message type
* Notes:
*	This is an intermediary subclass function that we run until we
*	can attach it to an MFC class
****************************************************************************/

LRESULT CALLBACK subclassproc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
     switch(msg)
        { /* message */
	 case WM_COMMAND:
		 subclass_OnCommand(hwnd, wParam, lParam);
		 return 0;
	 case WM_RBUTTONUP:
		 trackMenu(hwnd, CPoint(LOWORD(lParam), HIWORD(lParam)));
		 return 0;
	 case WM_ACTIVATE:
		 AfxGetMainWnd()->SendMessage(CMainFrame::UWM_ACTIVATE, 
		 					     (WPARAM)hwnd, 0);
		 break;
	 case WM_PARENTNOTIFY:
		 // log message here
		 break;
	} /* message */
     return DefWindowProc(hwnd, msg, wParam, lParam);
    }

/****************************************************************************
*                      CWndExpApp::registerGenericClass
* Inputs:
*       int classid: ID of class resources
*	LPCTSTR cursorid: ID of cursor
* Result: BOOL
*       
* Effect: 
*       Registers the class
****************************************************************************/

BOOL CWndExpApp::registerGenericClass(int id, LPCTSTR cursorid)
    {
     WNDCLASSEX wcex ;

     CString s;

     s.LoadString(id);

     // Fill in window class structure with parameters that describe
     // the main window.
     wcex.cbSize        = sizeof (WNDCLASSEX) ;
     wcex.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS ;
     wcex.lpfnWndProc   = subclassproc ;
     wcex.cbClsExtra    = 0 ;
     wcex.cbWndExtra    = 0 ;
     wcex.hInstance     = AfxGetInstanceHandle() ;
     wcex.hIcon         = AfxGetApp()->LoadIcon (id) ;
     wcex.hCursor       = AfxGetApp()->LoadStandardCursor (cursorid) ;
     wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW+1) ;

     wcex.lpszMenuName  = MAKEINTRESOURCE (id) ;
     wcex.lpszClassName = (LPCTSTR)s ;
     wcex.hIconSm       = (HICON)::LoadImage (AfxGetInstanceHandle(),
				     MAKEINTRESOURCE (id),
				     IMAGE_ICON,
				     ::GetSystemMetrics (SM_CXSMICON),
				     ::GetSystemMetrics (SM_CYSMICON),
				     LR_SHARED) ;

     if(!internalRegisterClass (&wcex))
	return FALSE;

     return TRUE;     
    }

BOOL CWndExpApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_WINDOWTYPE,
		RUNTIME_CLASS(CWndExpDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CWndExpView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	//****************************************************************
	// Register our built-in generic classes
	//****************************************************************
	if(!registerGenericClass(IDS_GENERIC_OVERLAPPED, IDC_IBEAM))
	   return FALSE;
        if(!registerGenericClass(IDS_GENERIC_CHILD, IDC_CROSS))
	   return FALSE;
        if(!registerGenericClass(IDS_GENERIC_POPUP, IDC_ARROW))
	   return FALSE;
        
 	UWM_KILL = ::RegisterWindowMessage(_T("CCreateWindowEx.UWM_KILL"));

	return TRUE;
}

// App command to run the dialog
void CWndExpApp::OnAppAbout()
{
	CAbout aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CWndExpApp commands
