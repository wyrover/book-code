#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <stdio.h>
#include <OleAuto.h>
#include "listview.h"
#include "resource.h"
#include "AccessEnum.h"


void InitListViewColumns(HWND hListView, const LISTVIEW_COLUMN * Cols, int numColumns, long style)
{
    HKEY    hKey;
    DWORD   valLength;
    TCHAR   colName[32];
    RECT    rc;
    // Get window sizes
    rc.left = -1;
    rc.top = -1;
    RegCreateKey(HKEY_CURRENT_USER, _T("Software\\Sysinternals\\AccessEnum"), &hKey);
    valLength = sizeof(DWORD);
    RegQueryValueEx(hKey, _T("x"), NULL, NULL, (LPBYTE) &rc.left,
                    &valLength);
    RegQueryValueEx(hKey, _T("y"), NULL, NULL, (LPBYTE) &rc.top,
                    &valLength);
    RegQueryValueEx(hKey, _T("w"), NULL, NULL, (LPBYTE) &rc.right,
                    &valLength);
    RegQueryValueEx(hKey, _T("h"), NULL, NULL, (LPBYTE) &rc.bottom,
                    &valLength);

    if (rc.left != -1 && rc.top != -1)
        MoveWindow(GetParent(hListView), rc.left, rc.top, rc.right, rc.bottom, TRUE);

    // Set listview styles
    ListView_SetExtendedListViewStyleEx(hListView, style, style);

    // Create columns
    for (int i = 0; i < numColumns; ++i)  {
        LVCOLUMN    col = { 0 };
        col.mask    = LVCF_TEXT | LVCF_WIDTH;
        col.pszText = (TCHAR *)Cols[i].Title;
        col.cx      = Cols[i].Width;
        _stprintf(colName, _T("Column%d"), i);
        valLength = sizeof(DWORD);
        RegQueryValueEx(hKey, colName, NULL, NULL, (LPBYTE) &col.cx,
                        &valLength);
        ListView_InsertColumn(hListView, i, &col);
    }

    RegCloseKey(hKey);
    HINSTANCE hInstance = GetModuleHandle(NULL);
    // Create image list (order corresponds to LISTVIEW_IMG_* enumeration)
    HIMAGELIST  hImageList  = ImageList_Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), ILC_COLORDDB, 10, 30);
    UINT        loadFlags   = LR_DEFAULTCOLOR | LR_LOADTRANSPARENT | LR_SHARED;
    ImageList_SetBkColor(hImageList, GetSysColor(COLOR_BTNFACE));
    ImageList_AddIcon(hImageList, (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_DOWNARROW),  IMAGE_ICON, 16, 16, loadFlags));
    ImageList_AddIcon(hImageList, (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_UPARROW),        IMAGE_ICON, 16, 16, loadFlags));
    ImageList_SetBkColor(hImageList, GetSysColor(COLOR_WINDOW));
    ListView_SetImageList(hListView, hImageList, LVSIL_SMALL);
}

void SaveListViewColumns(HWND hListView)
{
    HKEY    hKey;
    DWORD   valLength;
    TCHAR   colName[32];
    RECT    rc;
    DWORD   width;
    // Get window sizes
    GetWindowRect(GetParent(hListView), &rc);
    rc.right -= rc.left;
    rc.bottom -= rc.top;
    RegCreateKey(HKEY_CURRENT_USER, _T("Software\\Sysinternals\\AccessEnum"), &hKey);
    valLength = sizeof(DWORD);
    RegSetValueEx(hKey, _T("x"), NULL, REG_DWORD, (LPBYTE) &rc.left,
                  sizeof(DWORD));
    RegSetValueEx(hKey, _T("y"), NULL, REG_DWORD, (LPBYTE) &rc.top,
                  sizeof(DWORD));
    RegSetValueEx(hKey, _T("w"), NULL, REG_DWORD, (LPBYTE) &rc.right,
                  sizeof(DWORD));
    RegSetValueEx(hKey, _T("h"), NULL, REG_DWORD, (LPBYTE) &rc.bottom,
                  sizeof(DWORD));

    if (rc.left != -1 && rc.top != -1)
        SetWindowPos(GetParent(hListView), NULL, rc.left, rc.top,
                     rc.right, rc.bottom, SWP_NOZORDER);

    for (int i = 0; i < 8; ++i)  {
        _stprintf(colName, _T("Column%d"), i);
        width = ListView_GetColumnWidth(hListView, i);
        RegSetValueEx(hKey, colName, NULL, REG_DWORD, (LPBYTE) &width,
                      sizeof(DWORD));
    }

    RegCloseKey(hKey);
}

