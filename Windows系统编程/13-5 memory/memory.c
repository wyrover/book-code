

/****************************** Module Header ******************************\
 * Module Name: memory.c
\***************************************************************************/


#include <windows.h>
#include "memory.h"


HANDLE ghModule;
HWND   ghwndMain = NULL;
HWND   ghwndClient;

HMENU  hMenu,       hMenuWindow;
HMENU  hServerMenu, hServerMenuWindow;
HMENU  hClientMenu, hClientMenuWindow;

CHAR   gszFile[20];
CHAR   gszMapName[20];


typedef struct _PerWndInfo
{
    HWND    hParent;
    HWND    hThisWnd;           // Used in locating the node
    RECT    rcClient;
    char    CaptionBarText[SIZEOFCAPTIONTEXT];
} PERWNDINFO, *PPERWNDINFO;


typedef struct _node
{
   PERWNDINFO      ChildWnd;
   HANDLE          hNext;
} NODE, *PNODE;


/*
 * Forward declarations.
 */
BOOL InitializeApp   (void);
LONG APIENTRY MainWndProc     (HWND, UINT, DWORD, LONG);
LONG APIENTRY ServerWndProc   (HWND, UINT, DWORD, LONG);
LONG APIENTRY ClientWndProc   (HWND, UINT, DWORD, LONG);
BOOL CALLBACK About          (HWND, UINT, DWORD, LONG);
BOOL CALLBACK FileType       (HWND, UINT, DWORD, LONG);
BOOL CALLBACK MapFileName     (HWND, UINT, DWORD, LONG);
LONG APIENTRY TextWndProc     (HWND, UINT, DWORD, LONG);

/***************************************************************************\
* main
*
\***************************************************************************/

int WINAPI WinMain(
    HANDLE hInstance,
    HANDLE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{
    MSG msg;

    // this will change to something more reasonable

    ghModule = GetModuleHandle(NULL);
    if (!InitializeApp())
    {
        MessageBox(ghwndMain,
                   GetStringRes(IDS_ERR_INITAPP_FAILED),
                   NULL, MB_OK);

        return 0;
    }

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (hMenuWindow && IsWindow(hMenuWindow))
        DestroyMenu(hMenuWindow);
    if (hClientMenuWindow && IsWindow(hClientMenuWindow))
        DestroyMenu(hClientMenuWindow);
    if (hServerMenuWindow && IsWindow(hServerMenuWindow))
        DestroyMenu(hServerMenuWindow);
    if (hMenu && IsWindow(hMenu))
        DestroyMenu(hMenu);
    if (hClientMenu && IsWindow(hClientMenu))
        DestroyMenu(hClientMenu);
    if (hServerMenu && IsWindow(hServerMenu))
        DestroyMenu(hServerMenu);

    return 1;

    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nShowCmd);
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
}


/***************************************************************************\
* InitializeApp
*
\***************************************************************************/

BOOL InitializeApp(void)
{
    WNDCLASS wc;

    wc.style            = CS_OWNDC;
    wc.lpfnWndProc      = (WNDPROC)MainWndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = sizeof(LONG);
    wc.hInstance        = ghModule;
    wc.hIcon            = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)(COLOR_APPWORKSPACE);
    wc.lpszMenuName     = "MainMenu";
    wc.lpszClassName    = "MemoryClass";

    if (!RegisterClass(&wc))
        return FALSE;

    wc.style            = CS_OWNDC;
    wc.lpfnWndProc      = (WNDPROC)ServerWndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;                      // LATER sizeof(LONG);
    wc.hInstance        = ghModule;
    wc.hIcon            = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)(COLOR_APPWORKSPACE);
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = "ServerClass";

    if (!RegisterClass(&wc))
        return FALSE;

    wc.style            = CS_OWNDC;
    wc.lpfnWndProc      = (WNDPROC)ClientWndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;                      // LATER sizeof(LONG);
    wc.hInstance        = ghModule;
    wc.hIcon            = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)(COLOR_APPWORKSPACE);
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = "ClientClass";

    if (!RegisterClass(&wc))
        return FALSE;

    wc.style            = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = (WNDPROC)TextWndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = ghModule;
    wc.hIcon            = NULL;
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)(COLOR_BTNSHADOW);
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = "Text";

    if (!RegisterClass(&wc))
            return FALSE;



    hMenu       = LoadMenu(ghModule, "MainMenu");
    hServerMenu = LoadMenu(ghModule, "ServerMenu");
    hClientMenu = LoadMenu(ghModule, "ClientMenu");
    hMenuWindow       = GetSubMenu(hMenu, 1);
    hServerMenuWindow = GetSubMenu(hServerMenu, 2);
    hClientMenuWindow = GetSubMenu(hClientMenu, 2);

    ghwndMain = CreateWindowEx(0L, "MemoryClass", "Memory",
            WS_OVERLAPPED   | WS_CAPTION     | WS_BORDER       |
            WS_THICKFRAME   | WS_MAXIMIZEBOX | WS_MINIMIZEBOX  |
            WS_CLIPCHILDREN | WS_VISIBLE     | WS_SYSMENU,
            80, 70, 592, 300,
            NULL, hMenu, ghModule, NULL);

    if (ghwndMain == NULL)
        return FALSE;

    SetWindowLong(ghwndMain, GWL_USERDATA, 0L);

    return TRUE;
}


