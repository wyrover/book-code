// MultiCastChatDoc.h : interface of the CMultiCastChatDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTICASTCHATDOC_H__0F62798B_5A90_11D5_B587_00E04C39B036__INCLUDED_)
#define AFX_MULTICASTCHATDOC_H__0F62798B_5A90_11D5_B587_00E04C39B036__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "multicast.h"

class CMultiCastChatDoc : public CRichEditDoc
{
protected: // create from serialization only
	CMultiCastChatDoc();
	DECLARE_DYNCREATE(CMultiCastChatDoc)

// Attributes
public:
	CMultiCast m_socket;
	char* str;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiCastChatDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL
	virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo) const;

// Implementation
public:
	void Receive();
	virtual ~CMultiCastChatDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMultiCastChatDoc)
	afx_msg void OnTest();
	afx_msg void OnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTICASTCHATDOC_H__0F62798B_5A90_11D5_B587_00E04C39B036__INCLUDED_)
