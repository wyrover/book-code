LPDLGITEMTEMPLATE CONTROL(LPDLGITEMTEMPLATE item, LPCWSTR text, 
					short id, LPCWSTR classname, 
					DWORD style, 
					int x, int y, int cx, int cy, 
					DWORD exstyle);

LPDLGITEMTEMPLATE CTEXT(LPDLGITEMTEMPLATE item, LPCWSTR text, short id, 
			int x, int y, int cx, int cy, DWORD style, 
			DWORD exstyle);

LPDLGITEMTEMPLATE PUSHBUTTON(LPDLGITEMTEMPLATE item, LPCWSTR text,
			short id, int x, int y, int cx, int cy, 
			DWORD style, DWORD exstyle);

LPDLGITEMTEMPLATE DEFPUSHBUTTON(LPDLGITEMTEMPLATE item, LPCWSTR text,
			short id, int x, int y, int cx, int cy, 
			DWORD style, DWORD exstyle);

LPDLGITEMTEMPLATE ICON(LPDLGITEMTEMPLATE item, LPCWSTR icon, 
					short id, int x, int y, int cx, int cy,
					DWORD style, DWORD exstyle);

LPDLGITEMTEMPLATE LTEXT(LPDLGITEMTEMPLATE item, LPCWSTR text, short id, 
			int x, int y, int cx, int cy, DWORD style, 
			DWORD exstyle);

LPDLGITEMTEMPLATE RTEXT(LPDLGITEMTEMPLATE item, LPCWSTR text, short id, 
			int x, int y, int cx, int cy, DWORD style, 
			DWORD exstyle);

LPDLGTEMPLATE DIALOG(LPDLGITEMTEMPLATE * item,
			UINT buffsize, int x, int y, int cx, int cy,
			DWORD style, DWORD exstyle, LPCWSTR menu,
			LPCWSTR class, LPCWSTR caption, LPCWSTR font, 
			int height);