/***************************************************************************\
* MainWndProc
*
\***************************************************************************/

long APIENTRY MainWndProc(
                          HWND hwnd,
                          UINT message,
                          DWORD wParam,
                          LONG lParam)
{
    static int         iSvrCount=1;
    static int         iCltCount=1;
    CLIENTCREATESTRUCT clientcreate;
    HWND               hwndChildWindow;
    BOOL               fSuccess;


    switch (message)
    {

      case WM_CREATE:
        SetWindowLong(hwnd, 0, (LONG)NULL);

        clientcreate.hWindowMenu  = hMenuWindow;
        clientcreate.idFirstChild = 1;

        ghwndClient = CreateWindow("MDICLIENT", NULL,
                                    WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
                                    0,0,0,0,
                                    hwnd, NULL, ghModule, (LPVOID)&clientcreate);
        return 0L;

      case WM_DESTROY: {
        PostQuitMessage(0);
        return 0L;
      }

      case WM_COMMAND:

        switch (LOWORD(wParam))
        {
            case IDM_TILE:
                SendMessage(ghwndClient, WM_MDITILE, 0L, 0L);
                return 0L;
            case IDM_CASCADE:
                SendMessage(ghwndClient, WM_MDICASCADE, 0L, 0L);
                return 0L;
            case IDM_ARRANGE:
                SendMessage(ghwndClient, WM_MDIICONARRANGE, 0L, 0L);
                return 0L;

            case MM_SERVER: {
                BOOL fSucess = FALSE;
                HANDLE hNode, hHead;
                PNODE  pNode;
                MDICREATESTRUCT mdicreate;

                hNode = LocalAlloc(LHND, (WORD) sizeof(NODE));
                if (hNode && (pNode = (PNODE)LocalLock(hNode)))
                {
                    wsprintf((LPSTR) &(pNode->ChildWnd.CaptionBarText),
                             GetStringRes(IDS_SERVER), iSvrCount);

                    mdicreate.szClass = "ServerClass";
                    mdicreate.szTitle = (LPTSTR)&(pNode->ChildWnd.CaptionBarText);
                    mdicreate.hOwner  = ghModule;
                    mdicreate.x       =
                    mdicreate.y       =
                    mdicreate.cx      =
                    mdicreate.cy      = CW_USEDEFAULT;
                    mdicreate.style   = 0l;
                    mdicreate.lParam  = 0L;

                    /*Create Child Window*/
                    hwndChildWindow =
                       (HWND) SendMessage(ghwndClient, WM_MDICREATE,
                                          0L,
                                          (LONG)(LPMDICREATESTRUCT)&mdicreate);

                    if (hwndChildWindow)
                    {
                       pNode->ChildWnd.hParent      = ghwndClient;
                       pNode->ChildWnd.hThisWnd     = hwndChildWindow;
                       hHead = (HANDLE)GetWindowLong(hwnd, 0);
                       pNode->hNext = hHead;
                       SetWindowLong(hwnd, 0, (LONG) hNode);

                       iSvrCount++;
                       LocalUnlock(hNode);

                       fSuccess = TRUE;
                    }
                    else
                    {
                       LocalUnlock (hNode);
                       LocalFree (hNode);

                       fSuccess = FALSE;
                    }
                }
                if (!fSuccess)
                    MessageBox(ghwndMain,
                               GetStringRes(IDS_ERR_CREATE_CHILD_FAILED),
                               NULL, MB_OK);

                 return 0L;
            }

            case MM_CLIENT:
            {
                BOOL fResult = FALSE;
                HANDLE hNode, hHead;
                PNODE  pNode;
                MDICREATESTRUCT mdicreate;

                hNode = LocalAlloc(LHND, (WORD) sizeof(NODE));
                if (hNode && (pNode = (PNODE)LocalLock(hNode)))
                {
                    wsprintf((LPSTR) &(pNode->ChildWnd.CaptionBarText),
                             GetStringRes(IDS_CLIENT), iCltCount);

                    mdicreate.szClass = "ClientClass";
                    mdicreate.szTitle = (LPSTR) &(pNode->ChildWnd.CaptionBarText);
                    mdicreate.hOwner  = ghModule;
                    mdicreate.x       =
                    mdicreate.y       =
                    mdicreate.cx      =
                    mdicreate.cy      = CW_USEDEFAULT;
                    mdicreate.style   = 0l;
                    mdicreate.lParam  = 0L;

                    /*Create Child Window*/
                    hwndChildWindow =
                        (HANDLE) SendMessage(ghwndClient, WM_MDICREATE,
                                             0L,
                                             (LONG)(LPMDICREATESTRUCT)&mdicreate);

                    if (hwndChildWindow)
                    {
                       pNode->ChildWnd.hParent      = ghwndClient;
                       pNode->ChildWnd.hThisWnd     = hwndChildWindow;
                       hHead = (HANDLE)GetWindowLong(hwnd, 0);
                       pNode->hNext = hHead;
                       SetWindowLong(hwnd, 0, (LONG) hNode);

                       iCltCount++;
                       LocalUnlock(hNode);

                       fSuccess = TRUE;
                    }
                    else
                    {
                       LocalUnlock (hNode);
                       LocalFree (hNode);

                       fSuccess = FALSE;
                    }
                }
                if (!fSuccess)
                   MessageBox(ghwndMain,
                              GetStringRes(IDS_ERR_CREATE_CHILD_FAILED),
                              NULL, MB_OK);

                return 0L;
            }

            case MM_ABOUT:
                if (DialogBox(ghModule, "AboutBox", ghwndMain, (DLGPROC)About) == -1)
                   MessageBox(ghwndMain,
                              GetStringRes(IDS_ERR_CREATE_DLG_FAILED), NULL, MB_OK);
                return 0L;

            case MM_OPT_1:
            case MM_OPT_2:
            case MM_OPT_3:
            case MM_OPT_4:
            case MM_OPT_5:
            case MM_OPT_6:
            case MM_OPT_7:
            case MM_OPT_8:
            {
                HWND hActiveChild;

                hActiveChild = (HANDLE) SendMessage(ghwndClient, WM_MDIGETACTIVE, 0L, 0L);
                if (hActiveChild)
                    SendMessage(hActiveChild, WM_COMMAND, wParam, lParam);
                return 0L;
            }

            default:
                return DefFrameProc(hwnd,  ghwndClient, message, wParam, lParam);
        }

    default:

        return DefFrameProc(hwnd,  ghwndClient, message, wParam, lParam);
    }
}