int FilenameCompare(const TCHAR * pa, const TCHAR * pb)
{
    int diff = CompareString(LOCALE_USER_DEFAULT, NORM_IGNORECASE | NORM_IGNOREKANATYPE | NORM_IGNOREWIDTH, pa, -1, pb, -1);

    if (diff == CSTR_EQUAL)
        diff = CompareString(LOCALE_USER_DEFAULT, SORT_STRINGSORT, pa, -1, pb, -1);

    return diff - CSTR_EQUAL;
}

int GetListViewSortColumn(LISTVIEW_COLUMN * ColumnList)
{
    for (int i = 0; ColumnList[i].Title; ++i)
        if (ColumnList[i].SortDir != SORTDIR_UNDEF)
            return i;

    return -1;
}

int CALLBACK CompareListViewColumn(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    LISTVIEW_SORT           *   ctx = (LISTVIEW_SORT *) lParamSort;
    WCHAR                       text1[ MAX_UNCPATH ];
    WCHAR                       text2[ MAX_UNCPATH ];
    int                         order = CSTR_EQUAL;
    int                         idx1, idx2;
    LVFINDINFO                  find;
    double                      num1, num2;

    // Get indices of items
    if (ctx->UsingEx)  {
        idx1 = lParam1;
        idx2 = lParam2;
    } else {
        find.flags = LVFI_PARAM;
        find.lParam = lParam1;
        idx1 = ListView_FindItem(ctx->hList, -1, &find);
        find.lParam = lParam2;
        idx2 = ListView_FindItem(ctx->hList, -1, &find);
    }

    ListView_GetItemText(ctx->hList, idx1, ctx->Column, text1, MAX_UNCPATH);
    ListView_GetItemText(ctx->hList, idx2, ctx->Column, text2, MAX_UNCPATH);

    switch (ctx->ColumnList[ ctx->Column ].Type)  {
    case DATATYPE_TEXT:
        order = FilenameCompare(text1, text2);
        break;

    case DATATYPE_NUMBER:
        _stscanf(text1, TEXT("%lf"), &num1);
        _stscanf(text2, TEXT("%lf"), &num2);

        if (num1 > num2)
            order = 1;
        else if (num1 < num2)
            order = -1;
        else
            order = 0;

        break;

    case DATATYPE_HEX:
        _stscanf(text1, TEXT("%lx"), &num1);
        _stscanf(text2, TEXT("%lx"), &num2);

        if (num1 > num2)
            order = 1;
        else if (num1 < num2)
            order = -1;
        else
            order = 0;

        break;

    case DATATYPE_DATE: {
        VarDateFromStr(text1, LANG_USER_DEFAULT, 0, &num1);
        VarDateFromStr(text2, LANG_USER_DEFAULT, 0, &num2);

        if (num1 < num2)
            order = -1;
        else if (num1 > num2)
            order = 1;
        else
            order = 0;
    }
    break;
    }

    if (ctx->ColumnList[ ctx->Column ].SortDir == SORTDIR_DOWN)
        order = -order;

    return order;
}


