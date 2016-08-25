// SXBtn.cpp : implementation file
//

#include "stdafx.h"
#include "Graphic.h"
#include "SXBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSXBtn

CSXBtn::CSXBtn()
{
}

CSXBtn::~CSXBtn()
{
}


BEGIN_MESSAGE_MAP(CSXBtn, CButton)
	//{{AFX_MSG_MAP(CSXBtn)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSXBtn message handlers

void CSXBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	UINT uStyle = BS_DEFPUSHBUTTON ;//DFCS_BUTTONPUSH;
	
	// This code only works with buttons.
	ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);
	
	// If drawing selected, add the pushed style to DrawFrameControl.
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		uStyle |= DFCS_PUSHED;
	
	// Draw the button frame.
	::DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, 
		DFC_BUTTON, uStyle);
	
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	
	// Get the button's text.
	CString strText;
	GetWindowText(strText);
	
	// Draw the button text using the text color red.
	CBrush B;
	CRect rect;
	CRect focusRect;
	focusRect.CopyRect(&lpDrawItemStruct->rcItem); 
	DrawFocusRect(lpDrawItemStruct->hDC, (LPRECT)&focusRect);
	focusRect.left += 4;
	focusRect.right -= 4;
	focusRect.top += 4;
	focusRect.bottom -= 4;
	
	rect.CopyRect(&lpDrawItemStruct->rcItem); 
	pDC->Draw3dRect(rect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));
	B.CreateSolidBrush(RGB(0,255,0));
	::FillRect(lpDrawItemStruct->hDC,&rect, (HBRUSH)B.m_hObject);
	::SetBkMode(lpDrawItemStruct->hDC,TRANSPARENT);
	COLORREF crOldColor = ::SetTextColor(lpDrawItemStruct->hDC, RGB(255,0,0));
	::DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(), 
		&lpDrawItemStruct->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	::SetTextColor(lpDrawItemStruct->hDC, crOldColor);
}