/***************************************************************************\
* ServerWndProc
*
\***************************************************************************/

long APIENTRY ServerWndProc( HWND hwnd,
                             UINT message,
                             DWORD wParam,
                             LONG lParam)
{
   static HANDLE MapFileHandle = NULL;
   static HANDLE hMem1         = NULL;
   static LPVOID pShrMem1      = NULL;
   static HANDLE hEdit;
   static BOOL   bDirty        = FALSE;
   static HWND   hTextWnd;
   BOOL          fSuccess;

   switch (message)
   {
      case WM_COMMAND:
      {

         switch (LOWORD(wParam))
         {
            case MM_OPT_1:
            { //Create File

               SetWindowText(hTextWnd, GetStringRes(IDS_CREATE_MAP_FILE));

               switch (DialogBox(ghModule, "FileType", hwnd, (DLGPROC)FileType))
               {
                  case -1:
                     fSuccess = FALSE;
                     break;

                  case IDBTN_MAP:
                     if (MapFileHandle = CreateMapFile(gszFile))
                        fSuccess = TRUE;
                     else
                        fSuccess = FALSE;

                     break;

                  default:
                     MapFileHandle = (HANDLE) 0xFFFFFFFF;
                     fSuccess = TRUE;
                     break;
               }

               if (fSuccess)
               {
                  EnableMenuItem(hServerMenu, MM_OPT_1, MF_GRAYED);
                  EnableMenuItem(hServerMenu, MM_OPT_2, MF_ENABLED);

                  SetWindowText(hTextWnd,
                                GetStringRes(IDS_SEL_CREATE_FILE_MAPPING));
               }
               else
               {
                  SetWindowText(hTextWnd,
                                GetStringRes (IDS_ERR_MAPFILE_FAILED));

               }
               return 0L;
            }

            case MM_OPT_2:
            { //Create File Mapping

               // This option should be disabled until MM_OPT_1 has been chosen.

               SetWindowText(hTextWnd,
                             GetStringRes(IDS_CREATE_MAPPING));


               if (MapFileHandle)
                  switch (DialogBox(ghModule, "MapName", hwnd, (DLGPROC)MapFileName))
                  {
                     case IDBTN_OK:
                        if (hMem1 = CreateMap(MapFileHandle, gszMapName))
                           fSuccess = TRUE;
                        else
                           fSuccess = FALSE;
                        break;

                     default:
                        fSuccess = FALSE;
                        break;
                  }
               else
                  fSuccess = FALSE;


               if (fSuccess)
               {
                  EnableMenuItem(hServerMenu, MM_OPT_2, MF_GRAYED);
                  EnableMenuItem(hServerMenu, MM_OPT_3, MF_ENABLED);

                  SetWindowText(hTextWnd,
                                GetStringRes (IDS_SEL_MAP_VIEW));
               }
               else
               {
                  SetWindowText(hTextWnd,
                                GetStringRes (IDS_ERR_MAPPING_FAILED));

               }
               return 0L;
            }

            case MM_OPT_3:
            { //Map View of File

               SetWindowText(hTextWnd, GetStringRes (IDS_MAPPING_VIEW));


               if (hMem1 && (pShrMem1 = (LPVOID)MapView(hMem1)))
               {
                  EnableMenuItem(hServerMenu, MM_OPT_3, MF_GRAYED);
                  EnableMenuItem(hServerMenu, MM_OPT_4, MF_ENABLED);

                  SetWindowText(hTextWnd, GetStringRes (IDS_SEL_ACCESS));

               }
               else
               {
                  SetWindowText(hTextWnd, GetStringRes(IDS_ERR_MAPVIEW_FAILED));
               }
               return 0L;
            }

            case MM_OPT_4:
            { //Access
               RECT    rcl;

               SetWindowText(hTextWnd,
                             GetStringRes (IDS_ACCESSING_SERVER_WRITE));

               if (pShrMem1)
               {
                  GetClientRect(hwnd, &rcl);
                  hEdit = CreateWindow("edit", (LPSTR) NULL,
                                        WS_CHILD      | WS_VISIBLE     |
                                        WS_HSCROLL    | WS_VSCROLL     |
                                        WS_BORDER     | ES_LEFT |
                                        ES_MULTILINE  | ES_AUTOHSCROLL |
                                        ES_AUTOVSCROLL,
                                        0,0, rcl.right - rcl.left,
                                        rcl.bottom - rcl.top -
                                        GetWindowLong(hTextWnd, GWL_USERDATA),
                                        hwnd, (HMENU)1, ghModule, (LPVOID)NULL);
                  if (hEdit)
                  {
                     EnableMenuItem(hServerMenu, MM_OPT_4, MF_GRAYED);
                     SetFocus(hEdit);
                     fSuccess = TRUE;
                  }
                  else
                     fSuccess = FALSE;
               }
               else
                  fSuccess = FALSE;

               if (!fSuccess)
               {
                  MessageBox(ghwndMain,
                             GetStringRes(IDS_ERR_ACCESS_WRITE_FAILED),
                             NULL, MB_OK);
                  SetWindowText(hTextWnd,
                                GetStringRes (IDS_ERR_ACCESS_WRITE_FAILED));
               }

               return 0L;
            }
         } // End of switch (LOWORD(wParam))

         switch (HIWORD(wParam))
         {
            case EN_UPDATE:
            {
               if (hEdit && (hEdit == (HWND)lParam))
               {
                  bDirty = TRUE;
               }
               return 0L;
            }
         }

      }
      case WM_TIMER:
         if (bDirty && IsWindow(hEdit))
         {
            int     iCnt;
            iCnt = SendMessage(hEdit, WM_GETTEXT, (WPARAM)4000, (LPARAM)pShrMem1);

            if (iCnt)
            {
               bDirty = FALSE;
            }

         }
         return 0L;

      case WM_MDIACTIVATE:
         if ((HWND) lParam == hwnd)
         {
            SendMessage(GetParent(hwnd), WM_MDISETMENU,
                        (DWORD)  hServerMenu,
                        (LONG)   hServerMenuWindow) ;
            DrawMenuBar(GetParent(GetParent(hwnd))) ;
         }
         return 0;

      case WM_SIZE:
         if (hEdit)
            MoveWindow(hEdit, 0, 0,
                       LOWORD(lParam),
                       HIWORD(lParam)-GetWindowLong(hTextWnd, GWL_USERDATA),
                       TRUE);

         MoveWindow(hTextWnd, 0,
                    HIWORD(lParam) - GetWindowLong(hTextWnd, GWL_USERDATA),
                    LOWORD(lParam),
                    HIWORD(lParam), TRUE);

         return DefMDIChildProc(hwnd, message, wParam, lParam);

      case WM_CREATE:
      {
         PPERWNDINFO      pWndInfo;
         PNODE            pHead;
         HANDLE           hHead, hTmp;
         RECT             rect;

         GetClientRect(hwnd, &rect);
         hTextWnd = CreateWindow("Text", NULL,
                                 WS_BORDER | SS_LEFT | WS_CHILD | WS_VISIBLE,
                                 0, 0, 0, 0,
                                 hwnd,
                                 (HMENU) 2,
                                 ghModule,
                                 NULL);

         EnableMenuItem(hServerMenu, MM_OPT_1, MF_ENABLED);


         SetWindowText(hTextWnd, GetStringRes (IDS_SEL_CREATE_FILE));

         // now find match
         hHead = (HANDLE) GetWindowLong(ghwndMain, 0);
         if (hHead && (pHead = (PNODE)LocalLock(hHead)))
         {
            while ((pHead->ChildWnd.hThisWnd != hwnd) && (pHead->hNext))
            {
               hTmp = hHead;
               hHead = pHead->hNext;
               LocalUnlock(hTmp);

               pHead = (PNODE)LocalLock(hHead);
            }

            if (pHead->ChildWnd.hThisWnd == hwnd)
            {
               pWndInfo = &pHead->ChildWnd;
               GetClientRect(pWndInfo->hThisWnd, &pWndInfo->rcClient);
            }

            LocalUnlock(hHead);
         }
         return DefMDIChildProc(hwnd, message, wParam, lParam);
      }

      case WM_CLOSE:
      {
         PPERWNDINFO      pWndInfo;
         PNODE            pHead, pTrail;
         HANDLE           hHead, hTmp;

         EnableMenuItem(hServerMenu, MM_OPT_2, MF_GRAYED);
         EnableMenuItem(hServerMenu, MM_OPT_3, MF_GRAYED);
         EnableMenuItem(hServerMenu, MM_OPT_4, MF_GRAYED);

         SendMessage(GetParent(hwnd), WM_MDISETMENU, (DWORD)hMenu, (LONG)hMenuWindow);
         DrawMenuBar(GetParent(GetParent(hwnd))) ;


         // Unmap view, close mapping and file!
         if (pShrMem1)
         {
            UnmapViewOfFile (pShrMem1);
            pShrMem1 = NULL;
         }

         if (hMem1)
         {
            CloseHandle (hMem1);
            hMem1 = NULL;
         }

         if (MapFileHandle && MapFileHandle != INVALID_HANDLE_VALUE)
         {
            CloseHandle (MapFileHandle);
            MapFileHandle = NULL;
         }

         // now find match
         hHead = (HANDLE) GetWindowLong(ghwndMain, 0);
         if ((hHead) && (pHead = (PNODE)LocalLock(hHead)))
         {
            pTrail = pHead;
            while ((pHead->ChildWnd.hThisWnd != hwnd) && (pHead->hNext))
            {
               hTmp = hHead;
               pTrail = pHead;
               hHead = pHead->hNext;
               LocalUnlock(hTmp);

               pHead = (PNODE) LocalLock(hHead);
            }

            if (pHead->ChildWnd.hThisWnd == hwnd)
            {
               pWndInfo = &pHead->ChildWnd;

               if (pTrail == pHead)
                  SetWindowLong(ghwndMain, 0, (LONG) pHead->hNext);
               else
                  pTrail->hNext = pHead->hNext;

               LocalUnlock(hHead);
               LocalFree(hHead);
            }
            else
            {
               LocalUnlock(hHead);
            }
         }
         return DefMDIChildProc(hwnd, message, wParam, lParam);
      }

      case WM_DESTROY:
         KillTimer(hwnd, 1);
         return 0L;

      case WM_PAINT:
         return DefMDIChildProc(hwnd, message, wParam, lParam);

      default:
         return DefMDIChildProc(hwnd, message, wParam, lParam);
   }
}


