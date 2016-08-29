// Tree.cpp : implementation file
//
#include "stdafx.h"
#include <afxtempl.h>
#include "ControlExplorer.h"
#include "msglog.h"
#include "tinybutt.h"
#include "NumericEdit.h"
#include "ixable.h"
#include "loggable.h"
#include "treeops.h"
#include "tvitem.h"
#include "treeins.h"
#include "indexPg.h"
#include "msgsheet.h"
#include "treemisc.h"
#include "treendx.h"
#include "demo.h"
#include "resize.h"
#include "Tree.h"
#include "copy.h"
#include "botany.h" // dem0 1 is botany
#include "TreeGetItem.h"
#include "tvimglst.h"
#include "callback.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define STATEIMAGEMASKTOINDEX(n) (((n) & TVIS_STATEIMAGEMASK) >> 12)
#define DIM(x) (sizeof(x) / sizeof((x)[0]))

IMPLEMENT_DYNCREATE(CTree, CPropertyPage)

#define SCROLL_TIMER 1
#define UWM_SCROLL (WM_USER + 101)

/////////////////////////////////////////////////////////////////////////////
// CTree property page

CTree::CTree() : CPropertyPage(CTree::IDD)
{
        inGetDispInfo = 0;
        isDragging = FALSE;
        //{{AFX_DATA_INIT(CTree)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CTree::~CTree()
{
}

void CTree::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CTree)
        DDX_Control(pDX, IDC_LABELEDIT, c_LabelEdit);
        DDX_Control(pDX, IDC_IMAGELIST, c_QuickImageList);
        DDX_Control(pDX, IDC_DOWN, c_Down);
        DDX_Control(pDX, IDC_UP, c_Up);
        DDX_Control(pDX, IDC_SMARTDRAGDROP, c_SmartDragDrop);
        DDX_Control(pDX, IDC_CALLBACKS, c_Callbacks);
        DDX_Control(pDX, IDC_USEIMAGES, c_UseImages);
        DDX_Control(pDX, IDC_DRAGDROP, c_DragDrop);
        DDX_Control(pDX, IDC_NOSELECT, c_NoSelect);
        DDX_Control(pDX, IDC_NOEXPAND, c_NoExpand);
        DDX_Control(pDX, IDC_RESIZE, c_Resize);
        DDX_Control(pDX, IDC_WS_BORDER, c_WS_Border);
        DDX_Control(pDX, IDC_TVS_SHOWSELALWAYS, c_TVS_ShowSelAlways);
        DDX_Control(pDX, IDC_TVS_HASLINESATROOT, c_TVS_HasLinesAtRoot);
        DDX_Control(pDX, IDC_TVS_HASLINES, c_TVS_HasLines);
        DDX_Control(pDX, IDC_TVS_HASBUTTONS, c_TVS_HasButtons);
        DDX_Control(pDX, IDC_TVS_EDITLABELS, c_TVS_EditLabels);
        DDX_Control(pDX, IDC_TVS_DISABLEDRAGDROP, c_TVS_DisableDragDrop);
        DDX_Control(pDX, IDC_TREE, c_Tree);
        DDX_Control(pDX, IDC_MESSAGES, c_Messages);
        DDX_Control(pDX, IDC_DEMO1, c_Demo1);
        DDX_Control(pDX, IDC_COPY, c_Copy);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTree, CPropertyPage)
        //{{AFX_MSG_MAP(CTree)
        ON_BN_CLICKED(IDC_COPY, OnCopy)
        ON_BN_CLICKED(IDC_SENDMESSAGES, OnSendmessages)
        ON_NOTIFY(NM_CLICK, IDC_TREE, OnClickTree)
        ON_NOTIFY(TVN_BEGINRDRAG, IDC_TREE, OnBeginrdragTree)
        ON_NOTIFY(NM_DBLCLK, IDC_TREE, OnDblclkTree)
        ON_NOTIFY(NM_KILLFOCUS, IDC_TREE, OnKillfocusTree)
        ON_NOTIFY(NM_OUTOFMEMORY, IDC_TREE, OnOutofmemoryTree)
        ON_NOTIFY(NM_RCLICK, IDC_TREE, OnRclickTree)
        ON_NOTIFY(NM_RDBLCLK, IDC_TREE, OnRdblclkTree)
        ON_NOTIFY(NM_RETURN, IDC_TREE, OnReturnTree)
        ON_NOTIFY(NM_SETFOCUS, IDC_TREE, OnSetfocusTree)
        ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE, OnBegindragTree)
        ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_TREE, OnBeginlabeleditTree)
        ON_NOTIFY(TVN_DELETEITEM, IDC_TREE, OnDeleteitemTree)
        ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE, OnEndlabeleditTree)
        ON_NOTIFY(TVN_GETDISPINFO, IDC_TREE, OnGetdispinfoTree)
        ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE, OnItemexpandedTree)
        ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE, OnItemexpandingTree)
        ON_NOTIFY(TVN_KEYDOWN, IDC_TREE, OnKeydownTree)
        ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelchangedTree)
        ON_NOTIFY(TVN_SELCHANGING, IDC_TREE, OnSelchangingTree)
        ON_NOTIFY(TVN_SETDISPINFO, IDC_TREE, OnSetdispinfoTree)
        ON_BN_CLICKED(IDC_TVS_DISABLEDRAGDROP, OnTvsDisabledragdrop)
        ON_BN_CLICKED(IDC_TVS_EDITLABELS, OnTvsEditlabels)
        ON_BN_CLICKED(IDC_TVS_HASBUTTONS, OnTvsHasbuttons)
        ON_BN_CLICKED(IDC_TVS_HASLINES, OnTvsHaslines)
        ON_BN_CLICKED(IDC_TVS_HASLINESATROOT, OnTvsHaslinesatroot)
        ON_BN_CLICKED(IDC_TVS_SHOWSELALWAYS, OnTvsShowselalways)
        ON_BN_CLICKED(IDC_WS_BORDER, OnWsBorder)
        ON_BN_CLICKED(IDC_DEMO1, OnDemo1)
        ON_BN_CLICKED(IDC_RESIZE, OnResize)
        ON_WM_SIZE()
        ON_BN_CLICKED(IDC_USEIMAGES, OnUseimages)
        ON_WM_LBUTTONUP()
        ON_WM_MOUSEMOVE()
        ON_BN_CLICKED(IDC_DRAGDROP, OnDragdrop)
        ON_BN_CLICKED(IDC_DOWN, OnDown)
        ON_BN_CLICKED(IDC_UP, OnUp)
        ON_WM_SETCURSOR()
        ON_BN_CLICKED(IDC_IMAGELIST, OnImagelist)
        //}}AFX_MSG_MAP
        ON_REGISTERED_MESSAGE(CCallback::UWM_RECORD_DISPINFO, OnRecordDispInfo)
        ON_MESSAGE(UWM_SCROLL, OnScroll)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTree message handlers

void CTree::OnCopy() 
{
 copyToClipboard(&c_Tree);
}

void CTree::OnSendmessages() 
{
 CMessageSheet sheet(IDS_TREE_MESSAGES);

 CTreeIndex ndx;
 CInsertItem ins;
 CTreeGetItem get;
 CTreeMisc misc;
 CTVImageList img;

 ins.c_Messages = &c_Messages;
 ins.c_Tree = &c_Tree;
 ins.imagelist = &imagelist;
 
 get.c_Messages = &c_Messages;
 get.c_Tree = &c_Tree;
 get.imagelist = &imagelist;

 misc.c_Messages = &c_Messages;
 misc.c_Tree = &c_Tree;
 misc.imagelist = &imagelist;

 img.c_Messages = &c_Messages;
 img.c_Tree = &c_Tree;
 img.imagelist = &imagelist;
 img.botlist = &statelist;
 img.pixel1 = &pixel1;

 sheet.AddPage(&ndx);
 sheet.AddPage(&ins);
 sheet.AddPage(&get);
 sheet.AddPage(&misc);
 sheet.AddPage(&img);

 // Move us down to the bottom left corner
 CRect r;
 GetParent()->GetWindowRect(&r);
 GetParent()->SetWindowPos(NULL, 0, 
                        ::GetSystemMetrics(SM_CYSCREEN) - r.Height(), 0, 0, 
                        SWP_NOSIZE | SWP_NOZORDER);

 sheet.DoModal();
        
 enableControls();
}

void CTree::logNMHDR_b(int msg, LPNMHDR hdr, CString ext, BOOL result)
{
 CString s;
 s.LoadString(result ? IDS_TRUE : IDS_FALSE);
 logNMHDR(msg, hdr, ext, &s);
}

void CTree::logNMHDR(int msg, LPNMHDR hdr, CString ext, CString * result)
{
 CString fmt;
 fmt.LoadString(IDS_TVNMHDR);
 // fmt: WM_NOTIFY(%d, &hdr {from: %08x, id: %d, code: %d (%s) %s})
 
 CString codestr;
 codestr.LoadString(msg);
 CString s;
 s.Format(fmt, hdr->idFrom, hdr->hwndFrom, hdr->idFrom, hdr->code, codestr,
                                ext);

 if(result != NULL)
    { /* has result */
     s += _T(" => ");
     s += *result;
    } /* has result */

 CString ctlname;
 ctlname.LoadString(IDS_TREE);
 c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);
}
            

/****************************************************************************
*                             CTree::logLoadImage
* Inputs:
*       HINSTANCE hinst:
*       LPCTSTR resid:
*       int width:
*       int grow:
*       COLORREF color:
*       int type:
*       int flags:      
* Result: void
*       
* Effect: 
*       Logs the LoadImage call
****************************************************************************/

