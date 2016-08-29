#include "StdAfx.h"

void copyToClipboard(CWnd * wnd)
{
 CWindowDC wdc(wnd);  // Get DC for entire window

 CDC memDC;

 memDC.CreateCompatibleDC(&wdc);

 CBitmap bm;
 CRect r;
 
 wnd->GetWindowRect(&r);

 CSize sz(r.Width(), r.Height());

 bm.CreateCompatibleBitmap(&wdc, sz.cx, sz.cy);
 
 CBitmap * oldbm = memDC.SelectObject(&bm);
 
 memDC.BitBlt(0, 0, sz.cx, sz.cy, &wdc, 0, 0, SRCCOPY);

 wnd->GetParent()->OpenClipboard();
 ::EmptyClipboard();
 ::SetClipboardData(CF_BITMAP, bm.m_hObject);
 ::CloseClipboard();

 memDC.SelectObject(oldbm);
 bm.Detach();  // make sure bitmap not deleted with CBitmap object
}