/***************************************************************************\
* ClientWndProc
*
\***************************************************************************/

long APIENTRY ClientWndProc( HWND hwnd,
                             UINT message,
                             DWORD wParam,
                             LONG lParam)
{
    static HANDLE hMem1    = NULL;
    static LPVOID pShrMem1 = NULL;
    static HANDLE hEdit;
    static HANDLE hTextWnd;
    BOOL   fSuccess = FALSE;

    switch (message)
    {
        case WM_COMMAND:
        {

          switch (LOWORD(wParam))
          {
             case MM_OPT_5:
             { //Open File Mapping

                 SetWindowText(hTextWnd, GetStringRes (IDS_OPENING_FILE));

                 switch (DialogBox(ghModule, "MapName", hwnd, (DLGPROC)MapFileName))
                 {
                     case IDBTN_OK:
                         if (hMem1 = OpenMap(gszMapName))
                             fSuccess = TRUE;
                         else
                            fSuccess = FALSE;
                         break;

                     default:
                         fSuccess = FALSE;
                         break;
                 }

                 if (fSuccess)
                 {
                    EnableMenuItem(hClientMenu, MM_OPT_5, MF_GRAYED);
                    EnableMenuItem(hClientMenu, MM_OPT_6, MF_ENABLED);

                    SetWindowText(hTextWnd, GetStringRes (IDS_SEL_MAP_VIEW));
                 }
                 else
                 {
                    SetWindowText(hTextWnd,
                                  GetStringRes (IDS_ERR_OPEN_MAPPING_FAILED));
                 }
                 return 0L;

            }
            case MM_OPT_6:
            { //Map View of File

                SetWindowText(hTextWnd, GetStringRes (IDS_MAPPING_VIEW));

                if (hMem1 && (pShrMem1 = (LPVOID) MapView(hMem1)))
                {
                   EnableMenuItem(hClientMenu, MM_OPT_6, MF_GRAYED);
                   EnableMenuItem(hClientMenu, MM_OPT_7, MF_ENABLED);

                   SetWindowText(hTextWnd, GetStringRes (IDS_SEL_ACCESS_READ));
                }
                else
                {
                   SetWindowText(hTextWnd,
                                 GetStringRes(IDS_ERR_MAPVIEW_FAILED));
                }

                return 0L;
            }

            case MM_OPT_7:
            { //Access
                RECT    rcl;

                 SetWindowText(hTextWnd,
                               GetStringRes(IDS_ACCESSING_SERVER_READ));

                 fSuccess = FALSE;  // assume failure.

                 if (pShrMem1)
                 {
                    GetClientRect(hwnd, &rcl);

                    hEdit = CreateWindow("edit", NULL,
                                   WS_CHILD     | WS_VISIBLE     |
                                   WS_HSCROLL   | WS_VSCROLL     |
                                   WS_BORDER    | ES_LEFT        |
                                   ES_MULTILINE | ES_AUTOHSCROLL |
                                   ES_READONLY  | ES_AUTOVSCROLL,
                                   0,0, rcl.right-rcl.left,
                                   rcl.bottom-rcl.top-GetWindowLong(hTextWnd, GWL_USERDATA),
                                   hwnd, (HMENU) 1, ghModule, NULL);
                    if (hEdit)
                    {
                       SetFocus(hEdit);
                       fSuccess = TRUE;
                    }
                 }

                 if (fSuccess)
                 {
                    SendMessage(hEdit, WM_SETTEXT, 0L, (LONG)pShrMem1);
                    SetTimer(hwnd, 2, 1000, NULL);
                    EnableMenuItem(hClientMenu, MM_OPT_7, MF_GRAYED);
                    EnableMenuItem(hClientMenu, MM_OPT_8, MF_ENABLED);
                 }
                 else
                 {
                    MessageBox(ghwndMain,
                               GetStringRes (IDS_ERR_ACCESS_READ_FAILED),
                               NULL, MB_OK);
                    SetWindowText(hTextWnd,
                                  GetStringRes (IDS_ERR_ACCESS_READ_FAILED));
                 }

             return 0L;
             }

            case MM_OPT_8:
            { // refresh now
               HANDLE hActive;

               hActive = (HANDLE) SendMessage(GetParent(hwnd), WM_MDIGETACTIVE, 0L, 0L);
               SendMessage(hEdit, WM_SETTEXT, 0L, (LONG)pShrMem1);
               return 0L;
            }
        }
    }

    case WM_TIMER:
    {
        HANDLE hActive;

        if (IsWindow (hEdit) && pShrMem1)
        {
           hActive = (HANDLE) SendMessage(GetParent(hwnd), WM_MDIGETACTIVE, 0L, 0L);
           SendMessage(hEdit, WM_SETTEXT, 0L, (LONG)pShrMem1);
        }
        return 0L;
    }

    case WM_MDIACTIVATE:
        if ((HWND) lParam == hwnd)
        {
           SendMessage(GetParent(hwnd), WM_MDISETMENU,
                       (DWORD)  hClientMenu,
                       (LONG)   hClientMenuWindow);
           DrawMenuBar(GetParent(GetParent(hwnd))) ;
        }
        return 0;

    case WM_SIZE:
        if (hEdit)
           MoveWindow(hEdit, 0, 0,
                      LOWORD(lParam),
                      HIWORD(lParam)-GetWindowLong(hTextWnd, GWL_USERDATA),
                      TRUE);

        MoveWindow(hTextWnd, 0,
                   HIWORD(lParam) - GetWindowLong(hTextWnd, GWL_USERDATA),
                   LOWORD(lParam),
                   HIWORD(lParam), TRUE);

        return DefMDIChildProc(hwnd, message, wParam, lParam);

    case WM_CREATE:
    {
        PPERWNDINFO      pWndInfo;
        PNODE            pHead;
        HANDLE           hHead, hTmp;
        RECT             rect;

        GetClientRect(hwnd, &rect);
        hTextWnd = CreateWindow("Text", NULL,
                                WS_BORDER | SS_LEFT | WS_CHILD | WS_VISIBLE,
                                0, 0, 0, 0,
                                hwnd,
                                (HMENU) 2,
                                ghModule,
                                NULL);

        EnableMenuItem(hClientMenu, MM_OPT_5, MF_ENABLED);

        SetWindowText(hTextWnd, GetStringRes (IDS_SEL_OPEN_FILE));

        // now find match
        hHead = (HANDLE) GetWindowLong(ghwndMain, 0);
        if (hHead && (pHead = (PNODE)LocalLock(hHead)))
        {

           while ((pHead->ChildWnd.hThisWnd != hwnd) && (pHead->hNext))
           {
              hTmp = hHead;
              hHead = pHead->hNext;
              LocalUnlock(hTmp);
              pHead = (PNODE) LocalLock(hHead);
           }

           if (pHead->ChildWnd.hThisWnd == hwnd)
           {
              pWndInfo = &pHead->ChildWnd;
              GetClientRect(pWndInfo->hThisWnd, &pWndInfo->rcClient);
           }

           LocalUnlock(hHead);
           return DefMDIChildProc(hwnd, message, wParam, lParam);
        }
        return DefMDIChildProc(hwnd, message, wParam, lParam);
    }

    case WM_CLOSE:
    {
        PPERWNDINFO      pWndInfo;
        PNODE            pHead, pTrail;
        HANDLE           hHead, hTmp;


        EnableMenuItem(hClientMenu, MM_OPT_6, MF_GRAYED);
        EnableMenuItem(hClientMenu, MM_OPT_7, MF_GRAYED);
        EnableMenuItem(hClientMenu, MM_OPT_8, MF_GRAYED);

        SendMessage(GetParent(hwnd), WM_MDISETMENU,
                    (DWORD) hMenu,
                    (LONG)   hMenuWindow) ;
        DrawMenuBar(GetParent(GetParent(hwnd))) ;

        // Don't need to read from the edit control anymore
        KillTimer(hwnd, 2);


        // Unmap view, close mapping!
        if (pShrMem1)
        {
           UnmapViewOfFile (pShrMem1);
           pShrMem1 = NULL;
        }

        if (hMem1)
        {
           CloseHandle (hMem1);
           hMem1 = NULL;
        }


        // now find match
        hHead = (HANDLE) GetWindowLong(ghwndMain, 0);
        if (hHead && (pHead = (PNODE)LocalLock(hHead)))
        {
            pTrail = pHead;
            while ((pHead->ChildWnd.hThisWnd != hwnd) && (pHead->hNext))
            {
               hTmp = hHead;
               pTrail = pHead;
               hHead = pHead->hNext;
               LocalUnlock(hTmp);

               pHead = (PNODE) LocalLock(hHead);
            }

            if (pHead->ChildWnd.hThisWnd == hwnd)
            {
                pWndInfo = &pHead->ChildWnd;

                if (pTrail == pHead)
                    SetWindowLong(ghwndMain, 0, (LONG) pHead->hNext);
                else
                    pTrail->hNext = pHead->hNext;

                LocalUnlock(hHead);
                LocalFree(hHead);
            }
            else
            {
                LocalUnlock(hHead);
            }
        }
        return DefMDIChildProc(hwnd, message, wParam, lParam);

    }

    case WM_DESTROY:
        return 0l;

    default:
         return DefMDIChildProc(hwnd, message, wParam, lParam);
    }

}