void CTree::logLoadImage(HINSTANCE hinst, LPCTSTR resid, int cx, int grow, COLORREF mask, UINT type, UINT flags, HIMAGELIST result)
    {
     // LoadImage(%08x, %s, %d, %d, RGB(%d,%d,%d), %s, %08x) => %08x
     CString t;
     t.LoadString(IDS_LOADIMAGE);
     
     CString resstr;
     if(HIWORD((LONG)resid) == 0)
        { /* id */
         resstr.Format(_T("%d"), LOWORD((LONG)resid));
        } /* id */
     else
        { /* string */
         resstr.Format(_T("\"%s\""), resid);
        } /* string */

     CString imagestr;
     imagestr.LoadString( type == IMAGE_BITMAP ? IDS_IMAGE_BITMAP 
                           : type == IMAGE_ICON ? IDS_IMAGE_ICON 
                                : IDS_IMAGE_CURSOR);

     CString s;
     s.Format(t, hinst, resstr, cx, grow, GetRValue(mask), GetGValue(mask),
                                GetBValue(mask), imagestr, flags, result);
    }

/****************************************************************************
*                           CTree::getStateShort
* Inputs:
*       �
* Result: �
*       �
* Effect: 
*       �
****************************************************************************/

static struct {int id; UINT mask; } shortnames[] = {
        {IDS_TVIS_BOLD_SHORT, TVIS_BOLD},
        {IDS_TVIS_CUT_SHORT, TVIS_CUT},
        {IDS_TVIS_DROPHILITED_SHORT, TVIS_DROPHILITED},
        {IDS_TVIS_EXPANDED_SHORT, TVIS_EXPANDED},
        {IDS_TVIS_EXPANDEDONCE_SHORT, TVIS_EXPANDEDONCE},
        // {IDS_TVIS_FOCUSED_SHORT, TVIS_FOCUSED},
        {IDS_TVIS_OVERLAYMASK_SHORT, TVIS_OVERLAYMASK},
        {IDS_TVIS_SELECTED_SHORT, TVIS_SELECTED},
        {IDS_TVIS_STATEIMAGEMASK_SHORT, TVIS_STATEIMAGEMASK},
        { 0, 0} // EOT
                                                   };

CString CTree::getStateShort(UINT state)
    {
     CString result;
     CString s;
     CString sep(_T(""));
     for(int i = 0; shortnames[i].id != 0; i++)
        { /* loop */
         if(shortnames[i].mask & state)
            { /* has bit */
             s.LoadString(shortnames[i].id);
             result += sep;
             result += s;
             if(shortnames[i].id == IDS_TVIS_STATEIMAGEMASK_SHORT)
                { /* add state # */
                 CString n;
                 n.Format(_T(":%d"), STATEIMAGEMASKTOINDEX(state));
                 result += n;
                } /* add state # */
             sep = _T(" ");
            } /* has bit */
        } /* loop */
     return result;
    }

/****************************************************************************
*                                 itemString
* Inputs:
*       LPTV_ITEM item: Item to be formatted for display
*       UINT mask: Mask of valid fields (not necessarily all will be 
*                       displayed depending on how ambitious I am)
* Result: CString
*       A displayable string of the item using compressed notation, suitable
*       for a c_Messages display
* Notes:
*       the constants for display include
*               exTVIF_MASK: display the mask
****************************************************************************/
#define exTVIF_MASK 0x10000 // any bit value that exceeds 16 bits...


CString CTree::itemString(LPTV_ITEM item, UINT mask)
{
 CString result;
 CString sep(_T(""));

 if(mask & exTVIF_MASK)
    { /* format mask */
    } /* format mask */

 if(mask & TVIF_HANDLE)
    { /* format handle */
     CString s;
     if(item->hItem != NULL)
        { /* valid item */
         // Avoid infinite recursion if we are already in GETDISPINFO handler..
         CString t;
         if(inGetDispInfo == 0)
            t = _T("\"") + c_Tree.GetItemText(item->hItem) + _T("\""); 
         else
            t.LoadString(IDS_LPSTR_CALLBACK);

         s.Format(_T("%08x: %s"), item->hItem, t);
        } /* valid item */
     else
        { /* NULL */
         s.LoadString(IDS_NULL);
        } /* NULL */

     result += sep;
     result += s;
     sep = _T(" ");
    } /* format handle */

 if(mask & TVIF_PARAM)
    { /* format lParam */
     CString s;
     s.Format(_T("[%08x]"), item->lParam);
     result += sep;
     result += s;
     sep = _T(" ");
    } /* format lParam */

 if(mask & TVIF_STATE)
    { /* format state */
     result += sep;
     result += getStateShort(item->state);
    } /* format state */
 return result;
}

void CTree::OnBeginrdragTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
        NM_TREEVIEW* nmtv = (NM_TREEVIEW*)pNMHDR;
        CString s = itemString(&nmtv->itemNew, exTVIF_MASK | TVIF_STATE | TVIF_PARAM | TVIF_HANDLE);
        logNMHDR(IDS_TVN_BEGINRDRAG, pNMHDR, s);
        
        *pResult = 0;
}

void CTree::OnClickTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
        logNMHDR(IDS_NM_CLICK, pNMHDR, NULL);
        
        *pResult = 0;
}

/****************************************************************************
*                               edit_OnDestroy
* Inputs:
*       HWND hwnd: Edit control handle
* Result: void
*       
* Effect: 
*       Removes the subclassing of the window
****************************************************************************/

static void 
edit_OnDestroy(HWND hwnd)
    {
     DWORD oldproc = ::GetWindowLong(hwnd, GWL_USERDATA);
     if(oldproc != 0)
        { /* had subclassing */
         ::SetWindowLong(hwnd, GWL_WNDPROC, oldproc);
        } /* had subclassing */
    }

/****************************************************************************
*                               edit_OnDlgCodes
* Inputs:
*       HWND hwnd: Window handle
* Result: UINT
*       DLGC_WANTCHARS
****************************************************************************/

static UINT 
edit_OnDlgCodes(HWND hwnd)
    {
     return DLGC_WANTCHARS;
    }

/****************************************************************************
*                                edit_OnCharEx
* Inputs:
*       HWND hwnd: Window handle
*       TCHAR ch: Character code
*       int repeat: Repeat count
* Result: (LRESULT) BOOL
*       0 if not handled
*       1 if handled
* Effect: 
*       If the character is ENTER or ESC, we terminate editing of the
*       tree, otherwise we ignore the character and return 0
****************************************************************************/

static LRESULT 
edit_OnCharEx(HWND hwnd, TCHAR ch, int repeat)
    {
     switch(ch)
        { /* ch */
         case _T('\r'):
                 return (LRESULT)TRUE;
         case _T('\033'):
                 return (LRESULT)TRUE;
        } /* ch */
     return (LRESULT)FALSE;
    }

/****************************************************************************
*                                 editWndProc
* Inputs:
*       HWND hwnd: The edit control handle
*       UINT msg: Message ID
*       WPARAM wParam:
*       LPARAM lParam:
* Result: LRESULT
*       as per message spec
* Effect: 
*       This subclasses the edit window for label editing.  In particular,
*       it makes sure that the Enter and Esc keys are handled correctly
****************************************************************************/

static LRESULT CALLBACK 
editWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
     switch(msg)
        { /* msg */
         // HANDLE_MSG(WM_DESTROY, edit_OnDestroy);
         // HANDLE_MSG(WM_GETDLGCODE, edit_OnDlgCodes);
         case WM_DESTROY:
                 edit_OnDestroy(hwnd);
                 return 0;
         case WM_GETDLGCODE:
                 return edit_OnDlgCodes(hwnd);
        } /* msg */
     return CallWindowProc((WNDPROC)::GetWindowLong(hwnd, GWL_USERDATA), 
                                                  hwnd, msg, wParam, lParam);
    }

void CTree::OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
        logNMHDR(IDS_NM_DBLCLK, pNMHDR, NULL);
        
        if((c_Tree.GetStyle() & TVS_EDITLABELS) && c_LabelEdit.GetCheck() == BST_CHECKED)
           { /* edit labels */
            HTREEITEM target = c_Tree.GetSelectedItem();
            if(target != NULL)
               { /* edit its label */
                HWND editctrl = (HWND)c_Tree.SendMessage(TVM_EDITLABEL, 0, (LPARAM)target);
                CString s;
                if(target == NULL)
                   s.LoadString(IDS_NULL);
                else
                   { /* use text */
                    CString is;
                    is = c_Tree.GetItemText(target);
                    s.Format(_T("%08x: \"%s\""), target, is);
                   } /* use text */

                showResult_N_s_x(IDS_TVM_EDITLABEL, s, (DWORD)editctrl, IDS_LABELEDIT);

                *pResult = (editctrl != NULL);
                return;
               } /* edit its label */
           } /* edit labels */

        *pResult = 0;
}

void CTree::OnKillfocusTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
        logNMHDR(IDS_NM_KILLFOCUS, pNMHDR, NULL);
        
        *pResult = 0;
}

void CTree::OnOutofmemoryTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
        logNMHDR(IDS_NM_OUTOFMEMORY, pNMHDR, NULL);
        
        *pResult = 0;
}

void CTree::OnRclickTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
        logNMHDR(IDS_NM_RCLICK, pNMHDR, NULL);
        
        *pResult = 0;
}

