typedef struct {
	int ctlid;
	DWORD style;} controlmap;

DWORD controlsToFlags(CWnd * wnd, controlmap * map, BOOL useall = FALSE);