/***************************************************************************\
* About
*
* About dialog proc.
*
\***************************************************************************/

BOOL CALLBACK APIENTRY About( HWND hDlg,
                              UINT message,
                              DWORD wParam,
                              LONG lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
            return TRUE;

        case WM_COMMAND:
            if (wParam == IDOK)
                EndDialog(hDlg, wParam);
            break;
    }
    return FALSE;

    UNREFERENCED_PARAMETER(lParam);
    UNREFERENCED_PARAMETER(hDlg);
}

/***************************************************************************\
* MapFileName
*
* MapFileName dialog proc.
*
\***************************************************************************/

BOOL CALLBACK MapFileName( HWND hDlg,
                           UINT message,
                           DWORD wParam,
                           LONG lParam)
{

    switch (message)
    {
    case WM_INITDIALOG:
        return TRUE;

    case WM_COMMAND:
        switch (wParam)
        {
            case IDBTN_OK:
            {
                if (!GetDlgItemText(hDlg, IDEDIT_MAPNAME, gszMapName, 20))
                {
                   strncpy(gszMapName, "MapName1", 10);        // default name
                }
                EndDialog(hDlg, IDBTN_OK);
                break;
            }
        }
    }
    return FALSE;

    UNREFERENCED_PARAMETER(lParam);
    UNREFERENCED_PARAMETER(hDlg);
}