void CTree::OnRdblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
        logNMHDR(IDS_NM_RDBLCLK, pNMHDR, NULL);
        
        *pResult = 0;
}

void CTree::OnReturnTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
        logNMHDR(IDS_NM_RETURN, pNMHDR, NULL);
        
        *pResult = 0;
}

void CTree::OnSetfocusTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
        logNMHDR(IDS_NM_SETFOCUS, pNMHDR, NULL);

        
        *pResult = 0;
}

void CTree::OnBegindragTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
        NM_TREEVIEW* nmtv = (NM_TREEVIEW*)pNMHDR;
        CString s = itemString(&nmtv->itemNew, exTVIF_MASK | TVIF_STATE | TVIF_PARAM | TVIF_HANDLE);

        logNMHDR(IDS_TVN_BEGINDRAG, pNMHDR, s);

        if(c_DragDrop.GetCheck() == BST_CHECKED)
           { /* do drag-drop */
            // This code is written more like C to make it easy to copy and
            // paste into the book
            HINSTANCE hinst = AfxGetInstanceHandle();

            RECT rc;

            // If the control key is held down, this is a copy operation
            // See: The Windows Interface Guidelines to Software Design, p.87
            dragCopy = ::GetKeyState(VK_CONTROL) < 0;

            // DragImage = TreeView_CreateDragImage(nmtv->hdr.hwndFrom,
            //                               nmtv->itemNew.hItem);

            CImageList * iml = c_Tree.CreateDragImage(nmtv->itemNew.hItem);
            if(iml == NULL)
               DragImage = NULL;
            else 
               DragImage = iml->m_hImageList;

            showResult_N_x_x(IDS_TVM_CREATEDRAGIMAGE, (DWORD)nmtv->itemNew.hItem, 
                                            (DWORD)DragImage);

            // If the image list is NULL, create one based on the standard cursor
            if(DragImage == NULL)
               { /* standard cursor */
                DragImage = ImageList_LoadImage(hinst, MAKEINTRESOURCE(OCR_NORMAL),
                                    ::GetSystemMetrics(SM_CXICON),
                                    0,
                                    (COLORREF)0, // loading icon, mask is free
                                    IMAGE_ICON,  
                                    0);
                logLoadImage(hinst, MAKEINTRESOURCE(OCR_NORMAL), 
                                    ::GetSystemMetrics(SM_CXICON),
                                    0,
                                    (COLORREF)0,
                                    IMAGE_ICON,
                                    0,
                                    DragImage);
               } /* standard cursor */

            if(dragCopy)
               { /* do a copy */
                // If this is a copy operation, load up the little [+] image so
                // we provide appropriate feedback to the user (WIGSD, p. 85)
                HIMAGELIST plus;
                plus = ImageList_LoadImage(hinst, 
                                    MAKEINTRESOURCE(IDI_PLUS),
                                    ::GetSystemMetrics(SM_CXICON),
                                    0,
                                    (COLORREF)0, // loading icon, mask is free
                                    IMAGE_ICON,  
                                    0);
                ImageList_Merge(DragImage, 0, plus, 0, 0, 0);
                ImageList_Destroy(plus);
               } /* do a copy */


            // TreeView_SelectItem(htree, NULL);
            c_Tree.SelectItem(NULL);  // turn off selection highlight

            DragItem = nmtv->itemNew.hItem;

            // Get the rectangle of the text
            //BOOL gir = TreeView_GetItemRect(nmtv->hdr.hwndFrom,
            //                          nmtv->itemNew.hItem,
            //                          &rc,
            //                          TRUE);  // text only
            BOOL gir = c_Tree.GetItemRect(nmtv->itemNew.hItem, &rc, TRUE);

            showResult_b_R_b(IDS_TVM_GETITEMRECT, TRUE, &rc, gir);

            BOOL result = ImageList_BeginDrag(DragImage,
                                    0,               // use first image 
                                    nmtv->ptDrag.x - rc.left,  // coords of image to drag
                                    nmtv->ptDrag.y - rc.top); // ...

            logBeginDrag(       DragImage,
                                    0,               // use first image 
                                    nmtv->ptDrag.x - rc.left,  // coords of image to drag
                                    nmtv->ptDrag.y - rc.top,  // ...
                                    result);

            ImageList_DragEnter(nmtv->hdr.hwndFrom, // clipping region
                                nmtv->ptDrag.x,
                                nmtv->ptDrag.y);

            logDragEnter(nmtv->hdr.hwndFrom, nmtv->ptDrag.x, nmtv->ptDrag.y);

            ::ShowCursor(FALSE);  // hide the regular cursor
            logShowCursor(FALSE);

            SetCapture();         // capture the mouse
            logCapture(IDS_SETCAPTURE);

            isDragging = TRUE; 
           } /* do drag-drop */

        *pResult = 0;
}

void CTree::OnBeginlabeleditTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
        TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

        // We return 0 to permit the label edit.  We would return 1 to
        // cancel it (for an uneditable entry)

        CString s = itemString(&pTVDispInfo->item, exTVIF_MASK | TVIF_PARAM | TVIF_HANDLE);
        logNMHDR_b(IDS_TVN_BEGINLABELEDIT, pNMHDR, s, FALSE);
        
        *pResult = 0;
}

void CTree::OnDeleteitemTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
        NM_TREEVIEW* nmtv = (NM_TREEVIEW*)pNMHDR;
        CString s = itemString(&nmtv->itemOld, exTVIF_MASK | TVIF_PARAM | TVIF_HANDLE);
        logNMHDR(IDS_TVN_DELETEITEM, pNMHDR, s);
        
        *pResult = 0;
}

void CTree::OnEndlabeleditTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
        TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

        CString s = itemString(&pTVDispInfo->item, exTVIF_MASK | TVIF_PARAM | TVIF_HANDLE);
        logNMHDR(IDS_TVN_ENDLABELEDIT, pNMHDR, s);
        
        // We need to actually set the value.  Since only the text is being
        // edited, we set the mask to TVIF_TEXT and set the item.  If
        // the pszText field is NULL, the edit was cancelled

        if(pTVDispInfo->item.pszText != NULL)
           { /* save edit result */
            pTVDispInfo->item.mask = TVIF_TEXT;
            // TreeView_SetItem(pTVDispInfo->hdr.hwndFrom, &pTVDIspInfo->item)
            int result = c_Tree.SetItem(&pTVDispInfo->item);
            showResult_N_s_d(IDS_TVM_SETITEM, _T("&item"), result, IDS_LABELEDIT);
           } /* save edit result */

        // Result not used, logically void
        *pResult = 0;
}


/****************************************************************************
*                                 CTree::addToCache
* Inputs:
*       TV_ITEM * item: Item descriptor, fully loaded
* Result: void
*       
* Effect: 
*       Adds the filled-in item to the cache.
* Notes:
*       we don't actually ever clean the cache out.  This is probably a TODO:
*       item, but for right now we deal with a simpler world.  Not a good
*       programming practice, but the book is behind schedule...
*
*       We assume that if we are called the entry is not in the cache.
****************************************************************************/

void CTree::addToCache(TV_ITEM * item)
    {
     TV_ITEM * newitem = new TV_ITEM;

     if(newitem == NULL)
        AfxThrowMemoryException();  // die horribly...

     *newitem = *item;  // this gets us most of it...

     if(item->mask & TVIF_TEXT)  // except for the text, if needed...
        { /* add text */
         newitem->pszText = new TCHAR[lstrlen(item->pszText) + 1];
         if(newitem->pszText == NULL)
            AfxThrowMemoryException();
         lstrcpy(newitem->pszText, item->pszText);
        } /* add text */

     cache.AddTail(newitem); // we could add anywhere, or even use a map...
    }

/****************************************************************************
*                          CTree::retrieveFromCache
* Inputs:
*       TV_ITEM * item: Item descriptor
* Result: BOOL
*       TRUE if item is now updated, FALSE if not updated
* Effect: 
*       If the matching item is found in the cache, we copy its appropriate
*       values into the item passed in and return TRUE.  If not found, we
*       return FALSE.
****************************************************************************/

BOOL CTree::retrieveFromCache(TV_ITEM * item)
    {
     // To "match", the hItem field must match, and the state field must
     // match.  If a match is found, the values specified by the state field
     // are copied from the cached copy to the item parameter and we return
     // TRUE


     POSITION pos = cache.GetHeadPosition();
     while( pos != NULL )
        { /* search */
         TV_ITEM * candidate = cache.GetNext( pos );
         if(candidate->hItem != item->hItem)
            continue;  // items don't match
         if(candidate->mask != item->mask)
            continue;  // states don't match

         // Let us assume that a change in lParam is significant...
         if(candidate->lParam != item->lParam)
            continue;

         // FOUND IT!

         if(item->mask & TVIF_TEXT)
             lstrcpy(item->pszText, candidate->pszText);

         if(item->mask & TVIF_CHILDREN)
            item->cChildren = candidate->cChildren;

         if(item->mask & TVIF_IMAGE)
            item->iImage = candidate->iImage;

         if(item->mask & TVIF_SELECTEDIMAGE)
            item->iSelectedImage = candidate->iSelectedImage;

         return TRUE;
        } /* search */

     return FALSE;
    }

