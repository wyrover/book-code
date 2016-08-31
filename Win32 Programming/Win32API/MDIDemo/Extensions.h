#if !defined (_EXTENSIONS_H_)
#define _EXTENSIONS_H_


//
// WINDOWSX.H corrections
//
// These definitions are incorrect in WINDOWSX.H.
//


#undef      GetWindowID
#define     GetWindowID(hwnd)            (int)(short)GetDlgCtrlID(hwnd)

#undef      HANDLE_WM_GETDLGCODE
#define     HANDLE_WM_GETDLGCODE(hwnd, wParam, lParam, fn) \
                    (LRESULT)(DWORD)(UINT)(fn)((hwnd))
#undef      FORWARD_WM_GETDLGCODE
#define     FORWARD_WM_GETDLGCODE(hwnd, fn) \
                    (UINT)(DWORD)(fn)((hwnd), WM_GETDLGCODE, 0, 0)

//
// WINDOWSX.H extensions
//
// These definitions should be in WINDOWSX.H but aren't.
//

/* int Cls_OnCaptureChanged(HWND hwnd, HWND newWnd) */
#define HANDLE_WM_CAPTURECHANGED(hwnd, wParam, lParam, fn) \
      (LRESULT)(fn)((hwnd), (HWND) lParam)
#define FORWARD_WM_CAPTURECHANGED(hwnd, newWnd, fn) \
       (LRESULT)(fn)((hwnd), WM_CAPTURECHANGED, 0, (LPARAM)(HWND)(newWnd))

/* BOOL Cls_OnContextMenu (HWND hwnd, HWND hwndCtl, int xPos, int yPos) */
#define HANDLE_WM_CONTEXTMENU(hwnd, wParam, lParam, fn) \
    (LRESULT)(DWORD)(BOOL)(fn)((hwnd), (HWND) wParam, (int)LOWORD(lParam), (int)HIWORD(lParam))
#define FORWARD_WM_CONTEXTMENU(hwnd, hwndCtl, xPos, yPos, fn) \
    (BOOL)(DWORD)(fn)((hwnd), WM_CONTEXTMENU, (WPARAM)(HWND)hwndCtl, MAKELPARAM((xPos), (yPos)))

/* void Cls_OnDisplayChange (HWND hwnd, UINT cBitsPerPixel, UINT cx, UINT cy) */
#define HANDLE_WM_DISPLAYCHANGE(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (UINT)(wParam), (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)),0L)
#define FORWARD_WM_DISPLAYCHANGE(hwnd, cBitsPerPixel, cxScreen, cyScreen, fn) \
    (void)(fn)((hwnd), WM_DISPLAYCHANGE, (WPARAM)(UINT)(cBitsPerPixel), MAKELPARAM((cxScreen), (cyScreen)))

/* void Cls_OnEnterMenuLoop(HWND hwnd, BOOL isTrackPopup) */
#define HANDLE_WM_ENTERMENULOOP(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (BOOL)wParam), 0L)
#define FORWARD_WM_ENTERMENULOOP(hwnd, isTrackPopup, fn) \
    (void)(fn)((hwnd), WM_ENTERMENULOOP, (WPARAM)(isTrackPopup), 0L)

/* void Cls_OnExitMenuLoop(HWND hwnd, BOOL isTrackPopup) */
#define HANDLE_WM_EXITMENULOOP(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (BOOL)wParam), 0L)
#define FORWARD_WM_EXITMENULOOP(hwnd, isTrackPopup, fn) \
    (void)(fn)((hwnd), WM_EXITMENULOOP, (WPARAM)(isTrackPopup), 0L)

/* void Cls_OnEnterSizeMove(HWND hwnd) */
#define HANDLE_WM_ENTERSIZEMOVE(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd)), 0L)
#define FORWARD_WM_ENTERSIZEMOVE(hwnd, fn) \
    (void)(fn)((hwnd), WM_ENTERSIZEMOVE, 0, 0)

/* void Cls_OnExitSizeMove(HWND hwnd) */
#define HANDLE_WM_EXITSIZEMOVE(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd)), 0L)
#define FORWARD_WM_EXITSIZEMOVE(hwnd, fn) \
    (void)(fn)((hwnd), WM_EXITSIZEMOVE, 0, 0)

