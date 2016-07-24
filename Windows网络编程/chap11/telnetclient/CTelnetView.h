// CTelnetView.h : interface of the CTelnetView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTelnetVIEW_H__243EC4A1_4FED_11D3_8C9E_00C0F0405B24__INCLUDED_)
#define AFX_CTelnetVIEW_H__243EC4A1_4FED_11D3_8C9E_00C0F0405B24__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTelnetDoc;
class CClientSocket;

const	unsigned char IAC		= 255;
const	unsigned char DO		= 253;
const	unsigned char DONT		= 254;
const	unsigned char WILL		= 251;
const	unsigned char WONT		= 252;
const	unsigned char SB		= 250;
const	unsigned char SE		= 240;
const	unsigned char IS		= '0';
const	unsigned char SEND		= '1';
const	unsigned char INFO		= '2';
const	unsigned char VAR		= '0';
const	unsigned char VALUE		= '1';
const	unsigned char ESC		= '2';
const	unsigned char USERVAR	= '3';

#define bufferLines 100
#define dtX 15
#define dtY 13

#define ioBuffSize 1024

class CTelnetView : public CScrollView
{
protected: // create from serialization only
	CTelnetView();
	DECLARE_DYNCREATE(CTelnetView)

	COLORREF cTextColor;
	COLORREF cBackgroundColor;

	CString cHostName;

//telnet related operations and variables
public:
	CClientSocket * cSock;
	void ArrangeReply(CString strOption);
	CString m_strNormalText;
	void RespondToOptions();
	void ProcessOptions();
	int TempCounter;
	CString m_strOptions;
	CStringList m_ListOptions;
	BOOL bNegotiating;
	BOOL bOptionsSent;
	CString m_strResp;
	CString m_strLine;
	unsigned char m_bBuf[ioBuffSize];
	BOOL GetLine( unsigned char * bytes, int nBytes, int& ndx );
	void DispatchMessage(CString strText);
	void ProcessMessage(CClientSocket * cSocket);

	char cText[80][bufferLines];
	long cCursX;
	CString m_strline;

	void DrawCursor(CDC * pDC, BOOL pDraw);
	void DoDraw(CDC* pDC);

private:
	void MessageReceived(LPCSTR pText);

// Attributes
public:
	CTelnetDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTelnetView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	int Find(CString str, char ch);
	virtual ~CTelnetView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTelnetView)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CTelnetView.cpp
inline CTelnetDoc* CTelnetView::GetDocument()
   { return (CTelnetDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTelnetVIEW_H__243EC4A1_4FED_11D3_8C9E_00C0F0405B24__INCLUDED_)
