// OleListDropTarget.h: interface for the COleListDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OLELISTDROPTARGET_H__6718724F_7D99_11D3_90F9_00105AA6C48C__INCLUDED_)
#define AFX_OLELISTDROPTARGET_H__6718724F_7D99_11D3_90F9_00105AA6C48C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxole.h>

class COleListDropTarget : public COleDropTarget  
{
public:
	void SetParent(CWnd* pParent);
	COleListDropTarget(CWnd* pParent = NULL);
	virtual ~COleListDropTarget();

   DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD 
                                                dwKeyState, CPoint point );
   DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD 
                                               dwKeyState, CPoint point );
   void OnDragLeave(CWnd* pWnd);
   
   BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT 
                                          dropEffect, CPoint point );

// Attributes
protected:
	CWnd* m_pParent;

};

#endif // !defined(AFX_OLELISTDROPTARGET_H__6718724F_7D99_11D3_90F9_00105AA6C48C__INCLUDED_)