/* void Cls_OnHelp(HWND hwnd, LPHELPINFO helpinfo) */
#define HANDLE_WM_HELP(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (LPHELPINFO)(lParam)),0L)
#define FORWARD_WM_HELP(hwnd, lphi, fn) \
    (void)(fn)((hwnd),WM_HELP,(WPARAM)0,(LPARAM)(LPHELPINFO)(lphi))

/* void Cls_OnPrintClient(HWND hwnd, HDC hdc, UINT uFlags) */
#define HANDLE_WM_PRINTCLIENT(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (HDC)(wParam), (UINT)(lParam)),0L)
#define FORWARD_WM_PRINTCLIENT(hwnd, hdc, uFlags, fn) \
    (void)(fn)((hwnd),WM_PRINTCLIENT,(WPARAM)(HDC)(wParam),(LPARAM)(UINT)(uFlags))

/* void Cls_OnSettingChange(HWND hwnd, UINT Flags, LPCTSTR key) */
#define HANDLE_WM_SETTINGCHANGE(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (UINT)(wParam),(LPCTSTR)(lParam)),0L)
#define FORWARD_WM_SETTINGCHANGE(hwnd, uiFlag, pszMetrics, fn) \
    (void)(fn)((hwnd),WM_SETTINGCHANGE,(WPARAM)(UINT)uiFlag,(LPARAM)(LPCTSTR)pszMetrics)

/* void Cls_OnUserChanged(HWND hwnd) */
#define HANDLE_WM_USERCHANGED(hwnd, wParam, lParam, fn) \
    ((fn)(hwnd),0L)
#define FORWARD_WM_USERCHANGED(hwnd, fn) \
    (void)(fn)((hwnd),WM_USERCHANGED,(WPARAM)0,(LPARAM)0)

/*void Button_Click(HWND hwnd) */
#define Button_Click(hwnd) \
    (void)SendMessage((hwnd), BM_CLICK, 0, 0)

/* int Edit_GetThumb(HWND hwnd) */
#define Edit_GetThumb(hwnd) \
    (int)SendMessage((hwnd), EM_GETTHUMB, 0, 0)

/* int Edit_CharFromPos(HWND hwnd, int x, int y) */
#define Edit_CharFromPos(hwnd, x, y) \
    (int)SendMessage((hwnd), EM_CHARFROMPOS, 0, MAKELPARAM((x), (y)))

/* void Edit_GetWordBreakProcEx(HWND hwnd) */
#define Edit_GetWordBreakProcEx(hwnd) \
    ((WORDBREAKPROCEX)SendMessage((hwndCtl), EM_GETWORDBREAKPROCEX, 0, 0))

/* BOOL Edit_LineScroll(HWND hwnd, int hscroll, int vscroll) */
#define Edit_LineScroll(hwnd, hscroll, vscroll) \
    (BOOL)SendMessage((hwnd), (WPARAM)(int)(hScroll), (LPARAM)(int)(vscroll))

/* void Edit_PosFromChar(HWND hwnd, LPPOINT pt, int pos) */
#define Edit_PosFromChar(hwnd, pt, pos) \
    (void)SendMessage((hwnd), EM_POSFROMCHAR, (WPARAM)(LPPOINT)(pt), (LPARAM)(int)(pos))

/* void Edit_SetMargins(HWND hwnd, int which, int left, int right) */
#define Edit_SetMargins(hwnd, which, left, right) \
   (void)SendMessage((hwnd), EM_SETMARGINS, (WPARAM)(which), MAKELPARAM((left), (right)))
    
/* void Edit_SetWordBreakProcEx(HWND hwnd, EDITWORDBREAKPROCEX wbp) */
#define Edit_SetWordBreakProcEx(hwndCtl, wbp) \
    ((void)SendMessage((hwndCtl), EM_SETWORDBREAKPROCEX, 0L, (LPARAM)(EDITWORDBREAKPROCEX)(wbp)))

