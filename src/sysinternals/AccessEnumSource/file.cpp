#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <stdio.h>
#include <CommDlg.h>

#include "AccessEnum.h"
#include "resource.h"
#include "resizer.h"


void CompareUsers(HWND hWnd, const TCHAR * share, CUser * aUser, CUser * bUser, void (*callback)(HWND hWnd, const TCHAR * format, ...), const TCHAR * type)
{
    // iterate over users
    for (;;)  {
        if (aUser == NULL  &&  bUser == NULL)
            break;

        int diff = aUser && bUser ? _tcsicmp(aUser->m_Acct, bUser->m_Acct) : aUser ? -1 : 1;

        if (diff < 0)  {
            // item deleted
            callback(hWnd, _T("%s lost %s access to %s"), type ? aUser->m_Acct : _T("Everyone"), type ? type : aUser->m_Acct, share);
            aUser = aUser->m_Next;
            continue;
        }

        if (diff > 0)  {
            // item added
            callback(hWnd, _T("%s gained %s access %s"), type ? bUser->m_Acct : _T("Everyone"), type ? type : bUser->m_Acct, share);
            bUser = bUser->m_Next;
            continue;
        }

        aUser = aUser->m_Next;
        bUser = bUser->m_Next;
    }
}


void CompareShares(HWND hWnd, CShare * aShare, CShare * bShare, void (*callback)(HWND hWnd, const TCHAR * format, ...))
{
    // iterate over shares
    for (;;)  {
#if 0

        // skip shares that we couldn't access permissions for
        while (aShare  &&  _tcschr(aShare->m_path + 2, '\\') == NULL)
            aShare = aShare->m_Next;

        while (bShare  &&  _tcschr(bShare->m_path + 2, '\\') == NULL)
            bShare = bShare->m_Next;

#endif

        if (aShare == NULL  &&  bShare == NULL)
            break;

        int diff = aShare && bShare ? _tcsicmp(aShare->m_path, bShare->m_path) : aShare ? -1 : 1;

        if (diff < 0)  {
            // item deleted
            callback(hWnd, _T("%s removed"), aShare->m_path);
            aShare = aShare->m_Next;
            continue;
        }

        if (diff > 0)  {
            // item added
            callback(hWnd, _T("%s added"), bShare->m_path);
            bShare = bShare->m_Next;
            continue;
        }

        CompareUsers(hWnd, aShare->m_path, aShare->m_OtherRead, bShare->m_OtherRead,    callback, _T("Read"));
        CompareUsers(hWnd, aShare->m_path, aShare->m_OtherWrite,    bShare->m_OtherWrite,   callback, _T("Write"));
        CompareUsers(hWnd, aShare->m_path, aShare->m_deny,          bShare->m_deny,         callback, _T("Deny"));
        aShare = aShare->m_Next;
        bShare = bShare->m_Next;
    }
}


// Split a CSV line into fields
int Split(TCHAR * line, TCHAR ** fields, TCHAR delim)
{
    TCHAR ** start = fields;
    TCHAR * dst = line;
    TCHAR * src = line;
    bool    got = false;

    while (*src)  {
        if (*src == delim)  {
            if (!got)
                *fields++ = dst;

            *dst++ = 0;
            got = false;
        } else if (*src != '"')  {
            if (!got)
                *fields++ = dst, got = true;

            *dst++ = *src;
        } else {
            ++src;

            while (*src != '"')  {
                if (*src == 0)
                    goto abort;

                if (!got)
                    *fields++ = dst, got = true;

                *dst++ = *src++;
            }
        }

        ++src;
    }

    if (!got)
        *fields++ = dst;

abort:
    *fields = NULL;
    *dst = 0;
    return fields - start;
}


CShare * ImportFile(const TCHAR * path)
{
    CShare  *   list    = NULL;
    FILE    *   fp      = _tfopen(path, _T("rb"));
    DWORD       line    = 0;

    if (fp == NULL)
        return NULL;

    // Mark file as unicode
#if UNICODE
    WORD uni = _fgettc(fp);

    if (uni != 0xFEFF)  {
        fclose(fp);
        return NULL;
    }

#endif
    TCHAR   Line[ 5 * MAX_TEXT ];
    int pos;

    for (;;)  {
        // read a line
        for (pos = 0; pos < sizeof Line / sizeof Line[0] - 1; ++pos)  {
            int ch = _fgettc(fp);

            if (ch == _TEOF)  {
                fclose(fp);
                return list;
            }

            Line[pos] = ch;

            if (pos >= 2  &&  Line[pos] == '\n' && Line[pos - 1] == '\r')  {
                ++pos;
                break;
            }
        }

        Line[pos] = 0;
        ++line;
        // remove trailing white space (particularly newlines)
        TCHAR   *   end = _tcschr(Line, 0);

        while (end > Line  &&  iswspace(end[-1]))
            *--end = 0;

        TCHAR   *   fields[ 20 ];
        int         cnt = Split(Line, fields, 0x9);

        if (cnt == 4)  {
            CShare * item = new CShare(fields[0], fields[1], fields[2], fields[3], ',');

            if (line != 1)
                item->InsertInList(&list);
        } else {
            // bad line
            MessageBox(NULL, _T("Bad file"), APPNAME, MB_OK | MB_ICONSTOP);
            return NULL;
        }
    }
}

void CompareCallback(HWND hWnd, const TCHAR * format, ...)
{
    TCHAR   msg[ 1024 ];
    va_list va;
    va_start(va, format);
    _vstprintf(msg, format, va);
    va_end(va);
#if 1
    // Add to listbox
    SendMessage(GetDlgItem(hWnd, IDC_LIST), LB_ADDSTRING, 0, (LPARAM)msg);
#else
    // Add to listview
    HWND hListview = GetDlgItem(hWnd, IDC_LIST);
    LVITEM  item;
    item.mask = LVIF_TEXT;
    item.iItem = 0x7FFFFFFF;
    item.iSubItem = 0;
    item.iImage = 0;
    item.pszText = msg;
    item.iItem = ListView_InsertItem(hListview, &item);
    item.iSubItem += 1;
    item.pszText = (TCHAR *) localpath;
    ListView_SetItem(hListview, &item);
#endif
}


INT_PTR CALLBACK CompareDialog(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
    static TCHAR        prevPath[ MAX_PATH ];
    OPENFILENAME        open;
    CShare          *   prev;

    switch (message)  {
    case WM_INITDIALOG:
        // Create listview columns
        new CResizer(hDlg);
        // get path to file
        memset(&open, 0, sizeof open);
        open.lStructSize    = sizeof open;
        open.hwndOwner      = hDlg;
        open.lpstrFilter    = TEXT("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");
        open.lpstrFile      = prevPath;
        open.nMaxFile       = MAX_PATH;
        open.Flags          = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
        open.lpstrTitle     = TEXT("Locate previously exported file");

        if (! GetOpenFileName(&open))  {
            EndDialog(hDlg, 0);
            break;
        }

        // read file
        prev = ImportFile(prevPath);

        if (prev == NULL)  {
            MessageBox(hDlg, _T("Error reading file"), APPNAME, MB_OK);
            break;
        }

        // compare files
        CompareShares(hDlg, prev, (CShare *)lParam, CompareCallback);
        // clean up
        delete prev;
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))  {
        case IDOK:
            EndDialog(hDlg, 0);
            break;
        }

        break;

    case WM_CLOSE:
        EndDialog(hDlg, 0);
        return TRUE;

    default:
        break;
    }

    return FALSE;
}