void CTree::OnGetdispinfoTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
 TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

 CString s;

 int wasinGetDispInfo = inGetDispInfo;

 inGetDispInfo++;

 s = itemString(&pTVDispInfo->item, exTVIF_MASK | TVIF_PARAM | TVIF_HANDLE);
 logNMHDR(IDS_TVN_GETDISPINFO, pNMHDR, s);


 // The 'inGetDispInfo' variable is an accidental requirement of the
 // Control Explorer.  Because the TVN_GETDISPINFO is triggered by the
 // WM_PAINT handler, and the dialog can pop up over the tree view, then
 // moving the dialog will trigger a recursive paint operation, and/or
 // dismissing it will trigger a new paint operation.  We avoid this by
 // locking the window against updates and using the inGetDispInfo variable
 // to keep us out of multiple repaints.

 if(c_Callbacks.GetCheck() == BST_CHECKED && wasinGetDispInfo == 0)
    { /* process callback */
     if(!retrieveFromCache(&pTVDispInfo->item)) 
        { /* prompt user */
         c_Tree.LockWindowUpdate();
         CMessageSheet sheet(IDS_TREE_GETDISPINFO);
         CCallback callback;

         callback.c_Messages = &c_Messages; // actually not used...
         callback.c_Tree = &c_Tree;
         callback.item   = &pTVDispInfo->item;
         callback.controller = this;

         sheet.AddPage(&callback);

         // There is the possibility of getting into an endless loop, so we let
         // the Cancel button terminate the handling of the TVN_GETDISPO info
         switch(sheet.DoModal())
            { /* response */
             case IDOK:
                     break;
             case IDCANCEL:
                     c_Callbacks.SetCheck(BST_UNCHECKED);
                     break;
            } /* response */

         c_Tree.UnlockWindowUpdate();
        } /* prompt user */
 
    } /* process callback */

 inGetDispInfo--;
 *pResult = 0;
}

void CTree::OnItemexpandedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
        NM_TREEVIEW* nmtv = (NM_TREEVIEW*)pNMHDR;
        CString s = itemString(&nmtv->itemNew, exTVIF_MASK | TVIF_STATE | TVIF_PARAM | TVIF_HANDLE);
        logNMHDR(IDS_TVN_ITEMEXPANDED, pNMHDR, s);
        
        // If we are doing one of the fancier simulations, we are going
        // to do something triggered by the lParam, which encodes an action
        // for our simulator.  If the user changes this code by using
        // TVM_SETITEM, we'll either get the behavior dictated by the code
        // or we won't do anything at all.

        // We do have to know the ranges, because we don't do subclassing
        // here.  In a proper C++ version, we would actually have subclassed
        // each control and supplied a virtual method, but because of the
        // way the Explorer does business, we don't *really* have a
        // subclassable window (in a real app, the class of the window would
        // be fixed at design time).

        if(nmtv->itemNew.lParam >= BOTANY_LOW &&
           nmtv->itemNew.lParam <= BOTANY_HIGH &&
           c_UseImages.GetCheck() == BST_CHECKED)
                OnBotanyExpanded(nmtv);
           
        enableControls();
        
        *pResult = 0;
}

void CTree::OnItemexpandingTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
        NM_TREEVIEW* nmtv = (NM_TREEVIEW*)pNMHDR;
        CString s = itemString(&nmtv->itemNew, exTVIF_MASK | TVIF_STATE | TVIF_PARAM | TVIF_HANDLE);
        LRESULT result = c_NoExpand.GetCheck() == BST_CHECKED ? TRUE : FALSE;
        logNMHDR_b(IDS_TVN_ITEMEXPANDING, pNMHDR, s, result);
        
        *pResult = result;
}

void CTree::OnKeydownTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
        TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
        // TODO: Add your control notification handler code here
        
        *pResult = 0;
}

void CTree::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
        NM_TREEVIEW* nmtv = (NM_TREEVIEW*)pNMHDR;
        CString s = itemString(&nmtv->itemOld, exTVIF_MASK | TVIF_STATE | TVIF_PARAM | TVIF_HANDLE) + _T("->") +
                    itemString(&nmtv->itemNew, exTVIF_MASK | TVIF_STATE | TVIF_PARAM | TVIF_HANDLE);
        logNMHDR(IDS_TVN_SELCHANGED, pNMHDR, s);

        
        *pResult = 0;
}

void CTree::OnSelchangingTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
        NM_TREEVIEW* nmtv = (NM_TREEVIEW*)pNMHDR;
        CString s = itemString(&nmtv->itemOld, exTVIF_MASK | TVIF_STATE | TVIF_PARAM | TVIF_HANDLE) + _T("->") +
                    itemString(&nmtv->itemNew, exTVIF_MASK | TVIF_STATE | TVIF_PARAM | TVIF_HANDLE);

        LRESULT result = c_NoSelect.GetCheck() == BST_CHECKED ? TRUE : FALSE;
        logNMHDR_b(IDS_TVN_SELCHANGING, pNMHDR, s, result);
        

        *pResult = result;
}

/****************************************************************************
*                            CTree::addStyle
* Inputs:
*       CString & s: String to add to
*       DWORD styles: Style flags
*       DWORD style: Style flag desired
*       int styleid: Style ID to add
* Result: void
*       
* Effect: 
*       If the style specified by the 'style' flag is present in 'styles',
*       appends " | " and style name to string
****************************************************************************/

void CTree::addStyle(CString & stylestr, DWORD styles, DWORD style, int styleid)
    {
     if((styles & style) == 0)
        return;
     CString s;
     s.LoadString(styleid);
     stylestr += _T( " | ");
     stylestr += s;
    }

/****************************************************************************
*                            CTree::styleToString
* Inputs:
*       DWORD style: Style flags
* Result: CString
*       
* Effect: 
*       Style flags as a string
****************************************************************************/

CString CTree::styleToString(DWORD style)
    {
     CString stylestr;
     addStyle(stylestr, style, TVS_DISABLEDRAGDROP, IDS_TVS_DISABLEDRAGDROP);
     addStyle(stylestr, style, TVS_EDITLABELS, IDS_TVS_EDITLABELS);
     addStyle(stylestr, style, TVS_HASBUTTONS, IDS_TVS_HASBUTTONS);
     addStyle(stylestr, style, TVS_HASLINES, IDS_TVS_HASLINES);
     addStyle(stylestr, style, TVS_LINESATROOT, IDS_TVS_LINESATROOT);
     addStyle(stylestr, style, TVS_SHOWSELALWAYS, IDS_TVS_SHOWSELALWAYS);
     addStyle(stylestr, style, WS_DISABLED, IDS_WS_DISABLED);
     addStyle(stylestr, style, WS_CLIPSIBLINGS, IDS_WS_CLIPSIBLINGS);
     addStyle(stylestr, style, WS_CHILD, IDS_WS_CHILD);
     addStyle(stylestr, style, WS_TABSTOP, IDS_WS_TABSTOP);
     addStyle(stylestr, style, WS_GROUP, IDS_WS_GROUP);
     addStyle(stylestr, style, WS_BORDER, IDS_WS_BORDER);
     addStyle(stylestr, style, WS_VISIBLE, IDS_WS_VISIBLE);
     return stylestr;
    }

/****************************************************************************
*                        CTree::logSetWindowLong
* Inputs:
*       DWORD style: Style bits to set
* Result: void
*       
* Effect: 
*       Logs the style bits for the SetWindowLong
****************************************************************************/

void CTree::logSetWindowLong(DWORD style)
    {
     CString fmt;
     fmt.LoadString(IDS_SETWINDOWLONG);

     CString s;
     s.Format(fmt, style, styleToString(style));
     
     CString ctlname;
     ctlname.LoadString(IDS_TREE);
     c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);
    }

void CTree::setStyle(BOOL set, DWORD style)
{
 DWORD newstyle = c_Tree.GetStyle();
 
 if(set)
    { /* set it */
     newstyle |= style;
    } /* set it */
 else
    { /* clear it */
     newstyle &= ~style;
    } /* clear it */
 ::SetWindowLong(c_Tree.m_hWnd, GWL_STYLE, newstyle);
 logSetWindowLong(newstyle);
}

void CTree::OnSetdispinfoTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
        TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
        // TODO: Add your control notification handler code here
        
        *pResult = 0;
}

void CTree::OnTvsDisabledragdrop() 
{
 setStyle(c_TVS_DisableDragDrop.GetCheck() == BST_CHECKED, TVS_DISABLEDRAGDROP);
}

void CTree::OnTvsEditlabels() 
{
 setStyle(c_TVS_EditLabels.GetCheck() == BST_CHECKED, TVS_EDITLABELS);
 enableControls();
}

void CTree::OnTvsHasbuttons() 
{
 setStyle(c_TVS_HasButtons.GetCheck() == BST_CHECKED, TVS_HASBUTTONS);
}

void CTree::OnTvsHaslines() 
{
 setStyle(c_TVS_HasLines.GetCheck() == BST_CHECKED, TVS_HASLINES);
}

void CTree::OnTvsHaslinesatroot() 
{
 setStyle(c_TVS_HasLinesAtRoot.GetCheck() == BST_CHECKED, TVS_LINESATROOT);
}

void CTree::OnTvsShowselalways() 
{
 setStyle(c_TVS_ShowSelAlways.GetCheck() == BST_CHECKED, TVS_SHOWSELALWAYS);
        
}

void CTree::OnWsBorder() 
{
        // TODO: Add your control notification handler code here
        
}