/* HANDLE Static_SetImage(HWND hwnd, int imagetype, HANDLE himage) */
#define Static_SetImage(hwnd, imagetype, himage) \
    (HANDLE)SendMessage((hwnd), STM_SETIMAGE, (WPARAM)(imagetype), \
                              (LPARAM)(HANDLE)(himage))

/* HANDLE Static_GetImage(HWND hwnd, int imagetype) */
#define Static_GetImage(hwnd, imagetype) \
    (HANDLE)SendMessage((hwnd), STM_GETIMAGE, (WPARAM)(imagetype), 0)

/* int ListBox_AddFile(HWND hwnd, LPCTSTR filename) */
#define ListBox_AddFile(hwnd, filename) \
    (int)SendMessage((hwnd), LB_ADDFILE, 0, (LPARAM)(LPCTSTR)(filename))

/* int ListBox_GetAnchorIndex(HWND hwnd) */
#define ListBox_GetAnchorIndex(hwnd) \
   (int)SendMessage((hwnd), LB_GETANCHORINDEX, 0, 0)

/* LCID ListBox_GetLocale(HWND hwnd) */
#define ListBox_GetLocale(hwnd) \
   (LCID)SendMessage((hwnd), LB_GETLOCALE, 0, 0)

/* int ListBox_InitStorage(HWND hwnd, int nItems, int stringBytes) */
#define ListBox_InitStorage(hwnd, nItems, stringBytes) \
   (int)SendMessage((hwnd), LB_INITSTORAGE, (WPARAM)(int)(nItems), \
            (LPARAM)(int)(stringBytes))

/* int ListBox_ItemFromPoint(HWND hwnd, int x, int y) */
#define ListBox_ItemFromPoint(hwnd, x, y) \
    (int)SendMessage((hwnd), LB_ITEMFROMPOINT, 0, MAKELONG((x), (y)))

/* int ListBox_SelItemRangeEx(HWND hwnd, int first, int last) */
#define ListBox_SelItemRangeEx(hwnd, first, last) \
    (int)SendMessage((hwnd), LB_SELITEMRANGEEX, (WPARAM)(int)(first), \
                (LPARAM)(int)(last))

/* int ListBox_SetAnchorIndex(HWND hwnd, int index) */
#define ListBox_SetAnchorIndex(hwnd, index) \
    (int)SendMessage((hwnd), LB_SETANCHORINDEX, (WPARAM)(int)(index), 0)

/* LCID ListBox_SetLocale(HWND hwnd, LCID locale) */
#define ListBox_SetLocale(hwnd, locale) \
    (LCID)SendMessage((hwnd), LB_SETLOCALE, (WPARAM)(LCID)(locale), 0)

#define ComboBox_GetDroppedWidth(hwnd) \
    ((int)(DWORD)SendMessage((hwnd), CB_GETDROPPEDWIDTH, 0, 0))

/* LCID ComboBox_GetLocale(HWND hwnd) */
#define ComboBox_GetLocale(hwnd) \
   (LCID)SendMessage((hwnd), CB_GETLOCALE, 0, 0)

#define ComboBox_GetTopIndex(hwndCtl) \
    ((int)(DWORD)SendMessage((hwndCtl), CB_GETTOPINDEX, 0L, 0L))

#define ComboBox_SetDroppedWidth(hwnd, width) \
    (int)SendMessage((hwnd), CB_SETDROPPEDWIDTH, (WPARAM)(int)width, 0)

/* int ComboBox_SetHorizontalExtent(HWND hwnd, int width) */
#define ComboBox_SetHorizontalExtent(hwnd, width) \
    (void)SendMessage((hwnd), CB_SETHORIZONTALEXTENT, (WPARAM)(int)(width), 0)

/* LCID ComboBox_SetLocale(HWND hwnd, LCID locale) */
#define ComboBox_SetLocale(hwnd, locale) \
    (LCID)SendMessage((hwnd), CB_SETLOCALE, (WPARAM)(LCID)(locale), 0)

#define ComboBox_SetTopIndex(hwndCtl, indexTop) \
    ((int)(DWORD)SendMessage((hwndCtl), CB_SETTOPINDEX, (WPARAM)(int)(indexTop), 0L))


