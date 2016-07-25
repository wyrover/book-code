// File CWCOMDLG.CPP Common Dialog wrapper functions

#include <string.h>
#include "cwdialgs.h"
#include <commdlg.h>

int get_file_name_dlg (HWND hWnd,const char *pfilter,
      const char *ptitle_str, char *file_name,
      int max_len) {
    // file_name must be allocated max_len + 1 bytes 
    OPENFILENAME ofn;
    char chReplace;
    static char the_filter[256] = "";
    static char szFile[256]="", szFileTitle[256]="";
    static char title_str[256];
    static char default_extension[4];
    UINT i, cbString;
    strcpy (the_filter,pfilter);
    strcpy (szFile,pfilter);
    cbString = strlen(the_filter);
    chReplace = the_filter[cbString];
    for (i=0; the_filter[i]; i++)
	if (the_filter[i] == chReplace)
	   the_filter[i] = '\0';
    memset(&ofn, 0, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = the_filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFile = szFile;
    if (strlen(pfilter) >= 5) {
       strncpy (default_extension,pfilter + 2,3);
       default_extension[3] = '\0'; }
    else default_extension[0] = '\0';
    ofn.lpstrDefExt = default_extension;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFileTitle = szFileTitle;
    ofn.nMaxFileTitle = sizeof(szFileTitle);
    ofn.lpstrInitialDir = NULL; 
    if (strlen(ptitle_str) > 0){
       strcpy(title_str,ptitle_str);
       ofn.lpstrTitle = title_str; }
    if (!GetOpenFileName(&ofn)) return 0;
    strncpy (file_name,szFile,max_len-1);
    file_name[max_len-1] = '\0';
    return 1; }

BOOL get_rgb_color (HWND hWnd,COLORREF *init_color) {
    CHOOSECOLOR cc;
    BOOL clr_result;
    COLORREF aclrCust[16];
    int i;
    // Set custom color controls to white 
    for (i=0;i<16;i++)
        aclrCust[i] = RGB(255,255,255);
    memset(&cc,0,sizeof(CHOOSECOLOR));
    cc.lStructSize = sizeof(CHOOSECOLOR);
    cc.hwndOwner = hWnd;
    cc.rgbResult = *init_color;
    cc.lpCustColors = aclrCust;
    cc.Flags = CC_RGBINIT | CC_PREVENTFULLOPEN;
    clr_result = ChooseColor (&cc);
    if (clr_result) *init_color = cc.rgbResult;
       return clr_result; }