BOOL CTree::OnInitDialog() 
{
        CPropertyPage::OnInitDialog();
        
        CRect r;
        c_Tree.GetWindowRect(&r);
        c_Tree.setLimits(&r);
        
        int n;
        CString s;
        s.LoadString(IDS_MSG);
        n = c_Messages.GetStringWidth(s);
        s.LoadString(IDS_TREEVIEW);
        n = max(n, c_Messages.GetStringWidth(s));

        c_Messages.initialize(n);
        c_Messages.setWrap(TRUE);

        c_SmartDragDrop.SetCheck(BST_UNCHECKED);

        OnTvsHaslines();
        
        // Now create the image lists.  We do this in pure C so it is
        // easy to copy-and-paste this code into the book
        HINSTANCE hinst = AfxGetInstanceHandle(); // make copy-and-paste easy

        timer = 0; // no timer running

        HIMAGELIST list;
        list = ImageList_LoadImage(hinst, 
                                MAKEINTRESOURCE(IDB_TREEPICTURES), 
                                16,  // Bitmap width of each image
                                4,   // grow size
                                RGB(0,0,128),  // DKBLUE is masking color
                                IMAGE_BITMAP,  // type of resource
                                0);            // flags

        // We want to add to it, so we have to explicitly load the next
        // bitmap

        HBITMAP bm = (HBITMAP)::LoadImage(hinst, MAKEINTRESOURCE(IDB_TREEBOOKS),
                                IMAGE_BITMAP,
                                0, 0, // width & height not used for bitmaps
                                0);  // load flags

        ImageList_AddMasked(list, bm, RGB(0,0,128));
        ::DeleteObject(bm);  // no longer need this bitmap
        
        bm = (HBITMAP)::LoadImage(hinst, MAKEINTRESOURCE(IDB_TREECHECKS),
                                IMAGE_BITMAP,
                                0, 0, // not used for bitmaps
                                0);

        ImageList_AddMasked(list, bm, RGB(0,0,128));
        ::DeleteObject(bm);  // no longer need this

        // Now that we've got the list, make it C++/MFC compatible
        imagelist.Attach(list);

        // Do the same thing for our dummy bitmap

        list = ImageList_LoadImage(hinst, MAKEINTRESOURCE(IDB_1PIXEL),
                                        12, 1, 
                                        RGB(0, 0, 128), // masking color
                                        IMAGE_BITMAP,
                                        0);
        pixel1.Attach(list);

        HIMAGELIST stlist;
        stlist = ImageList_LoadImage(hinst, 
                                MAKEINTRESOURCE(IDB_BOTLIST), 
                                16,  // Bitmap width of each image
                                1,   // grow size
                                RGB(0,0,128),  // DKBLUE is masking color
                                IMAGE_BITMAP,  // type of resource
                                0);            // flags

        statelist.Attach(stlist);

        enableControls();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

// The duplication of the showResult-style code here shows that I really
// did the original decomposition incorrectly.  I didn't discover this until
// quite late in the project, and I didn't want to change it.  Nobody's
// perfect...  - jmn

void CTree::showResult_b_R_b(int msg, BOOL w, LPRECT r, BOOL result)
{
 CString t;
 t.LoadString(msg);

 CString ws;
 ws.LoadString(w ? IDS_TRUE : IDS_FALSE);
 
 CString rs;
 rs.LoadString(result ? IDS_TRUE : IDS_FALSE);

 CString s;
 s.Format(_T("%s(%s, &r); r=>%s [%d, %d, %d, %d]"), t, ws, rs,
                                                r->left, r->top, 
                                                r->right, r->bottom);
 CString ctlname;
 ctlname.LoadString(IDS_DRAGDROP);
 c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);

}

void CTree::showResult_d_s_b(int msg, int w, CString l, BOOL result)
    {
     CString t;
     t.LoadString(msg);

     CString rstr;
     rstr.LoadString(result ? IDS_TRUE : IDS_FALSE);

     CString s;
     s.Format(_T("%s(%d, %s) => %s"), t, w, l, rstr);
     
     CString ctlname;
     ctlname.LoadString(IDS_TREE);
     c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);
    }

void CTree::showResult_d_d_b(int msg, int w, int l, BOOL result)
    {
     CString t;
     t.LoadString(msg);

     CString rstr;
     rstr.LoadString(result ? IDS_TRUE : IDS_FALSE);

     CString s;
     s.Format(_T("%s(%d, %d) => %s"), t, w, l, rstr);
     
     CString ctlname;
     ctlname.LoadString(IDS_TREE);
     c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);
    }

void CTree::showResult_N_s_d(int msg, CString l, int result, int id)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(0, %s) => %d"), t, l, result);
     
     CString ctlname;
     ctlname.LoadString(id);
     c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);
    }

void CTree::showResult_N_s_x(int msg, CString l, int result, int id)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(0, %s) => %08x"), t, l, result);
     
     CString ctlname;
     ctlname.LoadString(id);
     c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);
    }

void CTree::showResult_N_s_s(int msg, CString l, CString result)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(0, %s) => %s"), t, l, result);
     
     CString ctlname;
     ctlname.LoadString(IDS_DRAGDROP);
     c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);
    }

void CTree::showResult_s_s_x(int msg, CString w, CString l, DWORD result)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(%s, %s) => %08x"), t, w, l, result);
     
     CString ctlname;
     ctlname.LoadString(IDS_DRAGDROP);
     c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);
    }

void CTree::logShowCursor(BOOL arg)
{
 CString t;
 t.LoadString(IDS_SHOWCURSOR);
 
 CString r;
 r.LoadString(arg ? IDS_TRUE : IDS_FALSE);

 CString s;
 s.Format(_T("%s(%s)"), t, r);

 CString ctlname;
 ctlname.LoadString(IDS_DRAGDROP);
 c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);
}

void CTree::logCapture(int id)
{
 CString s;
 s.LoadString(id);

 CString ctlname;
 ctlname.LoadString(IDS_DRAGDROP);
 c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);
}

void CTree::logDragEnter(HWND hwnd, int x, int y)
{
 CString t;
 t.LoadString(IDS_IMAGELIST_DRAGENTER);

 CString s;
 s.Format(_T("%s(%08x, %d, %d)"), t, hwnd, x, y);

 CString ctlname;
 ctlname.LoadString(IDS_DRAGDROP);
 c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);
}

void CTree::logDragLeave(HWND hwnd)
{
 CString t;
 t.LoadString(IDS_IMAGELIST_DRAGLEAVE);

 CString s;
 s.Format(_T("%s(%08x)"), t, hwnd);

 CString ctlname;
 ctlname.LoadString(IDS_DRAGDROP);
 c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);
}

void CTree::logBeginDrag(HIMAGELIST list, int index, int x, int y, BOOL result)
{
     CString t; 
     t.LoadString(IDS_IMAGELIST_BEGINDRAG);

     CString s;
     s.Format(_T("%s(%08x, %d, %d, %d) => %08x"), t, list, x, y, result);

     CString ctlname;
     ctlname.LoadString(IDS_DRAGDROP);
     c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);
}

void CTree::logEndDrag()
{
        CString t;
        t.LoadString(IDS_IMAGELIST_ENDDRAG);

        CString s;
        s.Format(_T("%s()"), t);

        CString ctlname;
        ctlname.LoadString(IDS_DRAGDROP);
        c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);
}

void CTree::logImageListDestroy(HIMAGELIST list)
{
        CString t;
        t.LoadString(IDS_IMAGELIST_DESTROY);

        CString s;
        s.Format(_T("%s(%08x)"), t, list);

        CString ctlname;
        ctlname.LoadString(IDS_DRAGDROP);
        c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);
}

void CTree::showResult_N_x_x(int msg, DWORD l, DWORD result)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(0, %08x) => %08x"), t, l, result);
     
     CString ctlname;
     ctlname.LoadString(IDS_DRAGDROP);
     c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);
    }

void CTree::showResult_N_N_N(int msg)
    {
     CString t;
     t.LoadString(msg);

     CString ctlname;
     ctlname.LoadString(IDS_DRAGDROP);
     c_Messages.addMessage(c_Tree.m_hWnd, ctlname, t);
    }

void CTree::showResult_N_hti_h(int msg, HTREEITEM result)
    {
     CString t;
     t.LoadString(msg);

     CString rs;
     if(result == NULL)
        rs.LoadString(IDS_NULL);
     else
        rs.Format(_T("%08x"), result);

     CString ts;

     if(result != NULL)
        { /* has item */
         TV_ITEM tvi;
         TCHAR text[256];
         tvi.hItem = result;
         tvi.mask = TVIF_TEXT;
         tvi.pszText = text;
         tvi.cchTextMax = DIM(text);
         c_Tree.GetItem(&tvi);
         ts = _T("\"") + CString(text) + _T("\"");
        } /* has item */
     else
        { /* null */
         ts.LoadString(IDS_NULL);
        } /* null */

     CString s;
        
     s.Format(_T("%s(0, &hti) => %s %s"), t, rs, ts);
     
     CString ctlname;
     ctlname.LoadString(IDS_DRAGDROP);
     c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);
    }

void CTree::OnDemo1() 
{
 BOOL result = c_Tree.SendMessage(TVM_DELETEITEM, 0, (LPARAM)TVI_ROOT);

 CString s;
 s.LoadString(IDS_TVI_ROOT);
 showResult_d_s_b(IDS_TVM_DELETEITEM, 0, s, result);

 insertDemo(Demo1Data, TVI_ROOT, 1);
}

void CTree::OnResize() 
{
 c_Tree.setResize(c_Resize.GetCheck() == BST_CHECKED);
}


void CTree::OnSize(UINT nType, int cx, int cy) 
{
        CPropertyPage::OnSize(nType, cx, cy);
        
}