//
// COMMCTRL.H additions
//
// These definitions should be in COMMCTRL.H.
//

//
// Toolbar message macros
//

#define Toolbar_AddBitmap(hwnd, nButtons, lptbab) \
    (int)SendMessage((hwnd),TB_ADDBITMAP,(WPARAM)(UINT)(nButtons),(LPARAM)(LPTBADDBITMAP)(lptbab))

#define Toolbar_AddButtons(hwnd, uNumButtons, lpButtons) \
    (BOOL)SendMessage((hwnd),TB_ADDBUTTONS,(WPARAM)(UINT)(uNumButtons),(LPARAM)(LPTBBUTTON)(lpButtons))

#define Toolbar_AddString(hwnd, hinst, idString) \
    (int)SendMessage((hwnd),TB_ADDSTRING,(WPARAM)(HINSTANCE)(hinst),(LPARAM)(LPCTSTR)(idString))

#define Toolbar_AutoSize(hwnd) \
    (void)SendMessage((hwnd),TB_AUTOSIZE,(WPARAM)0,(LPARAM)0)

#define Toolbar_ButtonCount(hwnd) \
    (UINT)SendMessage((hwnd),TB_BUTTONCOUNT,(WPARAM)0,(LPARAM)0)

#define Toolbar_ButtonStructSize(hwnd) \
    (void)SendMessage((hwnd),TB_BUTTONSTRUCTSIZE,(WPARAM)sizeof(TBBUTTON),(LPARAM)0)

#define Toolbar_ChangeBitmap(hwnd, idButton, iBitmap) \
    (BOOL)SendMessage((hwnd),TB_CHANGEBITMAP,(WPARAM)(int)(idButton),(LPARAM)(UINT)(iBitmap))

#define Toolbar_CheckButton(hwnd, idButton, fCheck) \
    (BOOL)SendMessage((hwnd),TB_CHECKBUTTON,(WPARAM)(int)(idButton),(LPARAM)(BOOL)(fCheck))

#define Toolbar_CommandToIndex(hwnd, idButton) \
    (UINT)SendMessage((hwnd),TB_COMMANDTOINDEX,(WPARAM)(int)(idButton),(LPARAM)0)

#define Toolbar_Customize(hwnd) \
    (void)SendMessage((hwnd),TB_CUSTOMIZE,(WPARAM)0,(LPARAM)0)

#define Toolbar_DeleteButton(hwnd, iButton) \
    (BOOL)SendMessage((hwnd),TB_DELETEBUTTON,(WPARAM)(UINT)(iButton),(LPARAM)0)

#define Toolbar_EnableButton(hwnd, idButton, fEnable) \
    (BOOL)SendMessage((hwnd),TB_ENABLEBUTTON,(WPARAM)(int)(idButton),(LPARAM)(BOOL)(fEnable))

#define Toolbar_GetBitmap(hwnd, idButton) \
    (UINT)SendMessage((hwnd),TB_GETBITMAP,(WPARAM)(int)(idButton),(LPARAM)0)

#define Toolbar_GetBitmapFlags(hwnd) \
    (UINT)SendMessage((hwnd),TB_GETBITMAPFLAGS,(WPARAM)0,(LPARAM)0)

#define Toolbar_GetButton(hwnd, iButton, lpButton) \
    (BOOL)SendMessage((hwnd),TB_GETBUTTON,(WPARAM)(UINT)(iButton),(LPARAM)(LPTBBUTTON)(lpButton))

#define Toolbar_GetButtonText(hwnd, idButton, lpszText) \
    (BOOL)SendMessage((hwnd),TB_GETBUTTONTEXT,(WPARAM)(int)(idButton),(LPARAM)(LPTSTR)(lpszText))

#define Toolbar_GetItemRect(hwnd, iButton, lprc) \
    (BOOL)SendMessage((hwnd),TB_GETITEMRECT,(WPARAM)(UINT)(iButton),(LPARAM)(LPRECT)(lprc))

#define Toolbar_GetRows(hwnd) \
    (UINT)SendMessage((hwnd),TB_GETROWS,(WPARAM)0,(LPARAM)0)

