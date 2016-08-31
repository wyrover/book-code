//=============================================================================
//                              SIBLING_SETFONT
// wParam: LOWORD: int: CTLCOLOR_ code
//         HIWORD: int: SIBLING_SETFONT
// lParam:      CWnd *: Handle of window whose font is to be set
//
// Result: void
#define SIBLING_SETFONT 1

//=============================================================================
//                               SIBLING_SETDC
// wParam: LOWORD: int: CTLCOLOR_ code
//         HIWORD: int: SIBLING_SETDC
// lParam:      CDC *: Handle of DC whose parameters are to be set
//
// Result: HBRUSH
//      Brush to return to WM_CTLCOLORxxx handler
//
#define SIBLING_SETDC   2

//=============================================================================
//                              SIBLING_SETFONT
// wParam: LOWORD: int: CTLCOLOR_ code
//         HIWORD: int: SIBLING_SETFONT
// lParam:      NULL
//
// Result: HFONT
//      Font to set, or NULL

#define SIBLING_GETFONT 3