/****************************************************************************
*                           CTree::OnBotanyExpanded
* Inputs:
*       NMHDR * pNMHDR: Header
*       LRESULT * pResult: not used for this function
* Result: void
*       
* Effect: 
*       Handles the image management for the node.
* Notes:
*       This violates all sorts of nice modular design principles, but 
*       we have to do this to get a fully-general Explorer with some
*       neat hacks.  If we were doing this in a real app, the notification
*       would be reflected down into the class, which would be a derived
*       subclass of CTreeCtrl, and it would do this (we also wouldn't have
*       to deal with the fact that the user can change any value at any time
*       with the Explorer, which keeps us from making a special botany
*       subclass for this application).
* Notes:
*       Our state image list looks like this:
*
*             +-----------+-----------+-----------+-----------+
*             |   ORDER   |  FAMILY   |   GENUS   |  SPECIES  |
*       +-----+-----+-----+-----+-----+-----+-----+-----+-----+
*       |     |     |     |     |     |     |     |     |     |
*       |blank|books|books|book |book |foldr|foldr|bud  |flowr|
*       |     |     |     |     |     |     |     |     |     |
*       +-----+-----+-----+-----+-----+-----+-----+-----+-----+
*          0     1     2     3     4     5     6     7     8
*
****************************************************************************/


void CTree::OnBotanyExpanded(NM_TREEVIEW * hdr)
{
 int state = 0;
 switch(hdr->itemNew.lParam)
    { /* decode action */
     case DEMO_ORDER:
             state = IMAGE_BOOKS_UNSELECTED;
             break;
     case DEMO_FAMILY:
             state = IMAGE_BOOK_UNSELECTED;
             break;
     case DEMO_GENUS:
             state = IMAGE_FOLDER_UNSELECTED;
             break;
     case DEMO_SPECIES:
             state = IMAGE_FLOWER_UNSELECTED;
             break;
    } /* decode action */

 // state is now our "base state".  If it is 0 we are done, but if it is
 // nonzero, we need to determine if we should show the expanded or
 // unexpanded version.

 if(state != 0)
    { /* show expand state */
     if(hdr->itemNew.state & TVIS_EXPANDED)
        state++;  // talk about *really* ugly hacks.  I'd never do this
                  // in production code!!!!
    } /* show expand state */

 // We now have a state value.  We need to set the item
 TV_ITEM tv;
 tv.mask = TVIF_STATE | TVIF_HANDLE;
 tv.hItem = hdr->itemNew.hItem;
 tv.state = INDEXTOSTATEIMAGEMASK(state);
 tv.stateMask = TVIS_STATEIMAGEMASK;
 int result = c_Tree.SendMessage(TVM_SETITEM, 0, (LPARAM)&tv);
 
 CString s;
 CString fmt;
 fmt.LoadString(IDS_SIMUL_SETITEM); // TVM_SETITEM(0, {TVIF_ITEM | TVIF_STATE,
                                    //                 %08x, ix2st(%d) })
 s.Format(fmt, tv.iImage, state);
 showResult_N_s_d(IDS_TVM_SETITEM, s, result);

}


void CTree::OnUseimages() 
{
 // We enable the simulation by setting the image mask

 HIMAGELIST newimg = (c_UseImages.GetCheck() == BST_CHECKED) 
                                        ? statelist.m_hImageList
                                        : NULL;

 HIMAGELIST old = (HIMAGELIST)c_Tree.SendMessage(TVM_SETIMAGELIST, TVSIL_STATE, 
                                                            (LPARAM)newimg);

 CString tvsil;
 tvsil.LoadString(IDS_TVSIL_STATE);
 // 
 CString s;
 s.LoadString(IDS_STATELIST);
 showResult_s_s_x(IDS_TVM_SETIMAGELIST, tvsil, s, (DWORD)old);
}

/****************************************************************************
*                           CTree::OnRecordDispInfo
* Inputs:
*       WPARAM: ignored
*       LPARAM: (LPARAM)(TV_ITEM *)lParam
* Result: LRESULT
*       0, always
* Effect: 
*       Logs the item information so we can process it later for the next
*       TVN_GETDISPINFO message
****************************************************************************/

LRESULT CTree::OnRecordDispInfo(WPARAM, LPARAM lParam)
    {
     TV_ITEM * item = (TV_ITEM *)lParam;
     addToCache(item);

     return 0;
    }

/****************************************************************************
*                          void CTree::copyChildren
* Inputs:
*       // HWND htree: handle to tree control ['this' in C++]
*       HTREEITEM to: Item to copy children to
*       HTREEITEM from: Item to copy children from
* Result: void
*       
* Effect: 
*       (Recursively) copies the child items from the 'from' node to the
*       'to' node
****************************************************************************/

void CTree::copyChildren(HTREEITEM to, HTREEITEM from)
    {
     HTREEITEM child = c_Tree.GetChildItem(from);
     HTREEITEM newchild;

     while(child != NULL)
        { /* copy children */
         TV_ITEM newitem;
         TCHAR buffer[256];
         TV_INSERTSTRUCT tvins;
         
         newitem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_PARAM |
                        TVIF_STATE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;
         newitem.stateMask = 0xFFFFFFFF;
         newitem.hItem = child;
         newitem.pszText = buffer;
         newitem.cchTextMax = DIM(buffer);

         // TreeView_GetItem(htree, &newitem);

         c_Tree.GetItem(&newitem);  // retrieve current child data
         tvins.hParent = to;
         tvins.hInsertAfter = TVI_LAST;
         tvins.item = newitem;
         tvins.item.state &= ~TVIS_SELECTED;  // make sure to lose selection bit

         // newchild = TreeView_InsertItem(htree, &tvins);
         newchild = c_Tree.InsertItem(&tvins);

         copyChildren(newchild, child);
         child = c_Tree.GetNextSiblingItem(child);
        } /* copy children */
    }

/****************************************************************************
*                              CTree::treeLevel
* Inputs:
*       HTREEITEM item: The handle of a tree item
* Result: int
*       The depth of the item in the tree, as determined by the lParam
****************************************************************************/

int CTree::treeLevel(HTREEITEM item)
    {
     TV_ITEM tvi;
     tvi.hItem = item;
     tvi.mask = TVIF_PARAM;
     c_Tree.GetItem(&tvi);
     return tvi.lParam;
    }

void CTree::OnLButtonUp(UINT nFlags, CPoint point) 
    {
     HWND htree = c_Tree.m_hWnd;  // look like C for copy-and-paste ease

     if(timer != 0)
        { /* kill timer */
         logKillTimer(timer);
         KillTimer(timer);
         timer = 0;
        } /* kill timer */

     if(isDragging)
        { /* handle drop */
         HTREEITEM target;
         TV_ITEM tvi;
         TCHAR buffer [256];
         TV_INSERTSTRUCT tvins;
            
         ImageList_EndDrag();
         logEndDrag();
            
         ImageList_DragLeave(htree);
         logDragLeave(htree);

         ShowCursor(TRUE);  // restore mouse cursor
         logShowCursor(TRUE);

         ReleaseCapture();
         logCapture(IDS_RELEASECAPTURE);

         logImageListDestroy(DragImage);
         ImageList_Destroy(DragImage);
         DragImage = NULL;

         // OK, we've handled the bookkeeping.  Now actually do the drop

         // target = TreeView_GetDropHilight(htree);
         target = c_Tree.GetDropHilightItem();

         // To do the drop, we must
         //     (a) have a target
         //     (b) the target must not be the drag item if we are doing
         //         a drag-move (but OK if we're doing a drag-copy)
         //     (c) 0. We are doing unrestricted hierarchy drag-drop or
         //         1. the tree levels must either be compatible, or
         //         2. the target must be only one level above the source

         if(target != NULL && 
            (dragCopy || target != DragItem) && 
            ((c_SmartDragDrop.GetCheck() == BST_UNCHECKED) ||
             ( treeLevel(target) == treeLevel(DragItem) ||
               treeLevel(target) == treeLevel(DragItem) + 1)))
            { /* drop on target */
             tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE |
                                        TVIF_PARAM | TVIF_STATE;
             tvi.hItem = DragItem;
             tvi.pszText = buffer;
             tvi.cchTextMax = DIM(buffer);

             // TreeView_GetItem(htree, &tvi);
             c_Tree.GetItem(&tvi);  // get the current item
                
             if(c_SmartDragDrop.GetCheck() == BST_CHECKED)
                { /* smart */
                 // Determine where and how to insert it.  If we inserting at
                 // the same level, we use the parent of the target.  If we
                 // are inserting a child level the parent *is* the target

                 if(treeLevel(target) == treeLevel(DragItem) + 1)
                    { /* child insert */
                     tvins.hParent = target;
                    } /* child insert */
                 else
                    { /* sibling insert */
                     tvins.hParent = c_Tree.GetParentItem(target);
                    } /* sibling insert */
                } /* smart */
             else
                { /* normal */
                 //tvins.hParent = TreeView_GetParent(htree, target);
                 tvins.hParent = c_Tree.GetParentItem(target);
                } /* normal */

             // Now we have to figure out where to put it...

             //                           *        *         *
             // [+]--item1     S  S  S    T        T  *      T  *
             //  |
             // [+]--item2     T*         S  S  S     T         T  *
             //  |
             // [+]--item3        T*         T*    S  S  S         T
             //  |
             // [+]--item4           T*         T*       T*  S  S  S
             //
             // sibling is     D  D  D    U  D  D  U  U  D   U  U  U
                         
             // If we are moving down at the same level, then the insert-after
             // is the drop target.  If we are moving up at the same level,
             // the insert-after is the  predecessor of the drop target

             tvins.hInsertAfter = c_Tree.GetPrevSiblingItem(target);
             if(tvins.hInsertAfter == NULL)
                tvins.hInsertAfter = TVI_FIRST;

             if(isDownSibling(DragItem, target))
                { /* Drag down */
                 if(!dragCopy)
                    tvins.hInsertAfter = target;
                } /* Drag down */
             else
             if(!isUpSibling(DragItem, target))
                { /* child insert */
                 if(c_SmartDragDrop.GetCheck() == BST_CHECKED)
                    { /* smart insert */
                     if(treeLevel(target) == treeLevel(DragItem) + 1)
                        tvins.hInsertAfter = TVI_FIRST;
                    } /* smart insert */
                } /* child insert */

             tvins.item = tvi;  // copy entire item across


             // TreeView_InsertItem(htree, &tvins);
             HTREEITEM newItem = c_Tree.InsertItem(&tvins);

             CString instr = CInsertItem::formatInsertItem(&c_Tree, &tvins);
             CString newstr;
             if(newItem == NULL)
                newstr.LoadString(IDS_NULL);
             else
                newstr.Format(_T("%08x"), newItem);

             showResult_N_s_s(IDS_TVM_INSERTITEM, instr, newstr);
             

             // If item has children, copy all child items (recursively)
             // to new location

             // copyChildren(htree, newItem, DragItem);
             copyChildren(newItem, DragItem);

             if(!dragCopy)
                { /* delete old one */
                 // If the parent now has no children, we need to reset its
                 // TVIS_EXPANDED flag to make its button go away

                 // parent = TreeView_GetParent(htree, DragItem);
                 HTREEITEM parent = c_Tree.GetParentItem(DragItem);


                 // TreeView_DeleteItem(htree, DragItem);
                 c_Tree.DeleteItem(DragItem);

                 if(parent != NULL)
                    { /* check child state */
                     // See if, after delete, there are any children
                     if(c_Tree.GetChildItem(parent) == NULL)
                        { /* no children left */
                         TV_ITEM tvp;
                         tvp.mask = TVIF_STATE;
                         tvp.hItem = parent;

                         // TreeView_GetItem(htree, &tvp);
                         c_Tree.GetItem(&tvp);
                         if(tvp.state & TVIS_EXPANDED)
                            { /* unexpand */
                             tvp.state = 0;
                             tvp.stateMask = TVIS_EXPANDED;
                             // TreeView_SetItem(htree, &tvp);
                             c_Tree.SetItem(&tvp);
                            } /* unexpand */
                        } /* no children left */
                    } /* check child state */
                } /* delete old one */

            } /* drop on target */

         // Remove any drop highlight already present
         // TreeView_SelectDropTarget(htree, NULL);
         c_Tree.SelectDropTarget(NULL);
         isDragging = FALSE;
        } /* handle drop */
        
     CPropertyPage::OnLButtonUp(nFlags, point);
    }