#define Toolbar_GetState(hwnd, idButton) \
    (int)SendMessage((hwnd),TB_GETSTATE,(WPARAM)(int)(idButton),(LPARAM)0)

#define Toolbar_GetToolTips(hwnd) \
    (HANDLE)SendMessage((hwnd),TB_GETTOOLTIPS,(WPARAM)0,(LPARAM)0)

#define Toolbar_HideButton(hwnd, idButton, fShow) \
    (BOOL)SendMessage((hwnd),TB_HIDEBUTTON,(WPARAM)(int)(idButton),(LPARAM)(BOOL)(fShow))

#define Toolbar_Indeterminate(hwnd, idButton, fIndeterminate) \
    (BOOL)SendMessage((hwnd),TB_INDETERMINATE,(WPARAM)(int)(idButton),(LPARAM)(BOOL)(fIndeterminate))

#define Toolbar_InsertButton(hwnd, iButton, lpButton) \
    (BOOL)SendMessage((hwnd),TB_INSERTBUTTON,(WPARAM)(UINT)(iButton),(LPARAM)(LPCTBBUTTON)(lpButton))

#define Toolbar_IsButtonChecked(hwnd, idButton) \
    (int)SendMessage((hwnd),TB_ISBUTTONCHECKED,(WPARAM)(int)(idButton),(LPARAM)0)

#define Toolbar_IsButtonEnabled(hwnd, idButton) \
    (int)SendMessage((hwnd),TB_ISBUTTONENABLED,(WPARAM)(int)(idButton),(LPARAM)0)

#define Toolbar_IsButtonHidden(hwnd, idButton) \
    (int)SendMessage((hwnd),TB_ISBUTTONHIDDEN,(WPARAM)(int)(idButton),(LPARAM)0)

#define Toolbar_IsButtonIndeterminate(hwnd, idButton) \
    (int)SendMessage((hwnd),TB_ISBUTTONINDETERMINATE,(WPARAM)(int)(idButton),(LPARAM)0)

#define Toolbar_IsButtonPressed(hwnd, idButton) \
    (int)SendMessage((hwnd),TB_ISBUTTONPRESSED,(WPARAM)(int)(idButton),(LPARAM)0)

#define Toolbar_PressButton(hwnd, idButton, fPress) \
    (BOOL)SendMessage((hwnd),TB_PRESSBUTTON,(WPARAM)(int)(idButton),(LPARAM)(BOOL)(fPress))

#define Toolbar_SaveRestore(hwnd, fSave, ptbsp) \
    (void)SendMessage((hwnd),TB_SAVERESTORE,(WPARAM)(BOOL)(fSave),(LPARAM)(const TBSAVEPARAMS*)(ptbsp))

#define Toolbar_SetBitmapSize(hwnd, cx, cy) \
    (BOOL)SendMessage((hwnd),TB_SETBITMAPSIZE,(WPARAM)0,MAKELPARAM((cx), (cy)))

#define Toolbar_SetButtonSize(hwnd, cx, cy) \
    (BOOL)SendMessage((hwnd),TB_SETBUTTONSIZE,(WPARAM)0,MAKELPARAM((cx), (cy)))

#define Toolbar_SetCmdID(hwnd, iButton, idButton) \
    (BOOL)SendMessage((hwnd),TB_SETCMDID,(WPARAM)(UINT)(iButton),(LPARAM)(int)(idButton))

#define Toolbar_SetParent(hwnd, hwndParent) \
    (void)SendMessage((hwnd),TB_SETPARENT,(WPARAM)(HWND)(hwndParent),(LPARAM)0)

#define Toolbar_SetRows(hwnd, cRows, fLarger, lprc) \
    (void)SendMessage((hwnd),TB_SETROWS,(WPARAM)MAKELONG((UINT)(cRows), (BOOL)(fLarger)),(LPARAM)(LPRECT)lprc)

#define Toolbar_SetState(hwnd, idButton, fState) \
    (BOOL)SendMessage((hwnd),TB_SETSTATE,(WPARAM)(int)(idButton),(LPARAM)(UINT)(fState))

