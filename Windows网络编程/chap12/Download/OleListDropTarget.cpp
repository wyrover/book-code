// OleListDropTarget.cpp: implementation of the COleListDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OleListDropTarget.h"
#include "GetList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COleListDropTarget::COleListDropTarget(CWnd* pParent)
{
	m_pParent = pParent;
}

COleListDropTarget::~COleListDropTarget()
{

}



//
// OnDragEnter is called by OLE dll's when drag cursor enters
// a window that is REGISTERed with the OLE dll's
//
DROPEFFECT COleListDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* 
           pDataObject, DWORD dwKeyState, CPoint point )
{
	if (!pDataObject->IsDataAvailable(CF_TEXT))
	{
		return DROPEFFECT_NONE;
	
	}

    // if the control key is held down, return a drop effect COPY 
    if((dwKeyState&MK_CONTROL) == MK_CONTROL)
        return DROPEFFECT_COPY;
    // Otherwise return a drop effect of MOVE
    else
        return DROPEFFECT_MOVE;    
}

//
// OnDragLeave is called by OLE dll's when drag cursor leaves
// a window that is REGISTERed with the OLE dll's
//
void COleListDropTarget::OnDragLeave(CWnd* pWnd)
{
    // Call base class implementation
    COleDropTarget::OnDragLeave(pWnd);
}

// 
// OnDragOver is called by OLE dll's when cursor is dragged over 
// a window that is REGISTERed with the OLE dll's
//
DROPEFFECT COleListDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* 
           pDataObject, DWORD dwKeyState, CPoint point )
{     
	if (!pDataObject->IsDataAvailable(CF_TEXT))
		return DROPEFFECT_NONE;

    if((dwKeyState&MK_CONTROL) == MK_CONTROL)
        return DROPEFFECT_NONE;  
    else
        return DROPEFFECT_MOVE;  // move source
}


BOOL COleListDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, 
                 DROPEFFECT dropEffect, CPoint point )
{           
    HGLOBAL  hGlobal;
    LPCSTR   pData;                     

	if (pDataObject->IsDataAvailable(CF_TEXT))
	{		
		STGMEDIUM Stg;
		BOOL bValue = pDataObject->GetData(CF_TEXT, &Stg);		

		TCHAR *strText = (TCHAR*)GlobalLock(Stg.hGlobal);
		
		CString strUrl;
		strUrl.Format("%s",strText);

		((CGetList *)m_pParent)->AddUrl(strUrl);

		GlobalUnlock(Stg.hGlobal);
		GlobalFree(Stg.hGlobal);		
	}

    if((dropEffect & DROPEFFECT_MOVE) != DROPEFFECT_MOVE)
        return FALSE;

    // Get text data from COleDataObject
    hGlobal=pDataObject->GetGlobalData(CF_TEXT);

    // Get pointer to data
    pData=(LPCSTR)GlobalLock(hGlobal);    
    ASSERT(pData!=NULL); 

    // Unlock memory - Send dropped text into the "bit-bucket"
    GlobalUnlock(hGlobal);

    return TRUE;
}

void COleListDropTarget::SetParent(CWnd *pParent)
{
	m_pParent = pParent;
}
