//  ===========================================================================
//  File    CDialogSk.h
//  
//  Path    $(PROJECTROOT)
//  Desc    Definition for the class CButtonSK
//  
//  Rev
//  
//  This computer program is copyright to Texas Instruments Inc.
//  The program may not be used without the written permission
//  of Texas Instruments Inc. or against the terms and conditions
//  stipulated in the agreement under which this proram has been 
//  supplied.
//  
//  (c) Texas Instruments 2002
//  ===========================================================================
//  Revision History
//  
//  DEC 16, 2002    REF=ORG             Abhinaba Basu
//      Original implementation                   
//  ===========================================================================

#ifndef _ABHI_CDIALOG_H_
#define _ABHI_CDIALOG_H_

#include <afxcmn.h>

//  ===========================================================================
//  Layout styles for the background bmp
//  ===========================================================================
enum LayOutStyle
{
    LO_DEFAULT,
    LO_TILE,    // Tile the background picture
    LO_CENTER,  // Center the background picture
    LO_STRETCH, // Stretch the background picture to the dialog window size
    LO_RESIZE   // Resize the dialog so that it just fits the background 
};

//  ===========================================================================
//  class   CDialogSK
//  desc    This class can be used to skin Dialogs
//          To use this class create Dialog and then make the dialog class
//          derive from CDialogSK instead of CDialog
//  ===========================================================================
class CDialogSK : public CDialog
{
public:
    //  =======================================================================
    //  desc    constructor / desctructors
    //  
    //  arg1    handle to the bitmap for the bitmap
    //  =======================================================================
    CDialogSK(CWnd* pParent = NULL);
	CDialogSK(UINT uResourceID, CWnd* pParent = NULL);
	CDialogSK(LPCTSTR pszResourceID, CWnd* pParent = NULL);
    virtual  ~CDialogSK();

	//  =======================================================================
    //  desc    Set the bitmap for the button
    //  
    //  arg1    handle to the bitmap for the bitmap
    //  =======================================================================
    DWORD SetBitmap(HBITMAP hBitmap);
	
    //  =======================================================================
    //  desc    Set the bitmap for the button
    //  
    //  arg1    Resource Id for the bitmap
    //  =======================================================================
    DWORD SetBitmap(int nBitmap);
    
    //  =======================================================================
    //  desc    Set the bitmap for the button
    //  
    //  arg1    Name of the bitmap file
    //  =======================================================================
    DWORD SetBitmap(LPCTSTR lpszFileName);

    
    //  =======================================================================
    //  desc    Set the bitmap layout style
    //  
    //  arg1    See the definition of LayOutStyle above
    //  =======================================================================
    void SetStyle(LayOutStyle style);

	//  =======================================================================
    //  desc    Allows moving of the dialog by clicking anywhere in the dialog
    //  
    //  arg1    TRUE enables, FLASE disables
    //  =======================================================================
    void EnableEasyMove (BOOL pEnable = TRUE);

    //  =======================================================================
    //  desc    Makes the dialog transparent
    //  
    //  arg1    Alpha range is 0-255. If 255 then dialog becomes opaque
    //  ret     TRUE if successful
    //  =======================================================================
    BOOL SetTransparent (BYTE bAlpha);

    //  =======================================================================
    //  desc    Makes one color on the dialog transparent. This can be used 
    //          to create irregular shaped dialogs
    //  
    //  arg1    color to be made transparent
    //  arg2    TRUE will use the color, FALSE remove transparency
    //  ret     TRUE if successful
    //  =======================================================================
    BOOL SetTransparentColor (COLORREF col, BOOL bTrans = TRUE);

// Dialog Data
	//{{AFX_DATA(CDialogSK)
    // NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSK)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogSK)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    
	BOOL m_bEasyMove;
	void FreeResources();
	void Init();
	HBITMAP m_hBitmap;
    DWORD		m_dwWidth;			// Width of bitmap
	DWORD		m_dwHeight;			// Height of bitmap
    LayOutStyle m_loStyle;          // LayOutStyle style
};

#endif // _ABHI_CDIALOG_H_