/***************************************************************************\
* FileType
*
* FileType dialog proc.
*
\***************************************************************************/

BOOL CALLBACK FileType( HWND hDlg,
                        UINT message,
                        DWORD wParam,
                        LONG lParam)
{

    switch (message)
    {
       case WM_INITDIALOG:
           return TRUE;

       case WM_COMMAND:
           switch (wParam)
           {
              case IDBTN_PAGE:
              {
                 EndDialog(hDlg, IDBTN_PAGE);
                 break;
              }

              case IDBTN_MAP:
              {
                  if ((GetDlgItemText(hDlg, IDEDIT_MAPFILE, gszFile, 20)) == 0)
                      EndDialog(hDlg, IDBTN_PAGE);  // default to use PAGE file
                  else
                      EndDialog(hDlg, IDBTN_MAP);

                  break;
              }
           }
    }
    return FALSE;

    UNREFERENCED_PARAMETER(lParam);
    UNREFERENCED_PARAMETER(hDlg);
}


/*************************************************************************
*
* TextWndProc
*
* Text Window proc.
*
\***************************************************************************/

LONG APIENTRY TextWndProc (HWND hwnd, UINT message, DWORD wParam, LONG lParam)
{
    static HFONT hFont = (HFONT) NULL;

    switch (message)
    {
       case WM_CREATE:
       {
          LOGFONT    lf;
          HDC        hDC;
          HFONT      hOldFont;
          TEXTMETRIC tm;
          RECT       rect;
          LONG       lHeight;

          SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(lf), &lf, FALSE);

          hDC = GetDC(hwnd);
          // this is the height for 8 point size font in pixels
          lf.lfHeight = 8 * GetDeviceCaps(hDC, LOGPIXELSY) / 72;

          hFont = CreateFontIndirect(&lf);
          hOldFont = SelectObject(hDC, hFont);
          GetTextMetrics(hDC, &tm);
          GetClientRect(GetParent(hwnd), &rect);

          // base the height of the window on size of text
          lHeight = tm.tmHeight+6*GetSystemMetrics(SM_CYBORDER)+2;
          // saved the height for later reference
          SetWindowLong(hwnd, GWL_USERDATA, lHeight);
          SetWindowPos(hwnd, NULL, 0, rect.bottom-lHeight,
                       rect.right-rect.left, lHeight,
                       SWP_NOZORDER | SWP_NOMOVE);

          ReleaseDC(hwnd, hDC);
          break;
       }

    case WM_DESTROY:
       if (hFont)
          DeleteObject(hFont);
       break;

    case WM_SETTEXT:
       DefWindowProc(hwnd, message, wParam, lParam);
       InvalidateRect(hwnd,NULL,FALSE);
       UpdateWindow(hwnd);
       return 0L;

    case WM_PAINT:
    {
       PAINTSTRUCT ps;
       RECT   rc;
       char   ach[128];
       int    len, nxBorder, nyBorder;
       HFONT  hOldFont = NULL;

       BeginPaint(hwnd, &ps);

       GetClientRect(hwnd,&rc);

       nxBorder = GetSystemMetrics(SM_CXBORDER);
       rc.left  += 9*nxBorder;
       rc.right -= 9*nxBorder;

       nyBorder = GetSystemMetrics(SM_CYBORDER);
       rc.top    += 3*nyBorder;
       rc.bottom -= 3*nyBorder;

       // 3D Text
       len = GetWindowText(hwnd, ach, sizeof(ach));
       SetBkColor(ps.hdc, GetSysColor(COLOR_BTNFACE));

       SetBkMode(ps.hdc, TRANSPARENT);
       SetTextColor(ps.hdc, RGB(64,96,96));
       if (hFont)
           hOldFont = SelectObject(ps.hdc, hFont);
       ExtTextOut(ps.hdc, rc.left+2*nxBorder+2, rc.top+2,
                  ETO_OPAQUE | ETO_CLIPPED, &rc, ach, len, NULL);

       SetTextColor(ps.hdc, RGB(128,128,128));
       if (hFont)
           hOldFont = SelectObject(ps.hdc, hFont);
       ExtTextOut(ps.hdc, rc.left+2*nxBorder+1, rc.top+1, ETO_CLIPPED,
                   &rc, ach, len, NULL);

       SetTextColor(ps.hdc, RGB(255,255,255));
       if (hFont)
           hOldFont = SelectObject(ps.hdc, hFont);
       ExtTextOut(ps.hdc, rc.left+2*nxBorder, rc.top, ETO_CLIPPED,
                   &rc, ach, len, NULL);

       SetBkMode(ps.hdc, OPAQUE);

       if (hOldFont)
           SelectObject(ps.hdc, hOldFont);

       EndPaint(hwnd, &ps);
       return 0L;
       }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}


//---------------------------------------------------------------------------
//
// FUNCTION:    GetStringRes (int id INPUT ONLY)
//
// COMMENTS:    Load the resource string with the ID given, and return a
//              pointer to it.  Notice that the buffer is common memory so
//              the string must be used before this call is made a second time.
//
//---------------------------------------------------------------------------

LPTSTR GetStringRes (int id)
{
  static TCHAR buffer[MAX_PATH];

  buffer[0]=0;
  LoadString (GetModuleHandle (NULL), id, buffer, MAX_PATH);
  return buffer;
}

