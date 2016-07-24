#if !defined(AFX_UDP_H__DE6CDDCF_A43B_11D5_A956_0050BA0F0366__INCLUDED_)
#define AFX_UDP_H__DE6CDDCF_A43B_11D5_A956_0050BA0F0366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UDP.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CUDP command target

class CUDP : public CSocket
{
// Attributes
public:

// Operations
public:
	CUDP();
	virtual ~CUDP();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUDP)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CUDP)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDP_H__DE6CDDCF_A43B_11D5_A956_0050BA0F0366__INCLUDED_)
