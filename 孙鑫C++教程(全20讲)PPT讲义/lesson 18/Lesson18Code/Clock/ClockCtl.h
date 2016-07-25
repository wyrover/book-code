#if !defined(AFX_CLOCKCTL_H__7FB41526_B435_4DB1_8E3B_7DA2A750205B__INCLUDED_)
#define AFX_CLOCKCTL_H__7FB41526_B435_4DB1_8E3B_7DA2A750205B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ClockCtl.h : Declaration of the CClockCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CClockCtrl : See ClockCtl.cpp for implementation.

class CClockCtrl : public COleControl
{
	DECLARE_DYNCREATE(CClockCtrl)

// Constructor
public:
	CClockCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClockCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CClockCtrl();

	DECLARE_OLECREATE_EX(CClockCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CClockCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CClockCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CClockCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CClockCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CClockCtrl)
	short m_interval;
	afx_msg void OnIntervalChanged();
	afx_msg void Hello();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CClockCtrl)
	void FireNewMinute()
		{FireEvent(eventidNewMinute,EVENT_PARAM(VTS_NONE));}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CClockCtrl)
	dispidInterval = 1L,
	dispidHello = 2L,
	eventidNewMinute = 1L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLOCKCTL_H__7FB41526_B435_4DB1_8E3B_7DA2A750205B__INCLUDED)
