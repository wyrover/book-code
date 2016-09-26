#if !defined(AFX_CIRCCTL_H__A120B6B2_1D08_422F_8E9F_CE7AABC1A466__INCLUDED_)
#define AFX_CIRCCTL_H__A120B6B2_1D08_422F_8E9F_CE7AABC1A466__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

// CircCtl.h : Declaration of the CCircCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CCircCtrl : See CircCtl.cpp for implementation.

class CCircCtrl : public COleControl
{
    DECLARE_DYNCREATE(CCircCtrl)

// Constructor
public:
    CCircCtrl();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCircCtrl)
public:
    virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
    virtual void DoPropExchange(CPropExchange* pPX);
    virtual void OnResetState();
    //}}AFX_VIRTUAL

// Implementation
protected:
    ~CCircCtrl();
    short m_circleOffset;

    DECLARE_OLECREATE_EX(CCircCtrl)    // Class factory and guid
    DECLARE_OLETYPELIB(CCircCtrl)      // GetTypeInfo
    DECLARE_PROPPAGEIDS(CCircCtrl)     // Property page IDs
    DECLARE_OLECTLTYPE(CCircCtrl)       // Type name and misc status

// Message maps
    //{{AFX_MSG(CCircCtrl)
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

// Dispatch maps
    //{{AFX_DISPATCH(CCircCtrl)
    BOOL m_circleShape;
    afx_msg void OnCircleShapeChanged();
    afx_msg short GetCircleOffset();
    afx_msg void SetCircleOffset(short nNewValue);
    //}}AFX_DISPATCH
    DECLARE_DISPATCH_MAP()

    afx_msg void AboutBox();

// Event maps
    //{{AFX_EVENT(CCircCtrl)
    void FireClickIn(OLE_XPOS_PIXELS x, OLE_YPOS_PIXELS y)
    {
        FireEvent(eventidClickIn, EVENT_PARAM(VTS_XPOS_PIXELS  VTS_YPOS_PIXELS), x, y);
    }
    void FireClickOut()
    {
        FireEvent(eventidClickOut, EVENT_PARAM(VTS_NONE));
    }
    //}}AFX_EVENT
    DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
    void GetDrawRect(CRect* rc);
    BOOL InCircle(CPoint& point);
    BOOL InBounds(short nOffset);
    enum {
        //{{AFX_DISP_ID(CCircCtrl)
        dispidCircleShape = 1L,
        dispidCircleOffset = 3L,
        eventidClickIn = 1L,
        eventidClickOut = 2L,
        //}}AFX_DISP_ID
    };
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CIRCCTL_H__A120B6B2_1D08_422F_8E9F_CE7AABC1A466__INCLUDED)