#define Toolbar_SetToolTips(hwnd, hwndToolTip) \
    (void)SendMessage((hwnd),TB_SETTOOLTIPS,(WPARAM)(HWND)(hwndToolTip,(LPARAM)0)

// 
// Progress Bar Extension Macros
//

#define Progress_SetRange(hwnd, low, high) \
   (LRESULT)SendMessage((hwnd), PBM_SETRANGE, (WPARAM) 0, (LPARAM)MAKELONG((low), (high)))

#define Progress_SetStep(hwnd, delta) \
   (int)SendMessage((hwnd), PBM_SETSTEP, (WPARAM)(delta), 0)

#define Progress_StepIt(hwnd) \
   (int)SendMessage((hwnd), PBM_STEPIT, 0, 0)

#define Progress_SetPos(hwnd, pos) \
   (int)SendMessage((hwnd), PBM_SETPOS, (WPARAM)(pos), 0)

#define Progress_DeltaPos(hwnd, delta) \
   (int)SendMessage((hwnd), PBM_DELTAPOS, (WPARAM)(delta), 0)


//
// Trackbar Extension Macros
//

#define TB_LINELEFT (TB_LINEDOWN)
#define TB_LINERIGHT (TB_LINEUP)
#define TB_PAGELEFT (TB_PAGEUP)
#define TB_PAGERIGHT (TB_PAGEDOWN)
#define TB_LEFT (TB_TOP)
#define TB_RIGHT (TB_BOTTOM)

#define Trackbar_ClearSel (hwnd, redraw) \
        (void)SendMessage((hwnd), TBM_CLEARSEL, (WPARAM)(BOOL)(redraw), 0)

#define Trackbar_ClearTicks (hwnd, redraw) \
        (void)SendMessage((hwnd), TBM_CLEARTICS, (WPARAM)(BOOL)(redraw), 0)

#define Trackbar_GetChannelRect (hwnd, rc) \
        (void)SendMessage((hwnd), TBM_GETCHANNELRECT, 0, (LPARAM)(LPRECT)(rc))

#define Trackbar_GetLineSize (hwnd) \
        (int)SendMessage((hwnd), TBM_GETLINESIZE, 0, 0)

#define Trackbar_GetNumTicks (hwnd) \
        (int)SendMessage((hwnd), TBM_GETNUMTICS, 0, 0)

#define Trackbar_GetPageSize (hwnd) \
        (int)SendMessage((hwnd), TBM_GETPAGESIZE, 0, 0)

#define Trackbar_GetPos (hwnd) \
        (int)SendMessage((hwnd), TBM_GETPOS, 0, 0)

#define Trackbar_GetPTicks (hwnd) \
        (LPINT)SendMessage((hwnd), TBM_GETPTICS, 0, 0)

#define Trackbar_GetRangeMax (hwnd) \
        (int)SendMessage((hwnd), TBM_GETRANGEMAX, 0, 0)

#define Trackbar_GetRangeMin (hwnd) \
        (int)SendMessage((hwnd), TBM_GETRANGEMIN, 0, 0)

#define Trackbar_GetSelEnd (hwnd) \
        (int)SendMessage((hwnd), TBM_GETSELEND, 0, 0)

#define Trackbar_GetSelStart (hwnd) \
        (int)SendMessage((hwnd), TBM_GETSELSTART, 0, 0)

#define Trackbar_GetThumbLength (hwnd) \
        (int)SendMessage((hwnd), TBM_GETTHUMBLENGTH, 0, 0)

#define Trackbar_GetThumbRect (hwnd, rc) \
        (void)SendMessage((hwnd), TBM_GETTHUMBRECT, 0, (LPARAM)(LPRECT)(rc))

#define Trackbar_GetTick (hwnd, index) \
        (int)SendMessage((hwnd), TBM_GETTIC, (WPARAM)(WORD)(index), 0)

#define Trackbar_GetTickPos (hwnd, index) \
        (DWORD)SendMessage((hwnd), TBM_GETTICPOS, (WPARAM)(WORD)(index), 0)

#define Trackbar_SetLineSize (hwnd, size) \
        (int)SendMessage((hwnd), TBM_SETLINESIZE, 0, (LPARAM)(LONG)(size))

#define Trackbar_SetPageSize (hwnd, size) \
        (int)SendMessage((hwnd), TBM_SETPAGESIZE, 0, (LPARAM)(LONG)(size))

#define Trackbar_SetPos (hwnd, moveit, pos) \
                (void)SendMessage((hwnd), TBM_SETPOS, (WPARAM)(BOOL)(moveit), \
                                (LPARAM)(pos))

#define Trackbar_SetRange (hwnd, redraw, lo, hi) \
                (void)SendMessage((hwnd), TBM_SETRANGE, \
                                  (WPARAM)(BOOL)(redraw), \
                                  MAKELONG((WORD)(lo), (WORD)(hi)))

#define Trackbar_SetRangeMax (hwnd, redraw, hi) \
         (void)SendMessage((hwnd), TBM_SETRANGEMAX, (WPARAM)(BOOL)(redraw), \
                                (hi))

#define Trackbar_SetRangeMin (hwnd, redraw, lo) \
         (void)SendMessage((hwnd), TBM_SETRANGEMIN, (WPARAM)(BOOL)(redraw), \
                                (lo))

#define Trackbar_SetSel (hwnd, redraw, lo, hi) \
    (void)SendMessage((hwnd), TBM_SETSEL, (WPARAM)(BOOL)(redraw), \
                        MAKELONG((WORD)(lo), (WORD)(hi))) 

#define Trackbar_SetSelEnd (hwnd, redraw, hi) \
    (void)SendMessage((hwnd), TBM_SETSELEND, (WPARAM)(BOOL)(redraw), \
                        (LPARAM)(hi))

#define Trackbar_SetSelStart (hwnd, redraw, lo) \
    (void)SendMessage((hwnd), TBM_SETSELSTART, (WPARAM)(BOOL)(redraw), \
                        (LPARAM)(lo))

#define Trackbar_SetThumbLength (hwnd, size) \
    (void)SendMessage((hwnd), TBM_SETTHUMBLENGTH, (WPARAM)(UINT)size, 0)

#define Trackbar_SetTick (hwnd, pos) \
    (BOOL)SendMessage((hwnd), TBM_SETTIC, 0, (LPARAM)(LONG)(pos))

#define Trackbar_SetTickFreq (hwnd, freq, offset) \
    (void)SendMessage((hwnd), TBM_SETTICFREQ, (WPARAM)(freq), (LPARAM)(offset))


//
// Up/Down extensions
//
#define UpDown_GetAccel(hwnd, count, pAccels) \
        (int)SendMessage((hwnd), UDM_GETACCEL, (WPARAM)(count), \
                                             (LPARAM)(LPUDACCEL)(pAccels))

#define UpDown_GetBase(hwnd) \
        (int)SendMessage((hwnd), UDM_GETBASE, 0, 0)

#define UpDown_GetBuddy(hwnd) \
        (HWND)SendMessage((hwnd), UDM_GETBUDDY, 0, 0)

#define UpDown_GetPos(hwnd) \
        (DWORD)SendMessage((hwnd), UDM_GETPOS, 0, 0)

#define UpDown_GetRange(hwnd) \
        (DWORD)SendMessage((hwnd), UDM_GETRANGE, 0, 0)

#define UpDown_SetAccel(hwnd, count, pAccels) \
        (BOOL)SendMessage((hwnd), UDM_SETACCEL, (WPARAM)(count), \
                                (LPARAM)(LPUDACCEL)(pAccels))

#define UpDown_SetBase(hwnd, base) \
        (int)SendMessage((hwnd), UDM_SETBASE, (WPARAM)(base), 0)

#define UpDown_SetBuddy(hwnd, hBuddy) \
        (HWND)SendMessage((hwnd), UDM_SETBUDDY, (WPARAM)(HWND)(hBuddy), 0)

#define UpDown_SetPos(hwnd, pos) \
        (int)SendMessage((hwnd), UDM_SETPOS, (WPARAM)(int)(pos), 0)

#define UpDown_SetRange(hwnd, hi, low) \
        (void)SendMessage((hwnd), UDM_SETRANGE, 0, \
                MAKELONG((short)(hi), (short)(low))

#endif                                  /* _EXTENSIONS_H_ */
