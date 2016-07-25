class CWnd
{
public:
	BOOL CreateEx(DWORD dwExStyle,      // extended window style
				LPCTSTR lpClassName,  // registered class name
				LPCTSTR lpWindowName, // window name
				DWORD dwStyle,        // window style
				int x,                // horizontal position of window
				int y,                // vertical position of window
				int nWidth,           // window width
				int nHeight,          // window height
				HWND hWndParent,      // handle to parent or owner window
				HMENU hMenu,          // menu handle or child identifier
				HINSTANCE hInstance,  // handle to application instance
				LPVOID lpParam);        // window-creation data
	BOOL ShowWindow(int nCmdShow);
	BOOL UpdateWindow();
public:
	HWND m_hWnd;
};

BOOL CWnd::CreateEx(DWORD dwExStyle,      // extended window style
				LPCTSTR lpClassName,  // registered class name
				LPCTSTR lpWindowName, // window name
				DWORD dwStyle,        // window style
				int x,                // horizontal position of window
				int y,                // vertical position of window
				int nWidth,           // window width
				int nHeight,          // window height
				HWND hWndParent,      // handle to parent or owner window
				HMENU hMenu,          // menu handle or child identifier
				HINSTANCE hInstance,  // handle to application instance
				LPVOID lpParam)        // window-creation data
{
	m_hWnd=::CreateWindowEx(dwExStyle,lpClassName,dwStyle,x,y,
					nWidth,nHeight,hWndParent,hMenu,hInstance,
					lpParam);
	if(m_hWnd!=NULL)
		return TRUE;
	else
		return FALSE;
}

BOOL CWnd::ShowWindow(int nCmdShow)
{
	return ::ShowWindow(m_hWnd,nCmdShow);
}

BOOL CWnd::UpdateWindow()
{
	return ::UpdateWindow(m_hWnd);
}

int WINAPI WinMain(
  HINSTANCE hInstance,      // handle to current instance
  HINSTANCE hPrevInstance,  // handle to previous instance
  LPSTR lpCmdLine,          // command line
  int nCmdShow              // show state
)
{
	WNDCLASS wndcls;
	wndcls.cbClsExtra=0;
	wndcls.cbWndExtra=0;
	......
	RegisterClass(&wndcls);

	CWnd wnd;
	wnd.CreateEx(...);
	wnd.ShowWindow(SW_SHOWNORMAL);
	wnd.UpdateWindow();

	HWND hwnd;
	hwnd=CreateWindowEx();
	::ShowWindow(hwnd,SW_SHOWNORMAL);
	::UpdateWindow(hwnd);

	......
}
