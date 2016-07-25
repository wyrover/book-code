#if !defined(AFX_WEIXINBTN_H__81A59DBC_28B9_4D7A_8885_6D30C4B12C5E__INCLUDED_)
#define AFX_WEIXINBTN_H__81A59DBC_28B9_4D7A_8885_6D30C4B12C5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WeiXinBtn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWeiXinBtn window

class CWeiXinBtn : public CButton
{
// Construction
public:
	CWeiXinBtn();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWeiXinBtn)
	//}}AFX_VIRTUAL

// Implementation
public:
	CWeiXinBtn* m_pBtn;
	virtual ~CWeiXinBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWeiXinBtn)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEIXINBTN_H__81A59DBC_28B9_4D7A_8885_6D30C4B12C5E__INCLUDED_)
