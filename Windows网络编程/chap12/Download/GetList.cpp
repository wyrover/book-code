// GetList.cpp : implementation file
//

#include "stdafx.h"
#include "GetList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetList

CGetList::CGetList():m_nCount(0)
{

}

CGetList::~CGetList()
{
}


BEGIN_MESSAGE_MAP(CGetList, CListCtrl)
	//{{AFX_MSG_MAP(CGetList)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetList message handlers

int CGetList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    BOOL success = m_OleTarget.Register(this);
    if(!success )
      MessageBox("Ole Register Drop Target Failed");             
	
	m_OleTarget.SetParent(this);

	return 0;
}


//*****************************************************************************
// CGetList::AddUrl()
// Purpose: [ OleTarget 에서 Drop 메세지가 발생하였을때 처리하는 루틴]
// Parameters:	
// [strUrl] : 드래그 되어 들어온 Url
// Returns:	none
//*****************************************************************************
void CGetList::AddUrl(CString strUrl)
{
	InsertItem(m_nCount,(LPCTSTR)strUrl);

	m_nCount++;
}


