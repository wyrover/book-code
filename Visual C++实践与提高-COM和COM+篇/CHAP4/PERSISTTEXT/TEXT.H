#if !defined(AFX_TEXT_H__5310B305_76A5_11D4_8BDA_0EB916000000__INCLUDED_)
#define AFX_TEXT_H__5310B305_76A5_11D4_8BDA_0EB916000000__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000
// Text.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CText command target

class CText : public CCmdTarget
{
    DECLARE_DYNCREATE(CText)

    CText();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CText)
public:
    virtual void OnFinalRelease();
    //}}AFX_VIRTUAL

// Implementation
protected:
    virtual ~CText();
    char* m_pchText;

    // Generated message map functions
    //{{AFX_MSG(CText)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
    DECLARE_OLECREATE(CText)

    // Generated OLE dispatch map functions
    //{{AFX_DISPATCH(CText)
    afx_msg VARIANT GetText();
    afx_msg void SetText(const VARIANT FAR& newValue);
    //}}AFX_DISPATCH
    DECLARE_DISPATCH_MAP()
    DECLARE_INTERFACE_MAP()

    BEGIN_INTERFACE_PART(PersistStream, IPersistStream)
    STDMETHOD(GetClassID)(LPCLSID);
    STDMETHOD(IsDirty)();
    STDMETHOD(Load)(LPSTREAM);
    STDMETHOD(Save)(LPSTREAM, BOOL);
    STDMETHOD(GetSizeMax)(ULARGE_INTEGER FAR*);
    END_INTERFACE_PART(PersistStream)
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXT_H__5310B305_76A5_11D4_8BDA_0EB916000000__INCLUDED_)
