#define DLGCX_VALID 0x80000000
#define DLGCX_ACTIVE 0x40000000

__inline UINT getDlgCode(HWND m_hWnd) { return ::GetWindowLong(m_hWnd, GWL_USERDATA);}
__inline void setDlgCode(HWND m_hWnd, UINT code) { ::SetWindowLong(m_hWnd, GWL_USERDATA, code); }