/****************************************************************************
*                             CTree::isUpSibling
* Inputs:
*       HTREEITEM source: Source item
*       HTREEITEM target: Target item
* Result: BOOL
*       TRUE if the target is a predecessor sibling of source
****************************************************************************/

BOOL CTree::isUpSibling(HTREEITEM source, HTREEITEM target)
    {
     while(source != NULL)
        { /* scan up */
         source = c_Tree.GetPrevSiblingItem(source);
         if(source == target)
            return TRUE;
        } /* scan up */
     return FALSE;
    }

/****************************************************************************
*                             CTree::isDownSibling
* Inputs:
*       HTREEITEM source: Source item
*       HTREEITEM target: Target item
* Result: BOOL
*       TRUE if the target is a predecessor sibling of source
****************************************************************************/

BOOL CTree::isDownSibling(HTREEITEM source, HTREEITEM target)
    {
     while(source != NULL)
        { /* scan up */
         source = c_Tree.GetNextSiblingItem(source);
         if(source == target)
            return TRUE;
        } /* scan up */
     return FALSE;
    }

/****************************************************************************
*                               CTree::isParent
* Inputs:
*       HTREEITEM target: Hypothesized target node
* Result: BOOL
*       TRUE if an upward tree walk from 'target' encounters 'dragee'
*       FALSE otherwise
****************************************************************************/

BOOL CTree::isParent(HTREEITEM target, HTREEITEM dragee)
    {
#ifdef WANTISPARENTTRACE
     CString s;
     s.Format(_T("isParent(%08x, %08x)"), target, dragee);
     c_Messages.addMessage(c_Tree.m_hWnd, _T("Trace"), s);
#endif

     if(dragee == NULL)
        return FALSE;    // no dragee, shouldn't even be here
     if(target == NULL)
        return FALSE;    // hit root of tree without finding dragee
     if(dragee == target)
        return TRUE;     // we would be copying into a subtree! No!
     return isParent(c_Tree.GetParentItem(target), dragee);
    }

/****************************************************************************
*                              CTree::timerProc
* Inputs:
*       HWND hwnd: Window handle of tree control
*       UINT: message value, WM_TIMER, ignored
*       UINT: timer ID, ignored
*       DWORD: system time, ignored
* Result: void
*       
* Effect: 
*       Handles the time event.  We use a callback instead of a WM_TIMER
*       message because we want resolution finer than WM_TIMER guarantees.
*       But we actually fake out the timer event by sending a message to
*       the window
****************************************************************************/

void CALLBACK CTree::timerProc(HWND hwnd, UINT, UINT, DWORD)
    {
     ::PostMessage(hwnd, UWM_SCROLL, 0, 0);
    }

/****************************************************************************
*                             CTree::logKillTimer
* Inputs:
*       int timerid: Timer id
* Result: void
*       
* Effect: 
*       Logs timer kill event
****************************************************************************/

void CTree::logKillTimer(int timerid)
    {
     CString t;
     t.LoadString(IDS_KILLTIMER);
     CString s;
     s.Format(_T("%s(%d)"), t, timerid);
     
     CString ctlname;
     ctlname.LoadString(IDS_TIMER);
     c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);
     
    }

/****************************************************************************
*                              CTree::autoScroll
* Inputs:
*       int SBcode: direction to scroll
*       int timecode: Indication of how fast to scroll
* Result: void
*       
* Effect: 
*       Autoscrolls the tree control.
* Notes:
*       It is assumed that if this is called the tree control has a
*       scroll bar
****************************************************************************/

#define SCROLL_OFF    0
#define SCROLL_SLOWLY 1
#define SCROLL_FASTER 2

void CTree::autoScroll(int SBcode, int timecode)
    {
     if(timecode == SCROLL_OFF)
        { /* stop scroll */
         if(timer != 0)
            { /* kill active timer */
             logKillTimer(timer);
             KillTimer(timer);
             timer = 0;
            } /* kill active timer */
         return;
        } /* stop scroll */

     ScrollDirection = SBcode;

     int time;
     if(timecode == SCROLL_SLOWLY)
        time = 250;
     else
        time = 100;

     timer = SetTimer(SCROLL_TIMER, time, timerProc);

     CString t;
     t.LoadString(IDS_SETTIMER);
     CString s;
     s.Format(_T("%s(%d, %d, ...)"), t, SCROLL_TIMER, time);
     
     CString ctlname;
     ctlname.LoadString(IDS_TIMER);
     c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);
    }

/****************************************************************************
*                            CTree::logautoScroll
* Inputs:
*       int dir: String ID of direction
*       int y: Cursor y-position
*       LPCTSTR compare: "<" or ">"
*       int zid: Zone string ID
*       int limit: Zone limit value
* Result: void
*       
* Effect: 
*       Logs the autoscroll conditions and reason
****************************************************************************/

void CTree::logautoScroll(int dir, int y, LPCTSTR compare, int zid, int limit)
    {
     // autoScroll: 46 < 52 (top)
     //

     CString dirstr;
     dirstr.LoadString(dir);

     CString zstr;
     zstr.LoadString(zid);

     CString t;
     t.LoadString(IDS_AUTOSCROLL);

     CString s;
     s.Format(_T("%s: %d %s %d (%s)"), t, y, compare, limit, zstr);

     CString ctlname;
     ctlname.LoadString(IDS_DRAGDROP);
     c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);
    }