void SortListView(HWND hListview, int iSubItem, LISTVIEW_COLUMN * ColumnList, bool Reverse)
{
    bool            SortReversed = false;
    int             PrevColumn = 0;

    // If column is negative then repeat the last sort
    if (iSubItem < 0)  {
        iSubItem = GetListViewSortColumn(ColumnList);

        if (iSubItem >= 0)
            SortListView(hListview, iSubItem, ColumnList, false);

        return;
    }

    HCURSOR hSaveCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

    // if sorting the same column twice in a row then reverse the sort order
    if (ColumnList[ iSubItem ].SortDir == SORTDIR_UNDEF)  {
        // clicked on a new column
        for (PrevColumn = 0; ColumnList[PrevColumn].Title; ++PrevColumn)
            if (ColumnList[PrevColumn].SortDir != SORTDIR_UNDEF)
                break;

        ColumnList[PrevColumn].SortDir = SORTDIR_UNDEF;
        SortReversed = false;
    } else {
        // reverse sort direction if requested
        if (Reverse)
            SortReversed = ColumnList[ iSubItem ].SortDir == SORTDIR_UP;
        else
            SortReversed = ColumnList[ iSubItem ].SortDir != SORTDIR_UP;
    }

    ColumnList[ iSubItem ].SortDir = SortReversed ? SORTDIR_DOWN : SORTDIR_UP;
    // remove arrow from previously sorted column
    LVCOLUMN        lvc;
    lvc.mask = LVCF_FMT;
    ListView_GetColumn(hListview, PrevColumn, &lvc);
    lvc.fmt &= ~(LVCFMT_IMAGE | LVCFMT_COL_HAS_IMAGES | LVCFMT_BITMAP_ON_RIGHT);
    ListView_SetColumn(hListview, PrevColumn, &lvc);
    // add arrow to sorted column
    lvc.mask = LVCF_FMT | LVCF_IMAGE;
    ListView_GetColumn(hListview, iSubItem, &lvc);
    lvc.fmt |= LVCFMT_IMAGE | LVCFMT_COL_HAS_IMAGES | LVCFMT_BITMAP_ON_RIGHT;
    lvc.iImage = SortReversed ? LISTVIEW_IMG_DESCENDING : LISTVIEW_IMG_ASCENDING;
    ListView_SetColumn(hListview, iSubItem, &lvc);
    // Perform sort
    LISTVIEW_SORT   ctx;
    ctx.Column      = iSubItem;
    ctx.ColumnList  = ColumnList;
    ctx.hList       = hListview;
    ctx.UsingEx     = true;
    // Version 5.80 and later of Comctl32.dll
    BOOL ok = ListView_SortItemsEx(hListview, CompareListViewColumn, (LPARAM)&ctx);

    if (!ok)  {
        // Use old, slow way of sorting.
        ctx.UsingEx = false;
        ListView_SortItems(hListview, CompareListViewColumn, (LPARAM)&ctx);
    }

    SetCursor(hSaveCursor);
}

LPARAM ListView_GetParam(HWND hListView, UINT iItem)
{
    LV_ITEM lvi;
    lvi.mask    = LVIF_PARAM;
    lvi.iItem   = iItem;
    lvi.iSubItem = 0;

    if (! ListView_GetItem(hListView, &lvi))
        lvi.lParam = (LPARAM) - 1;

    return lvi.lParam;
}

void PopupListviewColumnsMenu(HWND hListview, LISTVIEW_COLUMN * ColumnList)
{
    HMENU           hMenu = CreatePopupMenu();
    MENUITEMINFO    item;
    ZeroMemory(&item, sizeof(item));
    // Build list of columns
    int col;

    for (col = 0; ColumnList[col].Title; ++col) {
        item.cbSize         = sizeof item;
        item.fMask          = MIIM_ID | MIIM_STATE | MIIM_CHECKMARKS | MIIM_TYPE;
        item.wID            = col + 1;  // zero is used for errors or no selection
        item.fType          = MFT_STRING;
        item.fState         = ListView_GetColumnWidth(hListview, col) ? MFS_CHECKED : 0;
        item.hbmpChecked    = NULL;
        item.hbmpUnchecked  = NULL;
        item.hbmpItem       = HBMMENU_SYSTEM;
        item.dwTypeData     = (TCHAR *) ColumnList[col].Title;
        InsertMenuItem(hMenu, col, true, &item);
    }

    // add separator
    item.fType = MFT_SEPARATOR;
    InsertMenuItem(hMenu, col++, true, &item);
    // add "more..." entry
    item.fType          = MFT_STRING;
    item.fState         = 0;
    item.dwTypeData     = TEXT("More...");
    item.wID            = col + 1;  // zero is used for errors or no selection
    InsertMenuItem(hMenu, col++, true, &item);
    // create menu
    POINT pt;
    GetCursorPos(&pt);
    int i = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY,
                           pt.x, pt.y, 0, hListview, NULL);

    if (i > 0) {
        // process result
        --i;

        if (i >= col - 1) {
            MessageBox(hListview, TEXT("More"), NULL, MB_OK);
        } else {
            if (ListView_GetColumnWidth(hListview, i) > 0) {
                // delete item
                ColumnList[i].Width = ListView_GetColumnWidth(hListview, i);
                ListView_SetColumnWidth(hListview, i, 0);
#if 0
                HWND hHeader = ListView_GetHeader(hListview);
                Header_DeleteItem(hHeader, i);
#endif
            } else {
                // insert item
                ListView_SetColumnWidth(hListview, i, ColumnList[i].Width);
#if 0
                HWND hHeader    = ListView_GetHeader(hListview);
                HDITEM  item    = { 0 };
                item.mask       = HDI_TEXT | HDI_WIDTH;
                item.pszText    = (TCHAR *) ColumnList[i].Title;
                item.cxy        = ColumnList[i].Width;
                Header_InsertItem(hHeader, i, &item);
#endif
            }
        }
    }
}
