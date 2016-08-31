typedef struct {
	BOOL subclass;
	BOOL docolor;
	HMENU menu;
	DWORD styles;
	       } options, *LPoptions;

int setOptions(HWND hwnd, LPoptions opts);
void styleToTitle(HWND hwnd, DWORD styles, LPCTSTR suffix);

extern options CustomOptions;