void CTree::OnMouseMove(UINT nFlags, CPoint point) 
    {
     HWND htree = c_Tree.m_hWnd;  // look like C so paste into book is easy
     int x = point.x;             // ...
     int y = point.y;             // ...

     if(isDragging)
        { /* handle drag */
         HTREEITEM target;   // place we are likely to drop item
         TV_HITTESTINFO hti; // use for hit-test

         BOOL result = ImageList_DragMove(x, y);
         showResult_d_d_b(IDS_IMAGELIST_DRAGMOVE, x, y, result);

         // Now see where we are
         hti.pt.x = x;
         hti.pt.y = y;

         //target =  TreeView_HitTest(htree, &hti);
         target = c_Tree.HitTest(&hti);

         // see if we're over the window
         showResult_N_hti_h(IDS_TVM_HITTEST, target);

         // Note that the target may be NULL, in which case we will
         // deselect the drop target entirely

         // Since we are copying the subtree, we can't copy an item if
         // we are copying it into a subtree of itself.  So if a walk up
         // to the root of the tree from the proposed target hits the
         // DragItem, we set the target to NULL

         // Note that we want to allow the target to be the original
         // item.  Because of how isParent is written, we have to test
         // for this special case here.

         if(target != DragItem && isParent(target, DragItem))
            target = NULL;

         // If we are doing "smart" level-maintaining Drag & Drop, we
         // check here that we are not violating tree hierarchy

         if(c_SmartDragDrop.GetCheck() == BST_CHECKED && target != NULL)
            { /* be smart */
             // This is almost too easy.  We've encoded the level in the
             // lParam field, so we can only select the target if the
             // lParam fields are identical

             if(treeLevel(target) != treeLevel(DragItem))
                { /* not same level */
                 // Are we trying to drop a child on an empty parent?
                 if(treeLevel(target) != treeLevel(DragItem) + 1)
                    { /* not compatible */
                     target = NULL;  // can't do it
                    } /* not compatible */
                } /* not same level */
            } /* be smart */

         ImageList_DragShowNolock(FALSE); // hide the image

         // TreeView_SelectDropTarget(htree, target);
         c_Tree.SelectDropTarget(target);

         ImageList_DragShowNolock(TRUE); // make it visible again

         // Now the question is, are we outside the window, and is it
         // scrollable?

         // If the window is not scrollable, we don't bother

         checkAutoScroll(x, y);
        } /* handle drag */
            
        
     CPropertyPage::OnMouseMove(nFlags, point);
    }


/****************************************************************************
*                           CTree::checkAutoScroll
* Inputs:
*       int x: Mouse position
*       int y: Mouse position
* Result: void
*       
* Effect: 
*       If the mouse is within a certain distance of the edge of the tree
*       control, or completely outside it above or below, initiate an
*       autoscroll action
****************************************************************************/

void CTree::checkAutoScroll(int x, int y)
    {
     SCROLLINFO si;
     BOOL hasscroll = c_Tree.GetScrollInfo(SB_VERT, &si, SIF_RANGE);
     if(hasscroll && si.nMin != si.nMax)
        { /* has scroll bar */
         // If we have a scroll bar, see if the mouse is in the window
         POINT pt;
         RECT rc;
         pt.x = x;
         pt.y = y;
         c_Tree.GetClientRect(&rc);
         if(PtInRect(&rc, pt))
            { /* in window */
             // See if we're in the "scroll zone".  WIGSD suggests 
             // 2xscroll height, but we choose 1xscroll height
             if(pt.y < rc.top + ::GetSystemMetrics(SM_CYHSCROLL))
                { /* scroll up */
                 logautoScroll(IDS_SB_LINEUP, pt.y, _T("<"), IDS_TOPSZ,
                           rc.top + ::GetSystemMetrics(SM_CYHSCROLL));
                 autoScroll(SB_LINEUP, SCROLL_SLOWLY);
                } /* scroll up */
             else
             if(pt.y > rc.bottom - ::GetSystemMetrics(SM_CYHSCROLL))
                { /* scroll down */
                 logautoScroll(IDS_SB_LINEDOWN, pt.y, _T(">"), IDS_BOTTOMSZ,
                        rc.bottom - ::GetSystemMetrics(SM_CYHSCROLL));
                 autoScroll(SB_LINEDOWN, SCROLL_SLOWLY);
                } /* scroll down */
             else
                autoScroll(0, SCROLL_OFF);
            } /* in window */
         else
            { /* outside window */
             if(pt.y < rc.top)
                { /* fast up */
                 logautoScroll(IDS_SB_LINEUP, pt.y, _T("<"), IDS_TOP,
                        rc.top);
                 autoScroll(SB_LINEUP, SCROLL_FASTER);
                } /* fast up */
             else
             if(pt.y > rc.bottom)
                { /* fast down */
                 logautoScroll(IDS_SB_LINEDOWN, pt.y, _T(">"), IDS_BOTTOM, rc.bottom);
                 autoScroll(SB_LINEDOWN, SCROLL_FASTER);
                } /* fast down */
            } /* outside window */
        } /* has scroll bar */
     
    }
void CTree::enableControls()
{
 c_SmartDragDrop.EnableWindow( c_DragDrop.GetCheck() == BST_CHECKED); 
 
 // We do not enable drag and drop unless there is an image list

 c_DragDrop.EnableWindow(c_Tree.GetImageList(TVSIL_NORMAL) != NULL);

 SCROLLINFO si;
 BOOL hasScroll = c_Tree.GetScrollInfo(SB_VERT, &si, SIF_POS);

 c_Up.EnableWindow(hasScroll);
 c_Down.EnableWindow(hasScroll);

 BOOL enable = (c_Tree.GetStyle() & TVS_EDITLABELS) != 0;
 c_LabelEdit.EnableWindow(enable);
}

void CTree::OnDragdrop() 
{
 enableControls();
}

/****************************************************************************
*                               CTree::OnScroll
* Inputs:
*       WPARAM: ignored
*       LPARAM: ignored
* Result: LRESULT
*       0, always
* Effect: 
*       Scrolls the window by the amount shown in the 'ScrollDirection'
*       parameter
****************************************************************************/

LRESULT CTree::OnScroll(WPARAM, LPARAM)
    {
     CString t;
     t.LoadString(IDS_WM_VSCROLL);
     CString dir;

     switch(ScrollDirection)
        { /* direction */
         case SB_LINEUP:
                 dir.LoadString(IDS_SB_LINEUP);
                 break;
         case SB_LINEDOWN:
                 dir.LoadString(IDS_SB_LINEDOWN);
                 break;
        } /* direction */

     POINT pt;
     ::GetCursorPos(&pt);

     checkAutoScroll(pt.x, pt.y);

     CString null;
     null.LoadString(IDS_NULL);

     CString s;
     s.Format(_T("%s(%s, %s)"), t, dir, null);
     
     CString ctlname;
     ctlname.LoadString(IDS_TIMER);
     c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);

     ImageList_DragShowNolock(FALSE); // hide image
     ::SendMessage(c_Tree.m_hWnd, WM_VSCROLL,
                        MAKELONG(ScrollDirection, 0), 
                        (LPARAM)NULL);
     ImageList_DragShowNolock(TRUE); // show image

     return 0;
    }

void CTree::OnDown() 
{
 ::SendMessage(c_Tree.m_hWnd, WM_VSCROLL, MAKELONG(SB_LINEDOWN, 0), NULL);
}

void CTree::OnUp() 
{
 ::SendMessage(c_Tree.m_hWnd, WM_VSCROLL, MAKELONG(SB_LINEUP, 0), NULL);
}

void CTree::OnImagelist() 
{
 c_Tree.SendMessage(TVM_SETIMAGELIST, TVSIL_NORMAL, 
                                       (LPARAM)imagelist.m_hImageList);

 enableControls();

 CString t;
 t.LoadString(IDS_TVM_SETIMAGELIST);
 
 CString type;
 type.LoadString(IDS_TVSIL_NORMAL);
 
 CString imgname;
 imgname.LoadString(IDS_IMG);

 CString s;
 s.Format(_T("%s(%s, %s)"), t, type, imgname);
 
 CString ctlname;
 ctlname.LoadString(IDS_MSG);
 c_Messages.addMessage(c_Tree.m_hWnd, ctlname, s);
        
}


/****************************************************************************
*                          CTree:PreTranslateMessage
* Inputs:
*       MSG * pMsg: MSG structure
* Result: BOOL
*       �
* Effect: 
*       This must be overridden to allow the edit control within the
*       edit-labels structure to receive the Enter and Esc keys.
* Notes:
*       See KB article Q125645: FIX: Edit Control in Property Page Does
*       Not Get Return Keys.  Several bug fixes were made to that code
*       when it was installed here, not the least of which is the
*       incorrect use of sizeof.  The most serious bug fix is that the
*       correct value to return is TRUE, because otherwise the 
*       property *sheet* will next get the message and mess over the world.
*       But returning TRUE will terminate the processing of the message,
*       so we have to ::TranslateMessage and ::DispatchMessage right here!
****************************************************************************/

BOOL CTree::PreTranslateMessage(MSG* pMsg) 
{
 if(pMsg->message == WM_KEYDOWN &&
    (pMsg->wParam == VK_RETURN ||
     pMsg->wParam == VK_ESCAPE))
        { /* special key */
         static const TCHAR editclassname[] = _T("edit");
         // We add +1 not for the NUL, which is already accounted for
         // by the DIM size, but to guarantee that a class name like "editor"
         // is not truncated to "edit" and appear to be equal.
         TCHAR focusname[DIM(editclassname) + 1];
         HWND hFocus = ::GetFocus();

         ::GetClassName(hFocus, focusname, DIM(focusname));
         if(lstrcmpi(focusname, editclassname) == 0)
            { /* edit control */
             if(::GetWindowLong(hFocus, GWL_STYLE) & ES_WANTRETURN)
                { /* deal with it */
                 ::TranslateMessage(pMsg);
                 ::DispatchMessage(pMsg);
                 return TRUE; // don't pretranslate
                } /* deal with it */
            } /* edit control */

         if(::SendMessage(hFocus, WM_GETDLGCODE, 0, 0) & 
             (DLGC_WANTALLKEYS | DLGC_WANTCHARS | DLGC_WANTMESSAGE))
                { /* deal with it here */
                 ::TranslateMessage(pMsg);
                 ::DispatchMessage(pMsg);
                 return TRUE;
                } /* deal with it here */
        } /* special key */

 return CPropertyPage::PreTranslateMessage(pMsg);
}
